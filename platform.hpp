/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_PLATFORM_HPP
#define __UTIL_PLATFORM_HPP

#if defined(__linux__) || defined (__linux)
    #define PLATFORM_LINUX
#elif defined(__WIN32) || defined (_WIN32)
    #define PLATFORM_WIN32
#else
    #error "Unknown platform"
#endif

// Clang needs to be checked before GCC as it pretends to be GCC 
#if defined(__clang__)
    #define COMPILER_CLANG
#elif defined(__GNUC__)
    #define COMPILER_GCC
#else
    #error "Unknown compiler"
#endif

#endif
