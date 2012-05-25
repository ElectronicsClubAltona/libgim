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

#ifndef __UTIL_TYPES_TRAITS_HPP
#define __UTIL_TYPES_TRAITS_HPP

template <typename T> struct is_dereferencable     : std::false_type { };
template <typename T> struct is_dereferencable<T*> : std::true_type  { };
template <typename T> struct is_dereferencable<std::unique_ptr<T>> : std::true_type { };
template <typename T> struct is_dereferencable<std::shared_ptr<T>> : std::true_type { };
template <typename T> struct is_dereferencable<std::weak_ptr<T>>   : std::true_type { };
template <typename T> struct is_dereferencable<std::auto_ptr<T>>   : std::true_type { };


template <typename T> struct dereferenced_type {
    typedef typename std::enable_if<
        std::is_pointer<T>::value,
        std::remove_pointer<T>
    >::type type;
};

template <typename T> struct dereferenced_type<std::unique_ptr<T>> { typedef T type; };
template <typename T> struct dereferenced_type<std::shared_ptr<T>> { typedef T type; };
template <typename T> struct dereferenced_type<std::weak_ptr<T>>   { typedef T type; };
template <typename T> struct dereferenced_type<std::auto_ptr<T>>   { typedef T type; };

#endif
