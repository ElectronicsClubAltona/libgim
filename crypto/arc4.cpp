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

#include "arc4.hpp"

#include <algorithm>

using util::crypto::ARC4;


///////////////////////////////////////////////////////////////////////////////
ARC4::ARC4 (const uint8_t *restrict key, size_t len):
    x (0),
    y (0)
{
    std::iota (S.begin (), S.end (), 0);

    for (size_t i = 0, j = 0; i < 256; ++i) {
        j = (j + S[i] + key[i % len]) % 256;
        std::swap (S[i], S[j]);
    }
}


//-----------------------------------------------------------------------------
void
ARC4::discard (size_t len)
{
    while (len--)
        get ();
}


//-----------------------------------------------------------------------------
void
ARC4::generate (uint8_t *restrict dst, size_t len)
{
    std::generate_n (dst, len, [this] (void) { return get (); });
}


//-----------------------------------------------------------------------------
uint8_t
ARC4::get (void)
{
    x = (x +    1) % 256;
    y = (y + S[x]) % 256;

    std::swap (S[x], S[y]);

    return S[(S[x] + S[y]) % 256];
}
