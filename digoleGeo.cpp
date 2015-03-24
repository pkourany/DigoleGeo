/* *********************************************************************************** */
/* Digole Serial Display Library (With primitives extension) - Version 006					                       */
/* Copyright 2014 Paul Kourany / Timothy Brown / Digole 			                   */
/* *********************************************************************************** */

// ************************
// * DigoleGeo.cpp *
// ************************

#include "DigoleGeo.h"
#include <math.h>

#define swap(a, b) { int16_t t = a; a = b; b = t; }

const float pi = 3.1415926535;


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

void DigoleSerialDisp::setScreenOn(void) {
    Print::print("SOO");
    write((uint8_t) 1);
}

void DigoleSerialDisp::setScreenOff(void) {
    Print::print("SOO");
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


void DigoleSerialDisp::drawRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
		swap(x1, x2);

	if (y1>y2)
		swap(y1, y2);

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawLine(x1+1,y1+1,x1+1,y1+1);
		drawLine(x2-1,y1+1,x2-1,y1+1);
		drawLine(x1+1,y2-1,x1+1,y2-1);
		drawLine(x2-1,y2-1,x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}


void DigoleSerialDisp::fillRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
		swap(x1, x2);

	if (y1>y2)
		swap(y1, y2);

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
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

void DigoleSerialDisp::drawEllipse(int CX, int CY, int XRadius, int YRadius) {
    plotEllipse(CX, CY, XRadius, YRadius, 0);
}

void DigoleSerialDisp::drawFilledEllipse(int CX, int CY, int XRadius, int YRadius) {
    plotEllipse(CX, CY, XRadius, YRadius, 1);
}

void DigoleSerialDisp::plotEllipse(int CX, int CY, int XRadius, int YRadius, int fill) {
	int X, Y;
	int XChange, YChange;
	int EllipseError;
	int TwoASquare,TwoBSquare;
	int StoppingX, StoppingY;
	TwoASquare = 2*XRadius*XRadius;
	TwoBSquare = 2*YRadius*YRadius;
	X = XRadius;
	Y = 0;
	XChange = YRadius*YRadius*(1-2*XRadius);

	YChange = XRadius*XRadius;
	EllipseError = 0;
	StoppingX = TwoBSquare*XRadius;
	StoppingY = 0;

	while ( StoppingX >=StoppingY ) //first set of points,y'>-1
	{
		plot4EllipsePoints(CX,CY,X,Y,fill);
		Y++;
		StoppingY=StoppingY+ TwoASquare;
		EllipseError = EllipseError+ YChange;
		YChange=YChange+TwoASquare;
		if ((2*EllipseError + XChange) > 0 ) {
			X--;
			StoppingX=StoppingX- TwoBSquare;
			EllipseError=EllipseError+ XChange;
			XChange=XChange+TwoBSquare;
		}
	}
	// first point set is done; start the 2nd set of points
	Y = YRadius;
	X = 0;
	YChange = XRadius*XRadius*(1-2*YRadius);
	XChange = YRadius*YRadius;
	EllipseError = 0;
	StoppingY = TwoASquare*YRadius;
	StoppingX = 0;
	while ( StoppingY >=StoppingX ) //2nd set of points, y'< -1
	{
		plot4EllipsePoints(CX,CY,X,Y,fill);
		X++;
		StoppingX=StoppingX + TwoBSquare;
		EllipseError=EllipseError+ XChange;
		XChange=XChange+TwoBSquare;
		if ((2*EllipseError + YChange) > 0 ) {
			Y--;
			StoppingY=StoppingY- TwoASquare;
			EllipseError=EllipseError+ YChange;
			YChange=YChange+TwoASquare;
		}
	}
}

void DigoleSerialDisp::plot4EllipsePoints(int CX, int CY, int X, int Y, int fill) {
    int _CXaddX, _CXsubX, _CYaddY, _CYsubY;

    _CXaddX = (CX+X); 
    _CXsubX = (CX-X);
    _CYaddY = (CY+Y);
    _CYsubY = (CY-Y);
    
	if (fill == 0) {    //Not fill so use pixels for outline
	    //For each quadrant, if point is outside display area, don't draw it
	    if ((_CXaddX <= _max_x) || (_CYaddY <= _max_y))               
		    drawPixel(_CXaddX, _CYaddY);                //{point in quadrant 1}
		    
	    if ((_CXsubX >= 0) || (_CYaddY <= _max_y))
		    drawPixel(_CXsubX, _CYaddY);                //{point in quadrant 2}

	    if ((_CXsubX >= 0) || (_CYaddY >= 0))
		    drawPixel(_CXsubX, _CYsubY);                //{point in quadrant 3}

	    if ((_CXaddX <= _max_x) || (_CYaddY >= 0))
		    drawPixel(_CXaddX, _CYsubY);                //{point in quadrant 4}
	}
	else {
		// to fill rather than draw a line, plot between the points
		// Constrain the endpoits to inside the display area
        _CXaddX = constrain(_CXaddX, 0, _max_x); 
        _CXsubX = constrain(_CXsubX, 0, _max_x);
        _CYaddY = constrain(_CYaddY, 0, _max_y);
        _CYsubY = constrain(_CYsubY, 0, _max_y);

        drawLine(_CXaddX, _CYaddY, _CXsubX, _CYaddY);
		drawLine(_CXsubX, _CYsubY, _CXaddX, _CYsubY);
	}
}

