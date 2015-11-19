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

#ifndef __UTIL_STRONGDEF_HPP
#define __UTIL_STRONGDEF_HPP

#include <limits>

namespace util {
    /// A transparent wrapper around a (typically lightweight) type for the
    /// purposes of overload disambiguation. It acts like a typesafe typedef.
    template <typename T,typename Tag>
    struct strongdef {
    public:
        using value_type = T;

        constexpr strongdef () = default;
        constexpr explicit strongdef (const T &_data): data (_data)    { ; }
        constexpr strongdef (const strongdef &rhs):    data (rhs.data) { ; }

        strongdef& operator= (const strongdef &rhs) { data = rhs.data; return *this; }
        strongdef& operator= (const T &rhs)         { data = rhs;      return *this; }

        operator const T& (void) const { return data; }
        operator       T& (void)       { return data; }

        bool operator== (const strongdef &rhs) const { return data == rhs.data; }

        T data;
    };
}


namespace std {
    template <typename T, typename Tag>
    struct numeric_limits<util::strongdef<T,Tag>> {
        using value_type = typename util::strongdef<T,Tag>::value_type;

        static constexpr bool is_specialized =                  numeric_limits<value_type>::is_specialized;
        static constexpr bool is_signed =                       numeric_limits<value_type>::is_signed;
        static constexpr bool is_integer =                      numeric_limits<value_type>::is_integer;
        static constexpr bool is_exact =                        numeric_limits<value_type>::is_exact;
        static constexpr bool has_infinity =                    numeric_limits<value_type>::has_infinity;
        static constexpr bool has_quiet_NaN =                   numeric_limits<value_type>::has_quiet_NaN;
        static constexpr bool has_signaling_NaN =               numeric_limits<value_type>::has_signaling_NaN;
        static constexpr bool has_denorm =                      numeric_limits<value_type>::has_denorm;
        static constexpr bool has_denorm_loss =                 numeric_limits<value_type>::has_denorm_loss;
        static constexpr std::float_round_style round_style =   numeric_limits<value_type>::round_style;
        static constexpr bool is_iec559 =                       numeric_limits<value_type>::is_iec559;
        static constexpr bool is_bounded =                      numeric_limits<value_type>::is_bounded;
        static constexpr bool is_modulo =                       numeric_limits<value_type>::is_modulo;
        static constexpr int digits =                           numeric_limits<value_type>::digits;
        static constexpr int digits10 =                         numeric_limits<value_type>::digits10;
        static constexpr int max_digits10 =                     numeric_limits<value_type>::max_digits10;
        static constexpr int radix =                            numeric_limits<value_type>::radix;
        static constexpr int min_exponent =                     numeric_limits<value_type>::min_exponent;
        static constexpr int min_exponent10 =                   numeric_limits<value_type>::min_exponent10;
        static constexpr int max_exponent =                     numeric_limits<value_type>::max_exponent;
        static constexpr int max_exponent10 =                   numeric_limits<value_type>::max_exponent10;
        static constexpr bool traps =                           numeric_limits<value_type>::traps;
        static constexpr bool tinyness_before =                 numeric_limits<value_type>::tinyness_before;

        static constexpr value_type min (void)           { return numeric_limits<value_type>::min (); }
        static constexpr value_type lowest (void)        { return numeric_limits<value_type>::lowest (); }
        static constexpr value_type max (void)           { return numeric_limits<value_type>::max (); }
        static constexpr value_type epsilon (void)       { return numeric_limits<value_type>::epsilon (); }
        static constexpr value_type round_error (void)   { return numeric_limits<value_type>::round_error (); }
        static constexpr value_type infinity (void)      { return numeric_limits<value_type>::infinity (); }
        static constexpr value_type quiet_NaN (void)     { return numeric_limits<value_type>::quiet_NaN (); }
        static constexpr value_type signaling_NaN (void) { return numeric_limits<value_type>::signaling_NaN (); }
        static constexpr value_type denorm_min (void)    { return numeric_limits<value_type>::denorm_min (); }
    };
}

#endif
