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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#if defined(__UTIL_HASH_MURMUR_MURMUR2_IPP)
#error
#endif

#define __UTIL_HASH_MURMUR_MURMUR2_IPP


///////////////////////////////////////////////////////////////////////////////
namespace util::hash::murmur2::detail {
    template <typename T> struct constants;

    template <>
    struct constants<uint32_t> {
        static const uint32_t m = 0x5bd1e995;
        static const  int32_t r = 24;
    };

    template <>
    struct constants<uint64_t> {
        static const uint64_t m = 0xc6a4a7935bd1e995;
        static const  int64_t r = 47;
    };
}


/////////////////////////////////////////////////////////////////////
constexpr uint32_t
util::hash::murmur2::mix (uint32_t h, uint32_t k)
{
    constexpr uint32_t m = detail::constants<uint32_t>::m;
    constexpr uint32_t r = detail::constants<uint32_t>::r;

    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    return h;
}


//-----------------------------------------------------------------------------
// 64 bit murmur2 mixing function. Note the last two lines are swapped
// compared with 32 bit murmur2_mix. It's not clear if this is deliberate
// in the canonical implementation, so we just leave it to help compatibility.
constexpr uint64_t
util::hash::murmur2::mix (uint64_t h, uint64_t k)
{
    constexpr uint64_t m = detail::constants<uint64_t>::m;
    constexpr uint64_t r = detail::constants<uint64_t>::r;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;

    return h;
}
