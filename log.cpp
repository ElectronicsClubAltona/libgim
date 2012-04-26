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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */


#include "log.hpp"

#include "debug.hpp"
#include "types.hpp"

#include <ctime>
#include <cstring>

#include <boost/format.hpp>

using namespace util;
using std::string;

void
check_level (level_t l)
    { check (l >= 0 && l < NUM_LEVELS); }


const string&
level_to_string (level_t l) {
    check_level (l);

    static const std::string level_names[] = {
        "EMERGENCY",
        "ALERT",
        "CRITICAL",
        "ERROR",
        "WARN",
        "NOTICE",
        "INFO",
        "DEBUG"
    };

    return level_names[l];
}


std::ostream&
util::operator<< (std::ostream& os, level_t l) {
    os << level_to_string (l);
    return os;
}


void
util::log (level_t l, const std::string &format)
    { detail::log (l, boost::format (format)); }


void
util::detail::log (level_t level, boost::format &&format) {
    static const boost::format LEVEL_FORMAT ("%s [%s] ");
    
    char time_string[strlen("YYYY-mm-dd HHMMh") + 1];
    time_t unix_time = time (nullptr);
    if (0 == strftime (time_string,
                       elems (time_string),
                       "%Y-%m-%d %H%Mh",
                       localtime (&unix_time))) {
        unusual ();
        return;
    }

    std::cerr << boost::format (LEVEL_FORMAT)
                    % time_string
                    % level
              << format
              << "\n";
}
