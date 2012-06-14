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

#include <array>
#include <ctime>
#include <cstring>
#include <map>
#include <string>

#include <boost/format.hpp>

using namespace util;
using std::string;
using std::map;

void
check_level (level_t l)
    { CHECK (l >= 0 && l < NUM_LEVELS); }

const string&
level_to_string (level_t l) {
    check_level (l);

    static const std::array <std::string, NUM_LEVELS> LEVEL_NAMES ({{
        "EMERGENCY",
        "ALERT",
        "CRITICAL",
        "ERROR",
        "WARN",
        "NOTICE",
        "INFO",
        "DEBUG"
    }});

    return LEVEL_NAMES[l];
}


level_t
string_to_level (const string &name) {
    static const map <string, level_t> NAME_LEVELS = {
        { "EMERGENCY",     EMERGENCY },
        { "ALERT",         ALERT },
        { "CRITICAL",      CRITICAL },
        { "ERROR",         ERROR },
        { "WARN",          WARN },
        { "WARNING",       WARN },
        { "NOTICE",        NOTICE },
        { "INFO",          INFO },
        { "INFORMATIONAL", INFO },
        { "DEBUG",         DEBUG }
    };

    auto pos = NAME_LEVELS.find (name);
    if (pos == NAME_LEVELS.end ())
        throw std::range_error (name);

    return pos->second;
}


std::ostream&
util::operator<< (std::ostream& os, level_t l) {
    os << level_to_string (l);
    return os;
}


void
util::log (level_t l, const std::string &format)
    { detail::log (l, boost::format (format)); }


static level_t
log_level (void) {
    const char *env = getenv ("LOG_LEVEL");
    if (!env)
        return DEFAULT;

    try {
        return string_to_level (env);
    } catch (...) {
        return DEFAULT;
    }
}


void
util::detail::log (level_t level, boost::format &&format) {
    static const level_t LOG_LEVEL = log_level ();
    if (level > LOG_LEVEL)
        return;

    static const boost::format LEVEL_FORMAT ("%s [%s] ");
    
    static const size_t time_len = strlen("YYYY-mm-dd HHMMhSS") + 1;
    std::string time_string (time_len - 1, '\0');
    time_t unix_time = time (nullptr);

    if (0 == strftime (&time_string[0],
                       time_len,
                       "%Y-%m-%d %H%Mh%S",
                       localtime (&unix_time))) {
        unusual ();
        return;
    }

    std::cerr << boost::format (LEVEL_FORMAT)
                    % time_string
                    % level
              << format
              << std::endl;
}
