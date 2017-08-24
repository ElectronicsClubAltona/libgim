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


#include "./aabb.hpp"

#include "./iostream.hpp"
#include "../coord/iostream.hpp"
#include "../debug.hpp"

using util::geom::aabb;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
aabb<S,T>::aabb (const point<S,T> _p0, const point<S,T> _p1):
    p0 (_p0),
    p1 (_p1)
{
    debug::sanity (*this);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
T
aabb<S,T>::diameter (void) const
{
    return magnitude ().diameter ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>
aabb<S,T>::magnitude (void) const
{
    return (p1 - p0).template as<util::extent> ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
aabb<S,T>::closest (const point<S,T> q) const
{
    return limit (q, p0, p1);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
aabb<S,T>
aabb<S,T>::expand (const vector<S,T> mag) const noexcept
{
    CHECK (all (mag >= T{0}));

    return { p0 - mag, p1 + mag };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
aabb<S,T>
aabb<S,T>::expand (const T mag) const noexcept
{
    CHECK_GE (mag, T{0});

    return { p0 - mag, p1 + mag };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
aabb<S,T>
aabb<S,T>::contract (const util::vector<S,T> mag) const noexcept
{
    CHECK (all (mag >= T{0}));
    CHECK (all (2 * mag <= p1 - p0));

    return { p0 + mag, p1 - mag };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
aabb<S,T>
aabb<S,T>::contract (const T mag) const noexcept
{
    CHECK_GE (mag, T{0});
    CHECK (all (2 * mag <=  p1 - p0));

    return { p0 + mag, p1 - mag };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
aabb<S,T>::cover (const point<S,T> p)
{
    p0 = min (p, p0);
    p1 = max (p, p1);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
aabb<S,T>
aabb<S,T>::operator+ (const vector<S,T> v) const
{
    return { p0 + v, p1 + v };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
aabb<S,T>
aabb<S,T>::operator- (const vector<S,T> v) const
{
    return { p0 - v, p1 - v };
}


///////////////////////////////////////////////////////////////////////////////
namespace util::debug {
    template <size_t S, typename T>
    struct validator<aabb<S,T>> {
        static bool is_valid (const aabb<S,T> &b)
        {
            return all (b.p0 <= b.p1);
        }
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::geom::operator<< (std::ostream &os, util::geom::aabb<S,T> b)
{
    os << "aabb(" << b.p0 << ", " << b.p1 << ")";
    return os;
}


//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)                             \
namespace util::geom { template struct aabb<S,T>; }      \
template bool util::debug::is_valid (const aabb<S,T>&);  \
template std::ostream& util::geom::operator<< (std::ostream&, aabb<S,T>);

#define INSTANTIATE(T)  \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)

INSTANTIATE( int32_t)
INSTANTIATE( int64_t)
INSTANTIATE(uint32_t)
INSTANTIATE(uint64_t)
INSTANTIATE(float)
INSTANTIATE(double)
