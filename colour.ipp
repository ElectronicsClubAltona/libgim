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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_COLOUR_IPP
#error
#endif
#define __UTIL_COLOUR_IPP

template <size_t S, typename T>
template <typename U>
util::colour<S,U>
util::colour<S,T>::cast (void) const
{
    colour<S,U> ret;
    std::transform (this->begin (),
                    this->end (),
                    ret.begin (),
                    renormalise<T,U>);
    return ret;
}
