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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#ifdef __UTIL_POLYNOMIAL_IPP
#error "twice included ipp"
#endif

#define __UTIL_POLYNOMIAL_IPP


//-----------------------------------------------------------------------------
template <size_t S, typename T, typename U>
T
util::polynomial::eval (const std::array<T,S> coeffs, const U x)
{
    U x_ = 1.f;
    T sum {0.f};

    for (size_t i = 0; i < S; ++i) {
        sum += coeffs[S-i-1] * x_;
        x_ *= x;
    }

    return sum;
}
