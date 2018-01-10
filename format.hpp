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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_FORMAT_HPP
#define CRUFT_UTIL_FORMAT_HPP

#include "view.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <vector>

namespace util::format {
    /// denotes the stated data type of one specifier
    enum class type_t {
        /// an internal type that indicates a span of literal text to copy
        /// from the format specifier string
        LITERAL,
        /// a type that has implemented an ostream operator
        USER,

        /// a literal '%' symbol
        ESCAPE,

        /// numeric types
        SIGNED,
        UNSIGNED,
        REAL,

        /// a C style string, or equivalent C++ type (std::string,
        /// std::string_view, util::view, etc, ...)
        STRING,
        /// a single character
        CHAR,

        /// a raw pointer (rather than value that needs to be dereferenced)
        POINTER,

        /// number of characters written
        COUNT
    };

    /// formatting information for a single specifier.
    ///
    /// TODO: investigate using a proper tagged union to compress the data size
    struct specifier {
        /// the sub-region of the format specifier that we parsed for this
        /// information. probably only useful for the LITERAL type as we just
        /// copy the view into the output buffer directly.
        util::view<const char*> fmt = util::view<const char*> {nullptr};

        int parameter = -1;

        struct {
            bool plus = false;
            bool minus = false;
            bool space = false;
            bool zero = false;
            bool hash = false;
        } flags;

        int width = -1;

        int precision = -1;

        int length = -1;

        type_t type = type_t::USER;

        bool upper = false;
        int base = 10;
        enum {
            FIXED,
            SCIENTIFIC,
            DEFAULT,
            HEX,
        } representation = DEFAULT;
    };


    struct parsed;
    template <typename ...Args> class bound;
    template <typename ...Args> class stored;

    /// a sequence of parsed specifiers that can be used to render some
    /// collection parameters in the future.
    struct parsed {
        std::vector<specifier> m_specifiers;

        auto begin (void) const { return std::begin (m_specifiers); }
        auto end   (void) const { return std::end   (m_specifiers); }

        /// records a complete collection of parameters for rendering in the
        /// future. the caller must maintain the 'parsed' object for the
        /// lifetime of the return value.
        template <typename ...Args>
        bound<Args...>
        operator () (const Args &...args) &;

        /// records a complete collection of parameters for rendering in the
        /// future. takes ownership of the specifiers so there is no lifetime
        /// requirement.
        template <typename ...Args>
        stored<Args...>
        operator () (const Args &...args) &&;
    };


    /// parameter collection for a non-owning sequence of specifiers
    template <typename ...ValueT>
    class bound {
    public:
        bound (const parsed &_parsed, const ValueT &...args):
            m_parsed {_parsed},
            m_values {args...}
        { ; }

        auto specifiers (void) const
        { return util::make_view (m_parsed.m_specifiers); }

        template <size_t Index>
        auto
        get (void) const& { return std::get<Index> (m_values); }

    private:
        const parsed &m_parsed;
        std::tuple<const ValueT&...> m_values;
    };


    /// parameter collection for an owning squence of specifiers.
    template <typename ...ValueT>
    class stored {
    public:
        stored (std::vector<specifier> &&_specifiers, const ValueT &...args):
            m_specifiers {std::move (_specifiers)},
            m_values {args...}
        { ; }

        auto
        specifiers (void) const&
        {
            return util::make_view (m_specifiers);
        }

        template <size_t Index>
        const auto&
        get (void) const& { return std::get<Index> (m_values); }

    private:
        std::vector<specifier> m_specifiers;
        std::tuple<const ValueT&...> m_values;
    };


    template <typename ...Args>
    bound<Args...>
    parsed::operator () (const Args &...args) &
    { return bound { *this, args... }; }


    template <typename ...Args>
    stored<Args...>
    parsed::operator () (const Args &...args) &&
    {
        return stored { std::move (m_specifiers), args... };
    }


    /// parses a format string in the style of std::printf
    ///
    /// if the format specifier is invalid the function will throw an error at
    /// runtime. specifically does not make allowances for constexpr
    /// validation.
    parsed printf (util::view<const char*>);


    /// parses a format specifier in the style of PEP3101 (with the notable
    /// exception of named parameters).
    ///
    /// in the event of a parsing error the function will throw. makes no
    /// attempt to cater for constexpr validation.
    parsed python (util::view<const char*>);


    /// parses a printf format string and binds parameters for rendering.
    template <typename ...Args>
    auto
    printf (util::view<const char*> fmt, const Args &...args)
    {
        return printf (fmt) (args...);
    }


    /// parses a python format string and binds parameters for rendering.
    template <typename ...Args>
    auto
    python (util::view<const char*> fmt, const Args &...args)
    {
        return python (fmt) (args...);
    }


