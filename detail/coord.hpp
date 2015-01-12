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

#ifndef __UTIL_COORD_HPP
#define __UTIL_COORD_HPP

#include "../platform.hpp"

namespace util {
    namespace detail {
        // Disable GCC warnings about validity of anonyous structures in
        // unions. Push comes to shove I'll manually redsign everything to
        // keep this syntax anyway.
#pragma GCC diagnostic push
#if defined(COMPILER_GCC)
    #pragma GCC diagnostic ignored "-pedantic"
#endif
#if defined(COMPILER_CLANG)
    #pragma GCC diagnostic ignored "-Wgnu"
#endif
        template <size_t S, typename T>
        struct coord {
            typedef T value_type;

            coord () { ; }

            coord (T v)
            { std::fill (std::begin (data), std::end (data), v); }

            template <typename ...U>
            coord (U ..._u): data{_u...}
            { ; }

            T data[S];

            static const size_t dimension = S;

            T& operator[] (size_t i)       { return data[i]; }
            T  operator[] (size_t i) const { return data[i]; }
        };

        template <typename T>
        struct coord<1,T> {
            typedef T value_type;

            coord () { ; }

            coord (T v)
            { std::fill (std::begin (data), std::end (data), v); }

            template <typename ...U>
            coord (U ..._u): data{_u...}
            { ; }

            union {
                T data[1];
                T x;
            };

            static const size_t dimension = 1;

            T& operator[] (size_t i)       { return data[i]; }
            T  operator[] (size_t i) const { return data[i]; }
        };

        template <typename T>
        struct coord<2,T> {
            typedef T value_type;

            coord () { ; }

            coord (T v)
            { std::fill (std::begin (data), std::end (data), v); }

            template <typename ...U>
            coord (U ..._u): data{_u...}
            { ; }

            union {
                T data[2];
                struct {
                    T x;
                    T y;
                };
                struct {
                    T s;
                    T t;
                };
            };

            static const size_t dimension = 2;

            T& operator[] (size_t i)       { return data[i]; }
            T  operator[] (size_t i) const { return data[i]; }
        };

        template <typename T>
        struct coord<3,T> {
            typedef T value_type;

            union {
                T data[3];
                struct {
                    T x;
                    T y;
                    T z;
                };
                struct {
                    T r;
                    T g;
                    T b;
                };
            };

            static const size_t dimension = 3;

            coord () { ; }

            coord (T v)
            { std::fill (std::begin (data), std::end (data), v); }

            template <typename... U>
            coord (U... u): data{u...}
            { ; }

            T& operator[] (size_t i)       { return data[i]; }
            T  operator[] (size_t i) const { return data[i]; }
        };

        template <size_t S, typename T>
        T dot (const coord<S,T> &a, const coord<S,T> &b)
        {
            T sum { 0 };
            for (size_t i = 0; i < S; ++i)
                sum += a.data[i] * b.data[i];
            return sum;
        }

        template <typename T>
        struct coord<4,T> {
            typedef T value_type;

            union {
                T data[4];
                struct {
                    T x;
                    T y;
                    T z;
                    T w;
                };
                struct {
                    T r;
                    T g;
                    T b;
                    T a;
                };
            };

            static const size_t dimension = 4;

            coord () { ; }

            coord (T v)
            { std::fill (std::begin (data), std::end (data), v); }

            template <typename... U>
            coord (U... u): data{u...}
            { ; }

            T& operator[] (size_t i)       { return data[i]; }
            T  operator[] (size_t i) const { return data[i]; }
        };
#pragma GCC diagnostic pop
    }
}

#endif
