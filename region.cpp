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


#include "region.hpp"

#include "enable_if.hpp"
#include "debug.hpp"

#include <cmath>
#include <type_traits>

using namespace util;


template <typename T>
region<T>::region (T _x, T _y, T _w, T _h):
    x (_x),
    y (_y),
    w (_w),
    h (_h)
{
}


template <typename T>
region<T>&
region<T>::operator+= (const vector<2> &rhs) {
    x += rhs.x;
    y += rhs.y;

    return *this;
}


template <typename T>
T 
region<T>::area (void) const
    { return w * h; }


template <typename T>
T 
region<T>::diameter (void) const {
    return (T)sqrt (w * w + h * h);
}


template <typename T>
bool 
region<T>::empty (void) const
    { return almost_equal (area (), 0); }


template <typename T>
point<2>
region<T>::base (void) const {
    return { static_cast<double> (x), static_cast<double> (y) };
}


template <typename T>
point<2>
region<T>::centre (void) const {
    double cx = x + static_cast<T>(w / 2.0),
           cy = y + static_cast<T>(h / 2.0);

    return { cx, cy };
}


template <typename T>
bool
region<T>::includes (const point<2> &p) const {
    return p.x >= x &&
           p.y >= y &&
           p.x <= x + w &&
           p.y <= y + h;
}


template <typename T>
bool
region<T>::contains (const point<2> &p) const {
    return p.x > x &&
           p.y > y &&
           p.x < x + w &&
           p.y < y + h;
}


template <typename T>
bool
region<T>::overlaps (const region<T> &rhs) const {
    //return !overlap (rhs).empty ();

    return x     < rhs.x + rhs.w && 
           x + w > rhs.x &&
           y     < rhs.y + rhs.h &&
           y + h > rhs.y;
}


template<typename T>
region<T>
region<T>::overlap (const region<T> &rhs) const {
    double newx1 = max (x, rhs.x),
           newy1 = max (y, rhs.y),
           newx2 = min (x + w, rhs.x + w),
           newy2 = min (y + h, rhs.y + h);

    return region<T> (newx1, newy1, newx2 - newx1, newy2 - newy1);
    
}


template <typename T>
bool
region<T>::operator ==(const region& rhs) const
    { return almost_equal (x, rhs.x) &&
             almost_equal (y, rhs.y) &&
             almost_equal (w, rhs.w) &&
             almost_equal (h, rhs.h); }


template <typename T>
void region<T>::sanity (void) const
    { CHECK (w >= 0 && h >= 0); }


namespace util {
    template <>
    void region<unsigned int>::sanity (void) const
        { return; }


    template <>
    void region<unsigned long>::sanity (void) const
        { return; }
}


template <typename T>
std::ostream&
operator<< (std::ostream &os, const region<T> &rhs) {
    os << "region(" << rhs.x << ", " << rhs.y << ", " << rhs.w << ", " << rhs.h << ")";
    return os;
}


template struct region<unsigned int>;
template struct region<unsigned long>;
template struct region<double>;

template std::ostream& operator<< (std::ostream&, const region<unsigned int>&);
template std::ostream& operator<< (std::ostream&, const region<double>&);

