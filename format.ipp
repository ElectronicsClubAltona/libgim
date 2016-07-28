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

#include "./ascii.hpp"
#include "./debug.hpp"
#include "./maths.hpp"

#include <cstring>
#include <cstddef>
#include <sstream>


namespace util { namespace format { namespace detail {
    ///////////////////////////////////////////////////////////////////////////
    // GCC: workaround which allows a throw to appear in constexpr codepaths
    // that do not execute at compile time. See gcc#67371
    template <typename ExceptT, typename ...Args>
    constexpr void
    constexpr_throw [[noreturn]] (Args&& ...args)
    {
        ! true
        ? constexpr_throw<ExceptT> (std::forward<Args> (args)...)
        : throw ExceptT (std::forward<Args> (args)...);
    }


    ///////////////////////////////////////////////////////////////////////////
    // record all formatting information for a specifier.
    //
    // does not record redundant/unneeded information, and will default values
    // like width and precision, so may not guarantee round-trip to/from
    // string specifiers.
    struct specifier {
        bool alternate_form = false;
        bool left_adjusted  = false;
        bool thousands_grouping = false;

        char padding_char = ' ';
        char positive_char  = '\0';

        bool uppercase = false;

        // the rendered base for the value.
        //    8 for octal
        //   16 for hex
        //   10 for decimal
        //
        // other values are theoretically supportable, but do not form part of
        // the printf specification.
        unsigned base = 10;

        enum class repr {
            FIXED,
            SCIENTIFIC,
            AUTO
        } r = repr::AUTO;

        enum class kind {
            UNSIGNED,
            SIGNED,
            REAL,

            STRING,
            POINTER,
            CHARACTER,
            ESCAPE,

            OSTREAM
        } k;

        unsigned width = 0; // field width, ie: how many characters
        int precision = -1; // how many digits after the decimal

        size_t length = 0; // bytesize of underlying type
    };


    ///////////////////////////////////////////////////////////////////////////
    // provides the kind, a conversion specifier, and expected length for a
    // given type.
    //
    // the conversion specifier is only one valid specifier. there may be
    // multiple valid values, eg 'd' and 'i' for signed integers; or 'e', 'f',
    // 'g', and 'a' for reals.
    template <typename T>
    struct specifier_traits {
        static constexpr specifier::kind kind = specifier::kind::OSTREAM;
        static constexpr char conversion = '!';
        static constexpr unsigned length = sizeof (T);
    };

    #define MAKE_SPECIFIER_TRAIT(NATIVE,KIND,CONV)                      \
    template <>                                                         \
    struct specifier_traits<NATIVE> {                                   \
        static constexpr specifier::kind kind = specifier::kind:: KIND; \
        static constexpr char conversion = CONV;                        \
        static constexpr unsigned length = sizeof (NATIVE);             \
    };

    MAKE_SPECIFIER_TRAIT(uint8_t, UNSIGNED,'u');
    MAKE_SPECIFIER_TRAIT(uint16_t,UNSIGNED,'u');
    MAKE_SPECIFIER_TRAIT(uint32_t,UNSIGNED,'u');
    MAKE_SPECIFIER_TRAIT(uint64_t,UNSIGNED,'u');

    MAKE_SPECIFIER_TRAIT(int8_t, SIGNED,'i');
    MAKE_SPECIFIER_TRAIT(int16_t,SIGNED,'i');
    MAKE_SPECIFIER_TRAIT(int32_t,SIGNED,'i');
    MAKE_SPECIFIER_TRAIT(int64_t,SIGNED,'i');

    MAKE_SPECIFIER_TRAIT(float,REAL,'g');
    MAKE_SPECIFIER_TRAIT(double,REAL,'g');

    MAKE_SPECIFIER_TRAIT(char*,STRING,'s');
    MAKE_SPECIFIER_TRAIT(const char*,STRING,'s');
    MAKE_SPECIFIER_TRAIT(const unsigned char*,STRING,'s');

