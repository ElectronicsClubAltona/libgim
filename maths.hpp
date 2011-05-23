/*
 * This file is part of waif.
 *
 * Waif is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * Waif is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with waif.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#ifndef __MATHS_HPP
#define __MATHS_HPP


template <typename T>
T 
pow2 (T value);


template <typename T>
double
rootsquare (T a, T b);


/**
 * Check if two floating point numbers are approximately equal. Returns true
 * if the difference is less than a percentage of each individual value.
 *
 * @e maximum percentage difference for equal values
 */

template <typename T>
bool
almost_equal (T a, T b)
    { return a == b; }


template <typename Ta, typename Tb>
bool
almost_equal (Ta a, Tb b) {
    return almost_equal <decltype(a + b)> (static_cast<decltype(a + b)>(a),
                                           static_cast<decltype(a + b)>(b));
}


template <>
bool
almost_equal (float a, float b);


template <>
bool
almost_equal (double a, double b);


/// Variadic minimum
template <typename T>
const T&
min (const T &a)
    { return a ; }


template <typename T, typename ...Args>
const T&
min (const T &a , const T &b , const Args &...args )
    { return min ( b < a ? b : a, args...); }


/// Variadic maximum
template <typename T>
const T&
max (const T &a)
    { return a ; }


template <typename T, typename ...Args>
const T&
max (const T &a , const T &b , const Args &...args )
    { return max ( b > a ? b : a, args...); }



#endif // __MATHS_HPP
