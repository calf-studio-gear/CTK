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
    
    rescale(1.5);
    //rescale(5);
}
