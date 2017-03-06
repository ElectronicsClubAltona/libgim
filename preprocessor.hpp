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

#define MAP_0_17(F, X, ...) F(X)MAP_0_16(F, __VA_ARGS__)
#define MAP_0_18(F, X, ...) F(X)MAP_0_17(F, __VA_ARGS__)
#define MAP_0_19(F, X, ...) F(X)MAP_0_18(F, __VA_ARGS__)
#define MAP_0_20(F, X, ...) F(X)MAP_0_19(F, __VA_ARGS__)
#define MAP_0_21(F, X, ...) F(X)MAP_0_20(F, __VA_ARGS__)
#define MAP_0_22(F, X, ...) F(X)MAP_0_21(F, __VA_ARGS__)
#define MAP_0_23(F, X, ...) F(X)MAP_0_22(F, __VA_ARGS__)
#define MAP_0_24(F, X, ...) F(X)MAP_0_23(F, __VA_ARGS__)

#define MAP_0_25(F, X, ...) F(X)MAP_0_24(F, __VA_ARGS__)
#define MAP_0_26(F, X, ...) F(X)MAP_0_25(F, __VA_ARGS__)
#define MAP_0_27(F, X, ...) F(X)MAP_0_26(F, __VA_ARGS__)
#define MAP_0_28(F, X, ...) F(X)MAP_0_27(F, __VA_ARGS__)
#define MAP_0_29(F, X, ...) F(X)MAP_0_28(F, __VA_ARGS__)
#define MAP_0_30(F, X, ...) F(X)MAP_0_29(F, __VA_ARGS__)
#define MAP_0_31(F, X, ...) F(X)MAP_0_30(F, __VA_ARGS__)
#define MAP_0_32(F, X, ...) F(X)MAP_0_31(F, __VA_ARGS__)

#define MAP_0_33(F, X, ...) F(X)MAP_0_32(F, __VA_ARGS__)
#define MAP_0_34(F, X, ...) F(X)MAP_0_33(F, __VA_ARGS__)
#define MAP_0_35(F, X, ...) F(X)MAP_0_34(F, __VA_ARGS__)
#define MAP_0_36(F, X, ...) F(X)MAP_0_35(F, __VA_ARGS__)
#define MAP_0_37(F, X, ...) F(X)MAP_0_36(F, __VA_ARGS__)
#define MAP_0_38(F, X, ...) F(X)MAP_0_37(F, __VA_ARGS__)
#define MAP_0_39(F, X, ...) F(X)MAP_0_38(F, __VA_ARGS__)
#define MAP_0_40(F, X, ...) F(X)MAP_0_39(F, __VA_ARGS__)

#define MAP_0_41(F, X, ...) F(X)MAP_0_40(F, __VA_ARGS__)
#define MAP_0_42(F, X, ...) F(X)MAP_0_41(F, __VA_ARGS__)
#define MAP_0_43(F, X, ...) F(X)MAP_0_42(F, __VA_ARGS__)
#define MAP_0_44(F, X, ...) F(X)MAP_0_43(F, __VA_ARGS__)
#define MAP_0_45(F, X, ...) F(X)MAP_0_44(F, __VA_ARGS__)
#define MAP_0_46(F, X, ...) F(X)MAP_0_45(F, __VA_ARGS__)
#define MAP_0_47(F, X, ...) F(X)MAP_0_46(F, __VA_ARGS__)
#define MAP_0_48(F, X, ...) F(X)MAP_0_47(F, __VA_ARGS__)

#define MAP_0_49(F, X, ...) F(X)MAP_0_48(F, __VA_ARGS__)
#define MAP_0_50(F, X, ...) F(X)MAP_0_49(F, __VA_ARGS__)
#define MAP_0_51(F, X, ...) F(X)MAP_0_50(F, __VA_ARGS__)
#define MAP_0_52(F, X, ...) F(X)MAP_0_51(F, __VA_ARGS__)
#define MAP_0_53(F, X, ...) F(X)MAP_0_52(F, __VA_ARGS__)
#define MAP_0_54(F, X, ...) F(X)MAP_0_53(F, __VA_ARGS__)
#define MAP_0_55(F, X, ...) F(X)MAP_0_54(F, __VA_ARGS__)
#define MAP_0_56(F, X, ...) F(X)MAP_0_55(F, __VA_ARGS__)

