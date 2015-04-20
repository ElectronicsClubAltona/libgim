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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_INTROSPECTION_HPP
#define __UTIL_INTROSPECTION_HPP

#include "variadic.hpp"

#include <cstddef>
#include <string>
#include <tuple>

namespace util {
    ///////////////////////////////////////////////////////////////////////////
    /// Describes a single member variable in a type availabe for introspection
    ///
    /// K: target class
    /// R: member type
    /// M: pointer-to-member
    template <
        class K,
        typename R,
        R K::*M
    >
    struct field
    {
        typedef K klass;
        typedef R type;

        static const std::string name;
        static const R& get (const K &k) { return k.*M; }
        static R get (K &k) { return k.*M; }
    };

    ///////////////////////////////////////////////////////////////////////////
    /// Holds the fields of a type available for introspection
    ///
    /// Specialise the following type struct with a 'fields' tuple of the
    /// members that should be accessed like below:
    ///
    /// struct foo { int a; int b; };
    ///
    /// template <> struct type<foo>
    /// {
    ///     typedef std::tuple<
    ///         field<foo,int,&foo::a>,
    ///         field<foo,int,&foo::b>
    ///     > fields;
    /// };
    /// 
    /// template <> const std::string field<foo,int,&foo::a>::name = "a";
    /// template <> const std::string field<foo,int,&foo::b>::name = "b";

    template <class K>
    struct type { };

    ///////////////////////////////////////////////////////////////////////////
    /// traits class which converts an introspected type to a tuple
    ///
    /// K: target class
    template <
        typename K,
        typename I = typename make_indices<
            std::tuple_size<
                typename type<K>::fields
            >::value
        >::type
    >
    struct type_tuple;


    template <
        typename K,
        size_t ...I
    >
    struct type_tuple<
        K,
        indices<I...>
    >
    {
        typedef std::tuple<
            typename std::tuple_element<
                I,
                typename type<K>::fields
            >::type::type...
        > type;
    };

    ///////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <
            typename K,
            typename I = typename make_indices<
                std::tuple_size<
                    typename type<K>::fields
                >::value
            >::type
        >
        struct _as_tuple;

        template <
            typename K,
            size_t ...I
        >
        struct _as_tuple <
            K,
            indices<I...>
        >
        {
            static typename type_tuple<K>::type
            make (const K &k)
            {
                return std::make_tuple (
                    std::tuple_element<I, typename type<K>::fields>::type::get (k)...
                );
            }
        };
    }

    /// Convert an introspection capable class instance into a tuple instance
    ///
    /// K: source class
    template <typename K>
    auto
    as_tuple (const K &k)
    {
        return detail::_as_tuple<K>::make (k);
    }
}

#endif
