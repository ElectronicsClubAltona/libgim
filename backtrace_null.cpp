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
 * Copyright:
 *      2011-2012, Danny Robson <danny@nerdcruft.net>
 */

#include "backtrace.hpp"

#include <iostream>

using namespace std;


debug::backtrace::backtrace (void):
    m_frames (DEFAULT_DEPTH)
{ ; }


ostream&
debug::operator <<(ostream &os, const debug::backtrace&) {
    os << "null backtrace";
    return os;
}


