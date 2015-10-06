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
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "constant.hpp"

using util::noise::basis::constant;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
constant<S,T>::constant (seed_t _seed):
    seed (_seed),
    value (42)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template struct util::noise::basis::constant<2,float>;
template struct util::noise::basis::constant<2,double>;

template struct util::noise::basis::constant<3,float>;
template struct util::noise::basis::constant<3,double>;
