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
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#include "colour.hpp"
#include "range.hpp"

using namespace util;


colour::colour (double _red, double _green, double _blue, double _alpha):
    red   (_red),
    green (_green),
    blue  (_blue),
    alpha (_alpha)
{ ; }


namespace util {
    template<>
    colour
    random (void) {
        return colour (range<double>::UNIT.rand (),
                       range<double>::UNIT.rand (),
                       range<double>::UNIT.rand (),
                       range<double>::UNIT.rand ());
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
