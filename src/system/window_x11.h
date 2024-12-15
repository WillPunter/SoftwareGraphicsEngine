/* system.window.h 
    This module provides a cross-platform
    window structure and its associated
    functions. The purpose of this is to
    extract away the system-specific code
    for managing windows by providing a
    compatibility layer that can be used
    by the rest of the modules. 
    
    This is the x11 implementation.
    
    TODO - add support for Win32.
    Consider whether it is better
    to write a separate source
    file or if it is better to
    just use #ifdef statements
    to partition platform specific
    code. */

#ifndef SYSTEM_WINDOW_X11_H
#define SYSTEM_WINDOW_X11_H

#include "window_common.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>

#define SYSTEM_WINDOW_DEFAULT_X 100
#define SYSTEM_WINDOW_DEFAULT_Y 100

#define SYSTEM_WINDOW_DEFAULT_WIDTH 640
#define SYSTEM_WINDOW_DEFAULT_HEIGHT 480

#endif