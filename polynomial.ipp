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


#ifdef __UTIL_POLYNOMIAL_IPP
#error "twice included ipp"
#endif

#define __UTIL_POLYNOMIAL_IPP


//-----------------------------------------------------------------------------
template <size_t S, typename T, typename U>
T
util::polynomial::eval (const std::array<T,S> coeffs, const U x)
{
    U x_ = 1.f;
    T sum {0.f};

    for (size_t i = 0; i < S; ++i) {
        sum += coeffs[S-i-1] * x_;
        x_ *= x;
    }

    return sum;
}

