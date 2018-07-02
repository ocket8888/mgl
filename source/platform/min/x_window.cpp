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

#include "x_window.h"

bool min::check_gl_error()
{
    const GLenum error = glGetError();

    // Check for error and print to stdout
    const bool ret = error != GL_NO_ERROR;
    if (ret)
    {
        std::cout << "OpenGL Error: " << std::to_string(error) << std::endl;
    }

    return ret;
}

void min::throw_gl_error()
{
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        throw std::runtime_error("GL ERROR! glGetError(): " + std::to_string(error));
    }
}

GLboolean min::is_supported(const char *string)
{
    return glxewIsSupported(string);
}


//// x_window ////
void min::x_window::init_glew() const
{
    // Create opengl attributes, 32 bit color, 24 bit depth, 8 bit stencil, double buffering
    GLint attr[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        //GLX_STENCIL_SIZE, 8,
        None};

    // Create visual that fulfills attributes requested
    XVisualInfo *visual = glXChooseVisual(_display, 0, attr);
    if (visual == nullptr)
    {
        throw std::runtime_error("x_window: Could not create X11 visual from requested attributes.");
    }

    // Create GL context
    GLXContext context = glXCreateContext(_display, visual, 0, GL_TRUE);
    if (context == nullptr)
    {
        throw std::runtime_error("x_window: Failed creating a dummy opengl context.");
    }

    // Create color map for the temp window
    Colormap color_map = XCreateColormap(_display, _root, visual->visual, AllocNone);

    // Set the temp window attributes, color map only
    XSetWindowAttributes window_attr;
    window_attr.colormap = color_map;

    // Connects the color map to the temp window
    Window temp_window = XCreateWindow(_display, _root, 0, 0, _w, _h, 0, visual->depth, InputOutput,
                                       visual->visual, CWColormap, &window_attr);

    if (!temp_window)
    {
        throw std::runtime_error("x_window: Failed to create an X11 window.");
    }

    // Make the context curent
    if (!glXMakeCurrent(_display, temp_window, context))
    {
        throw std::runtime_error("x_window: Could not bind dummy rendering context");
    }

    // Load GLEW opengl extensions
    const GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        throw std::runtime_error("x_window: GLEW error on glewInit().");
    }

    // Delete the dummy context after GLEW loads
    if (!glXMakeCurrent(_display, None, NULL))
    {
        throw std::runtime_error("x_window: Could not disable dummy rendering context");
    }

    // Free the color map
    XFreeColormap(_display, color_map);

    // Free the temp window
    XDestroyWindow(_display, temp_window);

    // Free the visual info
    XFree(visual);

    // Destroy the context
    glXDestroyContext(_display, context);
}

