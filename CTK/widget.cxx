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

#include <cairo/cairo.h>
#include <ctime>

#include "typedef.hxx"
#include "widget.hxx"
#include "ui.hxx"

using namespace CTK;

int counter = 0;

void joinAreas (CTK::Area* a1, CTK::Area* a2)
{
    if (a1->x0 < 0) a1->x0 = a2->x0; else a1->x0 = std::min(a1->x0, a2->x0);
    if (a1->y0 < 0) a1->y0 = a2->y0; else a1->y0 = std::min(a1->y0, a2->y0);
    if (a1->x1 < 0) a1->x1 = a2->x1; else a1->x1 = std::max(a1->x1, a2->x1);
    if (a1->y1 < 0) a1->y1 = a2->y1; else a1->y1 = std::max(a1->y1, a2->y1);
}

Widget::Widget (CTK::UI* _ui) :
    ui (_ui),
    x(0), xs(0), xa(0), xsa(0),
    y(0), ys(0), ya(0), ysa(0),
    w(0), h(0), ws(0), hs(0), z(0),
    visible(true),
    scale(1.0),
    surface(0),
    parent(0),
    id(counter++),
    invalid({-1,-1,-1,-1})
{
    if (DEBUG) printf("%sConstruct Widget #%d%s\n", COL_RED, id, COL_0);
    resetInvalid();
    
    if (id)
        resize();
}

Widget::~Widget ()
{
    if (surface) cairo_surface_destroy(surface);
}

int Widget::event (const PuglEvent* event)
{
    return 0;
}

void Widget::resize ()
{
    if (DEBUG) printf(WIDGET_DEBUG_H " resize\n", id);
    
    addInvalidToParent();
    calcDimensions();
    addInvalidToParent();
    
    if (surface) cairo_surface_destroy(surface);
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ws, hs);
    
    expandInvalid();
    redraw();
}
void Widget::resize (int _w, int _h)
{
    if (DEBUG) printf(WIDGET_DEBUG_H " resize w:%d h:%d\n", id, _w, _h);
    w = _w;
    h = _h;
    
    resize();
}

void Widget::rescale (float s)
{
    if (DEBUG) printf(WIDGET_DEBUG_H " rescale s:%.2f\n", id, s);
    scale = s;
    
    resize();
}

void Widget::repos (int _x, int _y)
{
    if (DEBUG) printf(WIDGET_DEBUG_H " repos x:%d y:%d\n", id, _x, _y);
    x = _x;
    y = _y;
    addInvalidToParent();
    calcDimensions();
    addInvalidToParent();
}

void Widget::redraw ()
{
    if (DEBUG) printf(WIDGET_DEBUG_H " redraw x0:%d y0:%d x1:%d y1:%d\n", id, invalid.x0, invalid.y0, invalid.x1, invalid.y1);
    ui->requestExpose(this);
}

void Widget::expose ()
{
    if (DEBUG) printf(WIDGET_DEBUG_H " expose x0:%d y0:%d x1:%d y1:%d\n", id, invalid.x0, invalid.y0, invalid.x1, invalid.y1);
    
    cairo_t* cr = cairo_create(surface);
    setInvalidClip(cr);
    
    cairo_save(cr);
    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cr);
    cairo_restore(cr);
    
    // real drawing goes here
    cairo_rectangle(cr, 0, 0, ws, hs);
    std::srand(std::time(nullptr) + id);
    float r = (std::rand() % 100) / 100.;
    float g = (std::rand() % 100) / 100.;
    float b = (std::rand() % 100) / 100.;
    cairo_set_source_rgb(cr, r, g, b);
    cairo_fill(cr);
    // end of drawing
    
    cairo_destroy(cr);
}

void Widget::invalidate (CTK::Area* a)
{
    addInvalid(a);
    redraw();
}

void Widget::calcDimensions ()
{
    xs = (int)ceil((float)x * scale);
    ys = (int)ceil((float)y * scale);
    ws = (int)ceil((float)w * scale);
    hs = (int)ceil((float)h * scale);
    
    Widget* p = this;
    xa = 0;
    ya = 0;
    while (p) {
        xa += p->x;
        ya += p->y;
        p = p->parent;
    }
    
    xsa = (int)ceil((float)xa * scale);
    ysa = (int)ceil((float)ya * scale);
    if (DEBUG) printf(WIDGET_DEBUG_H " calcDimensions\n", id);
    
    if (DEBUG) printf("    scale:%.2f\n", scale);
    if (DEBUG) printf("    x:%d y:%d w:%d h:%d\n", x, y, w, h);
    if (DEBUG) printf("    ws:%d hs:%d\n", ws, hs);
    if (DEBUG) printf("    xs:%d ys:%d xa:%d ya:%d xsa:%d ysa:%d\n", xs, ys, xa, ya, xsa, ysa);
}

void Widget::setInvalidClip (cairo_t* cr)
{
    int x0 = std::max(invalid.x0, 0);
    int y0 = std::max(invalid.y0, 0);
    int x1 = std::min(invalid.x1, ws);
    int y1 = std::min(invalid.y1, hs);
    if (DEBUG) printf (WIDGET_DEBUG_H " setInvalidClip x0:%d y0:%d x1:%d y1:%d\n", id, x0, y0, x1, y1);
    cairo_rectangle(cr, x0, y0, x1-x0, y1-y0);
    cairo_clip(cr);
}

void Widget::resetInvalid ()
{
    if (DEBUG) printf(WIDGET_DEBUG_H " resetInvalid\n", id);
    invalid.x0 = -1;
    invalid.y0 = -1;
    invalid.x1 = -1;
    invalid.y1 = -1;
}
void Widget::expandInvalid ()
{
    if (DEBUG) printf(WIDGET_DEBUG_H " expandInvalid\n", id);
    invalid.x0 = 0;
    invalid.y0 = 0;
    invalid.x1 = ws;
    invalid.y1 = hs;
}

void Widget::addInvalid (CTK::Area* a)
{
    if (DEBUG) printf(WIDGET_DEBUG_H " addInvalid\n", id);
    joinAreas(&invalid, a);
}

void Widget::addInvalidToParent ()
{
    if (!parent) return;
    CTK::Area a;
    a.x0 = xs;
    a.y0 = ys;
    a.x1 = xs + ws;
    a.y1 = ys + hs;
    parent->addInvalid(&a);
}
