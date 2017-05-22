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

#include "tap.hpp"

#include "ascii.hpp"

int
main (int, char**)
{
    util::TAP::logger tap;

    static constexpr bool T = true;
    static constexpr bool F = false;

    static constexpr struct {
        char character;

        bool is_digit;
        bool is_hex;
        bool is_upper;
        bool is_lower;
        bool is_space;

        int integer;
        unsigned hexadecimal;

        const char *msg;
    } TESTS[] = {
        {  '/', F, F, F, F, F, 0,  0, "/" }, // one below '0'
        {  '0', T, T, F, F, F, 0,  0, "0" },
        {  '9', T, T, F, F, F, 9,  9, "9" },
        {  ':', F, F, F, F, F, 0,  0, ":" }, // one above '9'
        {  '`', F, F, F, F, F, 0,  0, "`" }, // one below 'a'
        {  'a', F, T, F, T, F, 0, 10, "a" },
        {  'f', F, T, F, T, F, 0, 15, "f" },
        {  'g', F, F, F, T, F, 0,  0, "g" },
        {  'A', F, T, T, F, F, 0, 10, "A" },
        {  'F', F, T, T, F, F, 0, 15, "F" },
        {  'G', F, F, T, F, F, 0,  0, "G" },

        {  ' ', F, F, F, F, T, 0,  0, "space" },
        { '\t', F, F, F, F, T, 0,  0, "tab" },
        { '\n', F, F, F, F, T, 0,  0, "newline" },
    };

#define TRY_BOOL(X) [&] () -> bool { try { return X; } catch (...) { return false; }; } ()

    for (auto &t: TESTS) {
        bool is_digit = TRY_BOOL(util::ascii::is_digit (t.character)) == t.is_digit;
        bool is_hex   = TRY_BOOL(util::ascii::is_hex   (t.character)) == t.is_hex;
        bool is_upper = TRY_BOOL(util::ascii::is_upper (t.character)) == t.is_upper;
        bool is_lower = TRY_BOOL(util::ascii::is_lower (t.character)) == t.is_lower;
        bool is_space = TRY_BOOL(util::ascii::is_space (t.character)) == t.is_space;

        bool integer     = !t.is_digit || util::ascii::to_integer (t.character) == t.integer;
        bool hexadecimal = !t.is_hex   || util::ascii::from_hex   (t.character) == t.hexadecimal;

        tap.expect (
            is_digit && is_hex && is_upper && is_lower && is_space && integer && hexadecimal,
            "%s", t.msg
        );
    }

    return tap.status ();
}
