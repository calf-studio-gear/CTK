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

#include "pugl/pugl.h"

#include <algorithm>
#include <iostream>

#include "ui.hxx"

using namespace CTK;

UI::UI(PuglNativeWindow parent, const char* _title) : Container(this),
    title(_title)
{
    if (DEBUG) printf("%sConstruct UI #%d%s\n", COL_RED, id, COL_0);
    w = 1;
    h = 1;
    
    view = puglInit(NULL, NULL);

    if (parent)
        puglInitWindowParent(view, parent);
        
    puglInitWindowSize  (view, w, h);
    puglInitResizable   (view, false);
    puglInitContextType (view, PUGL_CAIRO);
    puglIgnoreKeyRepeat (view, true);
    puglSetEventFunc    (view, UI::onEvent);
    puglSetHandle       (view, this);
    puglCreateWindow    (view, title);
    puglShowWindow      (view);
    
    rescale(scale);
}

UI::~UI ()
{
    puglDestroy(view);
    queue.clear();
}

int UI::idle ()
{
    puglProcessEvents(view);
    return quit;
}

int UI::run ()
{
    redraw();
    while (!quit) {
        puglProcessEvents(view);
        usleep( 25000 );
    }
    return 0;
}

void UI::rescale (float s)
{
    if (DEBUG) printf(UI_DEBUG_H "rescale s:%.2f\n", id, s);
    Container::rescale(s);
    resizeWindow(view, ws, hs);
}

void UI::resize ()
{
    if (DEBUG) printf(UI_DEBUG_H "resize\n", id);
    Container::resize();
    resizeWindow(view, ws, hs);
}

void UI::recalc ()
{
    if (DEBUG) printf(UI_DEBUG_H "recalc\n", id);
    Container::recalc();
    resizeWindow(view, ws, hs);
}

void UI::expose ()
{
    if (DEBUG) printf(UI_DEBUG_H "expose x0:%d y0:%d x1:%d y1:%d\n", id, invalid.x0, invalid.y0, invalid.x1, invalid.y1);
    Container::expose();
    resetInvalid();
}

void UI::resizeWindow (PuglView* view, int w, int h)
{
    if (DEBUG) printf(UI_DEBUG_H "resizeWindow w:%d h:%d\n", id, w, h);
    puglInitWindowMinSize(view, w, h);
    puglInitWindowSize(view, w, h);
    puglConfigureWindow(view);
}

void UI::addToQueue (CTK::Widget* w)
{
    if (DEBUG) printf(COL_YELLOW "UI #%d" COL_0 " addToQueue id:%d\n", id, w->id);

    for (std::list<CTK::Widget*>::iterator it = queue.begin(); it != queue.end(); it++) {
        if (*it == w) {
            it = queue.erase(it);
            break;
        }
    }
    queue.push_back(w);
    if (w->parent) {
        Area a;
        a.x0 = w->xs + w->invalid.x0;
        a.y0 = w->ys + w->invalid.y0;
        a.x1 = w->xs + w->invalid.x1;
        a.y1 = w->ys + w->invalid.y1;
        w->parent->invalidate(&a);
    }
}

void UI::requestExpose(CTK::Widget* w)
{
    if (DEBUG) printf(UI_DEBUG_H "requestExpose id:%d\n", id, w->id);
    addToQueue(w);
    if (view)
        puglPostRedisplay(view);
}

void UI::handleExpose(const PuglEventExpose event)
{
    if (DEBUG) printf(UI_DEBUG_H COL_YELLOW "handleExpose" COL_0 " x:%d y:%d w:%d h:%d\n", id, (int)event.x, (int)event.y, (int)event.width, (int)event.height);
    
    if (DEBUG) printf("    IDs:");
    for (std::list<CTK::Widget*>::iterator i = queue.begin(); i != queue.end(); i++)
        if (DEBUG) std::cout << ' ' << (*i)->id;
    if (DEBUG) printf("\n");
    
    cairo_t* cr = (cairo_t*)puglGetContext(event.view);
    
    for (std::list<CTK::Widget*>::iterator i = queue.begin(); i != queue.end();) {
        (*i)->expose();
        i = queue.erase(i);
    }
    
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    
}

