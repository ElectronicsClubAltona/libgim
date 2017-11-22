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
 *      2017, Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_ALGO_SORT_HPP
#define CRUFT_UTIL_ALGO_SORT_HPP

namespace cruft::util::sort {
    ///////////////////////////////////////////////////////////////////////////
    // rearrange the values in the arrays specified by the iterators value and
    // ...tail by moving values to the positions described by the mapping of
    // offset-to-dest_index in idx_first:idx_last.
    //
    // ie, the indices { 2, 1, 0 } will reverse a series of arrays of three
    // elements
    //
    // all operations occur in-place, and the indices may be rearranged during
    // the operation.
    template <typename IndexIt, typename ValueIt, typename ...OtherIt>
    void
    reorder (IndexIt idx_first, IndexIt idx_last, ValueIt value, OtherIt ...tail);

    ///////////////////////////////////////////////////////////////////////////
    // sort an array specified by the iterators key_first:key_last using a
    // comparator, and optionally a series of additional value iterators
    // specified by ...values.
    //
    // sorting is performed in-place and will invariably allocate memory.
    template <typename RandomIt, class Comparator, class ...Args>
    void
    soa (RandomIt key_first, RandomIt key_last, Comparator comp, Args ...values);
}

#include "sort.ipp"

#endif
