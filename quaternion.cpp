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
const quaternion quaternion::IDENTITY = { 1.0, 0.0, 0.0, 0.0 };


//-----------------------------------------------------------------------------
quaternion
quaternion::rotation (double radians, vector<3> axis) {
    radians /= 2.0;
    axis.normalise ();

    return {
        cos (radians),
        axis.x * sin (radians),
        axis.y * sin (radians),
        axis.z * sin (radians)
    };
}


quaternion
quaternion::rotation (vector<3> from, vector<3> to) {
    auto v = util::cross (from, to);

    return {
        acos (from.dot (to)),
        v.x,
        v.y,
        v.z
    };
}


quaternion
quaternion::operator* (const quaternion &rhs) const {
    return {
        w * rhs.w - (x * rhs.x + y * rhs.y + z * rhs.z),
        w * rhs.x + rhs.w * x + y * rhs.z - z * rhs.y,
        w * rhs.y + rhs.w * y + z * rhs.x - x * rhs.z,
        w * rhs.z + rhs.w * z + x * rhs.y - y * rhs.x
    };
}

