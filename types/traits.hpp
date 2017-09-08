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
 * Copyright 2012-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_TYPES_TRAITS_HPP
#define CRUFT_UTIL_TYPES_TRAITS_HPP

#include <memory>
#include <type_traits>


///////////////////////////////////////////////////////////////////////////////
/// applies a series of type-modifiers to a provided type.
///
/// modifiers are applied in the order they are provided.
///
/// use without any modifiers results in the identity modifier.
namespace util {
    //-------------------------------------------------------------------------
    template <typename TypeT, template <typename> class ...ModifierT>
    struct chain;


    //-------------------------------------------------------------------------
    template <typename TypeT>
    struct chain<TypeT> { using type = TypeT; };


    //-------------------------------------------------------------------------
    template <
        typename TypeT,
        template <typename> class HeadT,
        template <typename> class ...TailT
    >
    struct chain<TypeT, HeadT, TailT...> : public chain<
        typename HeadT<TypeT>::type,
        TailT...
    > { };


    //-------------------------------------------------------------------------
    template <typename T, template <typename> class ...Args>
    using chain_t = typename chain<T, Args...>::type;
}


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
template <typename T> struct remove_restrict              { using type = T;  };
template <typename T> struct remove_restrict<T *restrict> { using type = T*; };

template <typename T>
using remove_restrict_t = typename remove_restrict<T>::type;


///////////////////////////////////////////////////////////////////////////////
/// removes the noexcept type specifier from invokable types
namespace detail {
    template <typename T>
    struct remove_noexcept
    { using type = T; };


    //-------------------------------------------------------------------------
    template <typename ResultT, typename ...Args>
    struct remove_noexcept<ResultT(&)(Args...) noexcept> {
        using type = ResultT(&)(Args...);
    };


    //-------------------------------------------------------------------------
    template <typename ResultT, typename ...Args>
    struct remove_noexcept<ResultT(*const)(Args...) noexcept> {
        using type = ResultT(*const)(Args...);
    };


    //-------------------------------------------------------------------------
    template <typename ResultT, typename ...Args>
    struct remove_noexcept<ResultT(*)(Args...) noexcept> {
        using type = ResultT(*)(Args...);
    };


    //-------------------------------------------------------------------------
    template <typename ClassT, typename ResultT, typename ...Args>
    struct remove_noexcept<ResultT(ClassT::*)(Args...) noexcept> {
        using type = ResultT(ClassT::*)(Args...);
    };


    //-------------------------------------------------------------------------
    template <typename ClassT, typename ResultT, typename ...Args>
    struct remove_noexcept<ResultT(ClassT::*)(Args...) const noexcept> {
        using type = ResultT(ClassT::*)(Args...) const;
    };
};


//-----------------------------------------------------------------------------
template <typename T>
struct remove_noexcept : public detail::remove_noexcept<T> { };

//-----------------------------------------------------------------------------
template <typename T>
using remove_noexcept_t = typename remove_noexcept<T>::type;


///////////////////////////////////////////////////////////////////////////////
/// removes any `const' qualifier from the supplied member function
template <typename FuncT>
struct remove_member_const {
    using type = FuncT;
};


//-----------------------------------------------------------------------------
template <typename ClassT, typename ReturnT, typename ...Args>
struct remove_member_const<ReturnT(ClassT::*const)(Args...) const> {
    using type = ReturnT(ClassT::*const)(Args...);
};


//-----------------------------------------------------------------------------
template <typename ClassT, typename ReturnT, typename ...Args>
struct remove_member_const<ReturnT(ClassT::*)(Args...) const> {
    using type = ReturnT(ClassT::*)(Args...);
};


///////////////////////////////////////////////////////////////////////////////
/// type traits class for querying invokable type return values and argument
/// types.
///
/// if the type is invokable the alias `return_type' will be defined for the
/// return type, and the alias tuple `argument_types' will be defined for the
/// arguments;
namespace detail {
    template <typename T>
    struct func_traits { };


    //-------------------------------------------------------------------------
    template <typename ClassT, typename ResultT, typename ...Args>
    struct func_traits<ResultT(ClassT::*)(Args...)> {
        using return_type = ResultT;
        using argument_types = std::tuple<Args...>;
    };


    //-------------------------------------------------------------------------
    template <typename ResultT, typename ...Args>
    struct func_traits<ResultT(*)(Args...)>  {
        using return_type = ResultT;
        using argument_types = std::tuple<Args...>;
    };


    //-------------------------------------------------------------------------
    template <typename ResultT, typename ...Args>
    struct func_traits<ResultT(&)(Args...)>  {
        using return_type = ResultT;
        using argument_types = std::tuple<Args...>;
    };
};


//-----------------------------------------------------------------------------
template <typename T>
struct func_traits : public ::detail::func_traits<
    // we apply as many transforms as possible before palming it off to the
    // detail class so that we don't have to write as many distinct cases.
    ::util::chain_t<T,
        std::remove_cv,
        std::decay,
        remove_member_const,
        remove_noexcept
    >
> {
    // we may as well record the underlying type here. it might prove useful
    // to someone.
    using type = T;
};


#endif
