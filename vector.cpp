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
#include "maths.hpp"
#include "random.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

using util::vector;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
T
util::vector<S,T>::magnitude (void) const
{
    // TODO: this should not truncate for integral types
    return static_cast<T> (std::sqrt (magnitude2 ()));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::vector<S,T>::magnitude2 (void) const
{
    T total { 0 };
    for (size_t i = 0; i < S; ++i)
        total += pow2 (this->data[i]);
    return total;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::vector<S,T>::difference (const util::vector<S,T> &rhs) const
{
    // TODO: change the signature to ensure it does not truncate
    return static_cast<T> (std::sqrt (difference2 (rhs)));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::vector<S,T>::difference2 (const util::vector<S,T> &rhs) const
{
    T sum {0};
    for (size_t i = 0; i < S; ++i)
        sum += pow2 (this->data[i] - rhs.data[i]);
    return sum;
}

//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>&
util::vector<S,T>::normalise (void)
{
    T mag = magnitude ();

    for (size_t i = 0; i < S; ++i)
        this->data[i] /= mag;

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::vector<S,T>::normalised (void) const
{
    T mag = magnitude ();
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] / mag;

    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::vector<2,T>
util::polar_to_cartesian (util::vector<2,T> v)
{
    return util::vector<2,T> {
        v[0] * std::cos (v[1]),
        v[0] * std::sin (v[1])
    };
}


//-----------------------------------------------------------------------------
template <typename T>
util::vector<2,T>
util::cartesian_to_polar (util::vector<2,T> v)
{
    return util::vector<2,T> {
        std::hypot (v.x, v.y),
        std::atan2 (v.y, v.x)
    };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::vector<3,T>
util::cross (const util::vector<3,T> &a,
             const util::vector<3,T> &b)
{
    return util::vector<3,T> {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

template util::vector3f util::cross(const util::vector3f&, const util::vector3f&);
template util::vector3d util::cross(const util::vector3d&, const util::vector3d&);


//-----------------------------------------------------------------------------
template <typename T>
util::vector<3,T>
util::spherical_to_cartesian (const util::vector<3,T> &s)
{
    return util::vector<3,T> {
        s.x * sin (s.y) * cos (s.z),
        s.x * sin (s.y) * sin (s.z),
        s.x * cos (s.y),
    };
}


//-----------------------------------------------------------------------------
template <typename T>
util::vector<3,T>
util::cartesian_to_spherical (const util::vector<3,T> &c)
{
    T mag = c.magnitude ();

    return util::vector<3,T> {
        mag,
        acos  (c.z / mag),
        atan2 (c.y, c.x)
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
util::vector<S,T>::is_zero (void) const
{
    return std::all_of (std::begin (this->data),
                        std::end   (this->data),
                        [] (T i) { return almost_zero (i); });
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const util::vector<S,T>
util::vector<S,T>::ZERO (T{0});


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
util::vector<S,T>::sanity (void) const
{
    CHECK (std::all_of (std::begin (this->data),
                        std::end   (this->data),
                        [] (T i) { return !std::isnan (i); }));
}


///////////////////////////////////////////////////////////////////////////////
// ostream

template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const util::vector<S,T> &v)
{
    os << "vec" << S << "(" << v.data[0];
    for (size_t i = 1; i < S; ++i)
        os << ", " << v.data[i];
    os << ")";
    return os;
}


//-----------------------------------------------------------------------------
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
        v.data[i] = static_cast<T> (array[i].as_number ().native ());

    return node;
}


//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)                                                        \
template struct util::vector<S,T>;                                                  \
template std::ostream& util::operator<< (std::ostream&, const util::vector<S,T> &v);\
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
namespace util {
    template vector<2,float> polar_to_cartesian (util::vector<2,float>);
    template vector<2,float> cartesian_to_polar (util::vector<2,float>);

    template <> vector<1,float> random (void) { util::vector<1,float> out; randomise (out.data); return out; }
    template <> vector<2,float> random (void) { util::vector<2,float> out; randomise (out.data); return out; }
    template <> vector<3,float> random (void) { util::vector<3,float> out; randomise (out.data); return out; }
    template <> vector<4,float> random (void) { util::vector<4,float> out; randomise (out.data); return out; }

    template <> vector<1,double> random (void) { util::vector<1,double> out; randomise (out.data); return out; }
    template <> vector<2,double> random (void) { util::vector<2,double> out; randomise (out.data); return out; }
    template <> vector<3,double> random (void) { util::vector<3,double> out; randomise (out.data); return out; }
    template <> vector<4,double> random (void) { util::vector<4,double> out; randomise (out.data); return out; }
}

template <>
bool
almost_equal [[gnu::pure]] (const util::vector2f &a, const util::vector2f &b)
{
    return std::equal (a.begin (), a.end (), b.begin (), almost_equal<float>);
}
