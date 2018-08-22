# CTK

CTK is a user interface toolkit for host-agnostic audio plugin UIs written
in C++ under LGPL 3.0 license.

It comes with a slightly modified pugl library for event and window handling
and utilizes cairo graphics library for drawing.

CTK offers optimized rendering and compositing and full nesting and stacking
capabilities. All widgets own a cairo surface they are drawing on. Container
widgets take care of compositing their children onto the surface. All drawing
is done on the display managers event call while other calculations are done
whenever they appear. Drawing and compositing is limited to the parts/pixels
of the UI which really updated.

CTK can handle high resolution screens.
