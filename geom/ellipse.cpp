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

#include "ellipse.hpp"

#include "ops.hpp"

using util::geom::ellipse;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
static bool
intersects (ellipse<S,T> e, util::point<S,T> p)
{
    auto mag = (p - e.origin) * (p - e.origin) / (e.radius * e.radius);
    return std::accumulate (mag.begin (), mag.end (), 0) <= 1;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T, template <size_t,typename> class A, template <size_t,typename> class B>
bool
util::geom::intersects (A<S,T> a, B<S,T> b)
{
    return ::intersects (a, b);
}


//-----------------------------------------------------------------------------
template bool util::geom::intersects (ellipse<2,float>, util::point<2,float>);
template bool util::geom::intersects (ellipse<3,float>, util::point<3,float>);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
static util::geom::aabb<S,T>
bounds (ellipse<S,T> e)
{
    return {
        e.origin - e.radius,
        e.origin + e.radius
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T, template <size_t,typename> class K>
util::geom::aabb<S,T>
util::geom::bounds (K<S,T> k)
{
    return ::bounds (k);
}


//-----------------------------------------------------------------------------
template util::geom::aabb<2,float> util::geom::bounds (ellipse<2,float>);
template util::geom::aabb<3,float> util::geom::bounds (ellipse<3,float>);
