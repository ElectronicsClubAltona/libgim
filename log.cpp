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
 * Copyright 2012-2016 Danny Robson <danny@nerdcruft.net>
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


///////////////////////////////////////////////////////////////////////////////
/// convert a string representation of a log-level into an enumeration value.
///
/// conversion is case insensitive
/// throws std::range_error if unable to convert
static util::level_t
to_level (std::string name)
{
    static const std::map<std::string, util::level_t> NAME_LEVELS = {
        { "EMERGENCY",     util::EMERGENCY },
        { "ALERT",         util::ALERT },
        { "CRITICAL",      util::CRITICAL },
        { "ERROR",         util::ERROR },
        { "WARN",          util::WARN },
        { "WARNING",       util::WARN },
        { "NOTICE",        util::NOTICE },
        { "INFO",          util::INFO },
        { "INFORMATIONAL", util::INFO },
        { "DEBUG",         util::DEBUG }
    };

    std::transform (name.cbegin (), name.cend (), name.begin (), ::toupper);

    auto pos = NAME_LEVELS.find (name);
    if (pos == NAME_LEVELS.end ())
        throw std::range_error (name);

    return pos->second;
}


//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream& os, util::level_t l)
{
    switch (l) {
        case util::EMERGENCY:       return os << "EMERGENCY";
        case util::ALERT:           return os << "ALERT";
        case util::CRITICAL:        return os << "CRITICAL";
        case util::ERROR:           return os << "ERROR";
        case util::WARNING:         return os << "WARNING";
        case util::NOTICE:          return os << "NOTICE";
        case util::INFORMATIONAL:   return os << "INFO";
        case util::DEBUG:           return os << "DEBUG";

        default:
            unreachable ();
    }
}


///////////////////////////////////////////////////////////////////////////////
static util::level_t
log_level (void)
{
    const char *env = getenv ("LOG_LEVEL");
    if (!env)
        return util::DEFAULT_LOG_LEVEL;

    try {
        return to_level (env);
    } catch (...) {
        return util::DEFAULT_LOG_LEVEL;
    }
}


//-----------------------------------------------------------------------------
static bool
needs_break (util::level_t level)
{
    static util::level_t break_level;
    static bool has_level = [&] (void) {
        const char *env = getenv ("BREAK_LEVEL");
        if (!env)
            return false;

        try {
            break_level = to_level (env);
            return true;
        } catch (...) {
            return false;
        }
    } ();

    return has_level && level <= break_level;
}


///////////////////////////////////////////////////////////////////////////////
void
util::log (util::level_t level, const std::string &msg)
{
    // fire a breakpoint before the following early exit
    if (needs_break (level))
        breakpoint ();

    static const util::level_t LOG_LEVEL = log_level ();
    if (level > LOG_LEVEL)
        return;

    static const size_t time_len = strlen("YYYY-mm-dd HHMMhSS") + 1;
    std::string time_string (time_len - 1, '\0');
    time_t unix_time = time (nullptr);

    if (0 == strftime (&time_string[0],
                time_len,
                "%Y-%m-%d %H%Mh%S",
                localtime (&unix_time))) {
        warn ("failed to log time");
        return;
    }

    std::cerr << time_string << " [" << level << "] " << msg << std::endl;
}


///////////////////////////////////////////////////////////////////////////////
util::scoped_logger::scoped_logger (util::level_t _level,
                                    std::string   _message):
    m_level   (_level),
    m_message (std::move (_message))
{ ; }


//-----------------------------------------------------------------------------
util::scoped_logger::~scoped_logger ()
{
    log (m_level, m_message);
}


///////////////////////////////////////////////////////////////////////////////
util::scoped_timer::scoped_timer (util::level_t _level,
                                  std::string   _message):
    m_level (_level),
    m_message (std::move (_message)),
    m_start (util::nanoseconds ())
{ ; }


//-----------------------------------------------------------------------------
util::scoped_timer::~scoped_timer ()
{
    auto finish = util::nanoseconds ();
    auto duration = finish - m_start;

    log (m_level, "%fs, %s", duration / 1'000'000'000.f, m_message);
}
