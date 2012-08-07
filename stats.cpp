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

#include "stats.hpp"

#include <cmath>
#include <limits>


template <typename T>
util::stats::accumulator<T>::accumulator () {
    reset ();
}


template <typename T>
void
util::stats::accumulator<T>::reset (void) {
    count = 0;
    min   = std::numeric_limits<T>::max ();
    max   = std::numeric_limits<T>::min ();
    sum   = 0;
}


template <typename T>
void
util::stats::accumulator<T>::add (T val) {
    min  = std::min (val, min);
    max  = std::max (val, max);
    sum += val;

    ++count;
}


template <typename T>
void
util::stats::accumulator<T>::add (const accumulator<T> &rhs) {
    min    = std::min (rhs.min, min);
    max    = std::max (rhs.max, max);
    sum   += rhs.sum;
    count += rhs.count;
}


template <typename T>
T
util::stats::accumulator<T>::range (void) const
    { return max - min; }


template <typename T>
T
util::stats::accumulator<T>::mean (void) const
    { return sum / count; }


template struct util::stats::accumulator<uint64_t>;
template struct util::stats::accumulator<double>;
template struct util::stats::accumulator<float>;


template <typename T>
std::ostream&
util::stats::operator<< (std::ostream &os, const accumulator<T> &rhs) {
    os << "(min: " << rhs.min << ", max: " << rhs.max << ")";
    return os;
}


template std::ostream& util::stats::operator<< (std::ostream&, const accumulator<uint64_t> &);
template std::ostream& util::stats::operator<< (std::ostream&, const accumulator<double> &);
template std::ostream& util::stats::operator<< (std::ostream&, const accumulator<float > &);

