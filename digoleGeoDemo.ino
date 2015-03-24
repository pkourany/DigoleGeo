/* *********************************************************************************** */
/* Digole Serial Display Library - Version 006                                         */
/* Copyright 2014 Paul Kourany / Timothy Brown / Digole                                */
/* *********************************************************************************** */
/* Setup the class for your display *before* void setup():                             */
/*                                                                                     */
/* DigoleSerialDisp digole(arguments);                                                 */
/*                                                                                     */
/* Arguments:                                                                          */
/*                                                                                     */
/* [SPI] Chip Select Pin (SS for the default pin, 255 if CS is hardwired low.)         */
/* [SoftSPI] Data Pin, Clock Pin, Chip Select Pin (255 if CS is hardwired low.)        */
/* [I2C] Address of the Display (Default: 0x27)                                        */
/* [UART] Baud Rate [9600 to 115200]                                                   */
/*                                                                                     */
/* To use, call digole.begin(); *inside* void setup(); to start the display. */
/* You may also call digole.end(); to release the pins and clear the I2C/SPI/UART bus. */
/*                                                                                     */
/* *********************************************************************************** */

/* Uncomment the #define below for the desired interface: SPI, SoftSPI, I2C, UART. */

//#define _Digole_Serial_SPI_
//#define _Digole_Serial_SoftSPI_
//#define _Digole_Serial_I2C_
#define _Digole_Serial_UART_

/* Be sure to #include "DigoleSerialDisp.h" *after* the #define above or your compile will fail. */
/* !!! Don't forget to set display width and display height in file DigoleSerialDisp.h !!!!! */

#include "DigoleSerialDisp.h"

/* Below are sane defaults for the various interfaces. Uncomment one and customize as needed! */
//DigoleSerialDisp mydisp(D2, D3, SS);			//SOFT SPI setup - specify DATA, CLOCK, SS pins
//DigoleSerialDisp mydisp(SS);					//Hardware SPI setup - specify SS pin
//DigoleSerialDisp mydisp('\x27');				//I2C specified - specify Pointer to Wire object, I2C address of Digole
DigoleSerialDisp mydisp(&Serial1, 115200);	//USART (Serial 1 or 2) - specify &Serial1 or &Serial2 and baudrate

#define _TEXT_ 0
#define _GRAPH_ 1

//Colours defined as 8-bit value RRRG GGBB
#define VGA_BLACK   0x00
#define VGA_WHITE   0x01
#define VGA_BLUE    0x03
#define VGA_DARKRED 0x20

#define VGA_RED     0xE0
#define VGA_GREEN   0x1C
#define VGA_LIME    0x14
#define VGA_YELLOW  0xFC

const int max_x = DISP_W;
const int max_y = DISP_H;


void setup() {
  mydisp.begin();
  mydisp.backLightOn();
  mydisp.setColor(1);
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
    mydisp.drawRoundRect(x1, y1, x2, y2);
    delay(10);
  }

  delay(1000);
  mydisp.clearScreen(); //CLear screen

// Draw some random triangles
  for (int i=0; i<50; i++)
  {
    mydisp.setTrueColor(random(255), random(255), random(255));
    x1=random(max_x);
    y1=random(max_y);
    x2=random(max_x);
    y2=random(max_y);
    mydisp.fillRoundRect(x1, y1, x2, y2);
    delay(15);
  }

  delay(1000);
  mydisp.clearScreen(); //CLear screen

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
    delay(15);
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
    ae=random(360);    mydisp.drawPie(x1, y1, r, as, ae);
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
  mydisp.clearScreen(); //CLear screen

// Draw some random ellipses
  for (int i=0; i<50; i++)
  {
    mydisp.setTrueColor(random(255), random(255), random(255));
    x1=20+random(max_x-20);
    y1=20+random(max_y-20);
    x2=5+random(30);
    y2=5+random(30);
    mydisp.drawEllipse(x1, y1, x2, y2);
    delay(100);
  }

  delay(2000);
  mydisp.clearScreen(); //CLear screen

// Draw some random filled ellipses
  for (int i=0; i<50; i++)
  {
    mydisp.setTrueColor(random(255), random(255), random(255));
    x1=20+random(max_x-20);
    y1=20+random(max_y-20);
    x2=5+random(30);
    y2=5+random(30);
    mydisp.drawFilledEllipse(x1, y1, x2, y2);
    delay(100);
  }

}
