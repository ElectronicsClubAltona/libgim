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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TYPES_TRAITS_HPP
#define __UTIL_TYPES_TRAITS_HPP

#include <memory>
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////
template <typename T> struct is_dereferencable     : std::false_type { };
template <typename T> struct is_dereferencable<T*> : std::true_type  { };
template <typename T> struct is_dereferencable<std::unique_ptr<T>> : std::true_type { };
template <typename T> struct is_dereferencable<std::shared_ptr<T>> : std::true_type { };
template <typename T> struct is_dereferencable<std::weak_ptr<T>>   : std::true_type { };


///////////////////////////////////////////////////////////////////////////////
template <typename T> struct dereferenced_type {
    typedef typename std::enable_if<
        std::is_pointer<T>::value,
        std::remove_pointer<T>
    >::type type;
};


//-----------------------------------------------------------------------------
template <typename T> struct dereferenced_type<std::unique_ptr<T>> { typedef T type; };
template <typename T> struct dereferenced_type<std::shared_ptr<T>> { typedef T type; };
template <typename T> struct dereferenced_type<std::weak_ptr<T>>   { typedef T type; };


///////////////////////////////////////////////////////////////////////////////
/// find the unsigned version of a type if one exists
template <typename T>
struct try_unsigned
{
    typedef typename std::make_unsigned<T>::type type;
};


//-----------------------------------------------------------------------------
template <> struct try_unsigned<double> { typedef double type; };
template <> struct try_unsigned<float > { typedef float  type; };


///////////////////////////////////////////////////////////////////////////////
/// find the signed version of a type if one exists
template <typename T>
struct try_signed
{
    typedef typename std::make_signed<T>::type type;
};


//-----------------------------------------------------------------------------
template <> struct try_signed<double> { typedef double type; };
template <> struct try_signed<float > { typedef float  type; };


///////////////////////////////////////////////////////////////////////////////
/// checks if a type can be converted in all cases without modification
template <typename T, typename U> struct is_lossless_cast : std::enable_if<
    std::is_integral<T>::value &&
    std::is_integral<U>::value &&
    std::is_signed<T>::value == std::is_signed<U>::value &&
    sizeof (T) <= sizeof (U),

    std::true_type
>::value { };


///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct func_traits : public func_traits<decltype(&T::operator())>
{ };


//-----------------------------------------------------------------------------
template <typename C, typename R, typename ...Args>
struct func_traits<R(C::*)(Args...) const> {
    typedef R return_type;
};


//-----------------------------------------------------------------------------
template <typename R, typename ...Args>
struct func_traits<R(Args...)> {
    typedef R return_type;
};


///////////////////////////////////////////////////////////////////////////////
template <typename T> struct remove_restrict              { using type = T;  };
template <typename T> struct remove_restrict<T *restrict> { using type = T*; };

template <typename T>
using remove_restrict_t = typename remove_restrict<T>::type;


#endif
