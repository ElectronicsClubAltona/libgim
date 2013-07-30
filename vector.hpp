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
#include "annotations.hpp"

#include <array>
#include <iostream>
#include <initializer_list>

namespace util {
    template <size_t S>
    struct vector : public detail::coord_data<S> {
        static_assert (S > 0, "vector dimensions must be strictly positive");

        vector ();

        template <typename... T>
        vector (T ...t): detail::coord_data<S> {std::forward<T> (t)...} { ; }

        util::vector<S>  operator* (double) const;
        util::vector<S>& operator*=(double);

        util::vector<S>  operator/ (double) const;
        util::vector<S>& operator/=(double);

        util::vector<S>  operator+ (double) const;
        util::vector<S>& operator+=(double);

        util::vector<S>  operator- (double) const;
        util::vector<S>& operator-=(double);


        util::vector<S>  operator* (const util::vector<S>&) const;
        util::vector<S>& operator*=(const util::vector<S>&);

        util::vector<S>  operator+ (const util::vector<S>&) const;
        util::vector<S>& operator+=(const util::vector<S>&);

        util::vector<S>  operator- (void) const;
        util::vector<S>  operator- (const util::vector<S>&) const;
        util::vector<S>& operator-=(const util::vector<S>&);

        util::vector<S>& operator =(const util::vector <S>&);

        bool operator== (const util::vector <S>&) const;

        double magnitude  (void) const;
        double magnitude2 (void) const;

        double dot (const util::vector<S>&) const;

        util::vector<S>& normalise  (void);
        util::vector<S>  normalised (void) const mustuse;

        bool is_zero (void) const;

        void sanity (void) const;
    };

    vector<2> polar_to_cartesian (const vector<2>&);

    vector<3> cross (const vector<3>&, const vector<3>&);
    vector<3> spherical_to_cartesian (const util::vector <3>&);
    vector<3> cartesian_to_spherical (const util::vector <3>&);

    typedef vector<2> vector2;
    typedef vector<3> vector3;

    template <size_t S> util::vector<S> operator* (double, const util::vector<S>&);
    template <size_t S> util::vector<S> operator+ (double, const util::vector<S>&);
    template <size_t S> util::vector<S> operator- (double, const util::vector<S>&);

    template <size_t S> std::ostream& operator<< (std::ostream&, const util::vector<S>&);

    template <size_t S>
    const json::node& operator>> (const json::node&, util::vector<S>&);

}


#endif

