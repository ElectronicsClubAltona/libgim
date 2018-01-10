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


#include "plane.hpp"

#include "../debug.hpp"

using util::geom::plane;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
plane<S,T>::plane (point<S,T>  _p, vector<S,T> _n):
    p (_p),
    n (_n)
{
    CHECK (is_normalised (n));
}


///////////////////////////////////////////////////////////////////////////////
template struct util::geom::plane<2,float>;
template struct util::geom::plane<3,float>;
