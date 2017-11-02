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

#include "vector.hpp"

#include "debug.hpp"

#include "json/tree.hpp"

#include <cmath>

using util::vector;
using util::vector3f;
using util::vector3d;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
vector<2,T>
util::polar_to_cartesian (vector<2,T> v)
{
    return util::vector<2,T> {
        v[0] * std::cos (v[1]),
        v[0] * std::sin (v[1])
    };
}


//-----------------------------------------------------------------------------
template <typename T>
vector<2,T>
util::cartesian_to_polar (vector<2,T> v)
{
    return util::vector<2,T> {
        std::hypot (v.x, v.y),
        std::atan2 (v.y, v.x)
    };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
vector<3,T>
util::from_euler (vector<2,T> euler)
{
    return normalised (util::vector<3,T> {
         std::sin (euler.x) * std::cos (euler.y),
         std::cos (euler.x),
        -std::sin (euler.x) * std::sin (euler.y),
    });
}

template util::vector3f util::from_euler (util::vector2f);
template util::vector3d util::from_euler (util::vector2d);


//-----------------------------------------------------------------------------
template <typename T>
vector<2,T>
util::to_euler (vector<3,T> vec)
{
    CHECK (is_normalised (vec));

    return {
         std::acos  (vec.y),
        -std::atan2 (vec.z, vec.x),
    };
}

template util::vector2f util::to_euler (util::vector3f);
template util::vector2d util::to_euler (util::vector3d);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
void
util::vector<S,T>::sanity (void) const
{
    CHECK (std::all_of (std::begin (this->data),
                        std::end   (this->data),
                        [] (T i) { return !std::isnan (i); }));
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
const json::tree::node&
util::operator>> (const json::tree::node &node, util::vector<S,T> &v)
{
    const json::tree::array &array = node.as_array ();
    if (array.size () != S)
        throw std::runtime_error ("Invalid dimensionality for json-to-vector");

    // XXX: This used to be a std::transform but gcc 4.9.0 hit an internal
    // compiler error at this point in release mode, so we dumb it down a
    // little.
    for (size_t i = 0; i < array.size (); ++i)
        v.data[i] = array[i].as<T> ();

    return node;
}


//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)                                                    \
template struct util::vector<S,T>;                                              \
template const json::tree::node& util::operator>> (const json::tree::node&, util::vector<S,T>&);


#define INSTANTIATE(T)  \
INSTANTIATE_S_T(1,T)    \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)    \
INSTANTIATE_S_T(4,T)


INSTANTIATE(uint32_t)
INSTANTIATE(int32_t)
INSTANTIATE(uint64_t)
INSTANTIATE(int64_t)
INSTANTIATE(float)
INSTANTIATE(double)


//-----------------------------------------------------------------------------
template vector<2,float> util::polar_to_cartesian (util::vector<2,float>);
template vector<2,float> util::cartesian_to_polar (util::vector<2,float>);
