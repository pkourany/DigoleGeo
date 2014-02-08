#include <math.h>
 
#define SC_W 160  //screen width in pixels
#define SC_H 128  //screen Hight in pixels
#define _TEXT_ 0
#define _GRAPH_ 1

#define swap(a, b) { int16_t t = a; a = b; b = t; }

#define VGA_BLACK   0x00
#define VGA_BLUE    0x02 //0A
#define VGA_LIME    0x14
#define VGA_YELLOW  0x08

const float pi = 3.1415926535;


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

#define _Digole_Serial_SPI_

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
/*      
        PIN_MAP[_SS].gpio_peripheral->BRR = PIN_MAP[_SS].gpio_pin; //Low
        shiftOut(_Data, _Clock, MSBFIRST, value);
        PIN_MAP[_SS].gpio_peripheral->BSRR = PIN_MAP[_SS].gpio_pin; //High
*/

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

        return 1;
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
    void directCommand(uint8_t d); //send command to LCD drectly
    void directData(uint8_t d); //send data to LCD drectly
    void moveArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char xoffset, char yoffset); //move a area of screen to another place
    void uploadStartScreen(int lon, const unsigned char *data); //upload start screen
    void uploadUserFont(int lon, const unsigned char *data, uint8_t sect); //upload user font

private:
    uint8_t _I2Caddress;
    uint8_t _Clock;
    uint8_t _Data;
    uint8_t _SS;
    uint8_t _Comdelay;

};

// **************
// * Digole.cpp *
// **************

void DigoleSerialDisp::preprint(void) {
    //write((uint8_t)0);
    Print::print("TT");
}

/*----------Functions for Graphic LCD/OLED adapters only---------*/
void DigoleSerialDisp::drawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap) {
	#if defined(_Digole_Serial_SPI_)
	delay(50);
	#endif
    uint8_t i = 0;
    if ((w & 7) != 0)
        i = 1;
    Print::print("DIM");
    write(x);
    write(y);
    write(w);
    write(h);
    for (int j = 0; j < h * ((w >> 3) + i); j++) {
        write(bitmap[j]);
    }
}

void DigoleSerialDisp::setRot90(void) {
    Print::print("SD1");
}

void DigoleSerialDisp::setRot180(void) {
    Print::print("SD2");
}

void DigoleSerialDisp::setRot270(void) {
    Print::print("SD3");
}

void DigoleSerialDisp::undoRotation(void) {
    Print::print("SD0");
}

void DigoleSerialDisp::setRotation(uint8_t d) {
    Print::print("SD");
    write(d);
}

void DigoleSerialDisp::setContrast(uint8_t c) {
    Print::print("CT");
    write(c);
}

void DigoleSerialDisp::drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    Print::print("FR");
    write(x);
    write(y);
    write(x + w);
    write(y + h);
}

void DigoleSerialDisp::drawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t f) {
    Print::print("CC");
    write(x);
    write(y);
    write(r);
    write(f);
}

void DigoleSerialDisp::drawDisc(uint8_t x, uint8_t y, uint8_t r) {
    drawCircle(x, y, r, 1);
}

void DigoleSerialDisp::drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    Print::print("DR");
    write(x);
    write(y);
    write(x + w);
    write(y + h);
}

void DigoleSerialDisp::drawPixel(uint8_t x, uint8_t y, uint8_t color) {
    Print::print("DP");
    write(x);
    write(y);
    write(color);
}

void DigoleSerialDisp::drawLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1) {
    Print::print("LN");
    write(x);
    write(y);
    write(x1);
    write(y1);
}

void DigoleSerialDisp::drawLineTo(uint8_t x, uint8_t y) {
    Print::print("LT");
    write(x);
    write(y);
}

void DigoleSerialDisp::drawHLine(uint8_t x, uint8_t y, uint8_t w) {
    drawLine(x, y, x + w, y);
}

void DigoleSerialDisp::drawVLine(uint8_t x, uint8_t y, uint8_t h) {
    drawLine(x, y, x, y + h);
}

void DigoleSerialDisp::nextTextLine(void) {
    write((uint8_t) 0);
    Print::print("TRT");
}

void DigoleSerialDisp::setFont(uint8_t font) {
    Print::print("SF");
    write(font);
}

void DigoleSerialDisp::setColor(uint8_t color) {
    Print::print("SC");
    write(color);
}

void DigoleSerialDisp::backLightOn(void) {
    Print::print("BL");
    write((uint8_t) 1);
}

