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


point::point (double _x, double _y):
    x (_x),
    y (_y)
{ ; }


double
point::distance (const point &other) const {
    return sqrt ((x - other.x) * (x - other.x) +
                 (y - other.y) * (y - other.y));
}


double
point::manhattan (const point &other) const {
    return fabs (x - other.x) + 
           fabs (y - other.y);
}
