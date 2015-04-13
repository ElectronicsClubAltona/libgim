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
        std::acos (dot (from, to)),
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
