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

#ifdef CRUFT_UTIL_ALGO_SORT_IPP
#error
#endif

#define CRUFT_UTIL_ALGO_SORT_IPP

#include <algorithm>
#include <array>
#include <iterator>
#include <numeric>
#include <vector>

#include "../debug.hpp"
#include "../tuple.hpp"


///////////////////////////////////////////////////////////////////////////////
namespace cruft::util::sort::detail {
    template <typename IndexA, typename IndexB, typename RandomIt>
    void
    index_swap (IndexA a, IndexB b, RandomIt value)
    {
        std::swap (value[a], value[b]);
    }

    template <typename IndexA, typename IndexB, typename RandomIt, typename ...Tail>
    void
    index_swap (IndexA a, IndexB b, RandomIt value, Tail ...tail)
    {
        index_swap (a, b, value);
        index_swap (a, b, std::forward<Tail> (tail)...);
    };
}


///////////////////////////////////////////////////////////////////////////////
template <typename IndexIt, typename ValueIt, typename ...OtherIt>
void
cruft::util::sort::reorder (IndexIt idx_first,
                            IndexIt idx_last,
                            ValueIt value,
                            OtherIt ...tail)
{
    // Bail early on zero size arrays, partly so we can simplify some
    // debugging checks
    auto size = std::distance (idx_first, idx_last);
    if (!size)
        return;

    CHECK_LT (*std::max_element (idx_first, idx_last), size);

    for (decltype(size) i = 0; i < size - 1; ++i) {
        while (i != (decltype(size))idx_first[i]) {
            auto j = idx_first[i];

            detail::index_swap (i, j, value, std::forward<OtherIt> (tail)..., idx_first);
        }
    }
};


///////////////////////////////////////////////////////////////////////////////
template <typename RandomIt, class Comparator, class ...Args>
void
cruft::util::sort::soa (RandomIt key_first,
                        RandomIt key_last,
                        Comparator comp,
                        Args ...values)
{
    // bail early on guaranteed sorted or degenerate cases. we can make some
    // assumptions about minimum array sizes and non-wrapping indices later on
    // this way.
    if (std::distance (key_first, key_last) <= 1)
        return;

    // generate a list of indices into the key array and sort them so we have,
    // in effect, a sorted list of pointers.
    auto size = std::distance (key_first, key_last);
    std::vector<decltype(size)> indices (size);
    std::iota (std::begin (indices), std::end (indices), 0);

    std::sort (std::begin (indices),
               std::end   (indices),
               [&] (const auto &a, const auto &b)
    {
        return comp (key_first[a], key_first[b]);
    });

    // convert from a sorted list of pointers to a mapping of pointers to
    // desired final offsets. this is done so we can palm it off to the
    // reorder function.
    // TODO: avoid the need for this inverse array.
    decltype (indices) dest (indices.size ());
    for (decltype(size) i = 0; i < (decltype(size))dest.size (); ++i) {
        dest[indices[i]] = i;
    }

    // reorder all the arrays using the mapping we have discovered.
    reorder (std::begin (dest), std::end (dest), key_first, std::forward<Args> (values)...);
};
