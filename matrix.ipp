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
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */


#ifdef __UTIL_MATRIX_IPP
#error
#endif

#define __UTIL_MATRIX_IPP

template <typename T>
template <typename U>
util::matrix<U>
util::matrix<T>::cast (void) const
{
    util::matrix<U> out;

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            out.values[i][j] = values[i][j];

    return out;
}
