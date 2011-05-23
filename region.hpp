/*
 * This file is part of waif.
 *
 * Waif is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Waif is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with waif.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */


#ifndef __UTIL_REGION_HPP
#define __UTIL_REGION_HPP

/**
 * A pure two-dimensional size, without positioning
 */
template <typename T>
struct _rect {
    T width, height;

    _rect (const T  _width, const T  _height);

    T    area  (void) const;
    bool empty (void) const;

    bool operator ==(const _rect& rhs) const;
    bool operator !=(const _rect& rhs) const
        { return !(*this == rhs); }

    void sanity (void) const;
};

typedef _rect<unsigned int> rect;


/**
 * A two-dimensional rectangle, with size and position.
 */
template <typename T>
struct _region {
    T     x,      y;
    T width, height;

    _region (T _x, T _y, T _width, T _height);

    T    area  (void) const;
    bool empty (void) const;

    bool operator ==(const _region<T>& rhs) const;
    bool operator !=(const _region<T>& rhs) const
        { return !(*this == rhs); }

    void sanity (void) const;
};

typedef _region<unsigned int> region;

#endif 
