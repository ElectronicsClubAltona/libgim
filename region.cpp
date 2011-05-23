#include "region.hpp"

#include "debug.hpp"


/*
 * Rect
 */
template <typename T>
_rect<T>::_rect (const T  _width, const T  _height):
        width (_width),
        height (_height)
    { ; }


template <typename T>
T
_rect<T>::area (void) const
    { return width * height; }


template <typename T>
bool
_rect<T>::empty (void) const
    { return area() == 0; }


template <typename T>
bool
_rect<T>::operator ==(const _rect& rhs) const
    { return width == rhs.width && height == rhs.height; }


template <typename T>
void
_rect<T>::sanity (void) const
    { check (width >= 0 && height >= 0); }


// Replace the check, as unsigned types do not ever go negative. This would
// trigger a compile warning if left.
template <>
void
_rect<unsigned int>::sanity (void) const
    { check (true); }


template struct _rect<unsigned int>;

/*
 * Region
 */

template <typename T>
_region<T>::_region (T _x, T _y, T _width, T _height):
    x      (_x),
    y      (_y),
    width  (_width),
    height (_height)
{ ; }


template <typename T>
T 
_region<T>::area (void) const
    { return width * height; }


template <typename T>
bool 
_region<T>::empty (void) const
    { return area () == 0; }


template <typename T>
bool
_region<T>::operator ==(const _region& rhs) const
    { return x      == rhs.x       &&
             y      == rhs.y       &&
             width  == rhs.width   &&
             height == rhs.height; }


template <typename T>
void _region<T>::sanity (void) const
    { check (width >= 0 && height >= 0); }