    template <typename ValueT>
    struct value {
        static std::ostream&
        write (std::ostream &os, specifier spec, const ValueT &val)
        {
            os << std::resetiosflags (~std::ios_base::fmtflags{});

            switch (spec.type) {
            case type_t::REAL:
                if (!std::is_floating_point_v<ValueT>)
                    throw std::runtime_error ("expected real value");
                break;

            case type_t::UNSIGNED:
                if (!std::is_unsigned_v<ValueT>)
                    throw std::runtime_error ("expected unsigned value");
                break;

            case type_t::SIGNED:
                if (!std::is_signed_v<ValueT>)
                    throw std::runtime_error ("expected signed value");
                break;

            case type_t::STRING:
                if (!std::is_same_v<ValueT, util::view<const char*>> && !std::is_same_v<ValueT, std::string>)
                    throw std::runtime_error ("expected string value");
                break;

            case type_t::POINTER:
                if (!std::is_pointer_v<ValueT> && !std::is_integral_v<ValueT>)
                    throw std::runtime_error ("expected pointer value");
                break;

            case type_t::CHAR:
                if (!std::is_same_v<ValueT, char> &&
                    !std::is_same_v<ValueT, wchar_t> &&
                    !std::is_same_v<ValueT, char16_t> &&
                    !std::is_same_v<ValueT, char32_t> &&
                    !std::is_same_v<ValueT, signed char> &&
                    !std::is_same_v<ValueT, unsigned char>)
                    throw std::runtime_error ("expected character value");
                break;

            case type_t::COUNT:
                if (!std::is_pointer_v<ValueT> && !std::is_reference_v<ValueT>)
                    if (!std::is_integral_v<std::remove_pointer_t<std::remove_reference_t<ValueT>>>)
                        throw std::runtime_error ("expected pointer/reference to integral");
                break;

            case type_t::USER:
                break;

            case type_t::ESCAPE:
            case type_t::LITERAL:
                break;
            }

            // easy case where we just throw it to ostream
            if (spec.type == type_t::USER)
                return os << val;

            if (spec.length > 0 && sizeof (val) != spec.length)
                throw std::runtime_error ("mismatched argument size");

            const bool uses_space = std::is_arithmetic_v<ValueT> && spec.flags.space && !spec.flags.plus;
            if (uses_space)
                    os << ' ';

            if (spec.flags.plus)
                os << std::showpos;
            if (spec.flags.minus)
                os << std::left;
            if (spec.flags.zero)
                os << std::setfill ('0');

            if (spec.base >= 0) {
                switch (spec.base) {
                case 10: os << std::dec; break;
                case 16: os << std::hex; break;
                case  8: os << std::oct; break;
                default:
                    throw std::runtime_error ("unhandled numeric base");
                }
            }

            if (spec.precision >= 0) {
                os << std::setprecision (spec.precision);
            }

            if (spec.width >= 0)
                os << std::setw (spec.width - (uses_space ? 1 : 0));

            if (spec.upper)
                os << std::uppercase;

            if (spec.type == type_t::UNSIGNED || spec.type == type_t::SIGNED)
                if (spec.flags.hash)
                    os << std::showbase;
            if (spec.type == type_t::REAL)
                if (spec.flags.hash)
                    os << std::showpoint;

            if (spec.type == type_t::REAL) {
                switch (spec.representation) {
                case specifier::FIXED:      os << std::fixed; break;
                case specifier::SCIENTIFIC: os << std::scientific; break;
                case specifier::DEFAULT:    os << std::defaultfloat; break;
                case specifier::HEX:        os << std::hexfloat; break;
                }
            }

            if constexpr (std::is_integral_v<ValueT>) {
                if (spec.type == type_t::POINTER) {
                    if (!val)
                        return os << "(nil)";
                    return os << reinterpret_cast<const void*> (val);
                }
            }

            if constexpr (std::is_floating_point_v<ValueT>) {
                if (spec.type == type_t::REAL) {
                    if (std::isnan (val))
                        return os << (spec.upper ? "NAN" : "nan");
                    if (std::isinf  (val))
                        return os << (spec.upper ? "INF" : "inf");
                }
            }

            if constexpr (std::is_integral_v<ValueT>) {
                if (spec.type == type_t::SIGNED || spec.type == type_t::UNSIGNED) {
                    // explicitly handle the zero width case as blank because
                    // there's no easy way to do this using iomanip.
                    if (spec.precision == 0 && !val) {
                        return os;
                    }
                }
            }

            if constexpr (std::is_same_v<util::view<const char*>, ValueT>) {
                if (spec.precision >= 0) {
                    std::copy_n (
                        std::begin (val),
                        util::min (spec.precision, static_cast<int> (val.size ())),
                        std::ostream_iterator<char> (os)
                    );
                    return os;
                }
            }

            // the final output calls. we need to use unary plus so that
            // chars get promoted to ints for correct stream rendering when
            // the intention is to output a number.
            if constexpr (std::is_fundamental_v<ValueT>) {
                if (spec.type == type_t::CHAR)
                    return os <<  val;
                if constexpr (!std::is_null_pointer_v<ValueT>)
                    if (spec.type != type_t::USER)
                        return os << +val;
            }

            return os << val;
        }
    };


    template <typename ValueT>
    struct value<const ValueT*> {
        static std::ostream&
        write (std::ostream &os, specifier spec, const ValueT *val)
        {
            if (spec.type != type_t::POINTER && spec.type != type_t::USER)
                throw std::runtime_error ("expected pointer specification");

            if (!val)
                return os << "(nil)";
            return os << reinterpret_cast<const void*> (val);
        }
    };


