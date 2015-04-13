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
