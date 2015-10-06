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

#include "./gradient.hpp"


///////////////////////////////////////////////////////////////////////////////
template <size_t S>
static std::array<util::vectori<S>,util::pow(2,S)>
generate (void)
{
    std::array<util::vectori<S>,util::pow(2,S)> out;

    for (size_t i = 0; i < util::pow(2,S); ++i)
        for (size_t s = 0; s < S; ++s)
            out[i][s] = (i >> s) & 1;

    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S>
const std::array<util::vectori<S>,util::pow(2,S)>
util::noise::basis::type::gradient<S>::CORNERS = generate<S> ();

//-----------------------------------------------------------------------------
template struct util::noise::basis::type::gradient<1>;
template struct util::noise::basis::type::gradient<2>;
template struct util::noise::basis::type::gradient<3>;
template struct util::noise::basis::type::gradient<4>;