void UI::handleConfigure (const PuglEventConfigure event)
{
    //printf(COL_YELLOW "UI #%d" COL_0 " handleConfigure %.2f %.2f %.2f %.2f\n", id, event.x, event.y, event.width, event.height);
    //if ((int)event.width != w or (int)event.height != h) {
        //w = (int)event.width;
        //h = (int)event.height;
        //resize();
    //}
}

bool hitTest (CTK::Widget *w, int x, int y) {
    return x > w->xsa and x < w->xsa + w->ws and y > w->ysa and y < w->ysa + w->hs;
}

void UI::handleEvent (const CTK::Event* event)
{
    int ret;
    internalEvent(event);
    std::list<CTK::EventMeta*> items;
    std::list<CTK::EventMeta*>::iterator i;
    CTK::EventMeta* meta;
    
    switch (event->type) {
        case CTK::EVENT_BUTTON_PRESS: {
            int bmask = (1 << event->button.button-1);
            /* handle button press subscriptions */
            items = events[CTK::EVENT_BUTTON_PRESS];
            i = items.begin();
            while (i != items.end()) {
                meta = *i;
                if (hitTest(meta->widget, event->button.x, event->button.y)) {
                    meta->callback(meta->widget, &event->button, meta->data);
                    meta->buttons | bmask;
                }
                i++;
            }
            /* handle click event subscriptions */
            items = events[CTK::EVENT_CLICK];
            i = items.begin();
            while (i != items.end()) {
                meta = *i;
                if (hitTest(meta->widget, event->button.x, event->button.y)) {
                    meta->buttons |= bmask;
                }
                i++;
            }
            /* handle drag event subscriptions */
            if (event->button.button == 1) {
                items = events[CTK::EVENT_DRAG];
                i = items.begin();
                while (i != items.end()) {
                    meta = *i;
                    if (hitTest(meta->widget, event->button.x, event->button.y)) {
                        meta->buttons |= bmask;
                    }
                    i++;
                }
            }
            /* handle drag start event subscriptions */
            if (event->button.button == 1) {
                items = events[CTK::EVENT_DRAG_START];
                i = items.begin();
                while (i != items.end()) {
                    meta = *i;
                    if (hitTest(meta->widget, event->button.x, event->button.y)) {
                        meta->buttons |= bmask;
                    }
                    i++;
                }
            }
            /* handle drag start event subscriptions */
            if (event->button.button == 1) {
                items = events[CTK::EVENT_DRAG_END];
                i = items.begin();
                while (i != items.end()) {
                    meta = *i;
                    if (hitTest(meta->widget, event->button.x, event->button.y)) {
                        meta->buttons |= bmask;
                    }
                    i++;
                }
            }
        } break;
        case CTK::EVENT_BUTTON_RELEASE: {
            int bmask = (1 << event->button.button-1);
            /* handle button release subscriptions */
            items = events[CTK::EVENT_BUTTON_RELEASE];
            i = items.begin();
            while (i != items.end()) {
                meta = *i;
                if (hitTest(meta->widget, event->button.x, event->button.y)) {
                    meta->callback(meta->widget, &event->button, meta->data);
                }
                meta->buttons &= ~bmask;
                i++;
            }
            /* reset/handle drag event subscriptions */
            if (event->button.button == 1) {
                items = events[CTK::EVENT_DRAG_START];
                i = items.begin();
                while (i != items.end()) {
                    meta = *i;
                    meta->drag = NULL;
                    meta->buttons &= ~bmask;
                    i++;
                }
                items = events[CTK::EVENT_DRAG];
                i = items.begin();
                while (i != items.end()) {
                    meta = *i;
                    meta->drag = NULL;
                    meta->buttons &= ~bmask;
                    i++;
                }
                items = events[CTK::EVENT_DRAG_END];
                i = items.begin();
                while (i != items.end()) {
                    meta = *i;
                    if (meta->buttons & bmask and meta->drag) {
                        meta->callback(meta->widget, &event->button, meta->data);
                        meta->drag = NULL;
                    }
                    meta->buttons &= ~bmask;
                    i++;
                }
            }
            /* handle click event subscriptions */
            items = events[CTK::EVENT_CLICK];
            i = items.begin();
            while (i != items.end()) {
                meta = *i;
                if (hitTest(meta->widget, event->button.x, event->button.y)
                and meta->buttons & bmask) {
                    meta->callback(meta->widget, &event->button, meta->data);
                }
                meta->buttons &= ~bmask;
                i++;
            }
        } break;
        case CTK::EVENT_MOTION_NOTIFY: {
            /* handle drag start subscriptions */
            items = events[CTK::EVENT_DRAG_START];
            i = items.begin();
            while (i != items.end()) {
                meta = *i;
                if (meta->buttons & 1 and !meta->drag) {
                    meta->drag = &event->motion;
                    meta->callback(meta->widget, &event->motion, meta->data);
                }
                i++;
            }
            /* handle drag subscriptions */
            items = events[CTK::EVENT_DRAG];
            i = items.begin();
            while (i != items.end()) {
                meta = *i;
                if (meta->buttons & 1) {
                    meta->callback(meta->widget, &event->motion, meta->data);
                }
                i++;
            }
            /* handle drag end subscriptions */
            items = events[CTK::EVENT_DRAG_END];
            i = items.begin();
            while (i != items.end()) {
                meta = *i;
                if (meta->buttons & 1 and !meta->drag) {
                    meta->drag = &event->motion;
                }
                i++;
            }
        } break;
        case CTK::EVENT_SCROLL: {
            /* handle scroll subscriptions */
            items = events[CTK::EVENT_SCROLL];
            i = items.begin();
            while (i != items.end()) {
                meta = *i;
                if (hitTest(meta->widget, event->scroll.x, event->scroll.y)) {
                    meta->callback(meta->widget, &event->scroll, meta->data);
                }
                i++;
            }
        } break;
        case CTK::EVENT_EXPOSE: {
            handleExpose(event->expose);
        } break;
        case CTK::EVENT_CONFIGURE: {
            handleConfigure(event->configure);
        } break;
        case CTK::EVENT_CLOSE: {
            close();
        } break;
    }
}