    MAKE_SPECIFIER_TRAIT(char,CHARACTER,'c');
    MAKE_SPECIFIER_TRAIT(void*,POINTER,'p');

    template <size_t N>
    struct specifier_traits<char[N]> {
        static constexpr specifier::kind kind = specifier::kind::STRING;
        static constexpr char conversion = 's';
        static constexpr unsigned length = sizeof (const char*);
    };

    #undef MAKE_SPECIFIER_TRAIT


    ///////////////////////////////////////////////////////////////////////////
    static constexpr
    specifier::kind
    to_kind (const char c)
    {
        switch (c) {
        case 'd':
        case 'i':
            return specifier::kind::SIGNED;

        case 'u':
        case 'o':
        case 'x':
        case 'X':
            return specifier::kind::UNSIGNED;

        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
        case 'a':
        case 'A':
            return specifier::kind::REAL;

        case 'c':
        case 'C':
            return specifier::kind::CHARACTER;

        case 's':
        case 'S':
            return specifier::kind::STRING;

        case 'p':
            return specifier::kind::POINTER;

        case '%':
            return specifier::kind::ESCAPE;

        case '!':
            return specifier::kind::OSTREAM;
        }

        constexpr_throw<syntax_error> ("invalid conversion specifier");
    }


    //-------------------------------------------------------------------------
    inline constexpr
    size_t
    length (specifier::kind k)
    {
        switch (k) {
        case specifier::kind::SIGNED:       return sizeof (int);
        case specifier::kind::UNSIGNED:     return sizeof (unsigned);
        case specifier::kind::REAL:         return sizeof (double);
        case specifier::kind::CHARACTER:    return sizeof (char);
        case specifier::kind::STRING:       return sizeof (const char*);
        case specifier::kind::POINTER:      return sizeof (void*);
        case specifier::kind::ESCAPE:       return 0;
        case specifier::kind::OSTREAM:      return 0;
        }

        unreachable ();
    }


    //-------------------------------------------------------------------------
    inline constexpr
    int
    precision (specifier &s)
    {
        switch (s.k) {
        case specifier::kind::SIGNED:
        case specifier::kind::UNSIGNED:
            return 1;

        case specifier::kind::REAL:
            if (s.base != 16)
                return 6;
            else
                return -1;

        case specifier::kind::STRING:
            return std::numeric_limits<int>::max ();

        case specifier::kind::POINTER:
        case specifier::kind::CHARACTER:
        case specifier::kind::ESCAPE:
        case specifier::kind::OSTREAM:
            return 0;
        }

        unreachable ();
    }


    //-------------------------------------------------------------------------
    inline constexpr
    specifier::repr
    repr (const char c)
    {
        switch (c) {
        case 'e':
        case 'E':
            return specifier::repr::SCIENTIFIC;

        case 'f':
        case 'F':
            return specifier::repr::FIXED;

        default:
            return specifier::repr::AUTO;
        }
    }


    //-------------------------------------------------------------------------
    inline constexpr
    unsigned
    base (const char c)
    {
        switch (c) {
        case 'o':
            return 8;

        case 'x':
        case 'X':
        case 'a':
        case 'A':
           return 16;

        default:
           return 10;
        }
    }


    ///////////////////////////////////////////////////////////////////////////
    // returns the count of specifiers within the range.
    //
    // if the string is not a valid format string the count may be slightly high;
    // particularly in cases of truncation.
    inline constexpr
    size_t
    specifier_count (const char *first, const char *last)
    {
        size_t count = 0;

        for (auto cursor = first; cursor != last; ++cursor) {
            if (*cursor != '%')
                continue;

            ++cursor;
            if (cursor == last)
                return 1;

            ++count;
        }

        return count;
    }


    //-------------------------------------------------------------------------
    template <size_t N>
    inline constexpr
    size_t
    specifier_count (const char (&fmt)[N])
    {
        return specifier_count (fmt, fmt + N);
    }



