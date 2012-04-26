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

#ifndef __UTIL_LOG_HPP
#define __UTIL_LOG_HPP

#include <iostream>
#include <string>

namespace util {
    // rfc5424 log levels. It is assumed they are contiguous to simplify array
    // indexing in logging code.
    //
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
        DEBUG,                  /** debug-level messages */

        NUM_LEVELS,
        DEFAULT = INFO
    };

    std::ostream&
    operator<< (std::ostream&, level_t);

    void log (level_t, const std::string &format);

    template <typename ...tail>
    void log (level_t, const std::string &format, tail ..._tail);

    #define LOG_EMERGENCY(...)  do { log(util::EMERGENCY, ##__VA_ARGS__); } while (0)
    #define LOG_ALERT(...)      do { log(util::ALERT,     ##__VA_ARGS__); } while (0)
    #define LOG_CRITICAL(...)   do { log(util::CRITICAL,  ##__VA_ARGS__); } while (0)
    #define LOG_ERROR(...)      do { log(util::ERROR,     ##__VA_ARGS__); } while (0)
    #define LOG_WARNING(...)    do { log(util::WARNING,   ##__VA_ARGS__); } while (0)
    #define LOG_NOTICE(...)     do { log(util::NOTICE,    ##__VA_ARGS__); } while (0)
    #define LOG_INFO(...)       do { log(util::INFO,      ##__VA_ARGS__); } while (0)
    #define LOG_DEBUG(...)      do { log(util::DEBUG,     ##__VA_ARGS__); } while (0)
}

#include "log.ipp"

#endif

