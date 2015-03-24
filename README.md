DigoleGeo
=========

Digole display library for Spark Core extended to include extra geometry primitives.

The demo is set for hardware UART (Serial).

Added geometry extensions for TRIANGLE, FILLED TRIANGLE, PIE, FILLED PIE and ARC shapes.  The geometry extensions ONLY use the drawLine class commands in its algorithms.

NOTE
----

In the demo portion of the code, delays were introduced in the drawfillTriangle and drawPie loops.  Without the delays, shapes were not drawn completely.  This may be due to a large number of drawLine primitives overwhelming the PIC on the Digole display.

The demo code also includes a simple panel meter demo using only the drawArc command.

Feb 15,2014 - Add setScreenOn() and setScreenOff() functions.

Feb 9,2014 - Added drawRoundRect() and fillRoundRect() primitives to digoleGeo.cpp and to demo code.



