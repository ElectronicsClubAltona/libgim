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

#include "./event.hpp"
#include "./except.hpp"

#include "../debug.hpp"

using util::json2::event::packet;


///////////////////////////////////////////////////////////////////////////////
util::json2::event::type_t
packet::type (void) const noexcept
{
    CHECK_NEQ (first, last);

    const auto &c = *first;

    switch (c) {
    case '{': return type_t::OBJECT_BEGIN;
    case '}': return type_t::OBJECT_END;
    case '[': return type_t::ARRAY_BEGIN;
    case ']': return type_t::ARRAY_END;
    case '"': return type_t::STRING;
    case 'n': return type_t::NONE;

    case 't':
    case 'f':
        return type_t::BOOLEAN;

    // TODO: leading plus isn't valid json, but other similar formats support
    // this syntax and it's easier to claim it as a number globally here until
    // we do a little refactoring.
    case '-':
    case '+':
    case '0'...'9':
        return type_t::NUMBER;
    }

    unhandled (c);
}


///////////////////////////////////////////////////////////////////////////////
#include "./personality/rfc7519.hpp"
#include "./personality/jsonish.hpp"

template <typename PersonalityT>
const char*
util::json2::event::parse (const std::function<util::json2::callback_t> &cb,
                           const char *first,
                           const char *last)
{
    auto cursor = first;

    PersonalityT p {};
    cursor = p.consume_whitespace (cursor, last);
    cursor = p.parse_value (cb, cursor, last);
    cursor = p.consume_whitespace (cursor, last);

    return cursor;
}


//-----------------------------------------------------------------------------
template
const char* util::json2::event::parse<util::json2::personality::rfc7159> (
    const std::function<util::json2::callback_t> &,
    const char*,
    const char*
);


//-----------------------------------------------------------------------------
template
const char* util::json2::event::parse<util::json2::personality::jsonish> (
    const std::function<util::json2::callback_t> &,
    const char*,
    const char*
);
