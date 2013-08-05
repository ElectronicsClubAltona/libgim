/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
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
