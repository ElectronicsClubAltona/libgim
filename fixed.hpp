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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_FIXED_HPP
#define __UTIL_FIXED_HPP

#include "types.hpp"

#include <cstdint>

template <unsigned int INT, unsigned int FRAC>
class fixed {
    public:
        typedef typename bits_type<INT + FRAC>::uint combined_type;
        typedef typename bits_type<INT + FRAC>::uint integral_type;

        combined_type m_value;

    public:
        explicit fixed(double);
        explicit fixed(float);
        explicit fixed(integral_type);

        double        to_double   (void) const;
        float         to_float    (void) const;
        integral_type to_integral (void) const;

        fixed<INT, FRAC>& operator +=(const fixed<INT, FRAC>);
        fixed<INT, FRAC>& operator -=(const fixed<INT, FRAC>);
        fixed<INT, FRAC>& operator *=(const fixed<INT, FRAC>);
        fixed<INT, FRAC>& operator /=(const fixed<INT, FRAC>);

        fixed<INT, FRAC>  operator +(const fixed<INT, FRAC>) const;
        fixed<INT, FRAC>  operator -(const fixed<INT, FRAC>) const;
        fixed<INT, FRAC>  operator *(const fixed<INT, FRAC>) const;
        fixed<INT, FRAC>  operator /(const fixed<INT, FRAC>) const;

        fixed<INT, FRAC>& operator +=(integral_type);
        fixed<INT, FRAC>& operator -=(integral_type);
        fixed<INT, FRAC>& operator *=(integral_type);
        fixed<INT, FRAC>& operator /=(integral_type);

        fixed<INT, FRAC>  operator +(integral_type) const;
        fixed<INT, FRAC>  operator -(integral_type) const;
        fixed<INT, FRAC>  operator *(integral_type) const;
        fixed<INT, FRAC>  operator /(integral_type) const;
};

#endif // __UTIL_FIXED_HPP 
