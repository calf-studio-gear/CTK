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

#ifndef CTK_WIDGET_HXX
#define CTK_WIDGET_HXX

#include "pugl/pugl.h"

#include <cairo/cairo.h>
#include <math.h>
#include <stdlib.h>
#include <string>

#include "typedef.hxx"

namespace CTK {
    
class UI;
class Container;

class Widget
{
public:
    const char* type = "widget";
    
    int x, xs, xa, xsa;
    int y, ys, ya, ysa;
    int w, ws;
    int h, hs;
    int z;
    int id;
    float scale;
    bool visible;
    std::string role;
    
    CTK::Container* parent;
    CTK::UI* ui;
    cairo_surface_t* surface;
    CTK::Area invalid;
    
    Widget (CTK::UI* ui);
    virtual ~Widget();

    virtual void redraw ();
    virtual void resize ();
    virtual void resize (int _w, int _h);
    virtual void rescale (float s);
    virtual void repos (int _x, int _y);
    
    virtual void expose ();
    
    virtual int event (const PuglEvent* event);
    
    void calcDimensions();
    void setInvalidClip (cairo_t* cr);
    void addInvalid (CTK::Area* a);
    void resetInvalid ();
    void expandInvalid ();
};

};

#endif // CTK_WIDGET_HXX
