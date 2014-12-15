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

#include "debug.hpp"
#include "types/casts.hpp"

#include <cmath>
#include <type_traits>


//-----------------------------------------------------------------------------
using namespace util;


//-----------------------------------------------------------------------------
template <typename T>
region<T>::region (T _x, T _y, size_type _w, size_type _h):
    x (_x),
    y (_y),
    w (_w),
    h (_h)
{
    DEBUG_ONLY (sanity ());
}


//-----------------------------------------------------------------------------
template <typename T>
typename region<T>::size_type
region<T>::area (void) const
    { return w * h; }


template <typename T>
typename region<T>::size_type
region<T>::diameter (void) const {
    return static_cast<size_type> (std::sqrt (w * w + h * h));
}


template <typename T>
void
region<T>::scale (T factor) {
    x -= (w * factor - w) / T{2};
    y -= (h * factor - h) / T{2};

    w = w * factor;
    h = h * factor;
}


template <typename T>
bool 
region<T>::empty (void) const
    { return almost_equal (area (), 0); }


//-----------------------------------------------------------------------------
template <typename T>
point<2,T>
region<T>::base (void) const {
    return { x, y };
}


template <typename T>
point<2,T>
region<T>::centre (void) const {
    T cx = x + w / T{2},
      cy = y + h / T{2};

    return point<2,T> { cx, cy };
}


template <typename T>
point<2,T>
region<T>::closest (point<2,T> p) const {
    return {
        p.x < x     ? x     :
        p.x > x + w ? x + w :
        p.x,

        p.y < y     ? y     :
        p.y > y + h ? y + h :
        p.y
    };
}


//-----------------------------------------------------------------------------
template <typename T>
bool
region<T>::includes (const point<2,T> &p) const {
    return p.x >= x &&
           p.y >= y &&
           p.x - x <= w &&
           p.y - y <= h;
}


template <typename T>
bool
region<T>::contains (const point<2,T> &p) const {
    return p.x > x &&
           p.y > y &&
           p.x - x < w &&
           p.y - y < h;
}


// FIXME: This will fail with an actual infinite range (NaNs will be generated
// in the conditionals).
template <typename T>
bool
region<T>::overlaps (const region<T> &rhs) const {
    return  x     < rhs.x + rhs.w &&
            rhs.x < x     +     w &&
            y     < rhs.y + rhs.h &&
            rhs.y < y     +     h;
}


//-----------------------------------------------------------------------------
template <typename T>
void
region<T>::constrain (point<2,T> &p) const {
    p.x = std::min (std::max (p.x, x), x + w);
    p.y = std::min (std::max (p.y, y), y + h);
}


template <typename T>
point<2,T>
region<T>::constrained (const point<2,T> &p) const
{
    point<2,T> v;
    v.x = std::min (std::max (p.x, x), x + w);
    v.y = std::min (std::max (p.y, y), y + h);

    return v;
}

//-----------------------------------------------------------------------------
template<typename T>
region<T>
region<T>::overlap (const region<T> &rhs) const {
    T newx1 = max (x, rhs.x),
      newy1 = max (y, rhs.y),
      newx2 = min (x + sign_cast<T> (w), rhs.x + sign_cast<T> (rhs.w)),
      newy2 = min (y + sign_cast<T> (h), rhs.y + sign_cast<T> (rhs.h));

    if (newx2 < newx1 || newy2 < newy1)
        throw std::logic_error ("No overlap");

    size_type nw = sign_cast<size_type> (newx2 - newx1);
    size_type nh = sign_cast<size_type> (newy2 - newy1);
    return region<T> (newx1, newy1, nw, nh);
    
}


//-----------------------------------------------------------------------------
template <typename T>
bool
region<T>::operator== (const region& rhs) const
    { return almost_equal (x, rhs.x) &&
             almost_equal (y, rhs.y) &&
             almost_equal (w, rhs.w) &&
             almost_equal (h, rhs.h); }


//-----------------------------------------------------------------------------
template <typename T>
void
region<T>::sanity (void) const {
    CHECK_SOFT (w > 0);
    CHECK_SOFT (h > 0);
    static_assert(!std::is_floating_point<T>::value,
                  "Floating point types need width and height checks");
}


//-----------------------------------------------------------------------------
// The desired iterator semantics have been difficult to nail down; is it
// edge-inclusive, left-bottom inclusive, purely exclusive, integral only?
// The code has been left here because it was a little annoying to write and
// we're likely to need it again some day.
#if 0
template <typename T>
typename region<T>::iterator&
region<T>::iterator::operator++ (void) {
    if (++x > static_cast<T> (w)) {
        x = a;
        ++y;
    }

    return *this;
}


template <typename T>
typename region<T>::iterator&
region<T>::iterator::operator* (void) {
    return *this;
}


template <typename T>
bool
region<T>::iterator::operator== (const iterator &rhs) const {
    return almost_equal (rhs.x, x) && almost_equal (rhs.y, y);
}


template <typename T>
bool
region<T>::iterator::operator!= (const iterator &rhs) const {
    return !(*this == rhs);
}


template <typename T>
typename region<T>::iterator
region<T>::begin (void) {
    return { x, y, x, w, h };
}


template <typename T>
typename region<T>::iterator
region<T>::end (void) {
    return { x, y + sign_cast<T> (h) + 1, x, w, h };
}
#endif


namespace util {
    template <>
    void region<double>::sanity (void) const {
        CHECK (w >= 0 && h >= 0);
    }


    template <>
    void region<float>::sanity (void) const {
        CHECK (w >= 0 && h >= 0);
    }
}


//-----------------------------------------------------------------------------
template <typename T>
const region<T>
region<T>::MAX (std::numeric_limits<T>::lowest (),
                std::numeric_limits<T>::lowest (),
                std::numeric_limits<T>::has_infinity ? std::numeric_limits<T>::infinity () :
                                                       std::numeric_limits<T>::max (),
                std::numeric_limits<T>::has_infinity ? std::numeric_limits<T>::infinity () :
                                                       std::numeric_limits<T>::max ());


template  <typename T>
const region<T>
region<T>::UNIT (0, 0, 1, 1);


//-----------------------------------------------------------------------------
template <typename T>
std::ostream&
util::operator<< (std::ostream &os, const region<T> &rhs) {
    os << "region(" << rhs.x << ", " << rhs.y << ", " << rhs.w << ", " << rhs.h << ")";
    return os;
}


//-----------------------------------------------------------------------------
namespace util {
    template struct region<int32_t>;
    template struct region<int64_t>;
    template struct region<uint32_t>;
    template struct region<uint64_t>;
    template struct region<float>;
    template struct region<double>;

    template std::ostream& operator<< (std::ostream&, const region< int32_t>&);
    template std::ostream& operator<< (std::ostream&, const region< int64_t>&);
    template std::ostream& operator<< (std::ostream&, const region<uint32_t>&);
    template std::ostream& operator<< (std::ostream&, const region<uint64_t>&);
    template std::ostream& operator<< (std::ostream&, const region<   float>&);
    template std::ostream& operator<< (std::ostream&, const region<  double>&);
}