#define MAP_0_57(F, X, ...) F(X)MAP_0_56(F, __VA_ARGS__)
#define MAP_0_58(F, X, ...) F(X)MAP_0_57(F, __VA_ARGS__)
#define MAP_0_59(F, X, ...) F(X)MAP_0_58(F, __VA_ARGS__)
#define MAP_0_60(F, X, ...) F(X)MAP_0_59(F, __VA_ARGS__)
#define MAP_0_61(F, X, ...) F(X)MAP_0_60(F, __VA_ARGS__)
#define MAP_0_62(F, X, ...) F(X)MAP_0_61(F, __VA_ARGS__)
#define MAP_0_63(F, X, ...) F(X)MAP_0_62(F, __VA_ARGS__)
#define MAP_0_64(F, X, ...) F(X)MAP_0_63(F, __VA_ARGS__)

#define MAP_0_65(F, X, ...) F(X)MAP_0_64(F, __VA_ARGS__)
#define MAP_0_66(F, X, ...) F(X)MAP_0_65(F, __VA_ARGS__)
#define MAP_0_67(F, X, ...) F(X)MAP_0_66(F, __VA_ARGS__)
#define MAP_0_68(F, X, ...) F(X)MAP_0_67(F, __VA_ARGS__)
#define MAP_0_69(F, X, ...) F(X)MAP_0_68(F, __VA_ARGS__)
#define MAP_0_70(F, X, ...) F(X)MAP_0_69(F, __VA_ARGS__)
#define MAP_0_71(F, X, ...) F(X)MAP_0_70(F, __VA_ARGS__)
#define MAP_0_72(F, X, ...) F(X)MAP_0_71(F, __VA_ARGS__)

#define MAP_0_73(F, X, ...) F(X)MAP_0_72(F, __VA_ARGS__)
#define MAP_0_74(F, X, ...) F(X)MAP_0_73(F, __VA_ARGS__)
#define MAP_0_75(F, X, ...) F(X)MAP_0_74(F, __VA_ARGS__)
#define MAP_0_76(F, X, ...) F(X)MAP_0_75(F, __VA_ARGS__)
#define MAP_0_77(F, X, ...) F(X)MAP_0_76(F, __VA_ARGS__)
#define MAP_0_78(F, X, ...) F(X)MAP_0_77(F, __VA_ARGS__)
#define MAP_0_79(F, X, ...) F(X)MAP_0_78(F, __VA_ARGS__)
#define MAP_0_80(F, X, ...) F(X)MAP_0_79(F, __VA_ARGS__)

#define MAP_0_81(F, X, ...) F(X)MAP_0_80(F, __VA_ARGS__)
#define MAP_0_82(F, X, ...) F(X)MAP_0_81(F, __VA_ARGS__)
#define MAP_0_83(F, X, ...) F(X)MAP_0_82(F, __VA_ARGS__)
#define MAP_0_84(F, X, ...) F(X)MAP_0_83(F, __VA_ARGS__)
#define MAP_0_85(F, X, ...) F(X)MAP_0_84(F, __VA_ARGS__)
#define MAP_0_86(F, X, ...) F(X)MAP_0_85(F, __VA_ARGS__)
#define MAP_0_87(F, X, ...) F(X)MAP_0_86(F, __VA_ARGS__)
#define MAP_0_88(F, X, ...) F(X)MAP_0_87(F, __VA_ARGS__)

