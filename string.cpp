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

#include "./string.hpp"

#include "./cast.hpp"

#include <cstring>

// TODO: Horribly inefficient, but God help you if you're relying on this
// being efficient in the first place.
bool
strbegins (const char *restrict str,
           const char *restrict prefix) {
    return 0 == strncmp (prefix, str, strlen (prefix));
}