void min::x_window::create_opengl_context(const std::string &title, GLint major, GLint minor)
{
    // Create opengl attributes, 32 bit color, 24 bit depth, 8 bit stencil, double buffering
    GLint attr[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        //GLX_STENCIL_SIZE, 8,
        None};

    // Choose the best frame buffer config
    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig(_display, DefaultScreen(_display), attr, &fbcount);
    if (!fbc || fbcount == 0)
    {
        throw std::runtime_error("x_window: Failed to load a frame buffer configuration.");
    }

    // Pick the FB config/visual with the most samples per pixel
    int max_samples = 0;
    int index = 0;
    for (int i = 0; i < fbcount; i++)
    {
        // Get visual info from the frame buffer configuration
        XVisualInfo *visual = glXGetVisualFromFBConfig(_display, fbc[i]);
        if (visual)
        {
            int sample_buffer, samples;
            glXGetFBConfigAttrib(_display, fbc[i], GLX_SAMPLE_BUFFERS, &sample_buffer);
            glXGetFBConfigAttrib(_display, fbc[i], GLX_SAMPLES, &samples);

            // Choose the highest sample count
            if (sample_buffer > 0 && samples > max_samples)
            {
                max_samples = samples;
                index = i;
            }

            // Free the visual info
            XFree(visual);
        }
    }

    // Get the best configuration
    GLXFBConfig choosen_config = fbc[index];

    // Free the FBConfig list
    XFree(fbc);

    // Create visual that fulfills attributes requested
    XVisualInfo *visual = glXGetVisualFromFBConfig(_display, choosen_config);
    if (visual == nullptr)
    {
        throw std::runtime_error("x_window: Could not create X11 visual from requested attributes.");
    }

    // Create color map for the window, store so we can free in destructor
    _color_map = XCreateColormap(_display, _root, visual->visual, AllocNone);

    // Set the window attributes
    XSetWindowAttributes window_attr;
    window_attr.colormap = _color_map;
    window_attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

    // Connects the color map to the window and window will respond to 'Exposure' events
    _window = XCreateWindow(_display, _root, 0, 0, _w, _h, 0, visual->depth, InputOutput,
                            visual->visual, CWColormap | CWEventMask, &window_attr);

    if (!_window)
    {
        throw std::runtime_error("x_window: Failed to create an X11 window.");
    }

    // Show the window
    XMapWindow(_display, _window);

    // Set the window title
    XStoreName(_display, _window, title.c_str());

    // OpenGL 3.3+
    int context_attr[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, major,
        GLX_CONTEXT_MINOR_VERSION_ARB, minor,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        None, None};

    // Create GL context
    _context = glXCreateContextAttribsARB(_display, choosen_config, 0, true, context_attr);
    if (_context == nullptr)
    {
        throw std::runtime_error("x_window: Failed creating a opengl context.");
    }

    // Make the context curent
    if (!glXMakeCurrent(_display, _window, _context))
    {
        throw std::runtime_error("x_window: Could not bind rendering context");
    }

    // Load GLEW opengl extensions
    const GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        throw std::runtime_error("x_window: GLEW error on glewInit().");
    }

    // Free the visual info
    XFree(visual);
}

void min::x_window::create_window(const std::string &title)
{
    // Create display and send all output to 'this' PC
    _display = XOpenDisplay(0);
    if (_display == nullptr)
    {
        throw std::runtime_error("x_window: Could not create X11 display.");
    }

    // Create the root window
    // The root window is the "desktop background" window
    _root = DefaultRootWindow(_display);
    if (!_root)
    {
        throw std::runtime_error("x_window: Failed to create an X11 root window.");
    }

    // Check the GLX version
    int glx_max, glx_min;
    glXQueryVersion(_display, &glx_max, &glx_min);
    if (glx_max < 1 || (glx_max == 1 && glx_min < 3))
    {
        throw std::runtime_error("x_window: Query for GLX version returned " + std::to_string(glx_max) + "." + std::to_string(glx_min) + " expected 1.3+");
    }

    // Initialize GLEW with a dummy opengl context
    init_glew();

    // Create the real opengl context
    create_opengl_context(title, _major, _minor);

    // Print out the opengl version
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (major < _major || (major == _major && minor < _minor))
    {
        throw std::runtime_error("x_window: Could not create " + std::to_string(_major) + "." + std::to_string(_minor) + " opengl context.");
    }
    std::cout << "x_window: opening X11 opengl context version " << major << "." << minor << std::endl;
}

void min::x_window::on_lclick_down(const uint_fast16_t x, const uint_fast16_t y) const
{
    // Call the lclick_down callback
    if (_lclick_down)
    {
        _lclick_down(_data, x, y);
    }
}

void min::x_window::on_lclick_up(const uint_fast16_t x, const uint_fast16_t y) const
{
    // Call the lclick_up callback
    if (_lclick_up)
    {
        _lclick_up(_data, x, y);
    }
}

void min::x_window::on_rclick_down(const uint_fast16_t x, const uint_fast16_t y) const
{
    // Call the rlick_down callback
    if (_rclick_down)
    {
        _rclick_down(_data, x, y);
    }
}

