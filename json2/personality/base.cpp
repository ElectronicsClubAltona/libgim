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
#include "./base.hpp"

#include "../event.hpp"
#include "../except.hpp"
#include "../../debug.hpp"

#include "./rfc7519.hpp"
#include "./jsonish.hpp"

using util::json2::personality::base;


///////////////////////////////////////////////////////////////////////////////
static const char*
expect [[nodiscard]] (const char *first, const char *last, const char value)
{
    if (first == last || *first != value)
        throw util::json2::parse_error {first};
    return first + 1;
}


///////////////////////////////////////////////////////////////////////////////
template <typename ParentT>
const char*
base<ParentT>::consume_whitespace (const char *first, const char *last) noexcept
{
    auto cursor = first;

    while (cursor != last) {
        switch (*cursor) {
        case 0x20:
        case 0x09:
        case 0x0A:
        case 0x0D:
            ++cursor;
            continue;
        }

        break;
    }

    return cursor;
}


///////////////////////////////////////////////////////////////////////////////
template <typename ParentT>
const char*
base<ParentT>::parse_number (const std::function<util::json2::callback_t> &cb,
                             const char *first,
                             const char *last)
{
    (void)last;

    // number: minus? int frac? exp?
    auto cursor = first;

    // minus: '-'
    if (*cursor == '-')
        ++cursor;

    // int: '0' | [1-9] DIGIT*
    switch (*cursor) {
        case '1'...'9':
        {
            ++cursor;
            while ('0' <= *cursor && *cursor <= '9')
                ++cursor;
            break;
        }

        case '0':
            // leading zero means we _must_ be parsing a fractional value so we
            // look ahead to ensure we're about to do so. note that we don't use
            // `expect' here because it implies consumption of '.'
            ++cursor;
            if (*cursor != '.')
                throw util::json2::parse_error { cursor };
            break;

        default:
            throw util::json2::parse_error { cursor };
    }

    // frac: '.' digit+
    if (*cursor == '.') {
        ++cursor;

        auto frac_start = cursor;
        while ('0' <= *cursor && *cursor <= '9')
            ++cursor;
        if (frac_start == cursor)
            throw util::json2::parse_error { cursor };
    }

    // exp: [eE] [-+]? digit+
    if (*cursor == 'e' || *cursor == 'E') {
        ++cursor;

        if (*cursor == '-' || *cursor == '+')
            ++cursor;

        auto exp_digits = cursor;
        while ('0' <= *cursor && *cursor <= '9')
            ++cursor;
        if (exp_digits == cursor)
            throw util::json2::parse_error { cursor };
    }

    cb ({ first, cursor });
    return cursor;
}


//-----------------------------------------------------------------------------
template <typename ParentT>
template <int N>
const char*
base<ParentT>::parse_literal (const std::function<util::json2::callback_t> &cb,
                              const char *first,
                              const char *last,
                              const char (&value)[N])
{
    CHECK_LE (first, last);

    if (last - first < N - 1)
        throw util::json2::overrun_error { first };

    if (!std::equal (first, first + N - 1, value))
        throw util::json2::parse_error { first };

    cb ({ first, first + N - 1 });
    return first + N - 1;
}


//-----------------------------------------------------------------------------
template <typename ParentT>
const char*
base<ParentT>::parse_string (const std::function<util::json2::callback_t> &cb,
                             const char *first,
                             const char *last)
{
    CHECK_LE (first, last);

    auto cursor = first;

    cursor = expect (cursor, last, '"');

    for ( ; cursor != last && *cursor != '"'; ) {
        // advance the simple case first; unescaped character
        if (*cursor++ != '\\') {
            continue;
        }

        if (*cursor++ == 'u') {
            for (int i = 0; i < 4; ++i) {
                switch (*cursor) {
                    case 'a'...'f':
                    case 'A'...'F':
                        ++cursor;
                        continue;
                    default:
                        throw util::json2::parse_error { cursor };
                }
            }
        }
    }

    cursor = expect (cursor, last, '"');
    cb ({ first, cursor });
    return cursor;
}


