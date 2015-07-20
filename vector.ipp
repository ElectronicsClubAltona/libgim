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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#if defined(__UTIL_VECTOR_IPP)
#error
#else
#define __UTIL_VECTOR_IPP
#endif


namespace util {
    template <size_t S, typename T>
    template <size_t D>
    vector<D,T>
    vector<S,T>::homog (void) const
    {
        static_assert (D > S, "reducing size loses data");
        return (*this).template redim<D> (0.f);
    }
}