#define MAP_0_89(F, X, ...) F(X)MAP_0_88(F, __VA_ARGS__)
#define MAP_0_90(F, X, ...) F(X)MAP_0_89(F, __VA_ARGS__)
#define MAP_0_91(F, X, ...) F(X)MAP_0_90(F, __VA_ARGS__)
#define MAP_0_92(F, X, ...) F(X)MAP_0_91(F, __VA_ARGS__)
#define MAP_0_93(F, X, ...) F(X)MAP_0_92(F, __VA_ARGS__)
#define MAP_0_94(F, X, ...) F(X)MAP_0_93(F, __VA_ARGS__)
#define MAP_0_95(F, X, ...) F(X)MAP_0_94(F, __VA_ARGS__)
#define MAP_0_96(F, X, ...) F(X)MAP_0_95(F, __VA_ARGS__)


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

#define MAP_1_17(F, A, X, ...) F(A, X)MAP_1_16(F, A, __VA_ARGS__)
#define MAP_1_18(F, A, X, ...) F(A, X)MAP_1_17(F, A, __VA_ARGS__)
#define MAP_1_19(F, A, X, ...) F(A, X)MAP_1_18(F, A, __VA_ARGS__)
#define MAP_1_20(F, A, X, ...) F(A, X)MAP_1_19(F, A, __VA_ARGS__)
#define MAP_1_21(F, A, X, ...) F(A, X)MAP_1_20(F, A, __VA_ARGS__)
#define MAP_1_22(F, A, X, ...) F(A, X)MAP_1_21(F, A, __VA_ARGS__)
#define MAP_1_23(F, A, X, ...) F(A, X)MAP_1_22(F, A, __VA_ARGS__)
#define MAP_1_24(F, A, X, ...) F(A, X)MAP_1_23(F, A, __VA_ARGS__)

#define MAP_1_25(F, A, X, ...) F(A, X)MAP_1_24(F, A, __VA_ARGS__)
#define MAP_1_26(F, A, X, ...) F(A, X)MAP_1_25(F, A, __VA_ARGS__)
#define MAP_1_27(F, A, X, ...) F(A, X)MAP_1_26(F, A, __VA_ARGS__)
#define MAP_1_28(F, A, X, ...) F(A, X)MAP_1_27(F, A, __VA_ARGS__)
#define MAP_1_29(F, A, X, ...) F(A, X)MAP_1_28(F, A, __VA_ARGS__)
#define MAP_1_30(F, A, X, ...) F(A, X)MAP_1_29(F, A, __VA_ARGS__)
#define MAP_1_31(F, A, X, ...) F(A, X)MAP_1_30(F, A, __VA_ARGS__)
#define MAP_1_32(F, A, X, ...) F(A, X)MAP_1_31(F, A, __VA_ARGS__)

#define MAP_1_33(F, A, X, ...) F(A, X)MAP_1_32(F, A, __VA_ARGS__)
#define MAP_1_34(F, A, X, ...) F(A, X)MAP_1_33(F, A, __VA_ARGS__)
#define MAP_1_35(F, A, X, ...) F(A, X)MAP_1_34(F, A, __VA_ARGS__)
#define MAP_1_36(F, A, X, ...) F(A, X)MAP_1_35(F, A, __VA_ARGS__)
#define MAP_1_37(F, A, X, ...) F(A, X)MAP_1_36(F, A, __VA_ARGS__)
#define MAP_1_38(F, A, X, ...) F(A, X)MAP_1_37(F, A, __VA_ARGS__)
#define MAP_1_39(F, A, X, ...) F(A, X)MAP_1_38(F, A, __VA_ARGS__)
#define MAP_1_40(F, A, X, ...) F(A, X)MAP_1_39(F, A, __VA_ARGS__)

#define MAP_1_41(F, A, X, ...) F(A, X)MAP_1_40(F, A, __VA_ARGS__)
#define MAP_1_42(F, A, X, ...) F(A, X)MAP_1_41(F, A, __VA_ARGS__)
#define MAP_1_43(F, A, X, ...) F(A, X)MAP_1_42(F, A, __VA_ARGS__)
#define MAP_1_44(F, A, X, ...) F(A, X)MAP_1_43(F, A, __VA_ARGS__)
#define MAP_1_45(F, A, X, ...) F(A, X)MAP_1_44(F, A, __VA_ARGS__)
#define MAP_1_46(F, A, X, ...) F(A, X)MAP_1_45(F, A, __VA_ARGS__)
#define MAP_1_47(F, A, X, ...) F(A, X)MAP_1_46(F, A, __VA_ARGS__)
#define MAP_1_48(F, A, X, ...) F(A, X)MAP_1_47(F, A, __VA_ARGS__)

