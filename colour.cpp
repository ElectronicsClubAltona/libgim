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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "colour.hpp"

#include "range.hpp"
#include "random.hpp"
#include "stream.hpp"

#include <map>

//-----------------------------------------------------------------------------
using util::colour;
using util::colour3f;
using util::colour4f;


//-----------------------------------------------------------------------------
#define CONSTANT_COLOUR(NAME,R,G,B)     \
template <size_t S, typename T>         \
const util::colour<S,T>                 \
util::colour<S,T>::NAME =               \
    util::colour<4,float> (R,G,B,1)     \
    .template redim<S> ()               \
    .template cast<T> ()

CONSTANT_COLOUR(WHITE,1,1,1);
CONSTANT_COLOUR(BLACK,0,0,0);
CONSTANT_COLOUR(RED,  1,0,0);
CONSTANT_COLOUR(GREEN,0,1,0);
CONSTANT_COLOUR(BLUE, 0,0,1);


//-----------------------------------------------------------------------------
static const std::map<std::string, colour<4,uint8_t>>
HTML_COLOURS { {
    { "white",      { 0xff, 0xff, 0xff, 0xff } },
    { "silver",     { 0xc0, 0xc0, 0xc0, 0xff } },
    { "gray",       { 0x80, 0x80, 0x80, 0xff } },
    { "black",      { 0x00, 0x00, 0x00, 0xff } },
    { "red",        { 0xff, 0x00, 0x00, 0xff } },
    { "maroon",     { 0x80, 0x00, 0x00, 0xff } },
    { "yellow",     { 0xff, 0xff, 0x00, 0xff } },
    { "olive",      { 0x80, 0x80, 0x00, 0xff } },
    { "lime",       { 0x00, 0xff, 0x00, 0xff } },
    { "green",      { 0x00, 0x80, 0x00, 0xff } },
    { "aqua",       { 0x00, 0xff, 0xff, 0xff } },
    { "teal",       { 0x00, 0x80, 0x80, 0xff } },
    { "blue",       { 0x00, 0x00, 0xff, 0xff } },
    { "navy",       { 0x00, 0x00, 0x80, 0xff } },
    { "fuchsia",    { 0xff, 0x00, 0xff, 0xff } },
    { "purple",     { 0x80, 0x00, 0x80, 0xff } },
} };


