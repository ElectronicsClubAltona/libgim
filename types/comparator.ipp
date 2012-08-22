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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_TYPES_COMPARATOR_IPP
#error "Double inclusion for util/types/comparator.ipp"
#endif

#define __UTIL_TYPES_COMPARATOR_IPP


//-----------------------------------------------------------------------------
namespace util {
    template <typename T>
    bool
    pointer_comparator<T>::operator() (const std::unique_ptr<T> &lhs,
                                       const std::unique_ptr<T> &rhs)
        { return lhs < rhs; }


    template <typename T>
    bool
    pointer_comparator<T>::operator() (const T                  *lhs,
                                       const std::unique_ptr<T> &rhs)
        { return lhs < rhs.get (); }


    template <typename T>
    bool
    pointer_comparator<T>::operator() (const std::unique_ptr<T> &lhs,
                                       const T                  *rhs)
        { return lhs.get () < rhs; }
}

