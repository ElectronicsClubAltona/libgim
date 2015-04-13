/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

