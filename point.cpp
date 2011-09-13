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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "point.hpp"

#include <cmath>

using namespace std;
using namespace util;


// Unused components are zeroed so that higher dimensional operations can
// operate without fear of triggering NaNs or other such complications.
point::point (double _x, double _y, double _z):
    x (_x),
    y (_y),
    z (_z)
{ ; }


double
point::distance (const point &other) const {
    return sqrt ((x - other.x) * (x - other.x) +
                 (y - other.y) * (y - other.y) +
                 (z - other.z) * (z - other.z));
}


double
point::manhattan (const point &other) const {
    return fabs (x - other.x) + 
           fabs (y - other.y) +
           fabs (z - other.z);
}


point&
point::operator+= (const point &rhs) {
    x += rhs.x;
    y += rhs.y;

    return *this;
}


point
point::operator- (const point &rhs) const
    { return point (x - rhs.x, y - rhs.y); }


std::ostream&
operator<< (std::ostream &os, const point &p) {
    os << "point(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}
