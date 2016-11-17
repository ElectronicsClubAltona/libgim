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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_INTROSPECTION_HPP
#define __UTIL_INTROSPECTION_HPP

#include "./preprocessor.hpp"
#include "./variadic.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <tuple>

namespace util {
    // XXX: we should be using a const char[] here, but clang-3.9 will not
    // instantiate array values within template specialisations.
    template <typename T>
    struct type_name;

    template <> struct type_name<bool> { static constexpr const char *value = "bool"; };

    template <> struct type_name<char> { static constexpr const char *value = "char"; };
    template <> struct type_name<void*> { static constexpr const char *value = "void*"; };

    template <> struct type_name<  int8_t> { static constexpr const char *value = "int8"; };
    template <> struct type_name< int16_t> { static constexpr const char *value = "int16"; };
    template <> struct type_name< int32_t> { static constexpr const char *value = "int32"; };
    template <> struct type_name< int64_t> { static constexpr const char *value = "int64"; };

    template <> struct type_name< uint8_t> { static constexpr const char *value = "uint8"; };
    template <> struct type_name<uint16_t> { static constexpr const char *value = "uint16"; };
    template <> struct type_name<uint32_t> { static constexpr const char *value = "uint32"; };
    template <> struct type_name<uint64_t> { static constexpr const char *value = "uint64"; };

    template <> struct type_name<float   > { static constexpr const char *value = "float32"; };
    template <> struct type_name<double  > { static constexpr const char *value = "float64"; };

    template <> struct type_name<std::string> { static constexpr const char *value = "string"; };
    template <> struct type_name<char*>       { static constexpr const char *value = "cstring"; };
    template <> struct type_name<const char*> { static constexpr const char *value = "cstring"; };

    template <typename T>
    constexpr
    const char* type_name_v = type_name<T>::value;

    template <typename T>
    const char*
    to_string (void)
    { return type_name_v<T>; }


    ///////////////////////////////////////////////////////////////////////////
    /// Lists valid values of an enumeration
    ///
    /// E: enumeration type
    template <
        typename E
    >
    struct enum_traits {
        /// Specialisations must provide the following constexpr:
        ///
        /// value_type: typename
        /// value_count: size_t
        /// values: static const std::array<value_type,value_count>
    };

    ///////////////////////////////////////////////////////////////////////////
    /// Defines specialisations for introspection data structures for an
    /// enum E, in namespace NS, with variadic values __VA_ARGS__.
    ///
    /// Expects to be caleld from outside all namespaces.

    #define INTROSPECTION_ENUM_DECL(NS,E, ...)                  \
    namespace util {                                            \
        template <>                                             \
        struct enum_traits<::NS::E> {                           \
            using value_type = ::NS::E;                         \
                                                                \
            static constexpr                                    \
            size_t value_count = VA_ARGS_COUNT(__VA_ARGS__);    \
                                                                \
            static constexpr                                    \
            std::array<value_type,value_count>                  \
            values = {                                          \
                MAP1(NAMESPACE_LIST, ::NS::E, __VA_ARGS__)      \
            };                                                  \
                                                                \
            static constexpr                                    \
            std::array<const char*,value_count>                 \
            names = { MAP(STRINGIZE_LIST, __VA_ARGS__) };       \
        };                                                      \
                                                                \
        template <>                                             \
        struct type_name<::NS::E> {                             \
            static constexpr const char ns[] = #NS;             \
            static constexpr const char value[] = #E;           \
        };                                                      \
    }                                                           \


    ///------------------------------------------------------------------------
    /// Declares specialisations for introspection data structures for an
    /// enum E, in namespace NS, with variadic values __VA_ARGS__.
    ///
    /// Expects to be caleld from outside all namespaces.

    #define INTROSPECTION_ENUM_IMPL(NS,E, ...)                  \
    constexpr                                                   \
    std::array<                                                 \
        util::enum_traits<::NS::E>::value_type,                 \
        util::enum_traits<::NS::E>::value_count                 \
    > util::enum_traits<::NS::E>::values;                       \
                                                                \
    constexpr                                                   \
    std::array<                                                 \
        const char*,                                            \
        util::enum_traits<::NS::E>::value_count                 \
    > util::enum_traits<::NS::E>::names;                        \
                                                                \
    constexpr                                                   \
    const char util::type_name<::NS::E>::ns[];                  \
                                                                \
    constexpr                                                   \
    const char util::type_name<::NS::E>::value[];               \


    ///------------------------------------------------------------------------
    /// Defines an istream extraction operator for an enumeration E, within
    /// namespace NS
    ///
    /// Expects to be called from outside all namespaces.
    ///
    /// The user is responsible for specialising the ::util::enum_traits<NS::E>
    /// parameters which are used to drive the implementation (eg, through
    /// INTROSPECTION_ENUM_DECL, and INTROSPECTION_ENUM_IMPL).
    ///
    /// For trivial enumerations INTROSPECTION_ENUM may be easier to use.

