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


#include "quaternion.hpp"

//-----------------------------------------------------------------------------
using util::quaternion;


//-----------------------------------------------------------------------------
template<> const quaternion<float> quaternion<float>::IDENTITY = { 1, 0, 0, 0 };
template<> const quaternion<double> quaternion<double>::IDENTITY = { 1, 0, 0, 0 };


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::rotation (T radians, vector<3,T> axis) {
    radians /= T{2};
    axis.normalise ();

    return {
        std::cos (radians),
        axis.x * std::sin (radians),
        axis.y * std::sin (radians),
        axis.z * std::sin (radians)
    };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::rotation (vector<3,T> from, vector<3,T> to) {
    auto v = util::cross (from, to);

    return {
        std::acos (from.dot (to)),
        v.x,
        v.y,
        v.z
    };
}


//-----------------------------------------------------------------------------
template <typename T>
quaternion<T>
quaternion<T>::operator* (const quaternion<T> &rhs) const {
    return {
        w * rhs.w - (x * rhs.x + y * rhs.y + z * rhs.z),
        w * rhs.x + rhs.w * x + y * rhs.z - z * rhs.y,
        w * rhs.y + rhs.w * y + z * rhs.x - x * rhs.z,
        w * rhs.z + rhs.w * z + x * rhs.y - y * rhs.x
    };
}


//-----------------------------------------------------------------------------
template struct util::quaternion<float>;
template struct util::quaternion<double>;
