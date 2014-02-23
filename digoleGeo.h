/* *********************************************************************************** */
/* Digole Serial Display Library - Version 001 - Copyright 2014 Timothy Brown / Digole */
/* *********************************************************************************** */
/* Setup the class for your display *before* void setup():                             */
/*                                                                                     */
/* DigoleSerialDisp digole(arguments);                                                 */
/*                                                                                     */
/* Arguments:                                                                          */
/*                                                                                     */
/* [SPI] Chip Select Pin (SS for the default pin, 255 if CS is hardwired low.)         */
/* [SoftSPI] Data Pin, Clock Pin, Chip Select Pin (255 if CS is hardwired low.)        */
/* [I2C] Data Pin, Clock Pin                                                           */
/* [UART] TX Pin                                                                       */
/*                                                                                     */
/* To use, call digole.begin(); *inside* void setup(); (or loop) to start the display. */
/* You may also call digole.end(); to release the pins and clear the I2C/SPI/UART bus. */
/* *********************************************************************************** */

/* ************************************************* */
/* !Important! Change XXXX to the desired interface: */
/*              SPI, SoftSPI, I2C, UART              */
/* ************************************************* */
#ifndef DigoleSerialDisp_h
#define DigoleSerialDisp_h


/* **************************** */
/* *** Begin Digole Library *** */
/* **************************** */

// ************
// * Digole.h *
// ************

class DigoleSerialDisp : public Print {
public:

//
// UART/I2C/SoftSPI/SPI Functions
//

#if defined(_Digole_Serial_SPI_)
    DigoleSerialDisp(uint8_t pinSS) {
        
        _Comdelay = 10;
        _SS = pinSS;
        
	}
    
    void begin(void) {
        
        pinMode(_SS, OUTPUT);
        digitalWrite(_SS, HIGH);
        SPI.setBitOrder(MSBFIRST);
        SPI.setClockDivider(SPI_CLOCK_DIV32);
        SPI.setDataMode(1);
        SPI.begin();
        
    }
    
    void end(void) {
        pinMode(_SS, INPUT);
        SPI.end();
    }

    size_t write(uint8_t value) {
        
        PIN_MAP[_SS].gpio_peripheral->BRR = PIN_MAP[_SS].gpio_pin; //Low
        delayMicroseconds(1);
        //SPI.setDataMode(3);
        SPI.transfer(value);
        //SPI.setDataMode(0);
        delayMicroseconds(1);
        PIN_MAP[_SS].gpio_peripheral->BSRR = PIN_MAP[_SS].gpio_pin; //High
        return 1;
    }
#endif

#if defined(_Digole_Serial_SoftSPI_)
    DigoleSerialDisp(uint8_t pinData, uint8_t pinClock, uint8_t pinSS) {
        
        _Comdelay = 1;
        
        _Clock = pinClock;
        _Data = pinData;
        _SS = pinSS;
        
    }
    
    void begin(void) {
        
        pinMode(_Clock, OUTPUT);
        pinMode(_Data, OUTPUT);
        pinMode(_SS, OUTPUT);
        digitalWrite(_Clock, LOW);
        digitalWrite(_Data, LOW);
        digitalWrite(_SS, HIGH);
        
    }
    
    void end(void) {
        pinMode(_Clock, INPUT);
        pinMode(_Data, INPUT);
        pinMode(_SS, INPUT);
    }