    #define INTROSPECTION_ENUM_ISTREAM(NS,E)                    \
    std::istream&                                               \
    ::NS::operator>> (std::istream &is, ::NS::E &e)             \
    {                                                           \
        using traits = util::enum_traits<::NS::E>;              \
                                                                \
        std::string name;                                       \
        is >> name;                                             \
                                                                \
        std::transform (std::begin (name),                      \
                        std::end   (name),                      \
                        std::begin (name),                      \
                        ::toupper);                             \
                                                                \
        auto name_pos = std::find (                             \
            std::cbegin (traits::names),                        \
            std::cend   (traits::names),                        \
            name                                                \
        );                                                      \
                                                                \
        if (name_pos == std::cend (traits::names)) {            \
            is.setstate (std::istream::failbit);                \
        } else {                                                \
            auto d = std::distance (                            \
                std::begin (traits::names),                     \
                name_pos                                        \
            );                                                  \
                                                                \
            e = traits::values[d];                              \
        }                                                       \
                                                                \
        return is;                                              \
    }


    /// Defines an ostream insertion operator for an enumeration E, within
    /// namespace NS.
    ///
    /// Expects to be called from outside all namespaces.
    ///
    /// The user is responsible for specialising the ::util::enum_traits<NS::E>
    /// parameters which are used to drive the implementation (eg, through
    /// INTROSPECTION_ENUM_DECL, and INTROSPECTION_ENUM_IMPL).
    ///
    /// For trivial enumerations INTROSPECTION_ENUM may be easier to use.
    #define INTROSPECTION_ENUM_OSTREAM(NS,E)                    \
    std::ostream&                                               \
    ::NS::operator<< (std::ostream &os, ::NS::E e)              \
    {                                                           \
        using traits = ::util::enum_traits<::NS::E>;            \
                                                                \
        auto val_pos = std::find (                              \
            std::cbegin (traits::values),                       \
            std::cend   (traits::values),                       \
            e                                                   \
        );                                                      \
                                                                \
        if (val_pos == std::cend (traits::values)) {            \
            os.setstate (std::ostream::failbit);                \
        } else {                                                \
            auto d = std::distance (                            \
                std::cbegin (traits::values),                   \
                val_pos                                         \
            );                                                  \
                                                                \
            os << traits::names[d];                             \
        }                                                       \
                                                                \
        return os;                                              \
    }


    /// Defines an enum, its values, associated introspection structures, and
    /// istream and ostream operators.
    /// 
    /// This must be called from outside all namespaces as
    /// INTROSPECTION_ENUM_DECL and INTROSPECTION_ENUM_IMPL need to declare
    /// and define structures outside the user's namespace.
    ///
    /// The enum will be defined inside an inline namespace to simplify the
    /// passing of parameters to functions which require some namespace
    /// prefixing. This shouldn't have a practical effect on user code.

    #define INTROSPECTION_ENUM(E, ...)                      \
    inline namespace detail_intr_enum {                     \
        enum E { __VA_ARGS__ };                             \
        std::ostream& operator<< (std::ostream&, E);        \
        std::istream& operator>> (std::istream&, E&);       \
    }                                                       \
    INTROSPECTION_ENUM_DECL(detail_intr_enum,E,__VA_ARGS__) \
    INTROSPECTION_ENUM_IMPL(detail_intr_enum,E,__VA_ARGS__) \
    INTROSPECTION_ENUM_ISTREAM(detail_intr_enum,E)          \
    INTROSPECTION_ENUM_OSTREAM(detail_intr_enum,E)


    #define INTROSPECTION_ENUM_CLASS(E, ...)                \
    inline namespace detail_intr_enum {                     \
        enum class E { __VA_ARGS__ };                       \
        std::ostream& operator<< (std::ostream&, E);        \
        std::istream& operator>> (std::istream&, E&);       \
    }                                                       \
    INTROSPECTION_ENUM_DECL(detail_intr_enum,E,__VA_ARGS__) \
    INTROSPECTION_ENUM_IMPL(detail_intr_enum,E,__VA_ARGS__) \
    INTROSPECTION_ENUM_ISTREAM(detail_intr_enum,E)          \
    INTROSPECTION_ENUM_OSTREAM(detail_intr_enum,E)


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
        static       R& get (      K &k) { return k.*M; }
        static       R& get (      K &&) = delete;
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

    template <typename K>
    struct type_tuple;

    template <
        typename ...T
    > struct type_tuple<
        std::tuple<T...>
    > {
        typedef std::tuple<T...> type;
    };


    template <
        typename K,
        typename I = typename make_indices<
            std::tuple_size<
                typename type<K>::fields
            >::value
        >::type
    > struct _type_tuple;

    template <
        typename K,
        size_t ...I
    > struct _type_tuple <
        K,
        indices<I...>
    > {
        typedef std::tuple<
            typename std::tuple_element<
                I,
                typename type<K>::fields
            >::type::type...
        > type;
    };


    template <
        typename K
    > struct type_tuple {
        typedef typename _type_tuple<K>::type type;
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

            static auto make (K&&) = delete;
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

    template <typename K>
    auto as_tuple (K &_k)
    {
        const K &k = _k;
        return as_tuple (k);
    }

    template <typename K>
    auto as_tuple (K&&) = delete;
}

#endif
