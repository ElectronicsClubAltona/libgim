/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_PLATFORM_HPP
#define __UTIL_PLATFORM_HPP

#if defined(__ANDROID__)
    #define PLATFORM_ANDROID
#elif defined(__linux__) || defined (__linux)
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
