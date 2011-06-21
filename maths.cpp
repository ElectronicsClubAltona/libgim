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
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#include "maths.hpp"

#include "float.hpp"

#include <cmath>


template <typename T>
T 
pow2 (T value)
{ return value * value; }

template double pow2(double);
template    int pow2(   int);


template <typename T>
double
rootsquare (T a, T b)
{ return sqrt (pow2 (a) + pow2 (b)); }

template double rootsquare (double, double);
template double rootsquare (   int,    int);


template <>
bool
almost_equal (float a, float b)
    { return ieee_single::almost_equal (a, b); }


template <>
bool
almost_equal (double a, double b)
    { return ieee_double::almost_equal (a, b); }


