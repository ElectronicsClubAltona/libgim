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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#include "colour.hpp"
#include "range.hpp"

using namespace util;


colour&
colour::operator*= (double v) {
    red   *= v;
    green *= v;
    blue  *= v;
    alpha *= v;

    return *this;
}


const json::node&
operator>> (const json::node &node, colour &c) {
    c.red   = node[0].as_number ();
    c.green = node[1].as_number ();
    c.blue  = node[2].as_number ();

    try {
        c.alpha = node[3].as_number ();
    } catch (...) {
        c.alpha = 1.0;
    }

    return node;
}


namespace util {
    template<>
    colour
    random (void) {
        return colour ({ range<double>::UNIT.random (),
                         range<double>::UNIT.random (),
                         range<double>::UNIT.random (),
                         range<double>::UNIT.random () });
    }

    template <>
    colour&
    randomise (colour &c)
        { return c = random<colour> (); }
}


std::ostream&
operator<< (std::ostream &os, const util::colour &c) {
    os << "colour(" << c.red << ", " << c.green << ", " << c.blue << ", " << c.alpha << ")";
    return os;
}
