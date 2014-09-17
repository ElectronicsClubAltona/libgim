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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_BACKTRACE_HPP
#define __UTIL_BACKTRACE_HPP

#include <string>
#include <vector>
#include <iostream>


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
    operator <<(std::ostream&, const debug::backtrace&);
}


#endif