//-----------------------------------------------------------------------------
static const std::map<std::string, colour<4,uint8_t>> X11_COLOURS
{
    /* pink */
    { "pink",                   { 0xff, 0xc0, 0xcb, 0xff } },
    { "lightpink",              { 0xff, 0xb6, 0xc1, 0xff } },
    { "hotpink",                { 0xff, 0x69, 0xb4, 0xff } },
    { "deeppink",               { 0xff, 0x14, 0x93, 0xff } },
    { "palevioletred",          { 0xdb, 0x70, 0x93, 0xff } },
    { "mediumvioletred",        { 0xc7, 0x15, 0x85, 0xff } },

    /* red */
    { "lightsalmon",            { 0xff, 0xa0, 0x7a, 0xff } },
    { "salmon",                 { 0xfa, 0x80, 0x72, 0xff } },
    { "darksalmon",             { 0xe9, 0x96, 0x7a, 0xff } },
    { "lightcoral",             { 0xf0, 0x80, 0x80, 0xff } },
    { "indianred",              { 0xcd, 0x5c, 0x5c, 0xff } },
    { "crimson",                { 0xdc, 0x14, 0x3c, 0xff } },
    { "firebrick",              { 0xb2, 0x22, 0x22, 0xff } },
    { "darkred",                { 0x8b, 0x00, 0x00, 0xff } },
    { "red",                    { 0xff, 0x00, 0x00, 0xff } },

    /* orange */
    { "orangered",              { 0xff, 0x45, 0x00, 0xff } },
    { "tomato",                 { 0xff, 0x63, 0x47, 0xff } },
    { "coral",                  { 0xff, 0x7f, 0x50, 0xff } },
    { "darkorange",             { 0xff, 0x8c, 0x00, 0xff } },
    { "orange",                 { 0xff, 0xa5, 0x00, 0xff } },

    /* yellow */
    { "yellow",                 { 0xff, 0xff, 0x00, 0xff } },
    { "lightyellow",            { 0xff, 0xff, 0xe0, 0xff } },
    { "lemonchiffon",           { 0xff, 0xfa, 0xcd, 0xff } },
    { "lightgoldenrodyellow",   { 0xfa, 0xfa, 0xd2, 0xff } },
    { "papayawhip",             { 0xff, 0xef, 0xd5, 0xff } },
    { "moccasin",               { 0xff, 0xe4, 0xb5, 0xff } },
    { "peachpuff",              { 0xff, 0xda, 0xb9, 0xff } },
    { "palegoldenrod",          { 0xee, 0xe8, 0xaa, 0xff } },
    { "khaki",                  { 0xf0, 0xe6, 0x8c, 0xff } },
    { "darkkhaki",              { 0xbd, 0xb7, 0x6b, 0xff } },
    { "gold",                   { 0xff, 0xd7, 0x00, 0xff } },

    /* brown */
    { "cornsilk",               { 0xff, 0xf8, 0xdc, 0xff } },
    { "blanchedalmond",         { 0xff, 0xeb, 0xcd, 0xff } },
    { "bisque",                 { 0xff, 0xe4, 0xc4, 0xff } },
    { "navajowhite",            { 0xff, 0xde, 0xad, 0xff } },
    { "wheat",                  { 0xf5, 0xde, 0xb3, 0xff } },
    { "burlywood",              { 0xde, 0xb8, 0x87, 0xff } },
    { "tan",                    { 0xd2, 0xb4, 0x8c, 0xff } },
    { "rosybrown",              { 0xbc, 0x8f, 0x8f, 0xff } },
    { "sandybrown",             { 0xf4, 0xa4, 0x60, 0xff } },
    { "goldenrod",              { 0xda, 0xa5, 0x20, 0xff } },
    { "darkgoldenrod",          { 0xb8, 0x86, 0x0b, 0xff } },
    { "peru",                   { 0xcd, 0x85, 0x3f, 0xff } },
    { "chocolate",              { 0xd2, 0x69, 0x1e, 0xff } },
    { "saddlebrown",            { 0x8b, 0x45, 0x13, 0xff } },
    { "sienna",                 { 0xa0, 0x52, 0x2d, 0xff } },
    { "brown",                  { 0xa5, 0x2a, 0x2a, 0xff } },
    { "maroon",                 { 0x80, 0x00, 0x00, 0xff } },

    /* green */
    { "darkolivegreen",         { 0x55, 0x6b, 0x2f, 0xff } },
    { "olive",                  { 0x80, 0x80, 0x00, 0xff } },
    { "olivedrab",              { 0x6b, 0x8e, 0x23, 0xff } },
    { "yellowgreen",            { 0x9a, 0xcd, 0x32, 0xff } },
    { "limegreen",              { 0x32, 0xcd, 0x32, 0xff } },
    { "lime",                   { 0x00, 0xff, 0x00, 0xff } },
    { "lawngreen",              { 0x7c, 0xfc, 0x00, 0xff } },
    { "chartreuse",             { 0x7f, 0xff, 0x00, 0xff } },
    { "greenyellow",            { 0xad, 0xff, 0x2f, 0xff } },
    { "springgreen",            { 0x00, 0xff, 0x7f, 0xff } },
    { "mediumspringgreen",      { 0x00, 0xfa, 0x9a, 0xff } },
    { "lightgreen",             { 0x90, 0xee, 0x90, 0xff } },
    { "palegreen",              { 0x98, 0xfb, 0x98, 0xff } },
    { "darkseagreen",           { 0x8f, 0xbc, 0x8f, 0xff } },
    { "mediumseagreen",         { 0x3c, 0xb3, 0x71, 0xff } },
    { "seagreen",               { 0x2e, 0x8b, 0x57, 0xff } },
    { "forestgreen",            { 0x22, 0x8b, 0x22, 0xff } },
    { "green",                  { 0x00, 0x80, 0x00, 0xff } },
    { "darkgreen",              { 0x00, 0x64, 0x00, 0xff } },

    /* cyan */
    { "mediumaquamarine",       { 0x66, 0xcd, 0xaa, 0xff } },
    { "aqua",                   { 0x00, 0xff, 0xff, 0xff } },
    { "cyan",                   { 0x00, 0xff, 0xff, 0xff } },
    { "lightcyan",              { 0xe0, 0xff, 0xff, 0xff } },
    { "paleturquoise",          { 0xaf, 0xee, 0xee, 0xff } },
    { "aquamarine",             { 0x7f, 0xff, 0xd4, 0xff } },
    { "turquoise",              { 0x40, 0xe0, 0xd0, 0xff } },
    { "mediumturquoise",        { 0x48, 0xd1, 0xcc, 0xff } },
    { "darkturquoise",          { 0x00, 0xce, 0xd1, 0xff } },
    { "lightseagreen",          { 0x20, 0xb2, 0xaa, 0xff } },
    { "cadetblue",              { 0x5f, 0x9e, 0xa0, 0xff } },
    { "darkcyan",               { 0x00, 0x8b, 0x8b, 0xff } },
    { "teal",                   { 0x00, 0x80, 0x80, 0xff } },

    /* blue */
    { "lightsteelblue",         { 0xb0, 0xc4, 0xde, 0xff } },
    { "powderblue",             { 0xb0, 0xe0, 0xe6, 0xff } },
    { "lightblue",              { 0xad, 0xd8, 0xe6, 0xff } },
    { "skyblue",                { 0x87, 0xce, 0xeb, 0xff } },
    { "lightskyblue",           { 0x87, 0xce, 0xfa, 0xff } },
    { "deepskyblue",            { 0x00, 0xbf, 0xff, 0xff } },
    { "dodgerblue",             { 0x1e, 0x90, 0xff, 0xff } },
    { "cornflowerblue",         { 0x64, 0x95, 0xed, 0xff } },
    { "steelblue",              { 0x46, 0x82, 0xb4, 0xff } },
    { "royalblue",              { 0x41, 0x69, 0xe1, 0xff } },
    { "blue",                   { 0x00, 0x00, 0xff, 0xff } },
    { "mediumblue",             { 0x00, 0x00, 0xcd, 0xff } },
    { "darkblue",               { 0x00, 0x00, 0x8b, 0xff } },
    { "navy",                   { 0x00, 0x00, 0x80, 0xff } },
    { "midnightblue",           { 0x19, 0x19, 0x70, 0xff } },

    /* purple */
    { "lavender",               { 0xe6, 0xe6, 0xfa, 0xff } },
    { "thistle",                { 0xd8, 0xbf, 0xd8, 0xff } },
    { "plum",                   { 0xdd, 0xa0, 0xdd, 0xff } },
    { "violet",                 { 0xee, 0x82, 0xee, 0xff } },
    { "orchid",                 { 0xda, 0x70, 0xd6, 0xff } },
    { "fuchsia",                { 0xff, 0x00, 0xff, 0xff } },
    { "magenta",                { 0xff, 0x00, 0xff, 0xff } },
    { "mediumorchid",           { 0xba, 0x55, 0xd3, 0xff } },
    { "mediumpurple",           { 0x93, 0x70, 0xdb, 0xff } },
    { "blueviolet",             { 0x8a, 0x2b, 0xe2, 0xff } },
    { "darkviolet",             { 0x94, 0x00, 0xd3, 0xff } },
    { "darkorchid",             { 0x99, 0x32, 0xcc, 0xff } },
    { "darkmagenta",            { 0x8b, 0x00, 0x8b, 0xff } },
    { "purple",                 { 0x80, 0x00, 0x80, 0xff } },
    { "indigo",                 { 0x4b, 0x00, 0x82, 0xff } },
    { "darkslateblue",          { 0x48, 0x3d, 0x8b, 0xff } },
    { "rebeccapurple",          { 0x66, 0x33, 0x99, 0xff } },
    { "slateblue",              { 0x6a, 0x5a, 0xcd, 0xff } },
    { "mediumslateblue",        { 0x7b, 0x68, 0xee, 0xff } },

    /* white */
    { "white",                  { 0xff, 0xff, 0xff, 0xff } },
    { "snow",                   { 0xff, 0xfa, 0xfa, 0xff } },
    { "honeydew",               { 0xf0, 0xff, 0xf0, 0xff } },
    { "mintcream",              { 0xf5, 0xff, 0xfa, 0xff } },
    { "azure",                  { 0xf0, 0xff, 0xff, 0xff } },
    { "aliceblue",              { 0xf0, 0xf8, 0xff, 0xff } },
    { "ghostwhite",             { 0xf8, 0xf8, 0xff, 0xff } },
    { "whitesmoke",             { 0xf5, 0xf5, 0xf5, 0xff } },
    { "seashell",               { 0xff, 0xf5, 0xee, 0xff } },
    { "beige",                  { 0xf5, 0xf5, 0xdc, 0xff } },
    { "oldlace",                { 0xfd, 0xf5, 0xe6, 0xff } },
    { "floralwhite",            { 0xff, 0xfa, 0xf0, 0xff } },
    { "ivory",                  { 0xff, 0xff, 0xf0, 0xff } },
    { "antiquewhite",           { 0xfa, 0xeb, 0xd7, 0xff } },
    { "linen",                  { 0xfa, 0xf0, 0xe6, 0xff } },
    { "lavenderblush",          { 0xff, 0xf0, 0xf5, 0xff } },
    { "mistyrose",              { 0xff, 0xe4, 0xe1, 0xff } },

    /* grey/black */
    { "gainsboro",              { 0xdc, 0xdc, 0xdc, 0xff } },
    { "lightgrey",              { 0xd3, 0xd3, 0xd3, 0xff } },
    { "silver",                 { 0xc0, 0xc0, 0xc0, 0xff } },
    { "darkgray",               { 0xa9, 0xa9, 0xa9, 0xff } },
    { "gray",                   { 0x80, 0x80, 0x80, 0xff } },
    { "dimgray",                { 0x69, 0x69, 0x69, 0xff } },
    { "lightslategray",         { 0x77, 0x88, 0x99, 0xff } },
    { "slategray",              { 0x70, 0x80, 0x90, 0xff } },
    { "darkslategray",          { 0x2f, 0x4f, 0x4f, 0xff } },
    { "black",                  { 0x00, 0x00, 0x00, 0xff } },
};