    ///////////////////////////////////////////////////////////////////////////
    // parses a format specifier into the details struct
    //
    // * first:last must form a contiguous range
    // * first must be the first character of a specifier, ie a %
    //
    // returns the character after the last character of the specifier
    //
    // throws on an invalid specifier (including a truncated specifier)
    constexpr
    const char*
    parse (const char *first, const char *last, specifier &spec)
    {
        if (last - first < 2)
            constexpr_throw<syntax_error> ("specifiers require at least two characters");

        if (*first != '%')
            constexpr_throw<syntax_error> ("specifiers must start with %");

        auto cursor = first + 1;

        // read the format flags
        for (bool more_flags = true; more_flags; ) {
            switch (*cursor) {
            case '#':   spec.alternate_form = true; ++cursor; break;
            case '0':   spec.padding_char   = '0';  ++cursor; break;
            case '-':   spec.left_adjusted  = true; ++cursor; break;
            case ' ':   spec.positive_char  = ' ';  ++cursor; break;
            case '+':   spec.positive_char  = '+';  ++cursor; break;
            // ''', thousands grouping

            default:
                more_flags = false;
                break;
            }
        }

        // read the width
        spec.width = 0;
        while (1) {
            if (!ascii::is_digit (*cursor))
                break;

            spec.width *= 10;
            spec.width += *cursor - '0';
            ++cursor;
        }

        // read the precision
        if (*cursor == '.') {
            ++cursor;
            spec.precision = 0;

            while (1) {
                if (!ascii::is_digit (*cursor))
                    break;

                spec.precision *= 10;
                spec.precision += *cursor - '0';
                ++cursor;
            }
        }

        // read the length modifiers
        // TODO: ensure these values make sense given the conversion
        // specifier about to come.
        switch (*cursor) {
        case 'h': {
            spec.length = sizeof (short);
            ++cursor;

            if (*cursor == 'h') {
                spec.length = sizeof (char);
                ++cursor;
            }

            break;
        }

        case 'l': {
            spec.length = sizeof (long);
            ++cursor;

            if (*cursor == 'l') {
                spec.length = sizeof (long long);
                ++cursor;
            }

            break;
        }

        case 'L': spec.length = sizeof (long double); ++cursor; break;
        case 'j': spec.length = sizeof (uintmax_t);   ++cursor; break;
        case 'z': spec.length = sizeof (size_t);      ++cursor; break;
        case 't': spec.length = sizeof (ptrdiff_t);   ++cursor; break;
        default:
              break;
        }

        // read the conversion specifier
        auto conv = *cursor++;
        spec.k = to_kind (conv);
        spec.length = spec.length ?: length (spec.k);
        spec.uppercase = ascii::is_upper (conv);
        spec.base = base (conv);
        spec.r = repr (conv);
        if (spec.precision < 0)
            spec.precision = precision (spec);

        return cursor;
    }


    //-------------------------------------------------------------------------
    template <size_t N>
    constexpr
    auto
    parse (const char (&fmt)[N], specifier &spec)
    {
        return parse (fmt, fmt + N, spec);
    }



    ///////////////////////////////////////////////////////////////////////////
    template <char Conv>
    struct conversion_traits;

    #define CONVERSION_TRAITS(VALUE,TYPE,UPPER)     \
    template <> struct conversion_traits<VALUE> {   \
        using value_type = TYPE;                    \
        static constexpr bool is_upper = UPPER;     \
    };

    CONVERSION_TRAITS ('d', int, false)
    CONVERSION_TRAITS ('i', int, false)

    CONVERSION_TRAITS ('u', unsigned, false)
    CONVERSION_TRAITS ('o', unsigned, false)
    CONVERSION_TRAITS ('x', unsigned, false)
    CONVERSION_TRAITS ('X', unsigned, true)

    CONVERSION_TRAITS ('e', double, false)
    CONVERSION_TRAITS ('E', double, true)
    CONVERSION_TRAITS ('f', double, false)
    CONVERSION_TRAITS ('F', double, true)
    CONVERSION_TRAITS ('g', double, false)
    CONVERSION_TRAITS ('G', double, true)
    CONVERSION_TRAITS ('a', double, false)
    CONVERSION_TRAITS ('A', double, true)

