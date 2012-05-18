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

#include "debug.hpp"

#include <cmath>

using namespace std;


template <size_t S>
util::point<S>::point ()
{ ; }


template <size_t S>
double
util::point<S>::distance (const util::point<S> &other) const {
    return sqrt (distance2 (other));
}


template <size_t S>
double
util::point<S>::distance2 (const util::point<S> &other) const {
    double total = 0.0;

    for (size_t i = 0; i < S; ++i)
        total += pow2 (this->data[i] - other.data[i]);
    return total;
}



template <size_t S>
double
util::point<S>::manhattan (const util::point<S> &other) const {
    double total = 0.0;

    for (size_t i = 0; i < S; ++i)
        total += fabs (this->data[i] - other.data[i]);
    return total;
}


template <size_t S>
util::point<S>&
util::point<S>::operator*= (double f) {
    for (double &i: this->data)
        i *= f;

    return *this;
}


template <size_t S>
util::point<S>
util::point<S>::operator* (double f) const {
    util::point<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] * f;
    return out;
}


template <size_t S>
util::point<S>
util::point<S>::operator+ (const util::vector<S> &rhs) const {
    util::point<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] + rhs.data[i];
    return out;
}


template <size_t S>
util::point<S>&
util::point<S>::operator+= (const util::vector<S> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] += rhs.data[i];
    return *this;
}


template <size_t S>
util::point<S>
util::point<S>::operator- (const util::point<S> &rhs) const {
    util::point<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs.data[i];
    return out;
}


template <size_t S>
util::vector<S>
util::point<S>::to (const util::point<S> &rhs) const {
    util::vector<S> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = rhs.data[i] - this->data[i];
    return out;
}


template <size_t S>
void
util::point<S>::sanity (void) const {
    CHECK_SOFT (std::all_of (begin (this->data),
                             end   (this->data),
                             [] (double i) { return !std::isnan (i); }));
}


template <size_t S>
std::ostream&
util::operator<< (std::ostream &os, const util::point<S> &p) {
    os << "point" << S << "(";
    os << p.data[0];

    for (size_t i = 1; i < S; ++i)
        os << ", " << p.data[i];

    os << ")";
    return os;
}


template struct util::point<1>;
template struct util::point<2>;
template struct util::point<3>;
