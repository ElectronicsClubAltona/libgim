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

#include "./jsonish.hpp"

#include "./base.hpp"
#include "../event.hpp"
#include "../except.hpp"
#include "../../debug.hpp"

using util::json2::personality::jsonish;


///////////////////////////////////////////////////////////////////////////////
const char*
jsonish::consume_whitespace (const char *first, const char *last) noexcept
{
    auto cursor = base<jsonish>::consume_whitespace (first, last);

    // consume a comment
    if (cursor != last && *cursor == '#') {
        while (cursor != last && *cursor != '\n')
            ++cursor;

        return consume_whitespace (cursor, last);
    }

    return cursor;
}


///////////////////////////////////////////////////////////////////////////////
// format is:
//   int: '0x' hex+ | '0' oct+ | '0b' bit+
//
//   float: significand exp?
//   significand: digit+ ('.' digit*)?
//   exp: [eE] sign? digit+
//
//   number: [+-] (int | float)
const char*
jsonish::parse_number (const std::function<callback_t> &cb,
                       const char *first,
                       const char *last)
{
    auto cursor = first;

    if (cursor != last && (*cursor == '+' || *cursor == '-'))
        ++cursor;

    if (cursor != last && *cursor == '0') {
        ++cursor;

        if (cursor == last)
            throw parse_error {cursor};

        char max = '9';
        switch (*cursor) {
        case 'x': {
            // parse the hex integer here because we can simplify the
            // remaining cases somewhat if we don't need to care about the
            // multiple ranges of valid digits.
            ++cursor;

            auto digit_start = cursor;
            while (cursor != last && ('0' <= *cursor && *cursor <= '9' ||
                                      'a' <= *cursor && *cursor <= 'f' ||
                                      'A' <= *cursor && *cursor <= 'F'))
                ++cursor;
            if (digit_start == cursor)
                throw parse_error {cursor};

            cb ({first, cursor});
            return cursor;
        };

        case 'b': max = '1'; break;
        case '0'...'7': max = '7'; break;

        case '.':
            goto frac;
        }

        auto digit_start = ++cursor;
        while (cursor != last && '0' <= *cursor && *cursor <= max)
            ++cursor;
        if (digit_start == cursor)
            throw parse_error {cursor};

        cb ({first, cursor});
        return cursor;
    }

    while (cursor != last && '0' <= *cursor && *cursor <= '9')
        ++cursor;
    if (cursor == last)
        goto done;

    if (*cursor != '.')
        goto exp;

frac:
    ++cursor;
    while (cursor != last && *cursor >= '0' && *cursor <= '9')
        ++cursor;
    if (cursor == last)
        goto done;

exp:
    if (cursor != last && (*cursor == 'e'  || *cursor == 'E')) {
        ++cursor;

        if (cursor != last && (*cursor == '+' || *cursor == '-'))
            ++cursor;

        auto digit_start = cursor;
        while (cursor != last && '0' <= *cursor && *cursor <= '9')
            ++cursor;
        if (digit_start == cursor)
            throw parse_error {cursor};
    }

    if (first == cursor)
        throw parse_error {cursor};

done:
    cb ({first, cursor});
    return cursor;
}


///////////////////////////////////////////////////////////////////////////////
const char*
jsonish::parse_key (const std::function<callback_t> &cb,
                    const char *first,
                    const char *last)
{
    auto cursor = first;
    if (cursor == last)
        throw parse_error {cursor};

    // must start with alpha or underscore
    switch (*cursor) {
    case 'a'...'z':
    case 'A'...'Z':
    case '_':
        ++cursor;
        break;

    default:
        throw parse_error {cursor};
    }


    while (cursor != last) {
        switch (*cursor) {
        case 'a'...'z':
        case 'A'...'Z':
        case '_':
        case '0'...'9':
            ++cursor;
            break;

        default:
            cb ({first, cursor});
            return cursor;
        }
    }

    cb ({first, cursor});
    return cursor;
}


///////////////////////////////////////////////////////////////////////////////
const char*
jsonish::parse_string (const std::function<callback_t> &cb,
                       const char *first,
                       const char *last)
{
    if (first == last)
        throw parse_error {first};

    if (*first == '"')
        return base<jsonish>::parse_string (cb, first, last);
    else
        return parse_key (cb, first, last);
}
