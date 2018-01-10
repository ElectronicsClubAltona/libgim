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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#include "maths.hpp"

#include <type_traits>


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
util::log2up (T v)
{
    return log2 ((v << 1) - 1);
}

template uint32_t util::log2up (uint32_t);
template uint64_t util::log2up (uint64_t);


///////////////////////////////////////////////////////////////////////////////
template const float  util::PI<float>;
template const double util::PI<double>;


///////////////////////////////////////////////////////////////////////////////
// Simple instantiations. Some functions aren't used internally to the library
// so it's easier to instantiate early and check for broken code at library
// build time.

template float util::limit (float, float, float);

template float  util::smoothstep (float,  float,  float);
template double util::smoothstep (double, double, double);
