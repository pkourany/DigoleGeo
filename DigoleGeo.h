/*********************************************************************/
/* Digole Serial Display Library with Graphic Primitives Extensions
/* by Paul Kourany (peekay123), Feb 4, 2016
/* Digole Digital Solutions: www.digole.com
/*********************************************************************/
/* The following off-module graphics primitives have been added:
/*
/*	setNativeDisplaySize(int w, int h);
/*	getNativeDisplayWidth(void);
/*	getNativeDisplayHeight(void);
/*	
/*  drawRoundRect(x1, y1, x2, y2);
/*  fillRoundRect(x1, y1, x2, y2);
/*  drawTriangle(x1, y1, x2, y2, x3, y3);
/*  fillTriangle(x1, y1, x2, y2, x3, y3);
/*  drawArc(x, y, r, startAngle, endAngle, thickness);
/*  drawPie(x, y, r, startAngle, endAngle);
/*  drawEllipse(CX, CY, XRadius, YRadius);
/*  drawFilledEllipse(CX, CY, XRadius, YRadius);
/*
/*  NOTE: These new primitives use Digole line and pixel functions
/*		  for drawing.  These will not draw as fast as on-module
/*		  graphics primitives.
/*********************************************************************/


#ifndef DigoleSerial_h
#define DigoleSerial_h

#include "application.h"
#include "math.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

const float pi = 3.1415926535;

#define _TEXT_ 0
#define _GRAPH_ 1

// Set default native display width and height in pixels - common Digole display sizes are (HxW):	320x240, 128x64, 160x128, 96x64
#define DISP_W  240  //screen width in pixels
#define DISP_H  320  //screen Hight in pixels

class Digole{
public:

	virtual size_t read1(void);
};
class DigoleSerialDisp : public Print,public Digole {
public:
#if defined(_Digole_Serial_UART_)

    DigoleSerialDisp(USARTSerial *s, unsigned long baud) //UART set up
    {
        _mySerial = s;
        _Baud = baud;
        _Comdelay = 2;
    }

    size_t write(uint8_t value) {
        _mySerial->write((uint8_t) value);
        return 1; // assume sucess
    }

    void begin(void) {
        _mySerial->begin(9600);
        _mySerial->print("SB");
        _mySerial->println(_Baud);
        delay(100);
        _mySerial->begin(_Baud);
    }
	size_t read1(void)
	{
        int t;
        do {
            t = _mySerial->read();
        } while (t == -1);
        return t;
	}
#endif
#if defined(_Digole_Serial_I2C_)

    void begin(void) {
        _myWire->begin();
    }

    DigoleSerialDisp(TwoWire *s, uint8_t add) //U2C set up
    {
        _myWire = s;
        _I2Caddress = add;
        _Comdelay = 6;
    }

    size_t write(uint8_t value) {
        _myWire->beginTransmission(_I2Caddress);
        _myWire->write((uint8_t) value); //use yourself routing to send bulk of data, such as for…
        _myWire->endTransmission();
        return 1; // assume sucess
    }
    int readInt(void) {
        int t;
        _myWire->requestFrom((int) _I2Caddress, 2);
        while (_myWire->available() == 0);
        t = _myWire->read() << 8; //use yourself routing to read bulk of data, such as for…
        while (_myWire->available() == 0);
        t |= _myWire->read();
		_myWire->read();_myWire->read();
        return t;
    }

	size_t read1(void)
	{
        int t;
        _myWire->requestFrom((int) _I2Caddress, 1);
        while (_myWire->available() == 0);
        t = _myWire->read(); //use yourself routing to read bulk of data, such as for…
        return t; // assume sucess
	}
#endif
#if defined(_Digole_Serial_SPI_)

    void begin(void) {
    }

    DigoleSerialDisp(uint8_t pin_data, uint8_t pin_clock, uint8_t SS, uint8_t SI = 0) //spi set up
    {
        _Clockpin = pin_clock;
        _Datapin = pin_data;
        _SSpin = SS;
        _SIpin = SI;
        pinMode(_Clockpin, OUTPUT);
        pinMode(_Datapin, OUTPUT);
        pinMode(_SSpin, OUTPUT);
        digitalWrite(_SSpin, HIGH);
        digitalWrite(_Clockpin, LOW);
        digitalWrite(_Datapin, LOW);
        if (_SIpin != 0) {
            pinMode(_SIpin, INPUT);
        }
        _Comdelay = 6;
    }