    CONVERSION_TRAITS ('c', char, false)

    CONVERSION_TRAITS ('s', char*, false)
    CONVERSION_TRAITS ('p', void*, false)

    CONVERSION_TRAITS ('%', void, false)


    ///////////////////////////////////////////////////////////////////////////
    // calculates the length of a fully rendered specifier/value, or format
    // string and parameters. does not include any trailing null.
    template <typename T>
    std::enable_if_t<std::is_arithmetic<T>::value, size_t>
    format_length (const specifier &s, const T &t)
    {
        return digits (t, s.base);
    }


    //-------------------------------------------------------------------------
    inline constexpr
    size_t
    format_length (const specifier&, const char *str)
    {
        auto cursor = str;
        while (*cursor != '\0')
            ++cursor;

        return cursor - str;
    }


    //-------------------------------------------------------------------------
    inline
    size_t
    format_length (const char *first, const char *last)
    {
        size_t length = 0;

        for (auto cursor = first; cursor != last; ++cursor) {
            if (*cursor != '%') {
                ++length;
                continue;
            }

            specifier spec {};
            cursor = parse (cursor, last, spec);

            if (spec.k != specifier::kind::ESCAPE)
                unreachable ();

            ++length;
        }

        return length;
    }


    //-------------------------------------------------------------------------
    template <typename ValueT, typename ...Args>
    inline
    size_t
    format_length (const char *first, const char *last, ValueT value, Args&& ...args)
    {
        size_t length = 0;

        for (auto cursor = first; cursor != last; ++cursor) {
            if (*cursor != '%') {
                ++length;
                continue;
            }

            specifier spec {};
            cursor = parse (cursor, last, spec);

            return
                length +
                format_length (spec, value) + (
                    (spec.k == specifier::kind::ESCAPE) ?
                        format_length (cursor, last, value, std::forward<Args> (args)...) :
                        format_length (cursor, last,        std::forward<Args> (args)...)
                );
        }

        return length;
    }


    //-------------------------------------------------------------------------
    template <typename ...Args, size_t N>
    inline
    size_t
    format_length (const char (&fmt)[N], const Args& ...args)
    {
        if (N <= 1)
            return 0;

        return format_length<Args...> (fmt, fmt + N - 1, args...);
    }


    //-------------------------------------------------------------------------
    template <size_t N>
    inline
    size_t
    format_length (const char (&fmt)[N])
    {
        if (N <= 1)
            return 0;

        return format_length (fmt, fmt + N - 1);
    }


    ///////////////////////////////////////////////////////////////////////////
    // render a single value to an ostream given the parsed specifier

    //-------------------------------------------------------------------------
    // without a provided value we can only write escaped % characters
    template <typename OutputT>
    OutputT
    write (OutputT os, const specifier s)
    {
        if (s.k != specifier::kind::ESCAPE)
            throw missing_error ();

        *os = '%';
        return ++os;
    }


    //-------------------------------------------------------------------------
    template <typename OutputT>
    OutputT
    write (OutputT os, const specifier spec, const char *t)
    {
        if (spec.k != specifier::kind::STRING)
            throw conversion_error ("invalid specifier kind for string argumetn");

        const auto len = spec.precision < 0                  ? spec.precision :
                         (size_t)spec.precision < strlen (t) ? spec.precision :
                                                               strlen (t);

        // perform left padding
        if (spec.width > len && !spec.left_adjusted)
            os = std::fill_n (os, spec.width - len,  spec.padding_char);

        os = std::copy_n (t, len, os);

        // perform right padding
        if (spec.width > len && spec.left_adjusted)
            os = std::fill_n (os, spec.width - len,  spec.padding_char);

        return os;
    }


