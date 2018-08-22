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

#ifndef CTK_CONTAINER_HXX
#define CTK_CONTAINER_HXX

#include "pugl/pugl.h"

#include <cairo/cairo.h>
#include <vector>

#include "widget.hxx"

namespace CTK {

class UI;

class Container : public CTK::Widget
{
public:
    const char* type = "container";
    
    Container (CTK::UI* ui);
    virtual ~Container ();
    
    std::vector<CTK::Widget*> children;
    
    virtual void add (CTK::Widget* w, int x = 0, int y = 0, int z = 0);
    virtual void remove (CTK::Widget* w);
    
    virtual void recalc ();
    virtual void rescale (float s);
    virtual void repos (int x, int y);
    virtual void expose ();
    
    virtual void calcDimensions();
};

};

#endif // CTK_CONTAINER_HXX
