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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */


#include "tap.hpp"

#include "hash/simple.hpp"
#include "hash/xxhash.hpp"

int
main (int, char **)
{
    util::TAP::logger tap;

    static const struct {
        uint32_t hash32;
        uint64_t hash64;
        unsigned seed;
        std::string data;
        std::string msg;
    } TESTS[] = {
        { 0x02CC5D05, 0xef46db3751d8e999, 0, "", "empty string, 0 seed" },
        { 0x0b2cb792, 0xd5afba1336a3be4b, 1, "", "empty string, 1 seed" },
        { 0x550d7456, 0xd24ec4f1a98c6e5b, 0, "a", "single a, 0 seed" },
        { 0xf514706f, 0xdec2bc81c3cd46c6, 1, "a", "single a, 1 seed" },
        { 0x32d153ff, 0x44bc2cf5ad770999, 0, "abc", "abc, 0 seed" },
        { 0xaa3da8ff, 0xbea9ca8199328908, 1, "abc", "abc, 1 seed" },
        { 0x54ca7e46, 0x892a0760a6343391, 0x1234,
          "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+",
          "long alphabet" }
    };

    for (const auto &t: TESTS) {
        auto first = t.data.data ();
        auto last  = first + t.data.size ();

        auto digest32 = util::hash::simple<util::hash::xxhash32> (first, last, t.seed);
        //auto digest64 = util::hash::simple<util::hash::xxhash64> (first, last, t.seed);

        tap.expect_eq (digest32, t.hash32, "xxhash32 %s", t.msg);
        //tap.expect_eq (digest64, t.hash64, "xxhash64 %s", t.msg);
    }

    return tap.status ();
}