    //-------------------------------------------------------------------------
    template <typename OutputT>
    OutputT
    write (OutputT os, const specifier spec, const std::string &val)
    {
        return write (os, spec, val.c_str ());
    }


    //-------------------------------------------------------------------------
    template <typename OutputT>
    OutputT
    write (OutputT os, const specifier s, const char t)
    {
        if (s.k != specifier::kind::CHARACTER)
            throw conversion_error ("invalid specifier kind for char argument");

        *os = t;
        return ++os;
    }


    //-------------------------------------------------------------------------
    // if the value isn't a builtin type then, if we asked for an OSTREAM
    // conversion, render to a string and forward as such.
    template <typename OutputT, typename ValueT>
    std::enable_if_t<
        !std::is_fundamental<ValueT>::value && !std::is_pointer<ValueT>::value,
        OutputT
    >
    write (OutputT os, const specifier& spec, const ValueT &val)
    {
        if (spec.k != specifier::kind::OSTREAM)
            throw conversion_error ("invalid conversion specifier for user value");

        std::ostringstream ss;
        ss << val;

        specifier strspec = spec;
        strspec.k = specifier::kind::STRING;
        strspec.length = specifier_traits<std::string>::length;
        return write (os, strspec, ss.str ());
    }


    //-------------------------------------------------------------------------
    template <typename T, typename OutputT>
    std::enable_if_t<
        std::is_pointer<T>::value,
        OutputT
    >
    write (OutputT &os, const specifier &spec, const T t)
    {
        if (spec.k != specifier::kind::POINTER)
            throw conversion_error ("invalid conversion specifier for pointer value");

        // glibc at least uses a special form for null pointers
        auto uint = reinterpret_cast<uintptr_t> (t);
        if (!uint) {
            static const std::string MSG = "(nil)";
            return std::copy (std::cbegin (MSG), std::cend (MSG), os);
        }

        // %p specifiers are an implied %#x or %#lx
        specifier uintspec = spec;
        uintspec.k = specifier::kind::UNSIGNED;
        uintspec.alternate_form = true;
        uintspec.length = sizeof (t);
        uintspec.base = 16;

        return write (os, uintspec, reinterpret_cast<uintptr_t> (t));
    }


    //-------------------------------------------------------------------------
    template <typename OutputT>
    OutputT
    write (OutputT os, const specifier &spec, std::nullptr_t)
    {
        return write (os, spec, (void*)0);
    }


