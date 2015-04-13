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

#ifdef __UTIL_LOG_IPP
#error Double inclusion of util/log.ipp
#endif

#define __UTIL_LOG_IPP


//-----------------------------------------------------------------------------
#include <boost/format.hpp>


//-----------------------------------------------------------------------------
namespace util {
    namespace detail  {
        void
        log (level_t l, boost::format &&format);

        template <typename T, typename ...tail>
        void
        log (level_t l, boost::format &&format, const T &val, tail ..._tail) {
            ::util::detail::log (l, std::move (format % val), _tail...);
        }
    }

    template <typename ...tail>
    void log (level_t l, const std::string &format, tail ..._tail)
        { detail::log (l, std::move (boost::format (format)), _tail...); }
}

