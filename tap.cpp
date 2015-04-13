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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "tap.hpp"

#include "debug.hpp"

#include <iostream>

using util::TAP::logger;

//-----------------------------------------------------------------------------
logger::logger ():
    m_status (EXIT_SUCCESS),
    m_size (0)
{ ; }


//-----------------------------------------------------------------------------
logger::~logger ()
{
    std::cout << "1.." << m_size << '\n';
}


////////////0//////////////////////////////////////////////////////////////////
void
util::TAP::logger::expect (bool test, const std::string &msg)
{
    std::cout << (test ? "ok " : "not ok ") << ++m_size << " - " << msg << '\n';

    if (!test)
        m_status = EXIT_FAILURE;
}


///////////////////////////////////////////////////////////////////////////////
void
logger::skip (const std::string &msg)
{
    std::cout << "ok " << ++m_size << " - # SKIP " << msg << '\n';
}


//-----------------------------------------------------------------------------
void
logger::todo (const std::string &msg)
{
    std::cout << "not ok " << ++m_size << " - # TODO " << msg << '\n';
}


//-----------------------------------------------------------------------------
void
logger::noop (void)
{
    skip ("noop");
}


//-----------------------------------------------------------------------------
int
logger::status (void) const
{
    return m_status;
}
