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
#include "term.hpp"
#include "time.hpp"
#include "cast.hpp"

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

///////////////////////////////////////////////////////////////////////////////
static
const util::level_t
ALL_LEVELS[] = {
    util::EMERGENCY,
    util::ALERT,
    util::CRITICAL,
    util::ERROR,
    util::WARN,
    util::NOTICE,
    util::INFO,
    util::DEBUG,
};


///////////////////////////////////////////////////////////////////////////////
/// convert a string representation of a log-level into an enumeration value.
///
/// conversion is case insensitive
/// throws std::range_error if unable to convert
static util::level_t
to_level (std::string name)
{
    if (std::empty (name))
        return util::EMERGENCY;

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


///////////////////////////////////////////////////////////////////////////////
const std::string&
util::to_string (level_t l)
{
    switch (l) {
    #define CASE(L)                             \
        case util::L: {                         \
            static const std::string STR = #L;  \
            return STR;                         \
        }

    MAP_LEVEL_T(CASE)

    #undef CASE
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream& os, util::level_t l)
{
    return os << to_string (l);
}


///////////////////////////////////////////////////////////////////////////////
// Determine what the value for LOG_LEVEL should be at the beginning of
// execution given the system environment.
//
// Note that the LOG macros _cannot_ be used from within this function as it
// will likely result in infinite recursion.
static
util::level_t
initial_log_level (void)
{
    const char *env = getenv ("LOG_LEVEL");
    if (!env)
        return util::DEFAULT_LOG_LEVEL;

    try {
        return to_level (env);
    } catch (...) {
        std::clog << "Invalid environment LOG_LEVEL: '" << env << "'\n";
        return util::DEFAULT_LOG_LEVEL;
    }
}


//-----------------------------------------------------------------------------
// We shouldn't ever actually get to use the default value, but we set it to
// the most verbose option just in case we've made a mistake elsewhere.

static bool          s_log_level_done;
static util::level_t s_log_level_value;

//-----------------------------------------------------------------------------
util::level_t
util::log_level (level_t _level)
{
    s_log_level_value = _level;
    s_log_level_done  = true;
    return s_log_level_value;
}


//-----------------------------------------------------------------------------
util::level_t
util::log_level (void)
{
    if (!s_log_level_done) {
        s_log_level_value = initial_log_level ();
        s_log_level_done  = true;
    }

    return s_log_level_value;
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


//-----------------------------------------------------------------------------
static
util::term::csi::graphics
level_colour (util::level_t level)
{
    using util::term::csi::graphics;

    switch (level) {
    case util::EMERGENCY:
    case util::ALERT:
    case util::CRITICAL:
    case util::ERROR:
        return graphics (graphics::FOREGROUND, graphics::RED);

    case util::WARNING:
        return graphics (graphics::FOREGROUND, graphics::YELLOW);

    case util::NOTICE:
    case util::INFORMATIONAL:
        return graphics (graphics::FOREGROUND, graphics::GREEN);

    case util::DEBUG:
        return graphics (graphics::FOREGROUND, graphics::WHITE);
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
static
size_t
level_width (void)
{
    static size_t width = [] {
        size_t hi = 0;

        for (auto i: ALL_LEVELS)
            hi = util::max (to_string (i).size (), hi);

        return hi;
    } ();

    return width;
}


///////////////////////////////////////////////////////////////////////////////
void
util::log (util::level_t level, const std::string &msg)
{
    if (level <= log_level ()) {
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

        std::clog << time_string << " ["
            << level_colour (level)
            << std::setw (trunc_cast<int> (level_width ()))
            << std::left
            << level
            << std::setw (0)
            << util::term::csi::graphics::RESET
        << "] " << msg << std::endl;
    }

    if (needs_break (level))
        breakpoint ();
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

    log (m_level, "%fs, %s", float (duration) / 1'000'000'000.f, m_message);
}