    size_t write(uint8_t value) {
        
        PIN_MAP[_SS].gpio_peripheral->BRR = PIN_MAP[_SS].gpio_pin; //Low
        shiftOut(_Data, _Clock, MSBFIRST, value);
        PIN_MAP[_SS].gpio_peripheral->BSRR = PIN_MAP[_SS].gpio_pin; //High

/*
        PIN_MAP[_SS].gpio_peripheral->BRR = PIN_MAP[_SS].gpio_pin; // Latch Low
        delayMicroseconds(1);
        for (uint8_t i = 0; i < 8; i++)  {
            if (value & (1 << (7-i))) {     // walks down mask from bit 7 to bit 0
                PIN_MAP[_Data].gpio_peripheral->BSRR = PIN_MAP[_Data].gpio_pin; // Data High
            } 
            else {
                PIN_MAP[_Data].gpio_peripheral->BRR = PIN_MAP[_Data].gpio_pin; // Data Low
            }
            asm volatile("mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" ::: "r0", "cc", "memory");
            asm volatile("mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" ::: "r0", "cc", "memory");
            PIN_MAP[_Clock].gpio_peripheral->BSRR = PIN_MAP[_Clock].gpio_pin; // Clock High (Data Shifted In)
            asm volatile("mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" ::: "r0", "cc", "memory");
            asm volatile("mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" ::: "r0", "cc", "memory");
            PIN_MAP[_Clock].gpio_peripheral->BRR = PIN_MAP[_Clock].gpio_pin; // Clock Low
        }
        delayMicroseconds(1);
        PIN_MAP[_SS].gpio_peripheral->BSRR = PIN_MAP[_SS].gpio_pin;     // Latch High (Data Latched)
*/
        return 1;
    }
#endif

#if defined(_Digole_Serial_I2C_)
    DigoleSerialDisp(uint8_t add)
    {
        _I2Caddress = add;
        _Comdelay=15;
    }

	void begin(void) {
		Wire.begin();
    }

    void end(void) {
    }

    size_t write(uint8_t value) {
        Wire.beginTransmission(_I2Caddress);
        Wire.write(value);
        Wire.endTransmission();
        return 1;
    }
#endif

#if defined(_Digole_Serial_UART_)

DigoleSerialDisp(USARTSerial *s, unsigned long baud) //UART set up
    {
        _mySerial = s;
        _Baud = baud;
        _Comdelay=2;
    }

    size_t write(uint8_t value) {
        _mySerial->write(value);
        return 1; // assume sucess
    }

    void begin(void) {
        _mySerial->begin(9600);
        _mySerial->print("SB");
        _mySerial->println(_Baud);
        delay(100);
        _mySerial->begin(_Baud);
    }
#endif

    //
    // Print Functions
    //

    size_t println(const String &v) {
        preprint();
        Print::println(v);
        Print::println("\x0dTRT");
    }

    size_t println(const char v[]) {
        preprint();
        Print::println(v);
        Print::println("\x0dTRT");
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
        Print::println("\x0dTRT");
    }

    size_t println(unsigned int v, int base = DEC) {
        preprint();
        Print::println(v, base);
        Print::println("\x0dTRT");
    }

    size_t println(long v, int base = DEC) {
        preprint();
        Print::println(v, base);
        Print::println("\x0dTRT");
    }

    size_t println(unsigned long v, int base = DEC) {
        preprint();
        Print::println(v, base);
        Print::println("\x0dTRT");
    }

    size_t println(double v, int base = 2) {
        preprint();
        Print::println(v, base);
        Print::println("\x0dTRT");
    }

    size_t println(const Printable& v) {
        preprint();
        Print::println(v);
        Print::println("\x0dTRT");
    }

    size_t println(void) {
        Print::println("\x0dTRT");
    }


    size_t print(const String &v) {
        preprint();
        Print::println(v);
    }

    size_t print(const char v[]) {
        preprint();
        Print::println(v);
    }

    size_t print(char v) {
        preprint();
        Print::println(v);
    }

    size_t print(unsigned char v, int base = DEC) {
        preprint();
        Print::println(v, base);
    }

    size_t print(int v, int base = DEC) {
        preprint();
        Print::println(v, base);
    }

    size_t print(unsigned int v, int base = DEC) {
        preprint();
        Print::println(v, base);
    }

    size_t print(long v, int base = DEC) {
        preprint();
        Print::println(v, base);
    }

