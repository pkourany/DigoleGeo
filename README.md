DigoleGeometry
==============

Digole for Spark Core with adapted UTFT Geometry library, based on Digole library adapted for Spark Core by timb

The demo is set for hardware SPI.

Added UTFT Geometry library for TRIANGLE, FILLED TRIANGLE, PIE and ARC shapes.  This library ONLY uses drawLine class commands in its algorithms.

NOTE: in the demo portion of the code, delays were introduced in the drawfillTriangle and drawPie loops.  Without the delays, shapes were not drawn completely.  This may be due to a large number of drawLine primitives overwhelming the PIC on the Digole display.

The demo code also includes a simple panel meter demo using only the drawArc command.

Feb 15,2014 - Add setScreenOn() and setScreenOff() functions
Feb 9,2014 - Added drawRoundRect() and fillRoundRect() primitives to digoleGeo.cpp and to demo code.



