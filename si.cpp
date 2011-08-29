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
 * Copyright 2011 Danny Robson <danny@blubinc.net>
 */

#include "si.hpp"

uintmax_t to_KiB (uintmax_t bytes) { return bytes >> 10; }
uintmax_t to_MiB (uintmax_t bytes) { return bytes >> 20; }
uintmax_t to_GiB (uintmax_t bytes) { return bytes >> 30; }

uintmax_t from_KiB (uintmax_t bytes) { return bytes << 10; }
uintmax_t from_MiB (uintmax_t bytes) { return bytes << 20; }
uintmax_t from_GiB (uintmax_t bytes) { return bytes << 30; }

