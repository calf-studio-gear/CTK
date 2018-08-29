/*
 * Copyright(c) 2018, Markus Schmidt <schmidt@boomshop.net>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3.0 of the
 * License, or (at your option) any later version.
 *
 * CTK is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CTK; if not, see <http://www.gnu.org/licenses/>.
 */

#include "demo.hxx"

#include <algorithm>

int clicktest (CTK::Widget *widget, const void *event, void *data) {
    CTK::EventButton *ev = (CTK::EventButton*)event;
    printf("clicked id:%d x:%d y:%d button:%d\n",
        widget->id,
        (int)ev->x,
        (int)ev->y,
        (int)ev->button
    );
    return 0;
};

int presstest (CTK::Widget *widget, const void *event, void *data) {
    CTK::EventButton *ev = (CTK::EventButton*)event;
    printf("pressed id:%d x:%d y:%d button:%d\n",
        widget->id,
        (int)ev->x,
        (int)ev->y,
        (int)ev->button
    );
    return 0;
};

int releasetest (CTK::Widget *widget, const void *event, void *data) {
    CTK::EventButton *ev = (CTK::EventButton*)event;
    printf("released id:%d x:%d y:%d button:%d\n",
        widget->id,
        (int)ev->x,
        (int)ev->y,
        (int)ev->button
    );
    return 0;
};

int dragstarttest (CTK::Widget *widget, const void *event, void *data) {
    CTK::EventMotion *ev = (CTK::EventMotion*)event;
    printf("dragstart id:%d x:%d y:%d\n",
        widget->id,
        (int)ev->x,
        (int)ev->y
    );
    return 0;
};

int dragtest (CTK::Widget *widget, const void *event, void *data) {
    CTK::EventMotion *ev = (CTK::EventMotion*)event;
    printf("dragging id:%d x:%d y:%d\n",
        widget->id,
        (int)ev->x,
        (int)ev->y
    );
    return 0;
};

int scrolltest (CTK::Widget *widget, const void *event, void *data) {
    CTK::EventScroll *ev = (CTK::EventScroll*)event;
    printf("scrolling id:%d x:%d y:%d dx:%d dy:%d\n",
        widget->id,
        (int)ev->x,
        (int)ev->y,
        (int)ev->dx,
        (int)ev->dy
    );
    return 0;
};

int dragendtest (CTK::Widget *widget, const void *event, void *data) {
    CTK::EventButton *ev = (CTK::EventButton*)event;
    printf("dragend id:%d x:%d y:%d\n",
        widget->id,
        (int)ev->x,
        (int)ev->y
    );
    return 0;
};

DemoUI::DemoUI (PuglNativeWindow parent, const char* _title) : CTK::UI (parent, title)
{
    title = _title;
    
    // id 1
    CTK::Widget* box3 = new CTK::Widget(this);
    box3->resize(50, 10);
    this->add(box3, 10, 40, 2);
    
    // id 2
    CTK::Container* con1 = new CTK::Container(this);
    this->add(con1, 30, 30, 1);
    
    // id 3
    CTK::Widget* box1 = new CTK::Widget(this);
    box1->resize(30, 40);
    con1->add(box1, 30, 40, 1);
    
    // id 4
    CTK::Widget* box2 = new CTK::Widget(this);
    box2->resize(40, 80);
    con1->add(box2, 10, 20, 0);
    
    rescale(2.5);
    
    box3->repos(20,20);
    
    box2->addEvent(CTK::EVENT_CLICK, &clicktest);
    box2->addEvent(CTK::EVENT_BUTTON_PRESS, &presstest);
    box2->addEvent(CTK::EVENT_BUTTON_RELEASE, &releasetest);
    box2->addEvent(CTK::EVENT_DRAG_START, &dragstarttest);
    box2->addEvent(CTK::EVENT_DRAG, &dragtest);
    box2->addEvent(CTK::EVENT_DRAG_END, &dragendtest);
    box2->addEvent(CTK::EVENT_SCROLL, &scrolltest);
    
    box3->addEvent(CTK::EVENT_CLICK, &clicktest);
    box1->addEvent(CTK::EVENT_CLICK, &clicktest);
}
