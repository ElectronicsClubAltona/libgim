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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */


#include "log.hpp"

#include "debug.hpp"
#include "types.hpp"
#include "time.hpp"

#include <array>
#include <ctime>
#include <cstring>
#include <map>
#include <string>

#include <boost/format.hpp>

using namespace util;
using std::string;
using std::map;


///////////////////////////////////////////////////////////////////////////////
void
check_level (level_t l)
{
    (void)l; // Consume even in release mode
    CHECK (l >= 0 && l < NUM_LEVELS);
}

//-----------------------------------------------------------------------------
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


///////////////////////////////////////////////////////////////////////////////
static level_t
string_to_level (string name) {
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

    std::transform (name.cbegin (), name.cend (), name.begin (), ::toupper);

    auto pos = NAME_LEVELS.find (name);
    if (pos == NAME_LEVELS.end ())
        throw std::range_error (name);

    return pos->second;
}


//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream& os, level_t l) {
    os << level_to_string (l);
    return os;
}


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
void
util::log (level_t l, const std::string &format)
    { detail::log (l, boost::format (format)); }


//-----------------------------------------------------------------------------
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


///////////////////////////////////////////////////////////////////////////////
scoped_logger::scoped_logger (level_t       _level,
                              std::string &&_message):
    m_level   (_level),
    m_message (std::move (_message))
{ ; }


//-----------------------------------------------------------------------------
scoped_logger::~scoped_logger ()
{
    log (m_level, m_message);
}


///////////////////////////////////////////////////////////////////////////////
scoped_timer::scoped_timer (level_t _level,
                            std::string &&_message):
    m_level (_level),
    m_message (_message),
    m_start (util::nanoseconds ())
{ ; }


//-----------------------------------------------------------------------------
scoped_timer::~scoped_timer ()
{
    auto finish = util::nanoseconds ();
    auto duration = finish - m_start;

    log (m_level, "%fs, %s", duration / 1'000'000'000.f, m_message);
}
