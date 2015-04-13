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

#include "si.hpp"

uintmax_t to_KiB (uintmax_t bytes) { return bytes >> 10; }
uintmax_t to_MiB (uintmax_t bytes) { return bytes >> 20; }
uintmax_t to_GiB (uintmax_t bytes) { return bytes >> 30; }

uintmax_t from_KiB (uintmax_t bytes) { return bytes << 10; }
uintmax_t from_MiB (uintmax_t bytes) { return bytes << 20; }
uintmax_t from_GiB (uintmax_t bytes) { return bytes << 30; }

