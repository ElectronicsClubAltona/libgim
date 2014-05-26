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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#include "../stringid.hpp"
#include "../debug.hpp"

#include <cstdlib>
#include <stdexcept>

int
main (int, char**) {
    util::stringid map;

    CHECK_THROWS (std::out_of_range, map.find ("invalid"));

    auto id1 = map.add ("first");
    CHECK_EQ (id1, map.find ("first"));

    CHECK_THROWS (std::out_of_range, map.find ("invalid"));

    auto id2 = map.add ("second");
    CHECK_EQ (id1 + 1, id2);
    CHECK_EQ (id1, map.find ("first"));

    return EXIT_SUCCESS;
}