void min::x_window::on_rclick_up(const uint_fast16_t x, const uint_fast16_t y) const
{
    // Call the rclick_up callback
    if (_rclick_up)
    {
        _rclick_up(_data, x, y);
    }
}

void min::x_window::on_resize(const uint_fast16_t width, const uint_fast16_t height)
{
    _w = width;
    _h = height;

    // Reset opengl viewport
    glViewport(0, 0, _w, _h);

    // Call the update callback
    on_update(_w, _h);
}

void min::x_window::on_update(const uint_fast16_t width, const uint_fast16_t height) const
{
    // Call the update callback
    if (_update)
    {
        _update(_data, width, height);
    }
}

min::x_window::x_window(const std::string &title, const uint_fast16_t width, const uint_fast16_t height, int major, int minor)
    : _w(width), _h(height), _major(major), _minor(minor), _shutdown(false),
      _lclick_down(nullptr), _lclick_up(nullptr), _rclick_down(nullptr), _rclick_up(nullptr), _update(nullptr)
{
    // Create X11 window for certain opengl version
    create_window(title);

    // Query the window manager for shutdown event flag
    _shutdown_message = XInternAtom(_display, "WM_DELETE_WINDOW", False);

    // Set the window manager protocol on the child window
    if (XSetWMProtocols(_display, _window, &_shutdown_message, 1) == 0)
    {
        std::cout << "x_window: Could not set WM_PROTOCOL@WM_DELETE_WINDOW on child window" << std::endl;
    }
}
min::x_window::~x_window()
{
    // Disable the current rendering context
    if (_context)
    {
        if (!glXMakeCurrent(_display, None, NULL))
        {
            std::cout << "x_window: Could not disable rendering context" << std::endl;
        }

        // Destroy the context
        glXDestroyContext(_display, _context);
    }

    // Free the color map
    XFreeColormap(_display, _color_map);

    // Destroy the window
    if (_window)
    {
        XDestroyWindow(_display, _window);
    }

    // Close the display if we have one opened
    if (_display)
    {
        XCloseDisplay(_display);
    }
}

void min::x_window::display_cursor(const bool set) const
{
    // Set if cursor is visible
    if (!set)
    {
        // Create a set of empty pixels and set cursor to it
        Pixmap pixels;
        static char empty[] = {0, 0, 0, 0, 0, 0, 0, 0};
        pixels = XCreateBitmapFromData(_display, _window, empty, 8, 8);

        // Create invisible cursor on the child window
        Cursor cursor;
        XColor color;
        color.red = color.green = color.blue = 0;
        cursor = XCreatePixmapCursor(_display, pixels, pixels, &color, &color, 0, 0);
        XDefineCursor(_display, _window, cursor);

        // Cleanup unused resources
        XFreeCursor(_display, cursor);
        XFreePixmap(_display, pixels);
    }
    else
    {
        // Use the default cursor
        XUndefineCursor(_display, _window);
    }
}

void min::x_window::error_message(const std::string &error) const
{
    std::cout << error << std::endl;
}

const char *min::x_window::get_context_string(const GLenum str) const
{
    return reinterpret_cast<const char *>(glGetString(str));
}

std::pair<uint_fast16_t, uint_fast16_t> min::x_window::get_cursor() const
{
    int x, y;
    Window window;
    unsigned int mask;

    // Get the mouse coordinates on the root window
    XQueryPointer(_display, _window, &window, &window, &x, &y, &x, &y, &mask);
    return std::make_pair<uint_fast16_t, uint_fast16_t>(x, y);
}

uint_fast16_t min::x_window::get_height() const
{
    return _h;
}

min::keyboard<unsigned int, double> &min::x_window::get_keyboard()
{
    return _keyboard;
}

const min::keyboard<unsigned int, double> &min::x_window::get_keyboard() const
{
    return _keyboard;
}

bool min::x_window::get_shutdown() const
{
    return _shutdown;
}

uint_fast16_t min::x_window::get_width() const
{
    return _w;
}