    size_t print(unsigned long v, int base = DEC) {
        preprint();
        Print::println(v, base);
    }

    size_t print(double v, int base = 2) {
        preprint();
        Print::println(v, base);
    }

    size_t print(const Printable& v) {
        preprint();
        Print::println(v);
    }
    
    //
    // Text LCD Adapter Functions
    //

    void disableCursor(void) {
        Print::print("CS0");
    }

    void enableCursor(void) {
        Print::print("CS1");
    }

    void drawStr(uint8_t x, uint8_t y, const char *s) {
        Print::print("TP");
        write(x);
        write(y);
        Print::print("TT");
        Print::println(s);
    }

    void setPrintPos(uint8_t x, uint8_t y, bool graph = false) {
        if (graph == false) {
            Print::print("TP");
            write(x);
            write(y);
        } else {
            Print::print("GP");
            write(x);
            write(y);
        }
    }

    void clearScreen(void) {
        Print::print("CL");
    }

    void setLCDColRow(uint8_t col, uint8_t row) {
        Print::print("STCR");
        write(col);
        write(row);
        Print::print("\x80\xC0\x94\xD4");
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

    void setTextPosAbs(uint8_t x, uint8_t y) {
        Print::print("ETP");
        write(x);
        write(y);
    }
    void setLinePattern(uint8_t pattern) {
        Print::print("SLP");
        write(pattern);
    }
    void setLCDChip(uint8_t chip) {      //only for universal LCD adapter
        Print::print("SLCD");
        write(chip);
    }
     
     void digitalOutput(uint8_t x) {
         Print::print("DOUT");
         write(x);
    }
    
    void preprint(void);

	//
	// Graphic LCD/OLED Adapter Functions (U8GLIB Compatible)
	//

    void drawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);
    void drawBitmap256(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);
    void drawBitmap262K(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);
    void setTrueColor(uint8_t r, uint8_t g, uint8_t b);
    void setRot90(void);
    void setRot180(void);
    void setRot270(void);
    void undoRotation(void);
    void setRotation(uint8_t);
    void setContrast(uint8_t);
    void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t = 0);
    void drawDisc(uint8_t x, uint8_t y, uint8_t r);
    void drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawPixel(uint8_t x, uint8_t y, uint8_t = 1);
    void drawLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
    void drawLineTo(uint8_t x, uint8_t y);
    void drawHLine(uint8_t x, uint8_t y, uint8_t w);
    void drawVLine(uint8_t x, uint8_t y, uint8_t h);
    void drawRoundRect(int x1, int y1, int x2, int y2);
    void fillRoundRect(int x1, int y1, int x2, int y2);
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness);
    void drawPie(int x, int y, int r, int startAngle, int endAngle);

	//
	// Graphic LCD/OLED Adapter Functions (Special Functions)
	//

    void setFont(uint8_t font); //set font, availale: 6,10,18,51,120,123, user font: 200-203
    void nextTextLine(void); //got to next text line, depending on the font size
    void setColor(uint8_t); //set color for graphic function
    void backLightOn(void); //Turn on back light
    void backLightOff(void); //Turn off back light
    void setScreenOn(void);  //Turn screen on 
    void setScreenOff(void); //Turn screen off to save power
    void directCommand(uint8_t d); //send command to LCD drectly
    void directData(uint8_t d); //send data to LCD drectly
    void moveArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char xoffset, char yoffset); //move a area of screen to another place
    void uploadStartScreen(int lon, const unsigned char *data); //upload start screen
    void uploadUserFont(int lon, const unsigned char *data, uint8_t sect); //upload user font

private:
    unsigned long _Baud;
    USARTSerial *_mySerial;
    uint8_t _I2Caddress;
    TwoWire *_myWire;
    uint8_t _Clock;
    uint8_t _Data;
    uint8_t _SS;
    uint8_t _Comdelay;

};
#endif
