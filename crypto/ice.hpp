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

#ifndef __CRYPTO_ICE_HPP
#define __CRYPTO_ICE_HPP

#include <cstdint>
#include <array>
#include <vector>

// An implementation of the ICE symmetric-key block cipher
//
// we make a token attempt to zero our buffers, but we can't guarantee this
// will take place (given compiler optimisations). further security is
// outside the scope of this class.
class ice {
    public:
        ice (unsigned n, uint64_t key);

        ice (unsigned n,
             const uint64_t *key_first,
             const uint64_t *key_last);
        ~ice ();

        void
        set (const uint64_t *key_first, const uint64_t *key_last);

        uint64_t encrypt (uint64_t plaintext) const;
        uint64_t decrypt (uint64_t ciphertext) const;

        unsigned key_size () const;
        unsigned block_size () const;

        using subkey_t = std::array<uint32_t,3>;

    private:
        void
        scheduleBuild (std::array<uint16_t,4> &k,
                       int n,
                       const std::array<uint_fast8_t,8> &keyrot);

        unsigned   m_size;
        unsigned   m_rounds;

        std::vector<subkey_t> m_schedule;
};

struct ice_error : public std::runtime_error { using runtime_error::runtime_error; };
struct level_error : public ice_error { using ice_error::ice_error; };

#endif