void min::x_window::maximize() const
{
    // Maximize the window
    // This functionality is dependent on the window manager
}

void min::x_window::register_lclick_down(void (*down)(void *, const uint_fast16_t x, const uint_fast16_t y))
{
    // Register callback on lmouse down
    _lclick_down = down;
}

void min::x_window::register_lclick_up(void (*up)(void *, const uint_fast16_t x, const uint_fast16_t y))
{
    // Register callback on lmouse up
    _lclick_up = up;
}

void min::x_window::register_rclick_down(void (*down)(void *, const uint_fast16_t x, const uint_fast16_t y))
{
    // Register callback on rmouse down
    _rclick_down = down;
}

void min::x_window::register_rclick_up(void (*up)(void *, const uint_fast16_t x, const uint_fast16_t y))
{
    // Register callback on rmouse up
    _rclick_up = up;
}

void min::x_window::register_data(void *ptr)
{
    // Register callback data pointer
    _data = ptr;
}

void min::x_window::register_update(void (*update)(void *, const uint_fast16_t width, const uint_fast16_t height))
{
    // Register callback on resize
    _update = update;
}

void min::x_window::resize(const uint_fast16_t width, const uint_fast16_t height) const
{
    XResizeWindow(_display, _window, width, height);
}

void min::x_window::set_cursor(const uint_fast16_t x, const uint_fast16_t y) const
{
    XWarpPointer(_display, 0, _window, 0, 0, _w, _h, x, y);
}

void min::x_window::set_shutdown()
{
    _shutdown = true;
}

void min::x_window::set_title(const std::string &title)
{
    // Set the window title
    XStoreName(_display, _window, title.c_str());
}

void min::x_window::swap_buffers() const
{
    // Swaps the double buffer
    glXSwapBuffers(_display, _window);
}

void min::x_window::update()
{
    int events = XEventsQueued(_display, QueuedAfterReading);
    for (int i = 0; i < events; i++)
    {
        XEvent event;
        XNextEvent(_display, &event);
        if (event.type == Expose)
        {
            // Get the window attributes for this event
            XWindowAttributes attr;
            XGetWindowAttributes(_display, _window, &attr);

            // Update the window size
            on_resize(attr.width, attr.height);
        }
        else if (event.type == KeyPress)
        {
            // Convert key code into keysym
            KeySym key_sym = XkbKeycodeToKeysym(_display, event.xkey.keycode, 0, event.xkey.state & ShiftMask ? 1 : 0);

            // Forward key down events to the virtual keyboard
            _keyboard.key_down(key_sym, 0);
        }
        else if (event.type == KeyRelease)
        {
            // Convert key code into keysym
            KeySym key_sym = XkbKeycodeToKeysym(_display, event.xkey.keycode, 0, event.xkey.state & ShiftMask ? 1 : 0);

            // Forward key up events to the virtual keyboard
            _keyboard.key_up(key_sym, 0);
        }
        else if (event.type == ButtonPress)
        {
            // Call the left click callback
            if (event.xbutton.button == 1)
            {
                on_lclick_down(event.xbutton.x, _h - event.xbutton.y);
            }
            // Call the right click callback, this may not work for all mice :|
            else if (event.xbutton.button == 3)
            {
                on_rclick_down(event.xbutton.x, _h - event.xbutton.y);
            }
        }
        else if (event.type == ButtonRelease)
        {
            // Call the left click callback
            if (event.xbutton.button == 1)
            {
                on_lclick_up(event.xbutton.x, _h - event.xbutton.y);
            }
            // Call the right click callback, this may not work for all mice :|
            else if (event.xbutton.button == 3)
            {
                on_rclick_up(event.xbutton.x, _h - event.xbutton.y);
            }
        }
        else if (event.type == ClientMessage)
        {
            // Detect the WM_DELETE_WINDOW protocol
            if (event.xclient.data.l[0] == (long)_shutdown_message)
            {
                this->set_shutdown();
            }
        }
    }
}