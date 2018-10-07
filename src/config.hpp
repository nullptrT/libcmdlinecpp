/**
 * @file config.hpp
 * @brief The configuration headers
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
    LibCmdLineC++: A simple parser for command line arguments with C++
    Copyright (C) 2018 Sebastian Lau <lauseb644@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/


// Now check the compiler

#ifndef __cplusplus
#error A C++ compiler is required!
#elif __unix__
#if __cplusplus < 201103L
#error A C++ compiler supporting -std=c++11 is required!
#endif
#else
#if _MSC_VER < 1900
#error A MSVC++ compiler supporting -std=c++11 is required!
#endif
#endif

// Determine, which features we will enable

#ifdef __unix__

//#define LOGCPP_ENABLE_COLOR_SUPPORT false
#undef LOGCPP_ENABLE_COLOR_SUPPORT

#else

#ifdef LOGCPP_ENABLE_COLOR_SUPPORT
#undef LOGCPP_ENABLE_COLOR_SUPPORT
#endif

#endif


#ifndef LOGCPP_ENABLE_COLOR_SUPPORT

#ifdef LOGCPP_AUTOCOLOR
#undef LOGCPP_AUTOCOLOR
#endif

#endif
