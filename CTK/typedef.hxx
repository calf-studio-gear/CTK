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

#ifndef TYPEDEF_HXX
#define TYPEDEF_HXX

#include <string>

#define COL_0 "\033[0m"
#define COL_BLACK "\033[22;30m"k
#define COL_RED "\033[22;31m"
#define COL_GREEN "\033[22;32m"
#define COL_BROWN "\033[22;33m"
#define COL_BLUE "\033[22;34m"
#define COL_PURPLE "\033[22;35m"
#define COL_PETROL "\033[22;36m"
#define COL_GRAY "\033[22;37m"
#define COL_COAL "\033[01;30m"
#define COL_LRED "\033[01;31m"
#define COL_LGREEN "\033[01;32m"
#define COL_YELLOW "\033[01;33m"
#define COL_LBLUE "\033[01;34m"
#define COL_PINK "\033[01;35m"
#define COL_CYAN "\033[01;36m"
#define COL_WHITE "\033[01;37m"

#define UI_DEBUG_H COL_YELLOW "UI #%d" COL_0
#define WIDGET_DEBUG_H COL_PINK "Widget #%d" COL_0
#define CONTAINER_DEBUG_H COL_CYAN "Container #%d" COL_0

#define DEBUG 1

namespace CTK {

class Widget;

typedef struct
{
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct
{
    int x0;
    int y0;
    int x1;
    int y1;
} Area;

typedef struct
{
    int R;
    int G;
    int B;
    float alpha;
} Color;

typedef enum
{
    CTK_CONTAIN,
    CTK_COVER,
    CTK_STRETCH,
    CTK_SCALE
} ImageSizing;

typedef struct
{
    float m_top;
    float m_bottom;
    float m_left;
    float m_right;
    
    float  b_top;
    float  b_bottom;
    float  b_left;
    float  b_right;
    Color* b_color;
    
    Color*           bg_color;
    std::string      bg_image;
    Rect*            bg_dimensions;
    bool             bg_repeat;
    CTK::ImageSizing bg_sizing;
} Style;

}; // CTK

#endif // TYPEDEF_HXX
