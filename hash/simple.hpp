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
 * Copyright 2016, Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_HASH_SIMPLE_HPP
#define __UTIL_HASH_SIMPLE_HPP

#include <cstdint>
#include <utility>


///////////////////////////////////////////////////////////////////////////////
namespace util::hash {
    //template <class H, class InputT>
    template <class H, typename ...Args>
    typename H::digest_t
    simple (const void *restrict first, const void *restrict last, Args&&...args) noexcept
    {
        H h (std::forward<Args> (args)...);

        h.update (
            static_cast<const uint8_t*restrict> (first),
            static_cast<const uint8_t*restrict> (last)
        );
        h.finish ();

        return h.digest ();
    }
}

#endif
