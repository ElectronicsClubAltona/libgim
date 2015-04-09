/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010-2013 Danny Robson <danny@nerdcruft.net>
 */

#include "colour.hpp"

#include "range.hpp"
#include "random.hpp"
#include "stream.hpp"


//-----------------------------------------------------------------------------
using util::colour;
using util::colour4f;


//-----------------------------------------------------------------------------
#define CONSTANT_COLOUR(NAME,R,G,B)     \
template <size_t S, typename T>         \
const util::colour<S,T>                 \
util::colour<S,T>::NAME =               \
    util::colour<4,float> (R,G,B,1)     \
    .template redim<S> ()               \
    .template cast<T> ();

CONSTANT_COLOUR(WHITE,1,1,1);
CONSTANT_COLOUR(BLACK,0,0,0);
CONSTANT_COLOUR(RED,  1,0,0);
CONSTANT_COLOUR(GREEN,0,1,0);
CONSTANT_COLOUR(BLUE, 0,0,1);





///----------------------------------------------------------------------------
//! Extract a colour object from a JSON node.
//!
//! Data must be an array or 3 or 4 numbers. Guarantees success, or throws a
//! json::tree::type_error.
const json::tree::node&
operator>> (const json::tree::node &node, util::colour4f &c) {
    c.r = static_cast<float> (node[0].as_number ());
    c.g = static_cast<float> (node[1].as_number ());
    c.b = static_cast<float> (node[2].as_number ());

    try {
        c.a = static_cast<float> (node[3].as_number ());
    } catch (...) {
        c.a = 1;
    }

    return node;
}


//-----------------------------------------------------------------------------
namespace util {
    template<>
    colour4f
    random (void) {
        return colour4f ({ range<float>::UNIT.random (),
                           range<float>::UNIT.random (),
                           range<float>::UNIT.random (),
                           range<float>::UNIT.random () });
    }

    template <>
    colour4f&
    randomise (colour4f &c)
        { return c = random<colour4f> (); }
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, util::colour<S,T> c) {
    os << "colour(";
    for (size_t i = 0; i < S - 1; ++i)
        os << stream::numeric<T> (c[i]) << ", ";
    os << stream::numeric<T> (c[S-1]) << ")";
    return os;
}

//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)        \
template struct util::colour<S,T>;  \
template std::ostream& util::operator<< (std::ostream&, util::colour<S,T>);

#define INSTANTIATE_S(S)    \
INSTANTIATE_S_T(S,uint8_t)  \
INSTANTIATE_S_T(S,float)    \
INSTANTIATE_S_T(S,double)

INSTANTIATE_S(3)
INSTANTIATE_S(4)
