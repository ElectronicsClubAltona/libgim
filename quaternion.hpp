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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_QUATERNION_HPP
#define __UTIL_QUATERNION_HPP

#include "coord.hpp"

#include "vector.hpp"
#include "matrix.hpp"

#include <ostream>


namespace util {
    // quaternion's are _just_ different enough to other coord types that we
    // special case as a distinct POD type and provide many of the same
    // functions as distinct declarations.
    //
    // issues include:
    // * strictly 4 dimensions
    // * scalar operations sometimes don't make sense on the w component
    // * objects must be normalised to make sense
    template <typename T>
    struct quaternion {
        T w, x, y, z;

        static quaternion angle_axis (T radians, vector<3,T> axis);
        static quaternion from_euler (vector<3,T>);

        static quaternion from_to (vector<3,T>, vector<3,T>);
        static quaternion look (vector<3,T> fwd, vector<3,T> up);

        matrix4<T> as_matrix (void) const;

        static constexpr quaternion<T> identity (void);
    };


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    vector3<T>
    rotate (vector3<T>, quaternion<T>);


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    constexpr
    T
    norm2 (quaternion<T>);

    template <typename T>
    constexpr
    T
    norm (quaternion<T>);

    template <typename T>
    constexpr
    bool
    is_normalised (quaternion<T>);

    template <typename T>
    constexpr
    quaternion<T>
    normalised (quaternion<T>);


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    quaternion<T>
    conjugate (quaternion<T>);


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    quaternion<T>
    operator* (quaternion<T>, quaternion<T>);

    //-------------------------------------------------------------------------
    template <typename T>
    quaternion<T>&
    operator*= (quaternion<T>&, quaternion<T>);

    //-------------------------------------------------------------------------
    template <typename T>
    quaternion<T>
    operator/ (quaternion<T>, quaternion<T>);

    //-------------------------------------------------------------------------
    template <typename T>
    constexpr
    quaternion<T>
    operator/ (quaternion<T>, T);


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    constexpr
    bool operator== (quaternion<T>, quaternion<T>);

    //-------------------------------------------------------------------------
    template <typename T>
    bool almost_equal (quaternion<T>, quaternion<T>);

    ///////////////////////////////////////////////////////////////////////////
    typedef quaternion<float>  quaternionf;
    typedef quaternion<double> quaterniond;


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    std::ostream&
    operator<< (std::ostream&, quaternion<T>);
}

#include "./quaternion.ipp"

#endif
