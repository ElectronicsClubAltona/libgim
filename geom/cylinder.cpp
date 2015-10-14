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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "cylinder.hpp"

using util::geom::cylinder;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
bool
cylinder<S,T>::includes (util::point<S,T> p_) const
{
    auto p10 = p1 - p0;
    auto p_0 = p_ - p0;

    auto l = dot (p10, p_0);
    if (l < 0 || l > p10.magnitude2 ())
        return false;

    auto d = dot (p10, p10) - l * l * p10.magnitude2 ();
    if (d > radius * radius)
        return false;

    return true; // d
}


///////////////////////////////////////////////////////////////////////////////
template struct util::geom::cylinder<2,float>;
template struct util::geom::cylinder<3,float>;
