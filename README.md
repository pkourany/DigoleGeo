DigoleGeo
=========

Digole Serial Display Library with Graphic Primitives Extensions

by Paul Kourany (peekay123), Feb 4, 2016

Digole Digital Solutions: www.digole.com

Digole display library for Particle devices, extended to include extra geometry primitives:

`drawRoundRect(x1, y1, x2, y2);`
`fillRoundRect(x1, y1, x2, y2);`
`drawTriangle(x1, y1, x2, y2, x3, y3);`
`fillTriangle(x1, y1, x2, y2, x3, y3);`
`drawArc(x, y, r, startAngle, endAngle, thickness);`
`drawPie(x, y, r, startAngle, endAngle);`
`drawEllipse(CX, CY, XRadius, YRadius);`
`drawFilledEllipse(CX, CY, XRadius, YRadius);`

As well as, display geometry functions:

`setNativeDisplaySize(int w, int h);`
`getNativeDisplayWidth(void);`
`getNativeDisplayHeight(void);`

The demo is set for hardware UART (Serial).

The geometry extensions ONLY use the drawLine and drawPixel commands in its algorithms.

NOTE
----

In the primitives functions, delays were introduced to allow draw operations to complete prior to sending new ones. Without the delays, shapes were not drawn completely.  This may be due to a large number of drawLine primitives overwhelming the PIC on the Digole display.

The demo code also includes a simple panel meter demo using only the drawArc command.

Feb 4, 2016 - update to latest Digole library supporting new larger touch displays