void UI::internalEvent (const CTK::Event* event)
{
    if (event->type != CTK::EVENT_KEY_PRESS)
        return;
        
    if (event->key.character == 'q' ||
        event->key.character == 'Q' ||
        event->key.character == CTK::CHAR_ESCAPE) {
            quit = true;
    }
}

void UI::addEvent (CTK::Widget *widget, CTK::EventType type, int (*callback)(CTK::Widget*, const void*, void*), void *data)
{
    int t = (int)type;
    if (DEBUG) printf(UI_DEBUG_H "addEvent id:%d type:%d\n", id, widget->id, t);
    CTK::EventMeta *em = new CTK::EventMeta();
    em->widget = widget;
    em->callback = callback;
    em->data = data;
    em->buttons = 0;
    em->drag = NULL;
    events[t].push_back(em);
}

void UI::removeEvent (CTK::Widget *widget, CTK::EventType type, int (*callback)(CTK::Widget*, const void*, void*))
{
    int t = (int)type;
    if (DEBUG) printf(UI_DEBUG_H "removeEvent id:%d type:%d\n", id, widget->id, t);
    std::list<CTK::EventMeta*> items = events[t];
    std::list<CTK::EventMeta*>::iterator i = items.begin();
    while (i != items.end()) {
        CTK::EventMeta *em = *i;
        if (em->widget == widget and em->callback == callback) {
            delete em;
            i = items.erase(i);
        } else {
            ++i;
        }
    }
}
