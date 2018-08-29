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
#include <list>
#include <pugl/pugl.h>

#define DEBUG_MAIN 0
#define DEBUG_DRAW 0
#define DEBUG_EVENT 1
#define DEBUG_LAYOUT 0

#define COL_0 "\033[0m"
#define COL_BLACK "\033[22;30m"
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

#define UI_DEBUG_H COL_YELLOW "UI #%d " COL_0
#define WIDGET_DEBUG_H COL_PINK "Widget #%d " COL_0
#define CONTAINER_DEBUG_H COL_CYAN "Container #%d " COL_0

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
    BG_CONTAIN,
    BG_COVER,
    BG_STRETCH,
    BG_SCALE
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



typedef struct {
    uint32_t z;
    uint32_t i;
} ZDepth;

typedef struct {
    CTK::Widget *widget;
    int (*callback)(CTK::Widget*, void*);
    void *data;
} BasicEventMeta;

typedef struct : BasicEventMeta {
    int (*callback)(CTK::Widget*, const void*, void*);
    std::list<CTK::ZDepth> zDepth;
    unsigned int buttons;
    const void *drag;
    bool hover;
} GenericEventMeta;

typedef enum {
    CHAR_BACKSPACE = 0x08,
    CHAR_ESCAPE    = 0x1B,
    CHAR_DELETE    = 0x7F
} KeyChar;

typedef enum {
    MOD_SHIFT = 1,       /**< Shift key */
    MOD_CTRL  = 1 << 1,  /**< Control key */
    MOD_ALT   = 1 << 2,  /**< Alt/Option key */
    SUPER = 1 << 3   /**< Mod4/Command/Windows key */
} KeyMod;

typedef enum {
    KEY_F1 = 0xE000,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_INSERT,
    KEY_SHIFT,
    KEY_CTRL,
    KEY_ALT,
    KEY_SUPER
} Key;

typedef enum {
    /* generics */
    EVENT_NOTHING = 0,          /**< No event */
    EVENT_BUTTON_PRESS,         /**< Mouse button press */
    EVENT_BUTTON_RELEASE,       /**< Mouse button release */
    EVENT_CONFIGURE,            /**< View moved and/or resized */
    EVENT_EXPOSE,               /**< View exposed, redraw required */
    EVENT_CLOSE,                /**< Close view */
    EVENT_KEY_PRESS,            /**< Key press */
    EVENT_KEY_RELEASE,          /**< Key release */
    EVENT_ENTER_NOTIFY,         /**< Pointer entered view */
    EVENT_LEAVE_NOTIFY,         /**< Pointer left view */
    EVENT_MOTION_NOTIFY,        /**< Pointer motion */
    EVENT_SCROLL,               /**< Scroll */
    EVENT_FOCUS_IN,             /**< Keyboard focus entered view */
    EVENT_FOCUS_OUT,            /**< Keyboard focus left view */
    /* abstract */
    EVENT_CLICK,
    EVENT_HOVER,
    EVENT_DRAG_START,
    EVENT_DRAG,
    EVENT_DRAG_END,
    /* custom */
    EVENT_TOGGLED,
} EventType;
const int GENERIC_EVENT_TYPE_SIZE = 19;

struct EventAny : PuglEventAny { };
struct EventButton : PuglEventButton { };
struct EventConfigure : PuglEventConfigure { };
struct EventExpose : PuglEventExpose { };
struct EventClose : PuglEventClose { };
struct EventKey : PuglEventKey { };
struct EventCrossing : PuglEventCrossing { };
struct EventMotion : PuglEventMotion { };
struct EventScroll : PuglEventScroll { };
struct EventFocus : PuglEventFocus { };

typedef union {
    CTK::EventType      type;       /**< Event type. */
    CTK::EventAny       any;        /**< Valid for all event types. */
    CTK::EventButton    button;     /**< PUGL_BUTTON_PRESS, PUGL_BUTTON_RELEASE. */
    CTK::EventConfigure configure;  /**< PUGL_CONFIGURE. */
    CTK::EventExpose    expose;     /**< PUGL_EXPOSE. */
    CTK::EventClose     close;      /**< PUGL_CLOSE. */
    CTK::EventKey       key;        /**< PUGL_KEY_PRESS, PUGL_KEY_RELEASE. */
    CTK::EventCrossing  crossing;   /**< PUGL_ENTER_NOTIFY, PUGL_LEAVE_NOTIFY. */
    CTK::EventMotion    motion;     /**< PUGL_MOTION_NOTIFY. */
    CTK::EventScroll    scroll;     /**< PUGL_SCROLL. */
    CTK::EventFocus     focus;      /**< PUGL_FOCUS_IN, PUGL_FOCUS_OUT. */
} Event;

}; // CTK

#endif // TYPEDEF_HXX
