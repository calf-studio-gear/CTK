# CTK

![CTK Logo](CTK.png "CTK Logo") 

CTK is a user interface toolkit for host-agnostic audio plugin UIs written
in C++ under LGPL 3.0 license.

It comes with a slightly modified pugl library for event and window handling
and utilizes cairo graphics library for drawing.

## Rendering

CTK has optimized rendering and compositing and full nesting and stacking
capabilities. All widgets own a cairo surface they are drawing on. Container
widgets take care of compositing their children onto the surface. All drawing
is done on the display managers event call while other calculations are done
whenever they appear. Drawing and compositing is limited to the parts/pixels
of the UI which really updated.

CTK can handle high resolution screens.

## Events

CTK offers callback-based event handling per widget.

Generic events are handled by the UI in dedicated queues which fire the
events in correct order of the widgets visibility (z value and order
inside its container).
Some abstract events like click, dragging and hovering are available.
The widget class offers mechanisms for adding, removing and firing
custom events.

```
int clicktest (CTK::Widget *widget, const void *event, void *data) {
    printf("clicked id:%d\n", widget->id);
    return 0;
};
myWidget->addEvent(CTK::EVENT_CLICK, &clicktest);

int customtest (CTK::Widget *widget, void *data) {
    printf("fired id:%d\n", widget->id);
    return 0;
};
myWidget->addEvent(1024, &customtest);
myWidget->fireEvent(1024);
```

## Resources

CTK is fully self-contained without using external files. Resource files
like style definitions, background images or XML layout files can be
linked into the final executable via cmake build process.
