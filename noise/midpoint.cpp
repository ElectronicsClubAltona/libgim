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

#include "midpoint.hpp"

#include "region.hpp"
#include "point.hpp"
#include "rand.hpp"


///////////////////////////////////////////////////////////////////////////////
// assumes corner points have been assigned their weights
template <typename T>
static void
fill (util::image::buffer<T> &img,
      uint64_t seed,
      util::region2u target,
      float scale,
      float persistence,
      float sides)
{
    CHECK_EQ (target.e.w % 2, 1);
    CHECK_EQ (target.e.h % 2, 1);
    CHECK_GE (target.area (), 9);

    CHECK_GT (scale, 0);
    CHECK_GT (persistence, 0);
    CHECK_GE (sides, 0);

    const auto w = target.w;
    const auto h = target.h;

    // 0--1
    // |  |
    // 2--3

    const auto p0 = target.p + util::vector2u {0,   0  };
    const auto p1 = target.p + util::vector2u {w-1, 0  };
    const auto p2 = target.p + util::vector2u {0,   h-1};
    const auto p3 = target.p + util::vector2u {w-1, h-1};

    const auto v0 = img[p0];
    const auto v1 = img[p1];
    const auto v2 = img[p2];
    const auto v3 = img[p3];

    // do the centre
    {
        const auto avg = (v0 + v1 + v2 + v3) / 4;
        const auto val = avg + scale * util::noise::rand::scalar<T> (seed, target.centre ());
        const auto pos = target.p + target.e / 2;

        img[pos] = val * 2 - 1;
    }

    // average the sides
    {
        const auto p01 = target.p + util::vector2u{w/2,     0};
        const auto p13 = target.p + util::vector2u{w-1,   h/2};
        const auto p32 = target.p + util::vector2u{w/2,   h-1};
        const auto p20 = target.p + util::vector2u{0,     h/2};

        const auto v01 = (v0 + v1) / 2 + sides * scale * util::noise::rand::scalar<T> (seed, p01);
        const auto v13 = (v1 + v3) / 2 + sides * scale * util::noise::rand::scalar<T> (seed, p13);
        const auto v32 = (v3 + v2) / 2 + sides * scale * util::noise::rand::scalar<T> (seed, p32);
        const auto v20 = (v2 + v0) / 2 + sides * scale * util::noise::rand::scalar<T> (seed, p20);

        img[p01] = v01 * 2 - 1;
        img[p13] = v13 * 2 - 1;
        img[p32] = v32 * 2 - 1;
        img[p20] = v20 * 2 - 1;
    }

    // recurse
    if (target.area () > 9) {
        auto e = target.e / 2 + 1;

        fill (img, seed, {target.p + util::vector2u{ 0,     0 }, e}, scale * persistence, persistence, sides);
        fill (img, seed, {target.p + util::vector2u{ w/2,   0 }, e}, scale * persistence, persistence, sides);
        fill (img, seed, {target.p + util::vector2u{ 0,   h/2 }, e}, scale * persistence, persistence, sides);
        fill (img, seed, {target.p + util::vector2u{ w/2, h/2 }, e}, scale * persistence, persistence, sides);
    }
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
void
util::noise::midpoint (image::buffer<T> &img, uint64_t seed, float persistence, float sides)
{
    static const util::point2u CORNERS[] = {
        { 0,         0 },
        { 0,         img.w - 1 },
        { img.h - 1, 0 },
        { img.h - 1, img.w - 1 }
    };

    for (auto i: CORNERS)
        img[i] = util::noise::rand::scalar<T> (seed, i) * 2 - 1;

    fill (img, seed, { { 0, 0 }, img.extent () }, 1.f, persistence, sides);
}


//-----------------------------------------------------------------------------
template void util::noise::midpoint (image::buffer<float>&, uint64_t, float, float);
