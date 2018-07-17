/* Copyright [2013-2018] [Aaron Springstroh, Minimal Graphics Library]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef XWINDOW
#define XWINDOW

#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "glew.h"
#include "glxew.h"
#include "keyboard.h"

namespace min
{

bool check_gl_error();
void throw_gl_error();
GLboolean is_supported(const char *string);


class x_window
{
  public:
    // Virtual key codes for X11 platform
    using key_type = unsigned int;
    enum key_code : key_type
    {
        F1 = XK_F1,
        F2 = XK_F2,
        F3 = XK_F3,
        F4 = XK_F4,
        F5 = XK_F5,
        F6 = XK_F6,
        F7 = XK_F7,
        F8 = XK_F8,
        F9 = XK_F9,
        F10 = XK_F10,
        F11 = XK_F11,
        F12 = XK_F12,
        KEY0 = XK_0,
        KEY1 = XK_1,
        KEY2 = XK_2,
        KEY3 = XK_3,
        KEY4 = XK_4,
        KEY5 = XK_5,
        KEY6 = XK_6,
        KEY7 = XK_7,
        KEY8 = XK_8,
        KEY9 = XK_9,
        KEYA = XK_a,
        KEYB = XK_b,
        KEYC = XK_c,
        KEYD = XK_d,
        KEYE = XK_e,
        KEYF = XK_f,
        KEYG = XK_g,
        KEYH = XK_h,
        KEYI = XK_i,
        KEYJ = XK_j,
        KEYK = XK_k,
        KEYL = XK_l,
        KEYM = XK_m,
        KEYN = XK_n,
        KEYO = XK_o,
        KEYP = XK_p,
        KEYQ = XK_q,
        KEYR = XK_r,
        KEYS = XK_s,
        KEYT = XK_t,
        KEYU = XK_u,
        KEYV = XK_v,
        KEYW = XK_w,
        KEYX = XK_x,
        KEYY = XK_y,
        KEYZ = XK_z,
        NUM0 = XK_KP_0,
        NUM1 = XK_KP_1,
        NUM2 = XK_KP_2,
        NUM3 = XK_KP_3,
        NUM4 = XK_KP_4,
        NUM5 = XK_KP_5,
        NUM6 = XK_KP_6,
        NUM7 = XK_KP_7,
        NUM8 = XK_KP_8,
        NUM9 = XK_KP_9,
        LSHIFT = XK_Shift_L,
        RSHIFT = XK_Shift_R,
        LCONTROL = XK_Control_L,
        RCONTROL = XK_Control_R,
        TAB = XK_Tab,
        ENTER = XK_Return,
        BACKSPACE = XK_BackSpace,
        SPACE = XK_space,
        LALT = XK_Alt_L,
        RALT = XK_Alt_R,
        ESCAPE = XK_Escape,
        APOSTROPHE = XK_apostrophe,
        QUOTE = XK_quotedbl,
        COMMA = XK_comma,
        PERIOD = XK_period,
        SEMICOLON = XK_semicolon
    };

  private:
    uint_fast16_t _w;
    uint_fast16_t _h;
    int _major;
    int _minor;
    bool _shutdown;
    Display *_display;
    Window _root;
    Window _window;
    Colormap _color_map;
    GLXContext _context;
    Atom _shutdown_message;
    keyboard<key_type, double> _keyboard;

    // Callback functions
    void *_data;
    void (*_lclick_down)(void *, const uint_fast16_t width, const uint_fast16_t);
    void (*_lclick_up)(void *, const uint_fast16_t width, const uint_fast16_t);
    void (*_rclick_down)(void *, const uint_fast16_t width, const uint_fast16_t);
    void (*_rclick_up)(void *, const uint_fast16_t width, const uint_fast16_t);
    void (*_update)(void *, const uint_fast16_t width, const uint_fast16_t);

    // Window class string literal
    static constexpr const char *window_class = "minwl:x_window";

    void init_glew() const;
    void create_opengl_context(const std::string&, GLint, GLint);
    void create_window(const std::string&);
    void on_lclick_down(const uint_fast16_t, const uint_fast16_t) const;
    void on_lclick_up(const uint_fast16_t, const uint_fast16_t) const;
    void on_rclick_down(const uint_fast16_t, const uint_fast16_t) const;
    void on_rclick_up(const uint_fast16_t, const uint_fast16_t) const;
    void on_resize(const uint_fast16_t, const uint_fast16_t);
    void on_update(const uint_fast16_t, const uint_fast16_t) const;


  public:
    x_window(const std::string&, const uint_fast16_t, const uint_fast16_t, int, int);
    ~x_window();

    void display_cursor(const bool) const;
    void error_message(const std::string&) const;
    const char *get_context_string(const GLenum) const;
    std::pair<uint_fast16_t, uint_fast16_t> get_cursor() const;
    uint_fast16_t get_height() const;
    keyboard<key_type, double> &get_keyboard();
    const keyboard<key_type, double> &get_keyboard() const;
    bool get_shutdown() const;
    uint_fast16_t get_width() const;
    void maximize() const;
    void register_lclick_down(void(*)(void*, const uint_fast16_t, const uint_fast16_t));
    void register_lclick_up(void(*)(void*, const uint_fast16_t, const uint_fast16_t));
    void register_rclick_down(void(*)(void*, const uint_fast16_t, const uint_fast16_t));
    void register_rclick_up(void(*)(void*, const uint_fast16_t, const uint_fast16_t));
    void register_data(void*);
    void register_update(void(*)(void*, const uint_fast16_t, const uint_fast16_t));
    void resize(const uint_fast16_t, const uint_fast16_t) const;
    void set_cursor(const uint_fast16_t, const uint_fast16_t) const;
    void set_shutdown();
    void set_title(const std::string&);
    void swap_buffers() const;
    void update();

};
}

#endif
