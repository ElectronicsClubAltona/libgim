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


//-----------------------------------------------------------------------------
using namespace util;


//-----------------------------------------------------------------------------
const util::colour util::colour::WHITE ({ 1.f, 1.f, 1.f, 1.f });
const util::colour util::colour::BLACK ({ 0.f, 0.f, 0.f, 1.f });

const util::colour util::colour::RED   ({ 1.f, 0.f, 0.f, 1.f });
const util::colour util::colour::GREEN ({ 0.f, 1.f, 0.f, 1.f });
const util::colour util::colour::BLUE  ({ 0.f, 0.f, 1.f, 1.f });


//-----------------------------------------------------------------------------
//! Extract a colour object from a JSON node.
//! 
//! Data must be an array or 3 or 4 numbers. Guarantees success, or throws a
//! json::type_error.
const json::node&
operator>> (const json::node &node, colour &c) {
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
    colour
    random (void) {
        return colour ({ range<float>::UNIT.random (),
                         range<float>::UNIT.random (),
                         range<float>::UNIT.random (),
                         range<float>::UNIT.random () });
    }

    template <>
    colour&
    randomise (colour &c)
        { return c = random<colour> (); }
}


//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream &os, const util::colour &c) {
    os << "colour(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    return os;
}
