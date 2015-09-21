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

#ifdef __UTIL_RATIONAL_IPP
#error
#endif
#define __UTIL_RATIONAL_IPP


//-----------------------------------------------------------------------------
template <typename T, typename U>
util::rational<typename std::common_type<T,U>::type>
util::operator/ (U lhs, rational<T> rhs)
{
    return rhs.inverse () * lhs;
}


//-----------------------------------------------------------------------------
template <typename T, typename U>
util::rational<typename std::common_type<T,U>::type>
util::operator* (U lhs, rational<T> rhs)
{
    return rhs * lhs;
}
