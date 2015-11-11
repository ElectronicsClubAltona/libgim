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

#include "memory/buffer/circular.hpp"
#include "tap.hpp"

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    // provoke usage of the smallest size buffer we can get away with so we
    // might detect caching issues or similar.
    constexpr size_t CAPACITY = 1;
    util::memory::buffer::circular buffer (CAPACITY);

    // zero fill to ensure our value setting tests don't accidentall succeed
    std::fill_n (buffer.begin (), buffer.size () * 2, 0);

    // sanity check we haven't accidentally mapped an empty region
    tap.expect_neq (buffer.begin (), buffer.end (), "non-zero sized region");

    // check a near overrun is replicated
    buffer.end ()[0] = 1;
    tap.expect_eq (buffer.begin ()[0], buffer.end ()[0], "near overrun is replicated");

    // check a far overrun is replicated
    buffer.end ()[buffer.size () - 1] = 2;
    tap.expect_eq (buffer.begin ()[buffer.size () - 1],
                   buffer.end   ()[buffer.size () - 1],
                   "far overrun is replicated");

    return tap.status ();
}