void DigoleSerialDisp::backLightOff(void) {
    Print::print("BL");
    write((uint8_t) 0);
}

void DigoleSerialDisp::directCommand(uint8_t d) {
    Print::print("MCD");
    write(d);
}

void DigoleSerialDisp::directData(uint8_t d) {
    Print::print("MDT");
    write(d);
}

void DigoleSerialDisp::moveArea(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char xoffset, char yoffset) {
    Print::print("MA");
    write(x0);
    write(y0);
    write(w);
    write(h);
    write(xoffset);
    write(yoffset);
}

void DigoleSerialDisp::uploadStartScreen(int lon, const unsigned char *data) {
    Print::print("SSS");
    write((uint8_t) (lon % 256));
    write((uint8_t) (lon / 256));
    delay(300);
    for (int j = 0; j < lon; j++) {
        if((j%32)==0) {
            delay(50);
            delay(_Comdelay);
        }
         
        write(data[j]);
    }
}

void DigoleSerialDisp::uploadUserFont(int lon, const unsigned char *data, uint8_t sect) {
    Print::print("SUF");
    write(sect);
    write((uint8_t) (lon % 256));
    write((uint8_t) (lon / 256));
    for (int j = 0; j < lon; j++) {
        if((j%32)==0) {
            delay(50);
            delay(_Comdelay);
        }
         
        write(data[j]);
    }
}

void DigoleSerialDisp::drawBitmap256(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap) {
	#if defined(_Digole_Serial_SPI_)
	delay(50);
	#endif
    Print::print("EDIM1");
    write(x);
    write(y);
    write(w);
    write(h);
    for (int j = 0; j < h * w; j++) {
        if((j%1024)==0) {
            delay(_Comdelay);
        }
        write(bitmap[j]);
    }
}

void DigoleSerialDisp::drawBitmap262K(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap) {
	#if defined(_Digole_Serial_SPI_)
	delay(50);
	#endif
    Print::print("EDIM3");
    write(x);
    write(y);
    write(w);
    write(h);
    for (int j = 0; (j < h * w * 3); j++) {
        if((j%1024)==0) {
            delay(_Comdelay);
        }
        write(bitmap[j]);
    }
}

void DigoleSerialDisp::setTrueColor(uint8_t r, uint8_t g, uint8_t b) {	//Set true color
    Print::print("ESC");
    write(r); 
    write(g);
    write(b);
}

void DigoleSerialDisp::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x3, y3);
	drawLine(x3, y3, x1, y1);
}

void DigoleSerialDisp::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	int32_t xs, xe;
	int16_t y, ly;

	if (y1 > y2) {
		swap(y1, y2); 
		swap(x1, x2);
	}
	if (y2 > y3) {
		swap(y3, y2);
		swap(x3, x2);
	}
	if (y1 > y2) {
		swap(y1, y2);
		swap(x1, x2);
	}
	
	if(y1 == y3) {	// Single line triangles
		xs = xe = x1;
		if(x2 < xs)			xs = x2;
		else if(x2 > xe)	xe = x2;
		if(x3 < xs)			xs = x3;
		else if(x3 > xe)	xe = x3;
		drawHLine(xs, y1, xe-xs);
		return;
	}
	
	// Upper part
	if (y2 == y3) ly = y2;
	else          ly = y2-1;
	
	for(y=y1; y<=ly; y++) {
		xs = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		drawHLine(xs, y, xe-xs);
	}
	
	// Lower part
	for(; y<=y3; y++) {
		xs = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
		xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		drawHLine(xs, y, xe-xs);
	}
}

void DigoleSerialDisp::drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness)
{
	int rDelta = -(thickness/2);
	int px, py, cx, cy;

	startAngle -= 90;
	endAngle   -= 90;
	
	if (startAngle!=endAngle) {
		for (int i=0; i<thickness; i++) {
			px = x + cos((startAngle*pi)/180) * (r+rDelta+i);
			py = y + sin((startAngle*pi)/180) * (r+rDelta+i);
			for (int d=startAngle+1; d<endAngle+1; d++) {
				cx = x + cos((d*pi)/180) * (r+rDelta+i);
				cy = y + sin((d*pi)/180) * (r+rDelta+i);
				drawLine(px, py, cx, cy);
				px = cx;
				py = cy;
			}
		}
	}
	else {
		px = x + cos((startAngle*pi)/180) * (r+rDelta);
		py = y + sin((startAngle*pi)/180) * (r+rDelta);
		cx = x + cos((startAngle*pi)/180) * (r-rDelta);
		cy = y + sin((startAngle*pi)/180) * (r-rDelta);
		drawLine(px, py, cx, cy);
	}
}

