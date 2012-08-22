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

#ifndef __UTIL_TYPES_COMPARATOR_HPP
#define __UTIL_TYPES_COMPARATOR_HPP

#include <memory>

namespace util {
    template <typename T>
    struct pointer_comparator {
        bool operator() (const std::unique_ptr<T> &lhs, const std::unique_ptr<T> &rhs);
        bool operator() (const T                  *lhs, const std::unique_ptr<T> &rhs);
        bool operator() (const std::unique_ptr<T> &lhs, const T                  *rhs);
    };
}

#include "util/types/comparator.ipp"
#endif
