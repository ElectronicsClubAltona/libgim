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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_BACKTRACE_HPP
#define __UTIL_BACKTRACE_HPP

#include <string>
#include <vector>
#include <ostream>


//-----------------------------------------------------------------------------
namespace debug {
    class backtrace {
        protected:
            static const unsigned int DEFAULT_DEPTH = 16;
            std::vector<void *>       m_frames;

        public:
            backtrace (void);

            const decltype(m_frames)& frames(void) const
                { return m_frames; }
    };

    std::ostream&
    operator<< (std::ostream&, const debug::backtrace&);
}


#endif
