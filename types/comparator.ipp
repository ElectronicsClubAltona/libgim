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
 * Copyright 2012-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_TYPES_COMPARATOR_IPP
#error "Double inclusion for util/types/comparator.ipp"
#endif

#define __UTIL_TYPES_COMPARATOR_IPP


//-----------------------------------------------------------------------------
namespace util::comparator {
    template <typename T>
    bool
    pointer_comparator<T>::operator() (const std::unique_ptr<T> &lhs,
                                       const std::unique_ptr<T> &rhs)
        { return lhs < rhs; }


    template <typename T>
    bool
    pointer_comparator<T>::operator() (const T                  *lhs,
                                       const std::unique_ptr<T> &rhs)
        { return lhs < rhs.get (); }


    template <typename T>
    bool
    pointer_comparator<T>::operator() (const std::unique_ptr<T> &lhs,
                                       const T                  *rhs)
        { return lhs.get () < rhs; }
}

