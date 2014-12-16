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

#ifndef __UTIL_VECTOR_HPP
#define __UTIL_VECTOR_HPP

#include "json.hpp"
#include "detail/coord.hpp"

#include <array>
#include <iostream>
#include <initializer_list>

namespace util {
    template <size_t S, typename T>
    struct vector : public detail::coord<S, T> {
        static_assert (S > 0, "vector dimensions must be strictly positive");

        vector ();

        template <typename... U>
        explicit vector (U ...u): detail::coord<S, T> {std::forward<U> (u)...} { ; }

        // arithmetic operators
        vector<S,T>  operator* (T) const;
        vector<S,T>& operator*=(T);

        vector<S,T>  operator/ (T) const;
        vector<S,T>& operator/=(T);

        vector<S,T>  operator+ (T) const;
        vector<S,T>& operator+=(T);

        vector<S,T>  operator- (T) const;
        vector<S,T>& operator-=(T);

        // element operators
        vector<S,T>  operator* (const vector<S,T>&) const;
        vector<S,T>& operator*=(const vector<S,T>&);

        vector<S,T>  operator+ (const vector<S,T>&) const;
        vector<S,T>& operator+=(const vector<S,T>&);

        vector<S,T>  operator- (void) const;
        vector<S,T>  operator- (const vector<S,T>&) const;
        vector<S,T>& operator-=(const vector<S,T>&);

        vector<S, T>& operator =(const vector<S,T>&);

        // logical operators
        bool operator== (const vector<S,T>&) const;

        bool is_zero (void) const;

        // vector operators
        T magnitude  (void) const;
        T magnitude2 (void) const;

        T dot (const vector<S,T>&) const;

        vector<S,T>& normalise  (void);
        vector<S,T>  normalised [[gnu::warn_unused_result]] (void) const;

        template <size_t D> vector<D,T> redim (void) const;

        void sanity (void) const;
    };

    // free vector operators
    template <typename T> vector<2,T> polar_to_cartesian (const vector<2,T>&);

    template <typename T> vector<3,T> cross (const vector<3,T>&, const vector<3,T>&);
    template <typename T> vector<3,T> spherical_to_cartesian (const vector<3,T>&);
    template <typename T> vector<3,T> cartesian_to_spherical (const vector<3,T>&);

    template <size_t S, typename T> vector<S,T> operator* (T, const vector<S,T>&);
    template <size_t S, typename T> vector<S,T> operator+ (T, const vector<S,T>&);
    template <size_t S, typename T> vector<S,T> operator- (T, const vector<S,T>&);

    // output and serialisation operators
    template <size_t S, typename T> std::ostream& operator<< (std::ostream&, const vector<S,T>&);

    template <size_t S, typename T>
    const json::node& operator>> (const json::node&, vector<S,T>&);

    // convenience typedefs
    typedef vector<2,float> vector2f;
    typedef vector<3,float> vector3f;
    typedef vector<3,float> vector4f;

    typedef vector<2,double> vector2d;
    typedef vector<3,double> vector3d;
    typedef vector<3,double> vector4d;
}


#endif

