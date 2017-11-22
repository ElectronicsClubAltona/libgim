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

#ifndef __UTIL_LOG_HPP
#define __UTIL_LOG_HPP

#include "nocopy.hpp"
#include "preprocessor.hpp"

#include <ostream>
#include <string>

// Windows.h or one of its friends defines a macro 'ERROR'. Screw Microsoft.
#ifdef ERROR
#undef ERROR
#endif

namespace util {
    ///////////////////////////////////////////////////////////////////////////
    // rfc5424 log levels. It is assumed they are contiguous to simplify array
    // indexing in logging code.
    enum level_t {
        EMERGENCY,              /** system is unusable */
        ALERT,                  /** action must be taken immediately */
        CRITICAL,               /** critical conditions */
        ERROR,                  /** error conditions */
        WARNING,
        WARN = WARNING,         /** warning conditions */
        NOTICE,                 /** normal but significant condition */
        INFORMATIONAL,
        INFO = INFORMATIONAL,   /** informational messages */
        DEBUG                   /** debug-level messages */
    };

    #define MAP_LEVEL_T(F) MAP(F, EMERGENCY, ALERT, CRITICAL, ERROR, WARN, NOTICE, INFO, DEBUG)

    constexpr auto DEFAULT_LOG_LEVEL = NOTICE;

    //-------------------------------------------------------------------------
    const std::string& to_string (level_t);

    //-------------------------------------------------------------------------
    std::ostream&
    operator<< (std::ostream&, level_t);


    ///////////////////////////////////////////////////////////////////////////
    // Query or set the global logging level filter. If the severity passed to
    // a logging call is less than this then the output is discarded.
    //
    // The initial logging level can be controlled by setting the environment
    // variable LOG_LEVEL to a string corresponding to the names of the values
    // in level_t.
    //
    // Otherwise we fall back to the hard coded default from above.
    //
    // As a special case, a blank value for the environment variable LOG_LEVEL
    // corresponds to the maximum log level. It indicates a preference for
    // minimum output, and may be changed to disable all output in future
    // updates.
    level_t log_level (void);
    level_t log_level (level_t);

    ///////////////////////////////////////////////////////////////////////////
    void log (level_t, const std::string &msg);


    template <typename ...Args, size_t N>
    void
    log (level_t, const char (&fmt)[N], const Args&...);


    //-------------------------------------------------------------------------
    // Various convenience macros for logging specific strings with a well
    // known severity.
    //
    // LOG_DEBUG is treated similarly to assert; if NDEBUG is defined then we
    // compile out the statement so as to gain a little runtime efficiency
    // speed.
    #define LOG_EMERGENCY(...)  do { util::log(util::EMERGENCY, ##__VA_ARGS__); } while (0)
    #define LOG_ALERT(...)      do { util::log(util::ALERT,     ##__VA_ARGS__); } while (0)
    #define LOG_CRITICAL(...)   do { util::log(util::CRITICAL,  ##__VA_ARGS__); } while (0)
    #define LOG_ERROR(...)      do { util::log(util::ERROR,     ##__VA_ARGS__); } while (0)
    #define LOG_WARNING(...)    do { util::log(util::WARNING,   ##__VA_ARGS__); } while (0)
    #define LOG_WARN(...)       do { util::log(util::WARN,      ##__VA_ARGS__); } while (0)
    #define LOG_NOTICE(...)     do { util::log(util::NOTICE,    ##__VA_ARGS__); } while (0)
    #define LOG_INFO(...)       do { util::log(util::INFO,      ##__VA_ARGS__); } while (0)
#if !defined(NDEBUG)
    #define LOG_DEBUG(...)      do { util::log(util::DEBUG,     ##__VA_ARGS__); } while (0)
#else
    #define LOG_DEBUG(...)      do { ; } while (0)
#endif


    ///////////////////////////////////////////////////////////////////////////
    class scoped_logger : public nocopy {
    public:
        scoped_logger (level_t, std::string);
        ~scoped_logger ();

    protected:
        const level_t     m_level;
        const std::string m_message;
    };


    ///////////////////////////////////////////////////////////////////////////
    class scoped_timer : public nocopy {
    public:
        scoped_timer (level_t, std::string);
        ~scoped_timer ();

    private:
        const level_t m_level;
        const std::string m_message;
        uint64_t m_start;
    };
}

#include "log.ipp"

#endif

