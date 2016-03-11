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
 * Copyright 2012-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_PREPROCESSOR_HPP
#define __UTIL_PREPROCESSOR_HPP


#define PASTE_DETAIL(x, y) x##y
#define PASTE(x, y) PASTE_DETAIL(x, y)

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define STRINGIZE_LIST(x) STRINGIZE(x),


///////////////////////////////////////////////////////////////////////////////
// Varags functional style map
//
// Based off a technique seen on StackOverflow:
// http://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define MAP_01(F, X)      F(X)
#define MAP_02(F, X, ...) F(X)MAP_01(F, __VA_ARGS__)
#define MAP_03(F, X, ...) F(X)MAP_02(F, __VA_ARGS__)
#define MAP_04(F, X, ...) F(X)MAP_03(F, __VA_ARGS__)
#define MAP_05(F, X, ...) F(X)MAP_04(F, __VA_ARGS__)
#define MAP_06(F, X, ...) F(X)MAP_05(F, __VA_ARGS__)
#define MAP_07(F, X, ...) F(X)MAP_06(F, __VA_ARGS__)
#define MAP_08(F, X, ...) F(X)MAP_07(F, __VA_ARGS__)
#define MAP_09(F, X, ...) F(X)MAP_08(F, __VA_ARGS__)
#define MAP_10(F, X, ...) F(X)MAP_09(F, __VA_ARGS__)
#define MAP_11(F, X, ...) F(X)MAP_10(F, __VA_ARGS__)
#define MAP_12(F, X, ...) F(X)MAP_11(F, __VA_ARGS__)
#define MAP_13(F, X, ...) F(X)MAP_12(F, __VA_ARGS__)
#define MAP_14(F, X, ...) F(X)MAP_13(F, __VA_ARGS__)
#define MAP_15(F, X, ...) F(X)MAP_14(F, __VA_ARGS__)
#define MAP_16(F, X, ...) F(X)MAP_15(F, __VA_ARGS__)


// Uses the sliding pairs dispatch technique: by passing __VA_ARGS__ as the
// first set of variables and appending (reverse orderred) names of the
// corresponding macros, we get the name of the correct arity macro.
#define ARITY_DISPATCH(                 \
    _01,_02,_03,_04,_05,_06,_07,_08,    \
    _09,_10,_11,_12,_13,_14,_15,_16,    \
    NAME,...                            \
) NAME


// Map a macro across __VA_ARGS__. Can be easily expanded, but has non-obvious
// failure mode when supported arity is exceeded. Don't use this, unless you
// really need the stringize operator, or other macro magic.
//
// Just use C++ templates. Really...
#define MAP(FUNC,...)                                               \
ARITY_DISPATCH(__VA_ARGS__,                                         \
    MAP_16, MAP_15, MAP_14, MAP_13, MAP_12, MAP_11, MAP_10, MAP_09, \
    MAP_08, MAP_07, MAP_06, MAP_05, MAP_04, MAP_03, MAP_02, MAP_01  \
)(FUNC, __VA_ARGS__)

#endif
