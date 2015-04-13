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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "string.hpp"

#include <cstring>

#include "types/casts.hpp"

// TODO: Horribly inefficient, but God help you if you're relying on this
// being efficient in the first place.
bool
strbegins (const char *restrict str,
           const char *restrict prefix) {
    return 0 == strncmp (prefix, str, strlen (prefix));
}



#if !defined(HAVE_STRNDUP)
#include <cstdlib>

// Written by Niels Möller <nisse@lysator.liu.se>
// Placed in the public domain

char *
strndup (const char *restrict s, size_t size)
{
    char *end = static_cast<char *> (memchr (s, 0, size));

    if (end)
        // Length + 1
        size = sign_cast<size_t> (end - s) + 1u;

    char * r = static_cast<char *> (malloc (size));

    if (size) {
        memcpy (r, s, size-1);
        r[size-1] = '\0';
    }
    return r;
}
#endif