#define MAP_1_49(F, A, X, ...) F(A, X)MAP_1_48(F, A, __VA_ARGS__)
#define MAP_1_50(F, A, X, ...) F(A, X)MAP_1_49(F, A, __VA_ARGS__)
#define MAP_1_51(F, A, X, ...) F(A, X)MAP_1_50(F, A, __VA_ARGS__)
#define MAP_1_52(F, A, X, ...) F(A, X)MAP_1_51(F, A, __VA_ARGS__)
#define MAP_1_53(F, A, X, ...) F(A, X)MAP_1_52(F, A, __VA_ARGS__)
#define MAP_1_54(F, A, X, ...) F(A, X)MAP_1_53(F, A, __VA_ARGS__)
#define MAP_1_55(F, A, X, ...) F(A, X)MAP_1_54(F, A, __VA_ARGS__)
#define MAP_1_56(F, A, X, ...) F(A, X)MAP_1_55(F, A, __VA_ARGS__)

#define MAP_1_57(F, A, X, ...) F(A, X)MAP_1_56(F, A, __VA_ARGS__)
#define MAP_1_58(F, A, X, ...) F(A, X)MAP_1_57(F, A, __VA_ARGS__)
#define MAP_1_59(F, A, X, ...) F(A, X)MAP_1_58(F, A, __VA_ARGS__)
#define MAP_1_60(F, A, X, ...) F(A, X)MAP_1_59(F, A, __VA_ARGS__)
#define MAP_1_61(F, A, X, ...) F(A, X)MAP_1_60(F, A, __VA_ARGS__)
#define MAP_1_62(F, A, X, ...) F(A, X)MAP_1_61(F, A, __VA_ARGS__)
#define MAP_1_63(F, A, X, ...) F(A, X)MAP_1_62(F, A, __VA_ARGS__)
#define MAP_1_64(F, A, X, ...) F(A, X)MAP_1_63(F, A, __VA_ARGS__)

#define MAP_1_65(F, A, X, ...) F(A, X)MAP_1_64(F, A, __VA_ARGS__)
#define MAP_1_66(F, A, X, ...) F(A, X)MAP_1_65(F, A, __VA_ARGS__)
#define MAP_1_67(F, A, X, ...) F(A, X)MAP_1_66(F, A, __VA_ARGS__)
#define MAP_1_68(F, A, X, ...) F(A, X)MAP_1_67(F, A, __VA_ARGS__)
#define MAP_1_69(F, A, X, ...) F(A, X)MAP_1_68(F, A, __VA_ARGS__)
#define MAP_1_70(F, A, X, ...) F(A, X)MAP_1_69(F, A, __VA_ARGS__)
#define MAP_1_71(F, A, X, ...) F(A, X)MAP_1_70(F, A, __VA_ARGS__)
#define MAP_1_72(F, A, X, ...) F(A, X)MAP_1_71(F, A, __VA_ARGS__)

#define MAP_1_73(F, A, X, ...) F(A, X)MAP_1_72(F, A, __VA_ARGS__)
#define MAP_1_74(F, A, X, ...) F(A, X)MAP_1_73(F, A, __VA_ARGS__)
#define MAP_1_75(F, A, X, ...) F(A, X)MAP_1_74(F, A, __VA_ARGS__)
#define MAP_1_76(F, A, X, ...) F(A, X)MAP_1_75(F, A, __VA_ARGS__)
#define MAP_1_77(F, A, X, ...) F(A, X)MAP_1_76(F, A, __VA_ARGS__)
#define MAP_1_78(F, A, X, ...) F(A, X)MAP_1_77(F, A, __VA_ARGS__)
#define MAP_1_79(F, A, X, ...) F(A, X)MAP_1_78(F, A, __VA_ARGS__)
#define MAP_1_80(F, A, X, ...) F(A, X)MAP_1_79(F, A, __VA_ARGS__)

