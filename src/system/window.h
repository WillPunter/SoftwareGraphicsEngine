/* window.h - compatibility layer */
#ifndef SYSTEM_WINDOW_H
#define SYSTEM_WINDOW_H

#include "window_common.h"

#if defined (__linux__)
    #include "window_x11.h"
#elif defined (_WIN32) || defined (_WIN64)
    #error "Error - Win32 not yet implemented."
#else
    #error "Error - platform not supported."
#endif

#endif