//-----------------------------------------------------------------------------
template <typename ParentT>
const char*
base<ParentT>::parse_array (const std::function<util::json2::callback_t> &cb,
                            const char *first,
                            const char *last)
{
    CHECK_LE (first, last);

    auto cursor = first;

    if (*cursor != '[')
        throw util::json2::parse_error {cursor};
    cb ({ cursor, cursor + 1 });
    ++cursor;

    cursor = ParentT::consume_whitespace (cursor, last);

    if (*cursor == ']') {
        cb ({cursor, cursor + 1});
        return ++cursor;
    }

    cursor = ParentT::parse_value (cb, cursor, last);

    if (*cursor == ']') {
        cb ({cursor, cursor + 1});
        return ++cursor;
    }

    do {
        cursor = ParentT::consume_whitespace (cursor, last);
        cursor = expect (cursor, last, ',');
        cursor = ParentT::consume_whitespace (cursor, last);
        cursor = ParentT::parse_value (cb, cursor, last);
    } while (*cursor != ']');

    cb ({cursor, cursor + 1});
    ++cursor;

    return cursor;
}


//-----------------------------------------------------------------------------
template <typename ParentT>
const char*
base<ParentT>::parse_object (const std::function<util::json2::callback_t> &cb,
                             const char *first,
                             const char *last)
{
    CHECK_LE (first, last);

    auto cursor = first;
    cursor = expect (cursor, last, '{');
    cb ({ cursor - 1, cursor });

    cursor = ParentT::consume_whitespace (cursor, last);

    if (*cursor == '}') {
        cb ({cursor, cursor + 1});
        return ++cursor;
    };

    auto parse_member = [] (auto _cb, auto _cursor, auto _last) {
        _cursor = ParentT::parse_key (_cb, _cursor, _last);

        _cursor = ParentT::consume_whitespace (_cursor, _last);
        _cursor = expect (_cursor, _last, ':');
        _cursor = ParentT::consume_whitespace (_cursor, _last);

        _cursor = ParentT::parse_value (_cb, _cursor, _last);
        _cursor = ParentT::consume_whitespace (_cursor, _last);

        return _cursor;
    };

    cursor = parse_member (cb, cursor, last);

    if (*cursor == '}') {
        cb ({cursor, cursor + 1});
        return ++cursor;
    }

    do {
        cursor = expect (cursor, last, ',');
        cursor = ParentT::consume_whitespace (cursor, last);
        cursor = parse_member (cb, cursor, last);
    } while (*cursor != '}');

    cursor = expect (cursor, last, '}');
    cb ({cursor - 1, cursor});
    return cursor;
}



///////////////////////////////////////////////////////////////////////////////
template <typename ParentT>
const char*
base<ParentT>::parse_value (const std::function<util::json2::callback_t> &cb,
                            const char *first,
                            const char *last)
{
    switch (*first) {
    case '+':
    case '-':
    case '0'...'9':
        return ParentT::parse_number (cb, first, last);

    case '"':
        return ParentT::parse_string (cb, first, last);

    case 't': return ParentT::parse_literal (cb, first, last, "true");
    case 'f': return ParentT::parse_literal (cb, first, last, "false");
    case 'n': return ParentT::parse_literal (cb, first, last, "null");

    case '[': return ParentT::parse_array (cb, first, last);
    case '{': return ParentT::parse_object (cb, first, last);
    }

    return ParentT::parse_unknown (cb, first, last);
}


///////////////////////////////////////////////////////////////////////////////
template <typename ParentT>
const char*
base<ParentT>::parse_unknown (const std::function<util::json2::callback_t>&,
                              const char *first,
                              const char *last)
{
    (void)last;
    throw parse_error {first};
};



//-----------------------------------------------------------------------------
#define INSTANTIATE(KLASS) template struct util::json2::personality::base<KLASS>;
MAP_JSON2_PERSONALITY_TYPES (INSTANTIATE)