#define MAP_1_81(F, A, X, ...) F(A, X)MAP_1_80(F, A, __VA_ARGS__)
#define MAP_1_82(F, A, X, ...) F(A, X)MAP_1_81(F, A, __VA_ARGS__)
#define MAP_1_83(F, A, X, ...) F(A, X)MAP_1_82(F, A, __VA_ARGS__)
#define MAP_1_84(F, A, X, ...) F(A, X)MAP_1_83(F, A, __VA_ARGS__)
#define MAP_1_85(F, A, X, ...) F(A, X)MAP_1_84(F, A, __VA_ARGS__)
#define MAP_1_86(F, A, X, ...) F(A, X)MAP_1_85(F, A, __VA_ARGS__)
#define MAP_1_87(F, A, X, ...) F(A, X)MAP_1_86(F, A, __VA_ARGS__)
#define MAP_1_88(F, A, X, ...) F(A, X)MAP_1_87(F, A, __VA_ARGS__)

#define MAP_1_89(F, A, X, ...) F(A, X)MAP_1_88(F, A, __VA_ARGS__)
#define MAP_1_90(F, A, X, ...) F(A, X)MAP_1_89(F, A, __VA_ARGS__)
#define MAP_1_91(F, A, X, ...) F(A, X)MAP_1_90(F, A, __VA_ARGS__)
#define MAP_1_92(F, A, X, ...) F(A, X)MAP_1_91(F, A, __VA_ARGS__)
#define MAP_1_93(F, A, X, ...) F(A, X)MAP_1_92(F, A, __VA_ARGS__)
#define MAP_1_94(F, A, X, ...) F(A, X)MAP_1_93(F, A, __VA_ARGS__)
#define MAP_1_95(F, A, X, ...) F(A, X)MAP_1_94(F, A, __VA_ARGS__)
#define MAP_1_96(F, A, X, ...) F(A, X)MAP_1_95(F, A, __VA_ARGS__)

// Uses the sliding pairs dispatch technique: by passing __VA_ARGS__ as the
// first set of variables and appending (reverse orderred) names of the
// corresponding macros, we get the name of the correct arity macro.
//
// Ensure this is called with at least N+2 parameters otherwise GCC in
// particular will complain about requiring at least one value for varargs
#define ARITY_DISPATCH(                 \
    _01,_02,_03,_04,_05,_06,_07,_08,    \
    _09,_10,_11,_12,_13,_14,_15,_16,    \
    _17,_18,_19,_20,_21,_22,_23,_24,    \
    _25,_26,_27,_28,_29,_30,_31,_32,    \
    _33,_34,_35,_36,_37,_38,_39,_40,    \
    _41,_42,_43,_44,_45,_46,_47,_48,    \
    _49,_50,_51,_52,_53,_54,_55,_56,    \
    _57,_58,_59,_60,_61,_62,_63,_64,    \
    _65,_66,_67,_68,_69,_70,_71,_72,    \
    _73,_74,_75,_76,_77,_78,_79,_80,    \
    _81,_82,_83,_84,_85,_86,_87,_88,    \
    _89,_90,_91,_92,_93,_94,_95,_96,    \
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
    MAP_0_96, MAP_0_95, MAP_0_94, MAP_0_93, MAP_0_92, MAP_0_91, MAP_0_90, MAP_0_89, \
    MAP_0_88, MAP_0_87, MAP_0_86, MAP_0_85, MAP_0_84, MAP_0_83, MAP_0_82, MAP_0_81, \
    MAP_0_80, MAP_0_79, MAP_0_78, MAP_0_77, MAP_0_76, MAP_0_75, MAP_0_74, MAP_0_73, \
    MAP_0_72, MAP_0_71, MAP_0_70, MAP_0_69, MAP_0_68, MAP_0_67, MAP_0_66, MAP_0_65, \
    MAP_0_64, MAP_0_63, MAP_0_62, MAP_0_61, MAP_0_60, MAP_0_59, MAP_0_58, MAP_0_57, \
    MAP_0_56, MAP_0_55, MAP_0_54, MAP_0_53, MAP_0_52, MAP_0_51, MAP_0_50, MAP_0_49, \
    MAP_0_48, MAP_0_47, MAP_0_46, MAP_0_45, MAP_0_44, MAP_0_43, MAP_0_42, MAP_0_41, \
    MAP_0_40, MAP_0_39, MAP_0_38, MAP_0_37, MAP_0_36, MAP_0_35, MAP_0_34, MAP_0_33, \
    MAP_0_32, MAP_0_31, MAP_0_30, MAP_0_29, MAP_0_28, MAP_0_27, MAP_0_26, MAP_0_25, \
    MAP_0_24, MAP_0_23, MAP_0_22, MAP_0_21, MAP_0_20, MAP_0_19, MAP_0_18, MAP_0_17, \
    MAP_0_16, MAP_0_15, MAP_0_14, MAP_0_13, MAP_0_12, MAP_0_11, MAP_0_10, MAP_0_09, \
    MAP_0_08, MAP_0_07, MAP_0_06, MAP_0_05, MAP_0_04, MAP_0_03, MAP_0_02, MAP_0_01, \
    STATIC_ASSERT("invalid arity for MAP")                                          \
)(FUNC, __VA_ARGS__)