void DigoleSerialDisp::drawPie(int x, int y, int r, int startAngle, int endAngle)
{
	int px, py, cx, cy;

	startAngle -= 90;
	endAngle   -= 90;
	if (startAngle>endAngle)
		startAngle -= 360;
	
	px = x + cos((startAngle*pi)/180) * r;
	py = y + sin((startAngle*pi)/180) * r;
	drawLine(x, y, px, py);
	for (int d=startAngle+1; d<endAngle+1; d++) {
		cx = x + cos((d*pi)/180) * r;
		cy = y + sin((d*pi)/180) * r;
		drawLine(px, py, cx, cy);
		px = cx;
		py = cy;
	}
	drawLine(x, y, px, py);
}

/* ************************** */
/* *** End Digole Library *** */
/* ************************** */
 
//const float pi = 3.1415926535;
const int max_x = 160;
const int max_y = 128;

  
//DigoleSerialDisp mydisp(D2, D3, SS);
DigoleSerialDisp mydisp(SS);
 
int random(int maxRand) {
    return rand() % maxRand;
}
 


 
void setup() {
  Serial.begin(9600);
  mydisp.begin();
  //mydisp.clearScreen(); //CLear screen
}
 
void loop() {

  int x1,x2,x3,y1,y2,y3,r,as,ae;

  mydisp.clearScreen(); //CLear screen
  mydisp.disableCursor();

// Draw some random triangles
  for (int i=0; i<50; i++)
  {
    mydisp.setTrueColor(random(255), random(255), random(255));
    x1=random(max_x);
    y1=random(max_y);
    x2=random(max_x);
    y2=random(max_y);
    x3=random(max_x);
    y3=random(max_y);
    mydisp.drawTriangle(x1, y1, x2, y2, x3, y3);
  }

  delay(1000);
  mydisp.clearScreen(); //CLear screen

// Draw some random filled triangles
  for (int i=0; i<50; i++)
  {
    mydisp.setTrueColor(random(255), random(255), random(255));
    x1=random(max_x);
    y1=random(max_y);
    x2=random(max_x);
    y2=random(max_y);
    x3=random(max_x);
    y3=random(max_y);
    mydisp.fillTriangle(x1, y1, x2, y2, x3, y3);
    delay(10);
  }

  delay(1000);
  mydisp.clearScreen(); //CLear screen

// Draw some random pies
  for (int i=0; i<50; i++)
  {
    mydisp.setTrueColor(random(255), random(255), random(255));
    x1=30+random(max_x-60);
    y1=30+random(max_y-60);
    r=10+random(20);
    as=random(360);
    ae=random(360);
    mydisp.drawPie(x1, y1, r, as, ae);
    delay(10);
  }

  delay(1000);
  mydisp.clearScreen(); //CLear screen

  mydisp.setColor(VGA_BLUE);
  mydisp.drawFrame(0, 0, 160, 128);

  mydisp.setColor(VGA_LIME);
  mydisp.drawArc(80,128,108,-40,40,3);
  for (int i=-40; i<=40; i+=8)
    mydisp.drawArc(80,128,103,i,i,5);

  int l = 0;
  for (int i=0; i>=-40; i-=1)
  {
    mydisp.setColor(VGA_BLACK);
    mydisp.drawArc(80,128,55,l,l,75);
    l=i;
    mydisp.setColor(VGA_YELLOW);
    mydisp.drawArc(80,128,55,i,i,75);
    delay(40-abs(i));
  }

  for (int i=-40; i<=40; i+=1)
  {
    mydisp.setColor(VGA_BLACK);
    mydisp.drawArc(80,128,55,l,l,75);
    l=i;
    mydisp.setColor(VGA_YELLOW);
    mydisp.drawArc(80,128,55,i,i,75);
    delay(40-abs(i));
  }

  for (int i=40; i>=0; i-=1)
  {
    mydisp.setColor(VGA_BLACK);
    mydisp.drawArc(80,128,55,l,l,75);
    l=i;
    mydisp.setColor(VGA_YELLOW);
    mydisp.drawArc(80,128,55,i,i,75);
    delay(40-i);
  }

  delay(1000);
  
}