    size_t write(uint8_t value) {
        digitalWrite(_SSpin, LOW);
        digitalWrite(_SSpin, LOW);
        digitalWrite(_SSpin, LOW);
        shiftOut(_Datapin, _Clockpin, MSBFIRST, value);
        digitalWrite(_SSpin, HIGH);
        return 1; // assume sucess
    }
	size_t read1(void)
	{
        int t = 0;
        char c;
        while (digitalRead(_SIpin) == LOW); //check to see the data is ready(1) or not(0)
        digitalWrite(_SSpin, LOW); //tell display module I will read data
        digitalWrite(_SSpin, LOW); //delay about 5us to wait module prepare data
        digitalWrite(_SSpin, LOW); //
        for (c = 8; c > 0; c = c - 1) {
            t <<= 1;
            digitalWrite(_Clockpin, HIGH);
            if (digitalRead(_SIpin))
                t |= 1;
            digitalWrite(_Clockpin, LOW);
        }

        //	t=shiftIn(_Datapin, _Clockpin, MSBFIRST);
        digitalWrite(_SSpin, HIGH);
        return t; // assume sucess
	}
#endif
   /*---------fucntions for Text and Graphic LCD adapters---------*/

    void disableCursor(void) {
        Print::print("CS0");
    }

    void enableCursor(void) {
        Print::print("CS1");
    }

    void clearScreen(void) {
        Print::print("CL");
    }

    void setI2CAddress(uint8_t add) {
        Print::print("SI2CA");
        write(add);
        _I2Caddress = add;
    }

    void displayConfig(uint8_t v) {
        Print::print("DC");
        write(v);
    }
    //print function