//-----------------------------------------------------------------------------
template <size_t S, typename T>
static colour<S,T>
lookup_colour (const std::string &name,
               const std::map<std::string,colour<4,uint8_t>> &map)
{
    std::string lower (name);
    std::transform (lower.begin (), lower.end (), lower.begin (), tolower);

    auto pos = map.find (lower);
    if (pos == map.end ())
        throw std::out_of_range (name);

    static_assert (S <= 4, "cannot invent additional data");
    return pos->second.template redim<S> ().template cast<T> ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
colour<S,T>
colour<S,T>::from_html (const std::string &name)
{
    return lookup_colour<S,T> (name, HTML_COLOURS);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
colour<S,T>
colour<S,T>::from_x11 (const std::string &name)
{
    return lookup_colour<S,T> (name, X11_COLOURS);
}


///////////////////////////////////////////////////////////////////////////////
colour3f
util::rgb_to_hsv (colour3f rgb)
{
    // Calculate chroma
    auto M = max (rgb);
    auto m = min (rgb);
    auto C = M - m;

    // Undefined for zero chroma
    if (almost_zero (C))
        return { -1.f, 0.f, M };

    // Calculate hue
    float H = exactly_equal (rgb.r, M) ?     (rgb.g - rgb.b) :
              exactly_equal (rgb.g, M) ? 2 + (rgb.b - rgb.r) :
              exactly_equal (rgb.b, M) ? 4 + (rgb.r - rgb.g) :
                                                           0 ;

    H /=  C;
    H *= 60;

    if (H < 0)
        H += 360;

    // Calculate value
    auto V = M;

    // Calculate saturation
    auto S = almost_zero (V) ? 0.f : C / V;

    return { H, S, V };
}


//-----------------------------------------------------------------------------
colour3f
util::hsv_to_rgb (colour3f hsv)
{
    CHECK_GE (hsv.h,   0);
    CHECK_LT (hsv.h, 360);
    CHECK_GE (hsv.s,   0);
    CHECK_LE (hsv.s,   1);
    CHECK_GE (hsv.v,   0);
    CHECK_LE (hsv.v,   1);

    float C = hsv.v * hsv.s;
    float H = hsv.h / 60;
    float X = C * (1 - std::abs (std::fmod (H, 2.f) - 1));

    // monochromatic'ish
    if (almost_zero (hsv.s))
        return colour3f { hsv.v };

    colour3f rgb;

    unsigned hex = (unsigned)H;
    switch (hex) {
    case 0: rgb = { C, X, 0 }; break;
    case 1: rgb = { X, C, 0 }; break;
    case 2: rgb = { 0, C, X }; break;
    case 3: rgb = { 0, X, C }; break;
    case 4: rgb = { X, 0, C }; break;
    case 5: rgb = { C, 0, X }; break;
    }

    auto m = hsv.v - C;

    return rgb + m;
}


///----------------------------------------------------------------------------
/// Extract a colour object from a JSON node.
#include "json/tree.hpp"

namespace json { namespace tree {
    template <>
    util::colour4f
    io<util::colour4f>::deserialise (const node &root) {
        return {
            root[0].as<float> (),
            root[1].as<float> (),
            root[2].as<float> (),
            root[3].as<float> (),
        };
    }
} }


//-----------------------------------------------------------------------------
namespace util {
    template<>
    colour4f
    random (void) {
        return colour4f ({ range<float>::UNIT.random (),
                           range<float>::UNIT.random (),
                           range<float>::UNIT.random (),
                           range<float>::UNIT.random () });
    }

    template <>
    colour4f&
    randomise (colour4f &c)
        { return c = random<colour4f> (); }
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, util::colour<S,T> c) {
    os << "colour(";
    std::transform (std::cbegin (c),
                    std::cend   (c),
                    infix_iterator<stream::numeric<T>> (os, ", "),
                    stream::to_numeric<T>);
    os << ")";

    return os;
}

//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)        \
template struct util::colour<S,T>;  \
template std::ostream& util::operator<< (std::ostream&, util::colour<S,T>);

#define INSTANTIATE_S(S)    \
INSTANTIATE_S_T(S,uint8_t)  \
INSTANTIATE_S_T(S,float)    \
INSTANTIATE_S_T(S,double)

INSTANTIATE_S(3)
INSTANTIATE_S(4)
