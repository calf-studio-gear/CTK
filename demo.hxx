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

#ifndef CTK_DEMO_HXX
#define CTK_DEMO_HXX

#include "CTK/ctk.hxx"

//namespace CTK
//{
    //class Widget;
//};

class DemoUI : public CTK::UI
{
public:
    DemoUI(PuglNativeWindow parent = 0, const char* title = "CTK-Demo");
};

#endif // CTK_DEMO_HXX
