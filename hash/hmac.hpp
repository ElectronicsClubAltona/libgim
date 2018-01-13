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

#ifndef CRUFT_UTIL_HASH_HMAC_HPP
#define CRUFT_UTIL_HASH_HMAC_HPP

#include "../debug.hpp"
#include "../view.hpp"

#include <algorithm>
#include <utility>


namespace util::hash {
    template <class HashT>
    /// RFC 2104 key-hashing for message authentication
    class HMAC {
    public:
        using digest_t = typename HashT::digest_t;


        //---------------------------------------------------------------------
        HMAC (util::view<const std::uint8_t*> key)
        {
            CHECK (!key.empty ());

            static_assert (sizeof (m_ikey) == sizeof (m_okey), "key padding must match");

            // If the key is larger than the blocklength, use the hash of the key
            if (key.size () > HashT::BLOCK_SIZE) {
                auto d = HashT{} (key);
                auto tail = std::copy (d.begin (), d.end (), m_ikey.begin ());
                std::fill (tail, std::end (m_ikey), 0);
                // Use the key directly
            } else {
                auto tail = std::copy (key.begin (), key.end (), m_ikey.begin ());
                std::fill (tail, m_ikey.end (), 0);
            }

            // copy and xor the key data to the okey
            std::transform (
                std::begin (m_ikey),
                std::end   (m_ikey),
                std::begin (m_okey),
                [] (auto v) { return v ^ OFILL; });

            // just xor the ikey in place
            std::transform (
                m_ikey.begin (),
                m_ikey.end (),
                m_ikey.begin (),
                [] (auto v) { return v ^ IFILL; });
        }


        //---------------------------------------------------------------------
        template <typename ...DataT>
        digest_t
        operator() (DataT&&...data) const noexcept
        {
            HashT h;
            return h (m_okey, h (m_ikey, std::forward<DataT> (data)...));
        };


    private:
        //---------------------------------------------------------------------
        static constexpr uint8_t IFILL = 0x36;
        static constexpr uint8_t OFILL = 0x5C;

        std::array<uint8_t,HashT::BLOCK_SIZE> m_ikey;
        std::array<uint8_t,HashT::BLOCK_SIZE> m_okey;
    };
}

#endif
