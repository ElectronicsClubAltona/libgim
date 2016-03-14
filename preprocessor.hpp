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

#define PASTE_LIST(x,y) PASTE(x,y),

#define NAMESPACE_LIST(x,y) x::y,

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define STRINGIZE_LIST(x) STRINGIZE(x),

#define STATIC_ASSERT(MSG) static_assert(false, MSG);

///////////////////////////////////////////////////////////////////////////////
// Varags functional style map
//
// Based off a technique seen on StackOverflow:
// http://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define MAP_0_01(F, X)      F(X)
#define MAP_0_02(F, X, ...) F(X)MAP_0_01(F, __VA_ARGS__)
#define MAP_0_03(F, X, ...) F(X)MAP_0_02(F, __VA_ARGS__)
#define MAP_0_04(F, X, ...) F(X)MAP_0_03(F, __VA_ARGS__)
#define MAP_0_05(F, X, ...) F(X)MAP_0_04(F, __VA_ARGS__)
#define MAP_0_06(F, X, ...) F(X)MAP_0_05(F, __VA_ARGS__)
#define MAP_0_07(F, X, ...) F(X)MAP_0_06(F, __VA_ARGS__)
#define MAP_0_08(F, X, ...) F(X)MAP_0_07(F, __VA_ARGS__)
#define MAP_0_09(F, X, ...) F(X)MAP_0_08(F, __VA_ARGS__)
#define MAP_0_10(F, X, ...) F(X)MAP_0_09(F, __VA_ARGS__)
#define MAP_0_11(F, X, ...) F(X)MAP_0_10(F, __VA_ARGS__)
#define MAP_0_12(F, X, ...) F(X)MAP_0_11(F, __VA_ARGS__)
#define MAP_0_13(F, X, ...) F(X)MAP_0_12(F, __VA_ARGS__)
#define MAP_0_14(F, X, ...) F(X)MAP_0_13(F, __VA_ARGS__)
#define MAP_0_15(F, X, ...) F(X)MAP_0_14(F, __VA_ARGS__)
#define MAP_0_16(F, X, ...) F(X)MAP_0_15(F, __VA_ARGS__)

#define MAP_1_01(F, A, X)      F(A, X)
#define MAP_1_02(F, A, X, ...) F(A, X)MAP_1_01(F, A, __VA_ARGS__)
#define MAP_1_03(F, A, X, ...) F(A, X)MAP_1_02(F, A, __VA_ARGS__)
#define MAP_1_04(F, A, X, ...) F(A, X)MAP_1_03(F, A, __VA_ARGS__)
#define MAP_1_05(F, A, X, ...) F(A, X)MAP_1_04(F, A, __VA_ARGS__)
#define MAP_1_06(F, A, X, ...) F(A, X)MAP_1_05(F, A, __VA_ARGS__)
#define MAP_1_07(F, A, X, ...) F(A, X)MAP_1_06(F, A, __VA_ARGS__)
#define MAP_1_08(F, A, X, ...) F(A, X)MAP_1_07(F, A, __VA_ARGS__)
#define MAP_1_09(F, A, X, ...) F(A, X)MAP_1_08(F, A, __VA_ARGS__)
#define MAP_1_10(F, A, X, ...) F(A, X)MAP_1_09(F, A, __VA_ARGS__)
#define MAP_1_11(F, A, X, ...) F(A, X)MAP_1_10(F, A, __VA_ARGS__)
#define MAP_1_12(F, A, X, ...) F(A, X)MAP_1_11(F, A, __VA_ARGS__)
#define MAP_1_13(F, A, X, ...) F(A, X)MAP_1_12(F, A, __VA_ARGS__)
#define MAP_1_14(F, A, X, ...) F(A, X)MAP_1_13(F, A, __VA_ARGS__)
#define MAP_1_15(F, A, X, ...) F(A, X)MAP_1_14(F, A, __VA_ARGS__)
#define MAP_1_16(F, A, X, ...) F(A, X)MAP_1_15(F, A, __VA_ARGS__)

// Uses the sliding pairs dispatch technique: by passing __VA_ARGS__ as the
// first set of variables and appending (reverse orderred) names of the
// corresponding macros, we get the name of the correct arity macro.
//
// Ensure this is called with at least N+2 parameters otherwise GCC in
// particular will complain about requiring at least one value for varargs
#define ARITY_DISPATCH(                 \
    _01,_02,_03,_04,_05,_06,_07,_08,    \
    _09,_10,_11,_12,_13,_14,_15,_16,    \
    NAME,...                            \
) NAME

// Map a macro across __VA_ARGS__. Can be easily expanded, but has non-obvious
// failure mode when supported arity is exceeded.
//
// Don't use this, unless you really need the stringize operator, or other
// macro magic.
//
// Just use templates. Really...
#define MAP(FUNC,...)                                                               \
ARITY_DISPATCH(__VA_ARGS__,                                                         \
    MAP_0_16, MAP_0_15, MAP_0_14, MAP_0_13, MAP_0_12, MAP_0_11, MAP_0_10, MAP_0_09, \
    MAP_0_08, MAP_0_07, MAP_0_06, MAP_0_05, MAP_0_04, MAP_0_03, MAP_0_02, MAP_0_01, \
    STATIC_ASSERT("invalid arity for MAP")                                          \
)(FUNC, __VA_ARGS__)

#define MAP1(FUNC,...)                                                              \
ARITY_DISPATCH(__VA_ARGS__,                                                         \
              MAP_1_15, MAP_1_14, MAP_1_13, MAP_1_12, MAP_1_11, MAP_1_10, MAP_1_09, \
    MAP_1_08, MAP_1_07, MAP_1_06, MAP_1_05, MAP_1_04, MAP_1_03, MAP_1_02, MAP_1_01, \
    STATIC_ASSERT("invalid arity for MAP1")                                         \
)(FUNC, __VA_ARGS__)



#define LITERAL_00  0
#define LITERAL_01  1
#define LITERAL_02  2
#define LITERAL_03  3
#define LITERAL_04  4
#define LITERAL_05  5
#define LITERAL_06  6
#define LITERAL_07  7
#define LITERAL_08  8
#define LITERAL_09  9
#define LITERAL_10 10
#define LITERAL_11 11
#define LITERAL_12 12
#define LITERAL_13 13
#define LITERAL_14 14
#define LITERAL_15 15
#define LITERAL_16 16


#define VA_ARGS_COUNT(...)                          \
ARITY_DISPATCH(__VA_ARGS__,                         \
    LITERAL_16, LITERAL_15, LITERAL_14, LITERAL_13, \
    LITERAL_12, LITERAL_11, LITERAL_10, LITERAL_09, \
    LITERAL_08, LITERAL_07, LITERAL_06, LITERAL_05, \
    LITERAL_04, LITERAL_03, LITERAL_02, LITERAL_01, \
    STATIC_ASSERT("invalid value for VA_ARGS_COUNT")\
)


#endif