    template <typename ValueT>
    struct value<ValueT*> {
        static std::ostream&
        write (std::ostream &os, specifier spec, ValueT *val) {
            return value<const ValueT*>::write (os, spec, val);
        }
    };


    template <>
    struct value<std::nullptr_t> {
        static std::ostream&
        write (std::ostream &os, specifier s, const std::nullptr_t &val)
        {
            if (s.type != type_t::POINTER || s.type == type_t::USER)
                throw std::runtime_error ("expected pointer specifier");
            return value<const void*>::write (os, s, val);
        }
    };


    template <size_t N>
    struct value<const char[N]> {
        static std::ostream&
        write (std::ostream &os, specifier spec, const char (&val)[N]) {
            if (spec.type == type_t::STRING || spec.type == type_t::USER)
                return value<util::view<const char*>>::write (os, spec, util::view<const char*> (val));
            throw std::runtime_error ("invalid data type");
        }
    };


    template <size_t N>
    struct value<char[N]> {
        static std::ostream&
        write (std::ostream &os, specifier spec, const char (&val)[N]) {
            return value<util::view<const char*>>::write (os, spec, util::view<const char*> (val));
        }
    };


    template <>
    struct value<char*> {
        static std::ostream&
        write (std::ostream &os, specifier spec, char *val) {
            if (!val)
                return os << "(nil)";
            if (spec.type == type_t::STRING || spec.type == type_t::USER)
                return value<util::view<const char*>>::write (os, spec, util::view<const char*> { val, val + strlen (val) });
            if (spec.type == type_t::POINTER)
                return value<const void*>::write (os, spec, val);
            throw std::runtime_error ("invalid data type");
        }
    };


    template <>
    struct value<const char*> {
        static std::ostream&
        write (std::ostream &os, specifier spec, const char *val) {
            if (!val)
                return os << "(nil)";
            if (spec.type == type_t::STRING || spec.type == type_t::USER)
                return value<util::view<const char*>>::write (os, spec, util::view<const char*> { val, val + strlen (val) });
            if (spec.type == type_t::POINTER)
                return value<const void*>::write (os, spec, val);
            throw std::runtime_error ("invalid data type");
        }
    };


    template <>
    struct value<const std::string&> {
        static std::ostream&
        write (std::ostream &os, specifier spec, const std::string &val) {
            return value<util::view<const char*>>::write (
                os, spec, util::view<const char*> (val.data (), val.data () + val.size ())
            );
        }
    };

    template <>
    struct value<std::string&> {
        static std::ostream&
        write (std::ostream &os, specifier spec, std::string &val) {
            return value<const std::string&>::write (os, spec, val);
        }
    };


    /// renders an LITERAL specifiers followed by one parameter, then
    /// recurses for any following specifiers.
    ///
    /// \tparam Index       the index of the next parameter to render
    /// \tparam SpecifierT  a forward iterator container
    /// \tparam DataT       a tuple-like object template class
    /// \tparam Args        a paramater pack of all parameter types
    ///
    /// \param os           the ostream that we render to
    /// \param specifiers   the sequence of all specifiers to be rendered
    /// \param data         a tuple-like object containing references to all parameters
    template <int Index,  typename SpecifiersT, template <typename...> class HolderT, typename ...DataT>
    static std::ostream&
    write (std::ostream &os, const SpecifiersT &specifiers, const HolderT<DataT...> &data)
    {
        for (auto cursor = std::cbegin (specifiers); cursor != std::cend (specifiers); ++cursor) {
            const auto &s = *cursor;

            if (s.type == type_t::LITERAL) {
                std::copy (std::begin (s.fmt), std::end (s.fmt), std::ostream_iterator<char> (os));
                continue;
            }

            if (s.type == type_t::ESCAPE) {
                os << '%';
                continue;
            }

            if constexpr (Index < sizeof... (DataT)) {
                using value_t = std::tuple_element_t<Index,std::tuple<DataT...>>;
                value<value_t>::write (os, s, data.template get<Index> ());
                return write<Index+1> (os, util::make_view (cursor+1,specifiers.end ()), data);
            } else {
                throw std::runtime_error ("insufficient data parameters");
            }
        }

        return os;
    }

    /// dispatches rendering of formats with associated parameters
    template <
        typename ...Args
    >
    std::ostream&
    operator<< (std::ostream &os, const bound<Args...> &val)
    {
        return write<0> (os, val.specifiers (), val);
    }


    /// dispatches rendering of formats with associated parameters
    template <
        typename ...Args
    >
    std::ostream&
    operator<< (std::ostream &os, const stored<Args...> &val)
    {
        return write<0> (os, val.specifiers (), val);
    }


    template <typename ...Args>
    std::string
    to_string (const bound<Args...> &fmt)
    {
        std::ostringstream os;
        os << fmt;
        return os.str ();
    }

    template <typename ...Args>
    std::string
    to_string (const stored<Args...> &fmt)
    {
        std::ostringstream os;
        os << fmt;
        return os.str ();
    }
}

#endif
