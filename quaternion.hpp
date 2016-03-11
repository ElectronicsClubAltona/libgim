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

#ifndef __UTIL_QUATERNION_HPP
#define __UTIL_QUATERNION_HPP

#include "vector.hpp"
#include "matrix.hpp"

#include <ostream>


namespace util {
    template <typename T>
    struct quaternion {
        ///////////////////////////////////////////////////////////////////////
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

        union {
            struct { T w, x, y, z; };
            struct { T a, b, c, d; };
            T data[4];
        };

#pragma GCC diagnostic pop

        static const quaternion IDENTITY;

        static quaternion rotation (T radians, vector<3,T> axis);
        static quaternion rotation (vector<3,T> src, vector<3,T> dst);

        quaternion (T a, T b, T c, T d);
        quaternion (T a);
        quaternion ();
        quaternion (vector<3,T>);

        T norm  (void) const;
        T norm2 (void) const;
        quaternion normalised (void) const;

        quaternion operator- (void) const;
        quaternion conjugate (void) const;

        quaternion operator+ (const quaternion) const;
        quaternion operator- (const quaternion) const;
        quaternion operator* (const quaternion) const;
        quaternion operator/ (const quaternion) const;

        quaternion operator+ (const T) const;
        quaternion operator- (const T) const;
        quaternion operator* (const T) const;
        quaternion operator/ (const T) const;

        bool operator== (const quaternion) const;
        bool operator!= (const quaternion) const;

        matrix4<T> rotation_matrix (void) const;
    };

    typedef quaternion<float> quaternionf;

    template <typename T>
    std::ostream&
    operator<< (std::ostream&, quaternion<T>);
}


#endif
