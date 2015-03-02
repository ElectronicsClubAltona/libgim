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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */


#include "region.hpp"

#include "debug.hpp"
#include "types/casts.hpp"

#include <cmath>
#include <type_traits>


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (extent_t _extent):
    region (point_t::ORIGIN, _extent)
{ ; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (point_t _point,
                         extent_t _size):
    x (_point.x),
    y (_point.y),
    w (_size.w),
    h (_size.h)
{ ; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (point_t _a,
                         point_t _b):
    region (_a, _b - _a)
{
    CHECK_GE (_b.x, _a.x);
    CHECK_GE (_b.y, _a.y);
}

//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (position_type _x,
                         position_type _y,
                         size_type _w,
                         size_type _h):
    region (point_t {_x, _y}, extent<S,T> {_w, _h})
{ ; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::size_type
util::region<S,T>::area (void) const
{
    return w * h;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::size_type
util::region<S,T>::diameter (void) const
{
    return static_cast<size_type> (std::sqrt (w * w + h * h));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::extent_t
util::region<S,T>::magnitude (void) const
{
    return { w, h };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::extent_t
util::region<S,T>::magnitude (extent_t e)
{
    w = e.w;
    h = e.h;

    return magnitude ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
util::region<S,T>::scale (T factor)
{
    x -= (w * factor - w) / T{2};
    y -= (h * factor - h) / T{2};

    w = w * factor;
    h = h * factor;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
util::region<S,T>::empty (void) const
{
    return almost_zero (area ());
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::rebase (point_t p)
{
    x = p.x;
    y = p.y;

    return base ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::base (void) const
{
    return { x, y };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::away (void) const
{
    return { x + w, y + h };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::centre (void) const
{
    T cx = x + w / T{2},
      cy = y + h / T{2};

    return point_t { cx, cy };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::closest (point_t p) const
{
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
template <size_t S, typename T>
bool
util::region<S,T>::includes (point_t p) const
{
    return p.x >= x &&
           p.y >= y &&
           p.x - x <= w &&
           p.y - y <= h;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
util::region<S,T>::contains (point_t p) const
{
    return p.x > x &&
           p.y > y &&
           p.x - x < w &&
           p.y - y < h;
}


//-----------------------------------------------------------------------------
// FIXME: This will fail with an actual infinite range (NaNs will be generated
// in the conditionals).
template <size_t S, typename T>
bool
util::region<S,T>::intersects (region<S,T> rhs) const
{
    return  x     < rhs.x + rhs.w &&
            rhs.x < x     +     w &&
            y     < rhs.y + rhs.h &&
            rhs.y < y     +     h;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
util::region<S,T>::constrain (point_t &p) const
{
    p.x = std::min (std::max (p.x, x), x + w);
    p.y = std::min (std::max (p.y, y), y + h);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::constrained (point_t p) const
{
    point_t v;
    v.x = std::min (std::max (p.x, x), x + w);
    v.y = std::min (std::max (p.y, y), y + h);

    return v;
}


//-----------------------------------------------------------------------------
template<size_t S, typename T>
util::region<S,T>
util::region<S,T>::intersection (region<S,T> rhs) const
{
    T newx1 = max (x, rhs.x),
      newy1 = max (y, rhs.y),
      newx2 = min (x + sign_cast<T> (w), rhs.x + sign_cast<T> (rhs.w)),
      newy2 = min (y + sign_cast<T> (h), rhs.y + sign_cast<T> (rhs.h));

    if (newx2 < newx1 || newy2 < newy1)
        throw std::logic_error ("No overlap");

    size_type nw = sign_cast<size_type> (newx2 - newx1);
    size_type nh = sign_cast<size_type> (newy2 - newy1);
    return util::region<S,T> (newx1, newy1, nw, nh);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::inset (T mag)
{
    CHECK_GE (w, 2 * mag);
    CHECK_GE (h, 2 * mag);

    return { x + mag, y + mag, w - 2 * mag, h - 2 * mag };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>&
util::region<S,T>::expand (T _w, T _h)
{
    x -= _w;
    y -= _h;
    w += _w * 2;
    h += _h * 2;

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>&
util::region<S,T>::expand (T mag)
{
    return expand (mag, mag);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::expanded (T _w, T _h) const
{
    return {
        x - _w,
        y - _h,
        w + _w * 2,
        h + _h * 2,
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::expanded (T mag) const
{
    return expanded (mag, mag);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::operator+ (vector<S,T> rhs) const
{
    return { x + rhs.x, y + rhs.y, w, h };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::operator- (vector<S,T> rhs) const
{
    return { x - rhs.x, y - rhs.y, w, h };
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
bool
util::region<S,T>::operator== (region rhs) const
{
    return almost_equal (x, rhs.x) &&
           almost_equal (y, rhs.y) &&
           almost_equal (w, rhs.w) &&
           almost_equal (h, rhs.h);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
util::region<S,T>::sanity (void) const {
    CHECK_GE (w, 0);
    CHECK_GE (h, 0);
    static_assert(!std::is_floating_point<T>::value,
                  "Floating point types need width and height checks");
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    void region<2,double>::sanity (void) const {
        CHECK_GE (w, 0);
        CHECK_GE (h, 0);
    }


    template <>
    void region<2,float>::sanity (void) const {
        CHECK_GE (w, 0);
        CHECK_GE (h, 0);
    }
}


///----------------------------------------------------------------------------
/// The largest specifiable finite region.
///
/// Starts at half the minimum value to allow the width to cover some positive
/// range rather than just cancelling out the lowest value for signed types.
///
/// Specifically does not allow infinities. Use/define INFINITE when required.

template <size_t S, typename T>
const util::region<S,T>
util::region<S,T>::MAX (
    std::numeric_limits<T>::lowest () / 2,
    std::numeric_limits<T>::lowest () / 2,
    std::numeric_limits<T>::max (),
    std::numeric_limits<T>::max ()
);


template  <size_t S, typename T>
const util::region<S,T>
util::region<S,T>::UNIT (0, 0, 1, 1);


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const util::region<S,T> &rhs) {
    os << "region(" << rhs.x << ", " << rhs.y << ", " << rhs.w << ", " << rhs.h << ")";
    return os;
}


//-----------------------------------------------------------------------------
namespace util {
    template struct region<2,uint32_t>;
    template struct region<2,uint64_t>;
    template struct region<2,float>;
    template struct region<2,double>;

    template std::ostream& operator<< (std::ostream&, const region<2, int32_t>&);
    template std::ostream& operator<< (std::ostream&, const region<2, int64_t>&);
    template std::ostream& operator<< (std::ostream&, const region<2,uint32_t>&);
    template std::ostream& operator<< (std::ostream&, const region<2,uint64_t>&);
    template std::ostream& operator<< (std::ostream&, const region<2,   float>&);
    template std::ostream& operator<< (std::ostream&, const region<2,  double>&);
}