// Note: this is one fewer argument than MAP because of the leading parameter
// passed to all invokations.
#define MAP1(FUNC,...)                                                              \
ARITY_DISPATCH(__VA_ARGS__,                                                         \
              MAP_1_95, MAP_1_94, MAP_1_93, MAP_1_92, MAP_1_91, MAP_1_90, MAP_1_89, \
    MAP_1_88, MAP_1_87, MAP_1_86, MAP_1_85, MAP_1_84, MAP_1_83, MAP_1_82, MAP_1_81, \
    MAP_1_80, MAP_1_79, MAP_1_78, MAP_1_77, MAP_1_76, MAP_1_75, MAP_1_74, MAP_1_73, \
    MAP_1_72, MAP_1_71, MAP_1_70, MAP_1_69, MAP_1_68, MAP_1_67, MAP_1_66, MAP_1_65, \
    MAP_1_64, MAP_1_63, MAP_1_62, MAP_1_61, MAP_1_60, MAP_1_59, MAP_1_58, MAP_1_57, \
    MAP_1_56, MAP_1_55, MAP_1_54, MAP_1_53, MAP_1_52, MAP_1_51, MAP_1_50, MAP_1_49, \
    MAP_1_48, MAP_1_47, MAP_1_46, MAP_1_45, MAP_1_44, MAP_1_43, MAP_1_42, MAP_1_41, \
    MAP_1_40, MAP_1_39, MAP_1_38, MAP_1_37, MAP_1_36, MAP_1_35, MAP_1_34, MAP_1_33, \
    MAP_1_32, MAP_1_31, MAP_1_30, MAP_1_29, MAP_1_28, MAP_1_27, MAP_1_26, MAP_1_25, \
    MAP_1_24, MAP_1_23, MAP_1_22, MAP_1_21, MAP_1_20, MAP_1_19, MAP_1_18, MAP_1_17, \
    MAP_1_16, MAP_1_15, MAP_1_14, MAP_1_13, MAP_1_12, MAP_1_11, MAP_1_10, MAP_1_09, \
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
#define LITERAL_17 17
#define LITERAL_18 18
#define LITERAL_19 19
#define LITERAL_20 20
#define LITERAL_21 21
#define LITERAL_22 22
#define LITERAL_23 23
#define LITERAL_24 24
#define LITERAL_25 25
#define LITERAL_26 26
#define LITERAL_27 27
#define LITERAL_28 28
#define LITERAL_29 29
#define LITERAL_30 30
#define LITERAL_31 31
#define LITERAL_32 32
#define LITERAL_33 33
#define LITERAL_34 34
#define LITERAL_35 35
#define LITERAL_36 36
#define LITERAL_37 37
#define LITERAL_38 38
#define LITERAL_39 39
#define LITERAL_40 40
#define LITERAL_41 41
#define LITERAL_42 42
#define LITERAL_43 43
#define LITERAL_44 44
#define LITERAL_45 45
#define LITERAL_46 46
#define LITERAL_47 47
#define LITERAL_48 48
#define LITERAL_49 49
#define LITERAL_50 50
#define LITERAL_51 51
#define LITERAL_52 52
#define LITERAL_53 53
#define LITERAL_54 54
#define LITERAL_55 55
#define LITERAL_56 56
#define LITERAL_57 57
#define LITERAL_58 58
#define LITERAL_59 59
#define LITERAL_60 60
#define LITERAL_61 61
#define LITERAL_62 62
#define LITERAL_63 63
#define LITERAL_64 64
#define LITERAL_65 65
#define LITERAL_66 66
#define LITERAL_67 67
#define LITERAL_68 68
#define LITERAL_69 69
#define LITERAL_70 70
#define LITERAL_71 71
#define LITERAL_72 72
#define LITERAL_73 73
#define LITERAL_74 74
#define LITERAL_75 75
#define LITERAL_76 76
#define LITERAL_77 77
#define LITERAL_78 78
#define LITERAL_79 79
#define LITERAL_80 80
#define LITERAL_81 81
#define LITERAL_82 82
#define LITERAL_83 83
#define LITERAL_84 84
#define LITERAL_85 85
#define LITERAL_86 86
#define LITERAL_87 87
#define LITERAL_88 88
#define LITERAL_89 89
#define LITERAL_90 90
#define LITERAL_91 91
#define LITERAL_92 92
#define LITERAL_93 93
#define LITERAL_94 94
#define LITERAL_95 95
#define LITERAL_96 96


#define VA_ARGS_COUNT(...)                          \
ARITY_DISPATCH(__VA_ARGS__,                         \
    LITERAL_96, LITERAL_95, LITERAL_94, LITERAL_93, \
    LITERAL_92, LITERAL_91, LITERAL_90, LITERAL_89, \
    LITERAL_88, LITERAL_87, LITERAL_86, LITERAL_85, \
    LITERAL_84, LITERAL_83, LITERAL_82, LITERAL_81, \
    LITERAL_80, LITERAL_79, LITERAL_78, LITERAL_77, \
    LITERAL_76, LITERAL_75, LITERAL_74, LITERAL_73, \
    LITERAL_72, LITERAL_71, LITERAL_70, LITERAL_69, \
    LITERAL_68, LITERAL_67, LITERAL_66, LITERAL_65, \
    LITERAL_64, LITERAL_63, LITERAL_62, LITERAL_61, \
    LITERAL_60, LITERAL_59, LITERAL_58, LITERAL_57, \
    LITERAL_56, LITERAL_55, LITERAL_54, LITERAL_53, \
    LITERAL_52, LITERAL_51, LITERAL_50, LITERAL_49, \
    LITERAL_48, LITERAL_47, LITERAL_46, LITERAL_45, \
    LITERAL_44, LITERAL_43, LITERAL_42, LITERAL_41, \
    LITERAL_40, LITERAL_39, LITERAL_38, LITERAL_37, \
    LITERAL_36, LITERAL_35, LITERAL_34, LITERAL_33, \
    LITERAL_32, LITERAL_31, LITERAL_30, LITERAL_29, \
    LITERAL_28, LITERAL_27, LITERAL_26, LITERAL_25, \
    LITERAL_24, LITERAL_23, LITERAL_22, LITERAL_21, \
    LITERAL_20, LITERAL_19, LITERAL_18, LITERAL_17, \
    LITERAL_16, LITERAL_15, LITERAL_14, LITERAL_13, \
    LITERAL_12, LITERAL_11, LITERAL_10, LITERAL_09, \
    LITERAL_08, LITERAL_07, LITERAL_06, LITERAL_05, \
    LITERAL_04, LITERAL_03, LITERAL_02, LITERAL_01, \
    STATIC_ASSERT("invalid value for VA_ARGS_COUNT")\
)


#endif
