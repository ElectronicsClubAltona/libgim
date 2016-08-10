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

#include "coord.hpp"

#include "vector.hpp"
#include "matrix.hpp"

#include <ostream>


namespace util {
    // quaternions must be 4 elements, but we include a size parameter so it
    // fits with the generic coord infrastructure more easily.
    //
    // specifically: 
    // large regions of base code require a template template parameter with
    // size and type arguments, which is annoying to work around for this one
    // case.
    //
    // we protect against invalid instantiations through static_assert
    template <size_t S, typename T>
    struct quaternion : public coord::base<4,T,quaternion,coord::wxyz,coord::abcd> {
        static_assert (S == 4, "quaternions must be 4 elements");

        using coord::base<S,T,::util::quaternion,::util::coord::wxyz,::util::coord::abcd>::base;

        static quaternion rotation (T radians, vector<3,T> axis);
        static quaternion rotation (vector<3,T> src, vector<3,T> dst);

        T magnitude  (void) const;
        T magnitude2 (void) const;

        bool is_normalised (void) const;
        quaternion normalised (void) const;

        matrix4<T> as_matrix (void) const;

        static const quaternion IDENTITY;
    };

    template <size_t S, typename T> quaternion<S,T> operator* (const quaternion<S,T>, const quaternion<S,T>);
    template <size_t S, typename T> quaternion<S,T> operator/ (const quaternion<S,T>, const quaternion<S,T>);

    template <size_t S, typename T> quaternion<S,T> operator* (const quaternion<S,T>, const T);
    template <size_t S, typename T> quaternion<S,T> operator/ (const quaternion<S,T>, const T);

    typedef quaternion<4,float> quaternionf;

    template <size_t S, typename T>
    std::ostream&
    operator<< (std::ostream&, quaternion<S,T>);
}


#endif