    size_t println(const String &v) {
        preprint();
        Print::println(v);
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t println(const char v[]) {
        preprint();
        Print::println(v);
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t println(char v) {
        preprint();
        Print::println(v);
        Print::println("\x0dTRT");
    }

    size_t println(unsigned char v, int base = DEC) {
        preprint();
        Print::println(v, base);
        Print::println("\x0dTRT");
    }

    size_t println(int v, int base = DEC) {
        preprint();
        Print::println(v, base);
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t println(unsigned int v, int base = DEC) {
        preprint();
        Print::println(v, base);
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t println(long v, int base = DEC) {
        preprint();
        Print::println(v, base);
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t println(unsigned long v, int base = DEC) {
        preprint();
        Print::println(v, base);
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t println(double v, int base = 2) {
        preprint();
        Print::println(v, base);
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t println(const Printable& v) {
        preprint();
        Print::println(v);
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t println(void) {
	write((uint8_t)0);
        Print::print("TRT");
    }

    size_t print(const String &v) {
        preprint();
        Print::print(v);
	write((uint8_t)0);
    }

    size_t print(const char v[]) {
        preprint();
        Print::print(v);
	write((uint8_t)0);
    }

    size_t print(char v) {
        preprint();
        Print::print(v);
	write((uint8_t)0);
    }

    size_t print(unsigned char v, int base = DEC) {
        preprint();
        Print::print(v, base);
	write((uint8_t)0);
    }

    size_t print(int v, int base = DEC) {
        preprint();
        Print::print(v, base);
	write((uint8_t)0);
    }

    size_t print(unsigned int v, int base = DEC) {
        preprint();
        Print::print(v, base);
	write((uint8_t)0);
    }

    size_t print(long v, int base = DEC) {
        preprint();
        Print::print(v, base);
	write((uint8_t)0);
    }

    size_t print(unsigned long v, int base = DEC) {
        preprint();
        Print::print(v, base);
	write((uint8_t)0);
    }

    size_t print(double v, int base = 2) {
        preprint();
        Print::print(v, base);
	write((uint8_t)0);
    }

    size_t print(const Printable& v) {
        preprint();
        Print::print(v);
	write((uint8_t)0);
    }
    void preprint(void);
    /*----------Functions for Graphic LCD/OLED adapters only---------*/
    //the functions in this section compatible with u8glib
    void drawBitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void drawBitmap256(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void drawBitmap262K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void setTrueColor(uint8_t r, uint8_t g, uint8_t b);
    void setRot90(void);
    void setRot180(void);
    void setRot270(void);
    void undoRotation(void);
    void setRotation(uint8_t);
    void setContrast(uint8_t);
    void drawBox(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    void drawCircle(unsigned int x, unsigned int y, unsigned int r, uint8_t = 0);
    void drawDisc(unsigned int x, unsigned int y, unsigned int r);
    void drawFrame(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    void drawPixel(unsigned int x, unsigned int y, uint8_t = 1);
    void drawLine(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1);
    void drawLineTo(unsigned int x, unsigned int y);
    void drawHLine(unsigned int x, unsigned int y, unsigned int w);
    void drawVLine(unsigned int x, unsigned int y, unsigned int h);
	// Graphic primitives extension by P.Kourany
	int setNativeDisplaySize(int w, int h);
	int getNativeDisplayWidth(void);
	int getNativeDisplayHeight(void);
	
    void drawRoundRect(int x1, int y1, int x2, int y2);
    void fillRoundRect(int x1, int y1, int x2, int y2);
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness);
    void drawPie(int x, int y, int r, int startAngle, int endAngle);
    void drawEllipse(int CX, int CY, int XRadius, int YRadius);
    void drawFilledEllipse(int CX, int CY, int XRadius, int YRadius);
    //-------------------------------
    //special functions for our adapters
    void setFont(uint8_t font); //set font, availale: 6,10,18,51,120,123, user font: 200-203
    void nextTextLine(void); //got to next text line, depending on the font size
    void setColor(uint8_t); //set color for graphic function
    void backLightOn(void); //Turn on back light
    void backLightOff(void); //Turn off back light
    void directCommand(uint8_t d); //send command to LCD drectly
    void directData(uint8_t d); //send data to LCD drectly
    void moveArea(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, char xoffset, char yoffset); //move a area of screen to another place
    void drawStr(unsigned int x, unsigned int y, const char *s);
    void setPrintPos(unsigned int x, unsigned int y, uint8_t = 0);
    void setLCDColRow(uint8_t col, uint8_t row);
    void setTextPosAbs(unsigned int x, unsigned int y);
    /*-----Touch screen functions---*/
    void calibrateTouchScreen(void);
    void readTouchScreen(void);
    void readClick(void);
    int readBattery(void);
    int readAux(void);
    int readTemperature(void);

    /*-----Flash memory functions---*/
    void flashErase(unsigned long int addr, unsigned long int length);
    void flashWriteL(unsigned long int addr, unsigned long int len, const unsigned char *data);
    void flashWrite(unsigned long int addr, unsigned long int len, const unsigned char *data);
    void flashReadStart(unsigned long int addr, unsigned long int len);
    void setFlashFont(unsigned long int addr);
    void runCommandSet(unsigned long int addr);
	size_t read(void);

    void write2B(unsigned int v);
    //--- new function on V3.0 firmware ----//

    void setBgColor(void) //set current color as background
    {
        Print::print("BGC");
    }

    void setDrawWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
        Print::print("DWWIN");
        write2B(x);
        write2B(y);
        write2B(width);
        write2B(height);
    }
    void resetDrawWindow(void)
    {
        Print::print("RSTDW");
    }
    void cleanDrawWindow(void)
    {
        Print::print("WINCL");
    }
//---end of V3.0 functions
    void displayStartScreen(uint8_t m) {
        Print::print("DSS");
        write(m);
    } //display start screen

    void setMode(uint8_t m) {
        Print::print("DM");
        write(m);
    } //set display mode

    void setTextPosBack(void) {
        Print::print("ETB");
    } //set text position back to previous, only one back allowed

    void setTextPosOffset(char xoffset, char yoffset) {
        Print::print("ETO");
        write(xoffset);
        write(yoffset);
    }

    void setLinePattern(uint8_t pattern) {
        Print::print("SLP");
        write(pattern);
    }

    void setLCDChip(uint8_t chip) { //only for universal LCD adapter
        Print::print("SLCD");
        write(chip);
    }

    void setBackLight(uint8_t bl) {
        Print::print("BL");
        write(bl);
    }

    void uploadStartScreen(int lon, const unsigned char *data); //upload start screen
    void uploadUserFont(int lon, const unsigned char *data, uint8_t sect); //upload user font

    void digitalOutput(uint8_t x) {
        Print::print("DOUT");
        write(x);
    }
private:
    unsigned long _Baud;
    USARTSerial *_mySerial;
    uint8_t _I2Caddress;
    TwoWire *_myWire;
    uint8_t _Clockpin;
    uint8_t _Datapin;
    uint8_t _SSpin;
    uint8_t _SIpin;
    uint8_t _Comdelay;

    int _max_x = DISP_W;
    int _max_y = DISP_H;

    void plotEllipse(int CX, int CY, int XRadius, int YRadius, int fill);
    void plot4EllipsePoints(int CX, int CY, int X, int Y, int fill);
};

#endif
