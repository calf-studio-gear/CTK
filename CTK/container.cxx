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

#include <algorithm>

#include "container.hxx"
#include "ui.hxx"

using namespace CTK;

Container::Container (CTK::UI* ui) : CTK::Widget (ui)
{
    if (DEBUG) printf("%sConstruct Container #%d%s\n", COL_RED, id, COL_0);
}

Container::~Container ()
{
    children.clear();
}

void Container::expose ()
{
    if (DEBUG) printf(CONTAINER_DEBUG_H "expose x0:%d y0:%d x1:%d y1:%d\n", id, invalid.x0, invalid.y0, invalid.x1, invalid.y1);
    
    Widget::expose();
    
    cairo_t* cr = cairo_create(surface);
    setInvalidClip(cr);
    
    for (unsigned int i = 0; i < children.size(); i++) {
        CTK::Widget* w = children[i];
        if (DEBUG) printf("    testing #%d - xs:%d ys:%d ws:%d hs:%d\n", w->id, w->xs, w->ys, w->ws, w->hs); 
        if (w->xs <= invalid.x0 + invalid.x1 and
            w->ys <= invalid.y0 + invalid.y1 and
            w->xs + w->ws >= invalid.x0 and
            w->ys + w->hs >= invalid.y0
        ) {
            if (DEBUG) printf("    drawing #%d - x0:%d y0:%d x1:%d y1:%d\n", w->id, w->invalid.x0, w->invalid.y0, w->invalid.x1, w->invalid.y1);
            cairo_rectangle(cr, w->xs, w->ys, w->ws, w->hs);
            cairo_set_source_surface(cr, w->surface, w->xs, w->ys);
            cairo_paint(cr);
        }
        w->resetInvalid();
    }
    
    cairo_destroy(cr);
}

void Container::repos (int x, int y)
{
    if (DEBUG) printf(CONTAINER_DEBUG_H "repos x:%d y:%d\n", id, x, y);
    Widget::repos(x, y);
    for (unsigned int i = 0; i < children.size(); i++)
        children[i]->calcDimensions();
}

void Container::rescale (float s)
{
    if (DEBUG) printf(CONTAINER_DEBUG_H "rescale s:%.2f\n", id, s);
    for (unsigned int i = 0; i < children.size(); i++)
        children[i]->rescale(s);
    Widget::rescale(s);
}

void Container::recalc ()
{
    if (DEBUG) printf(CONTAINER_DEBUG_H "recalc\n", id);
    int w_ = 0;
    int h_ = 0;
    for (unsigned int i = 0; i < children.size(); i++) {
        CTK::Widget* w = children[i];
        w_ = std::max(w_, w->w + w->x);
        h_ = std::max(h_, w->h + w->y);
    }
    if (w_ != w or h_ != h)
        resize(w_, h_);
        
    if (DEBUG) printf("    w:%d h:%d\n", w_, h_);
    
    if (parent) {
        printf("    pid:%d\n", parent->id);
        parent->recalc();
    }
}

void Container::calcDimensions()
{
    if (DEBUG) printf(CONTAINER_DEBUG_H "calcDimensions\n", id);
    Widget::calcDimensions();
    for (unsigned int i = 0; i < children.size(); i++)
        children[i]->calcDimensions();
}


void Container::add (CTK::Widget* w, int x, int y, int z)
{
    if (DEBUG) printf(CONTAINER_DEBUG_H "add x:%d y:%d z:%d\n", id, x, y, z);
    w->z = z;
    w->rescale(scale);
    w->repos(x, y);
    remove(w);
    bool in = false;
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->z > z) {
            children.insert(children.begin() + i, w);
            in = true;
            break;
        }
    }
    if (!in)
        children.push_back(w);
    w->parent = this;
    
    recalc();
}

void Container::remove (CTK::Widget* w)
{
    if (DEBUG) printf(CONTAINER_DEBUG_H "remove id:%d\n", w->id);
    for (int i = 0; i < children.size(); i++) {
        if (children.at(i) == w) {
            if (w->parent)
                w->parent = 0;
            children.erase(children.begin() + i);
            break;
        }
    }
    recalc();
}
