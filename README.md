# CTK

![CTK Logo](CTK.png "CTK Logo") 

CTK is a user interface toolkit for host-agnostic audio plugin UIs written
in C++ under LGPL 3.0 license.

It comes with a slightly modified pugl library for event and window handling
and utilizes cairo graphics library for drawing.

## Events

CTK offers callback-based event handling per widget.

Generic events are handled by the UI in dedicated queues which fire the
events in correct order of the visibility (z value and order inside a container).
Some abstract events like click, dragging and hovering are available.
The widget class handles adding, removing and firing of custom events.

## Rendering

CTK has optimized rendering and compositing and full nesting and stacking
capabilities. All widgets own a cairo surface they are drawing on. Container
widgets take care of compositing their children onto the surface. All drawing
is done on the display managers event call while other calculations are done
whenever they appear. Drawing and compositing is limited to the parts/pixels
of the UI which really updated.

CTK can handle high resolution screens.
