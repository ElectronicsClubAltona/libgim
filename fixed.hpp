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
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_FIXED_HPP
#define __UTIL_FIXED_HPP

#include "types/bits.hpp"
#include "maths.hpp"
#include "endian.hpp"

#include <ostream>

namespace util {
    template <typename T, unsigned I, unsigned E>
    class [[gnu::packed]] fixed {
    public:
        static_assert (I > 0);
        static_assert (E > 0);
        static_assert ((I + E) % 8 == 0);

        using sint_t = typename bits_type<I+E>::sint;
        using uint_t = typename bits_type<I+E>::uint;

        using native_type = typename std::conditional<
            std::is_signed<T>::value,
            sint_t, uint_t
        >::type;

        using integer_type = typename std::conditional<
            std::is_signed<T>::value,
            typename bits_type<divup (I, 8u) * 8u>::sint,
            typename bits_type<divup (I, 8u) * 8u>::uint
        >::type;

        explicit fixed (double);
        explicit fixed (float);

        double to_double (void) const;
        float  to_float  (void) const;
        integer_type to_integer (void) const;
        native_type  to_native  (void) const;

        static fixed<T,I,E> from_native (native_type);
        static fixed<T,I,E> from_integer (integer_type);

        static integer_type to_integer (native_type);

        fixed<T,I,E>& operator +=(const fixed<T,I,E>);
        fixed<T,I,E>& operator -=(const fixed<T,I,E>);
        fixed<T,I,E>& operator *=(const fixed<T,I,E>);
        fixed<T,I,E>& operator /=(const fixed<T,I,E>);

        fixed<T,I,E>  operator +(const fixed<T,I,E>) const;
        fixed<T,I,E>  operator -(const fixed<T,I,E>) const;
        fixed<T,I,E>  operator *(const fixed<T,I,E>) const;
        fixed<T,I,E>  operator /(const fixed<T,I,E>) const;

        fixed<T,I,E>& operator +=(integer_type);
        fixed<T,I,E>& operator -=(integer_type);
        fixed<T,I,E>& operator *=(integer_type);
        fixed<T,I,E>& operator /=(integer_type);

        fixed<T,I,E>  operator +(integer_type) const;
        fixed<T,I,E>  operator -(integer_type) const;
        fixed<T,I,E>  operator *(integer_type) const;
        fixed<T,I,E>  operator /(integer_type) const;

    private:
        fixed () = default;

        native_type m_value;
    };


    template <typename T, unsigned I, unsigned E>
    constexpr
    fixed<T,I,E>
    bswap (fixed<T,I,E> val)
    {
        return fixed<T,I,E>::from_native (
            bswap (val.to_native ())
        );
    }


    template <typename T, unsigned I, unsigned E> bool operator== (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator!= (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator<  (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator<= (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator>  (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator>= (util::fixed<T,I,E>, util::fixed<T,I,E>);

    template <typename T, unsigned I, unsigned E> bool operator== (util::fixed<T,I,E>, typename util::fixed<T,I,E>::integer_type);
    template <typename T, unsigned I, unsigned E> bool operator!= (util::fixed<T,I,E>, typename util::fixed<T,I,E>::integer_type);
    template <typename T, unsigned I, unsigned E> bool operator<  (util::fixed<T,I,E>, typename util::fixed<T,I,E>::integer_type);
    template <typename T, unsigned I, unsigned E> bool operator<= (util::fixed<T,I,E>, typename util::fixed<T,I,E>::integer_type);
    template <typename T, unsigned I, unsigned E> bool operator>  (util::fixed<T,I,E>, typename util::fixed<T,I,E>::integer_type);
    template <typename T, unsigned I, unsigned E> bool operator>= (util::fixed<T,I,E>, typename util::fixed<T,I,E>::integer_type);

    template <typename T, unsigned I, unsigned E>
    std::ostream& operator<< (std::ostream&, fixed<T,I,E>);
}

#endif // __UTIL_FIXED_HPP
