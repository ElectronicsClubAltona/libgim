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
 * Copyright 2015-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_GEOM_SAMPLE_HPP
#define __UTIL_GEOM_SAMPLE_HPP

#include "../coord/fwd.hpp"
#include "./ops.hpp"

#include <cstddef>


///////////////////////////////////////////////////////////////////////////////
namespace util::geom {
    /// a function object that selects a uniformly random point inside a shape
    /// using a provided random generator. the point will lie within the shape,
    /// inclusive of boundaries.
    ///
    /// may be specialised for arbitrary shapes but uses rejection sampling
    /// as a safe default. this implies that execution may not take a constant
    /// time.
    ///
    /// \tparam S coordinate type dimension
    /// \tparam T value type for the shape/coordinate
    /// \tparam K the shape type to test
    /// \tparam G a UniformRandomBitGenerator, eg std::min19937
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename G
    >
    struct sampler {
        static point<S,T>
        fn (K<S,T> k, G &g)
        {
            auto b = bounds (k);

            while (true) {
                auto p = sample (b, g);
                if (intersects (k, p))
                    return p;
            }
        }
    };


    ///////////////////////////////////////////////////////////////////////////
    /// a convenience function that calls sample::fn to select a random point
    /// in a provided shape.
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename G  // random generator
    >
    point<S,T>
    sample (K<S,T> k, G &g)
    {
        return sampler<S,T,K,G>::fn (k, g);
    }
}

#endif
