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

#include "./iostream.hpp"

#include "./sphere.hpp"
#include "../coord/iostream.hpp"


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
std::ostream&
util::geom::operator<< (std::ostream &os, util::geom::sphere<S,T> s)
{
    return os << "sphere(" << s.centre << ',' << s.radius << ')';
}


//-----------------------------------------------------------------------------
template std::ostream& util::geom::operator<< (std::ostream&, util::geom::sphere<2,float>);
template std::ostream& util::geom::operator<< (std::ostream&, util::geom::sphere<3,float>);
