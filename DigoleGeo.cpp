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

#include "DigoleGeo.h"

void DigoleSerialDisp::write2B(unsigned int v) {
    if (v < 255)
        write(v);
    else {
        write(255);
        write(v - 255);
    }
}

void DigoleSerialDisp::preprint(void) {
    //write((uint8_t)0);
    Print::print("TT");
}

/*----------Functions for Graphic LCD/OLED adapters only---------*/
void DigoleSerialDisp::drawBitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) {
    uint8_t i = 0;
    if ((w & 7) != 0)
        i = 1;
    Print::print("DIM");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (int j = 0; j < h * ((w >> 3) + i); j++) {
        write(bitmap[j]);
        //delay(5);
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
    write2B(d);
}

void DigoleSerialDisp::setContrast(uint8_t c) {
    Print::print("CT");
    write2B(c);
}

void DigoleSerialDisp::drawBox(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    Print::print("FR");
    write2B(x);
    write2B(y);
    write2B(x + w);
    write2B(y + h);
}

void DigoleSerialDisp::drawCircle(unsigned int x, unsigned int y, unsigned int r, uint8_t f) {
    Print::print("CC");
    write2B(x);
    write2B(y);
    write2B(r);
    write(f);
}

void DigoleSerialDisp::drawDisc(unsigned int x, unsigned int y, unsigned int r) {
    drawCircle(x, y, r, 1);
}

void DigoleSerialDisp::drawFrame(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    Print::print("DR");
    write2B(x);
    write2B(y);
    write2B(x + w);
    write2B(y + h);
}

void DigoleSerialDisp::drawPixel(unsigned int x, unsigned int y, uint8_t color) {
    Print::print("DP");
    write2B(x);
    write2B(y);
    write(color);
}

void DigoleSerialDisp::drawLine(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1) {
    Print::print("LN");
    write2B(x);
    write2B(y);
    write2B(x1);
    write2B(y1);
}

void DigoleSerialDisp::drawLineTo(unsigned int x, unsigned int y) {
    Print::print("LT");
    write2B(x);
    write2B(y);
}

void DigoleSerialDisp::drawHLine(unsigned int x, unsigned int y, unsigned int w) {
    drawLine(x, y, x + w, y);
}

void DigoleSerialDisp::drawVLine(unsigned int x, unsigned int y, unsigned int h) {
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

void DigoleSerialDisp::moveArea(unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, char xoffset, char yoffset) {
    Print::print("MA");
    write2B(x0);
    write2B(y0);
    write2B(w);
    write2B(h);
    write(xoffset);
    write(yoffset);
}

void DigoleSerialDisp::uploadStartScreen(int lon, const unsigned char *data) {
    int j;
    uint8_t c;
    Print::print("SSS");
    write((uint8_t) (lon % 256));
    write((uint8_t) (lon / 256));
    delay(300);
    for (j = 0; j < lon; j++) {
        if ((j % 32) == 0)
            delay(50);
        delay(_Comdelay);
        c = data[j];
        write(c);
    }
}

void DigoleSerialDisp::uploadUserFont(int lon, const unsigned char *data, uint8_t sect) {
    uint8_t c;
    Print::print("SUF");
    write(sect);
    write((uint8_t) (lon % 256));
    write((uint8_t) (lon / 256));
    for (int j = 0; j < lon; j++) {
        if ((j % 32) == 0)
            delay(50);
        delay(_Comdelay);
        c = data[j];
        write(c);
    }
}

void DigoleSerialDisp::drawBitmap256(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
    Print::print("EDIM1");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (int j = 0; j < h * w; j++) {
        write(bitmap[j]);
        //delay(5);
    }
}

void DigoleSerialDisp::drawBitmap262K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
    Print::print("EDIM3");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (int j = 0; (j < h * w * 3); j++) {
        write(bitmap[j]);
        //        delay(5);
    }
}

void DigoleSerialDisp::setTrueColor(uint8_t r, uint8_t g, uint8_t b) { //Set true color
    Print::print("ESC");
    write(r);
    write(g);
    write(b);
}

void DigoleSerialDisp::drawStr(unsigned int x, unsigned int y, const char *s) {
    Print::print("TP");
    write2B(x);
    write2B(y);
    Print::print("TT");
    Print::print(s);
    write((uint8_t)0);
}

void DigoleSerialDisp::setPrintPos(unsigned int x, unsigned int y, uint8_t graph) {
    if (graph == 0) {
        Print::print("TP");
        write2B(x);
        write2B(y);
    } else {
        Print::print("GP");
        write2B(x);
        write2B(y);
    }
}

void DigoleSerialDisp::setLCDColRow(uint8_t col, uint8_t row) {
    Print::print("STCR");
    write(col);
    write(row);
    Print::print("\x80\xC0\x94\xD4");
}

void DigoleSerialDisp::setTextPosAbs(unsigned int x, unsigned int y) {
    Print::print("ETP");
    write2B(x);
    write2B(y);
}

//----Touch screen functions-----//

void DigoleSerialDisp::calibrateTouchScreen(void) {
    write('T');
    write('U');
    write('C');
    write('H');
    write('C');

}

void DigoleSerialDisp::readTouchScreen() {
    write('R');
    write('P');
    write('N');
    write('X');
    write('Y');
    write('W'); //change to 'I' if you don't want wait till pen touched
}

void DigoleSerialDisp::readClick() //read a click on touch screen
{
    write('R');
    write('P');
    write('N');
    write('X');
    write('Y');
    write('C');
}
int DigoleSerialDisp::readBattery(void)
{
  int c;
  write('R');
  write('D');
  write('B');
  write('A');
  write('T');
  c=read1();
  c<<=8;
  c|=read1();
  return c;
}
int DigoleSerialDisp::readAux(void)
{
  int c;
  write('R');
  write('D');
  write('A');
  write('U');
  write('X');
  c=read1();
  c<<=8;
  c+=read1();
  return c;
}
int DigoleSerialDisp::readTemperature(void)
{
  int c;
  write('R');
  write('D');
  write('T');
  write('M');
  write('P');
  c=read1();
  c<<=8;
  c|=read1();
  return c;
}
//-----Flash memory functions----//

void DigoleSerialDisp::flashErase(unsigned long int addr, unsigned long int length) {
    write('F');
    write('L');
    write('M');
    write('E');
    write('R');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(length >> 16);
    write(length >> 8);
    write(length);
}

void DigoleSerialDisp::flashWriteL(unsigned long int addr, unsigned long int len, const unsigned char *data) {
    while (len > 1024) {
        flashWrite(addr, 1024, data);
        data += 1024;
        addr += 1024;
        len = len - 1024;
    }
    flashWrite(addr, len, data);
}

void DigoleSerialDisp::flashWrite(unsigned long int addr, unsigned long int len, const unsigned char *data) {
    unsigned char c;
    write('F');
    write('L');
    write('M');
    write('W');
    write('R');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(len >> 16);
    write(len >> 8);
    write(len);
    for (long int i = 0; i < len; i++) {
        c = data[i];
        write(c);
    }
    //check write memory done
    while (read1()!= 17);
}

void DigoleSerialDisp::flashReadStart(unsigned long int addr, unsigned long int len) {
    write('F');
    write('L');
    write('M');
    write('R');
    write('D');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(len >> 16);
    write(len >> 8);
    write(len);
}

void DigoleSerialDisp::setFlashFont(unsigned long int addr) {
    write('S');
    write('F');
    write('F');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
}

void DigoleSerialDisp::runCommandSet(unsigned long int addr) {
    write('F');
    write('L');
    write('M');
    write('C');
    write('S');
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
}

// Graphic primitives extension

int DigoleSerialDisp::setNativeDisplaySize(int w, int h)
{
	if ((w > 320 || w < 0) || (h > 240 || h < 0))
		return -1;

	_max_x = w;
	_max_y = h;
	return 0;
}

int DigoleSerialDisp::getNativeDisplayWidth(void)
{
	return _max_x;
}

int DigoleSerialDisp::getNativeDisplayHeight(void)
{
	return _max_y;
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
		delayMicroseconds(1500);
		drawLine(x1+1,y2-1,x1+1,y2-1);
		drawLine(x2-1,y2-1,x2-1,y2-1);
		delayMicroseconds(1500);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		delayMicroseconds(1500);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
		delayMicroseconds(1500);
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
				delayMicroseconds(2000);
				drawHLine(x1+2, y2-i, x2-x1-4);
				delayMicroseconds(2000);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				delayMicroseconds(2000);
				drawHLine(x1+1, y2-i, x2-x1-2);
				delayMicroseconds(2000);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				delayMicroseconds(2000);
				drawHLine(x1, y2-i, x2-x1);
				delayMicroseconds(2000);
			}
		//delayMicroseconds(4000);
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
		delayMicroseconds(1000);
	}

	// Lower part
	for(; y<=y3; y++) {
		xs = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
		xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		drawHLine(xs, y, xe-xs);
		delayMicroseconds(1000);
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
			StoppingX=StoppingX - TwoBSquare;
			EllipseError=EllipseError + XChange;
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
		EllipseError=EllipseError + XChange;
		XChange=XChange+TwoBSquare;
		if ((2*EllipseError + YChange) > 0 ) {
			Y--;
			StoppingY=StoppingY- TwoASquare;
			EllipseError=EllipseError + YChange;
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
	    if ((_CXaddX <= _max_x) || (_CYaddY <= _max_y)) {
		    drawPixel(_CXaddX, _CYaddY);                //{point in quadrant 1}
			delayMicroseconds(1000);
		}

	    if ((_CXsubX >= 0) || (_CYaddY <= _max_y)){
		    drawPixel(_CXsubX, _CYaddY);                //{point in quadrant 2}
			delayMicroseconds(1000);
		}
	    if ((_CXsubX >= 0) || (_CYaddY >= 0)) {
		    drawPixel(_CXsubX, _CYsubY);                //{point in quadrant 3}
			delayMicroseconds(1000);
		}
	    if ((_CXaddX <= _max_x) || (_CYaddY >= 0)) {
		    drawPixel(_CXaddX, _CYsubY);                //{point in quadrant 4}
			delayMicroseconds(1000);
		}
	}
	else {
		// to fill rather than draw a line, plot between the points
		// Constrain the endpoits to inside the display area
        _CXaddX = constrain(_CXaddX, 0, _max_x);
        _CXsubX = constrain(_CXsubX, 0, _max_x);
        _CYaddY = constrain(_CYaddY, 0, _max_y);
        _CYsubY = constrain(_CYsubY, 0, _max_y);

        drawLine(_CXaddX, _CYaddY, _CXsubX, _CYaddY);
		delayMicroseconds(1000);
		drawLine(_CXsubX, _CYsubY, _CXaddX, _CYsubY);
		delayMicroseconds(1000);
	}
}