    //-------------------------------------------------------------------------
    template <typename ValueT, typename OutputT>
    std::enable_if_t<
        std::is_integral<ValueT>::value,
        OutputT
    >
    write (OutputT os, const specifier spec, ValueT t)
    {
        if (spec.k == specifier::kind::POINTER && !t)
        {
            return write (os, spec, reinterpret_cast<void*> (t));
        }

        if (spec.k != (std::is_unsigned<ValueT>::value ? specifier::kind::UNSIGNED : specifier::kind::SIGNED))
            throw conversion_error ("invalid conversion specifier for integer value");

        if (spec.length != sizeof (ValueT))
            throw length_error ("incorrect value size");

        const auto numerals   = digits (t, spec.base);
        const auto characters = numerals + (spec.positive_char ? 1 : 0);

        // add any requested positive signifier
        if (spec.positive_char)
            *os++ = spec.positive_char;

        // perform left padding
        if (spec.width > characters && !spec.left_adjusted)
            os = std::fill_n (os, spec.width - characters,  spec.padding_char);

        // write the base prefix
        if (spec.alternate_form) {
            switch (spec.base) {
            case  8:
                *os++ = '0';
                break;

            case 16:
                *os++ = '0';
                *os++ = spec.uppercase ? 'X' : 'x';
                break;

            case 10:
                break;
            }
        }

        // actually write the number in the desired base.
        //
        // as a special case, if the value is zero and precision is zero then the
        // output is blank (though space padding/etc is still preserved).
        if (t != 0 || spec.precision != 0) {
            const char *NUMERALS = spec.uppercase ?
                "0123456789ABCDEF" :
                "0123456789abcdef";

            char buffer[numerals];
            for (auto cursor = buffer; t; t /= spec.base)
                *cursor++ = NUMERALS[t % spec.base];
            std::reverse_copy (buffer, buffer + numerals, os);
        }

        // perform right padding
        if (spec.width > characters && spec.left_adjusted)
            os = std::fill_n (os, spec.width - characters,  spec.padding_char);

        return os;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename OutputT>
    std::enable_if_t<
        std::is_floating_point<T>::value,
        OutputT
    >
    write (OutputT os, const specifier spec, T t)
    {
        if (spec.k != specifier::kind::REAL)
            throw conversion_error ("invalid conversion specifier for real value");

        static const size_t buffer_len = strlen ("+0x") + std::numeric_limits<T>::digits10 + strlen ("e+999") + 1;
        char buffer[buffer_len];

        static const size_t format_len = strlen ("%0-+99.99lle") + 1;
        char format[format_len];

        {
            auto cursor = format;
            *cursor++ = '%';

            if (spec.alternate_form) *cursor++ = '#';
            if (spec.left_adjusted)  *cursor++ = '-';
            if (spec.positive_char)  *cursor++ = spec.positive_char;

            cursor += sprintf (cursor, "%u", spec.width);

            if (spec.precision >= 0) {
                *cursor++ = '.';

                if (spec.precision)
                    cursor += sprintf (cursor, "%i", spec.precision);
            }

            if (spec.r == specifier::repr::SCIENTIFIC)
                *cursor = 'e';
            else if (spec.r == specifier::repr::FIXED)
                *cursor = 'f';
            else if (spec.base == 16)
                *cursor = 'a';
            else
                *cursor = 'g';

            if (spec.uppercase)
                *cursor = ascii::to_upper (*cursor);
            ++cursor;

            *cursor++ = '\0';
        }

        auto len = snprintf (buffer, buffer_len, format, t);
        if (len < 0)
            throw error ("snprintf output error");

        CHECK_LT ((size_t)len, buffer_len);
        return std::copy_n (buffer, len, os);
    }


    //////////////////////////////////////////////////////////////////////////
    template <typename OutputT>
    OutputT
    _render (OutputT os, const char *first, const char *last)
    {
        auto start = std::find (first, last, '%');
        os = std::copy (first, start, os);
        if (start == last)
            return os;

        specifier spec;
        auto cursor = parse (start, last, spec);

        return _render (write (os, spec), cursor, last);
    }


    //------------------------------------------------------------------------
    template <typename ValueT, typename ...Args, typename OutputT>
    OutputT
    _render (OutputT os, const char *first, const char *last, ValueT val, Args&& ...args)
    {
        auto start = std::find (first, last, '%');
        os = std::copy (first, start, os);
        if (start == last)
            return os;

        specifier spec;
        auto cursor = parse (start, last, spec);
        return _render (write (os, spec, val), cursor, last, std::forward<Args> (args)...);
    }


    //------------------------------------------------------------------------
    template <typename ValueT, typename ...Args, typename OutputT, size_t N>
    OutputT
    render (OutputT os, const char (&fmt)[N], ValueT val, Args&& ...args)
    {
        if (N <= 1)
            return os;

        const auto first = fmt;
        const auto last  = fmt + N - 1;

        return _render (os, first, last, val, std::forward<Args> (args)...);
    }


    //------------------------------------------------------------------------
    template <typename OutputT, size_t N>
    OutputT
    render (OutputT os, const char (&fmt)[N])
    {
        if (N <= 1)
            return os;

        auto first = fmt;
        auto last  = fmt + N - 1;

        return _render (os, first, last);
    }
} } }


///////////////////////////////////////////////////////////////////////////////
namespace util { namespace format {
    template <typename ...Args, size_t N>
    std::string
    render (const char (&fmt)[N], const Args& ...args)
    {
        std::string res;
        detail::render (std::back_inserter (res), fmt, args...);
        return res;
    }
} }
