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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_COLOUR_IPP
#error
#endif
#define __UTIL_COLOUR_IPP

template <size_t S, typename T>
template <typename U>
util::colour<S,U>
util::colour<S,T>::cast (void) const
{
    colour<S,U> ret;
    std::transform (this->begin (),
                    this->end (),
                    ret.begin (),
                    renormalise<T,U>);
    return ret;
}
