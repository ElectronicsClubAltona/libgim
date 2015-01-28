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
#include "maths.hpp"

#include <cmath>
#include <cstdlib>

using namespace std;

#if defined(COMPILER_GCC)
    #pragma GCC optimize("-O3")
#endif

//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>&
util::point<S,T>::operator*= (T f) {
    for (auto &i: this->data)
        i *= f;

    return *this;
}


template <size_t S, typename T>
util::point<S,T>
util::point<S,T>::operator* (T f) const {
    util::point<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] * f;
    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
util::point<S,T>::operator/ (T f) const
{
    point<S,T> out;

    std::transform (std::begin (this->data),
                    std::end   (this->data),
                    std::begin (out.data),
                    [f] (auto i) { return i / f; });

    return out;
}

//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
util::point<S,T>::operator- (const vector<S,T> &rhs) const {
    util::point<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs.data[i];
    return out;
}


template <size_t S, typename T>
util::point<S,T>&
util::point<S,T>::operator-= (const util::vector<S,T> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] -= rhs.data[i];
    return *this;
}

//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
util::point<S,T>::operator+ (const vector<S,T> &rhs) const {
    util::point<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] + rhs.data[i];
    return out;
}


template <size_t S, typename T>
util::point<S,T>&
util::point<S,T>::operator+= (const util::vector<S,T> &rhs) {
    for (size_t i = 0; i < S; ++i)
        this->data[i] += rhs.data[i];
    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::point<S,T>::operator- (const point<S,T> &rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs.data[i];
    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::point<S,T>::to (const point<S,T> &rhs) const {
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = rhs.data[i] - this->data[i];
    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
util::point<S,T>::operator== (const util::point<S,T> &rhs) const
{
    return std::equal (std::begin (this->data),
                       std::end   (this->data),
                       std::begin (rhs.data));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
util::point<S,T>::sanity (void) const {
    CHECK (std::all_of (begin (this->data),
                        end   (this->data),
                        [] (double i) { return !std::isnan (i); }));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const util::point<S,T> util::point<S,T>::ORIGIN (T {0});


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
util::operator* (const vector<S,T> &v, const point<S,T> &p) {
    point<S,T> out;
    for (size_t i = 0; i < S; ++i)
        out.data[i] = p.data[i] + v.data[i];

    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
util::operator* (const point<S,T> &p, const vector<S,T> &v)
    { return v * p; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
util::operator* (T a, const point<S,T> &b)
    { return b * a; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const util::point<S,T> &p) {
    os << "point" << S << "(";
    os << p.data[0];

    for (size_t i = 1; i < S; ++i)
        os << ", " << p.data[i];

    os << ")";
    return os;
}


//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)                                                            \
template struct util::point<S,T>;                                                       \
template std::ostream& util::operator<< (std::ostream &os, const util::point<S,T>&);    \
template util::point<S,T> util::operator* (const point<S,T>&, const vector<S,T>&);      \
template util::point<S,T> util::operator* (const vector<S,T>&, const point<S,T>&);      \
template util::point<S,T> util::operator* (T, const point<S,T>&);

#define INSTANTIATE(T)  \
INSTANTIATE_S_T(1,T)    \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)    \
INSTANTIATE_S_T(4,T)


INSTANTIATE(int32_t)
INSTANTIATE(uint32_t)
INSTANTIATE(int64_t)
INSTANTIATE(uint64_t)
INSTANTIATE(float)
INSTANTIATE(double)
