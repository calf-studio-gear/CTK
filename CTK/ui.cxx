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
}

void UI::rescale (float s)
{
    if (DEBUG) printf(UI_DEBUG_H " rescale s:%.2f\n", id, s);
    Container::rescale(s);
    resizeWindow(view, ws, hs);
}

void UI::resize ()
{
    if (DEBUG) printf(UI_DEBUG_H " resize\n", id);
    Container::resize();
    resizeWindow(view, ws, hs);
}

void UI::recalc ()
{
    if (DEBUG) printf(UI_DEBUG_H " recalc\n", id);
    Container::recalc();
    resizeWindow(view, ws, hs);
}

void UI::expose ()
{
    if (DEBUG) printf(UI_DEBUG_H " expose x0:%d y0:%d x1:%d y1:%d\n", id, invalid.x0, invalid.y0, invalid.x1, invalid.y1);
    Container::expose();
    resetInvalid();
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
    if (DEBUG) printf(UI_DEBUG_H " requestExpose id:%d\n", id, w->id);
    addToQueue(w);
    if (view)
        puglPostRedisplay(view);
}

void UI::handleExpose(const PuglEventExpose event)
{
    if (DEBUG) printf(UI_DEBUG_H COL_YELLOW " handleExpose" COL_0 " x:%d y:%d w:%d h:%d\n", id, (int)event.x, (int)event.y, (int)event.width, (int)event.height);
    
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

void UI::handleEvent (const PuglEvent* event)
{
    int ret;
    switch (event->type) {
        default:
            ret = Container::event(event);
            if (ret) {
                Container::redraw();
                return;
            }
            internalEvent(event);
            break;
        case PUGL_EXPOSE:
            handleExpose(event->expose);
            break;
        case PUGL_CONFIGURE:
            handleConfigure(event->configure);
            break;
        case PUGL_CLOSE:
            close();
            break;
    }
}

void UI::internalEvent (const PuglEvent* event)
{
    if (event->type != PUGL_KEY_PRESS)
        return;
        
    if (event->key.character == 'q' ||
        event->key.character == 'Q' ||
        event->key.character == PUGL_CHAR_ESCAPE) {
            quit = true;
    }
}

void UI::resizeWindow (PuglView* view, int w, int h)
{
    if (DEBUG) printf(UI_DEBUG_H " resizeWindow w:%d h:%d\n", id, w, h);
    puglInitWindowMinSize(view, w, h);
    puglInitWindowSize(view, w, h);
    puglConfigureWindow(view);
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
