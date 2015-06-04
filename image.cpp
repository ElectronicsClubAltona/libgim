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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "image.hpp"

#include "debug.hpp"

using util::image::buffer;


//-----------------------------------------------------------------------------
struct box {
    static constexpr float support = 0.5f;

    static constexpr float weight [[gnu::pure]] (float x)
    {
        return (x >= -0.5f && x <= 0.5f) ? 1.f : 0.f;
    }
};


//-----------------------------------------------------------------------------
// XXX: Known to cause hard black ringing
template <size_t S>
struct lanczos {
    static constexpr float support = float {S};

    static float weight [[gnu::pure]] (float x)
    {
        if (x < 0.f)
            x = -x;

        if (x <= S)
            return sincn (x) * sincn (x / S);

        return 0.f;
    }
};

template struct lanczos< 3>;
template struct lanczos< 4>;
template struct lanczos< 6>;
template struct lanczos<12>;


//-----------------------------------------------------------------------------
struct bspline {
    static constexpr float support = 2.f;

    static float weight [[gnu::pure]] (float x)
    {
        if (x < 0.f)
            x = -x;

        if (x < 1.f)
            return .5f * x * x * x - x * x + (2.f / 3.f);

        if (x < 2.f) {
            x = 2.f - x;
            return (1.f / 6.f) * x * x * x;
        }

        return 0.f;
    }
};


//-----------------------------------------------------------------------------
// XXX: Known to cause hard black ringing
struct mitchell {
    static constexpr float support = 2.f;

    static float weight [[gnu::pure]] (float x)
    {
        return eval (x, 1.f / 3.f, 1.f / 3.f);
    }

private:
    static float eval (float x, float B, float C)
    {
        if (x < 0.f)
            x = -x;

        if (x < 1.f) {
            x =   ( 12.f -  9.f * B - 6.f * C) * x * x * x
                + (-18.f + 12.f * B + 6.f * C) * x * x
                +    6.f -  2.f * B;

            return x / 6.f;
        }

        if (x < 2.f) {
            x =   ( -1.f * B -  6.f * C) * x * x * x
                + (  6.f * B + 30.f * C) * x * x
                + (-12.f * B - 48.f * C) * x
                +    8.f * B + 24.f * C;

            return x / 6.f;
        }

        return 0.f;
    }
};


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>::buffer (util::extent2u _size):
    buffer<T> (_size.w, _size.h)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>::buffer (size_t _w, size_t _h, size_t _s):
    w (_w),
    h (_h),
    s (_s),
    m_data (std::make_unique<T[]> (_w * _s))
{
    CHECK_NEQ (w * h, 0);
    CHECK_GE (s, w);
}


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>::buffer (size_t _w, size_t _h):
    buffer<T> (_w, _h, _w)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>::buffer (size_t _w,
                                size_t _h,
                                size_t _s,
                                std::unique_ptr<T[]> &&_data):
    w (_w),
    h (_h),
    s (_s),
    m_data (std::move (_data))
{
    CHECK_NEQ (w * h, 0);
    CHECK_GE (s, w);
}


//-----------------------------------------------------------------------------
template <typename T>
template <typename U>
util::image::buffer<U>
util::image::buffer<T>::alloc (void) const
{
    return buffer<U> (w, h, s);
}


//-----------------------------------------------------------------------------
template <typename T>
void
util::image::buffer<T>::fill (const T v)
{
    std::fill (begin (), end (), v);
}


//-----------------------------------------------------------------------------
template <typename T, typename U>
static U
rescale (T v)
{
    return v * sizeof (U) / sizeof (T);
}


//-----------------------------------------------------------------------------
template <typename T>
template <typename U>
util::image::buffer<U>
util::image::buffer<T>::clone (void) const
{
    auto out = alloc<U> ();

    std::transform (begin (), end (), out.begin (), renormalise<T,U>);

    return out;
}


//-----------------------------------------------------------------------------
// HACK: This code hasn't really been tested with multiple filters (it is
// known to produce dark ringing artefacts with sinc) or robustly with
// fractional factors. But... it works more or less.
template <typename T>
util::image::buffer<T>
util::image::buffer<T>::downsample (float factor) const
{
    CHECK_GE (factor, 0);

    using filter = bspline;

    const buffer &src = *this;
    buffer dst (static_cast<size_t> (w / factor),
                static_cast<size_t> (h / factor));

    const float scale = float (dst.w) / src.w;
    const float half_width = filter::support / scale;
    const int half_pixels = int (std::ceil (half_width));

    // Calculate the weighted sum of the src pixels for each dst pixel
    for (size_t d_y = 0; d_y < dst.h; ++d_y)
        for (size_t d_x = 0; d_x < dst.w; ++d_x) {
            // Initialise value and weight sums
            float v = 0.f;
            float m = 0.f;

            // Find the centre of the src pixel
            float o_y = (d_y + 0.5f) / scale;
            float o_x = (d_x + 0.5f) / scale;

            // Do a full summation across the window. This isn't using
            // seperable filtering because:
            //    a) we need high precision with fractional factors
            //    b) it's much easier to implement as is
            //
            // TODO: seperable filters
            for (int s_y = -half_pixels; s_y <= half_pixels; ++s_y)
                for (int s_x = -half_pixels; s_x <= half_pixels; ++s_x) {
                    float m_x = filter::weight (s_x * scale);
                    float m_y = filter::weight (s_y * scale);
                    float m_  = m_x * m_y;

                    // Simple clamp to border for edges
                    int x = int (limit (o_x + s_x - 0.5f, 0, src.w));
                    int y = int (limit (o_y + s_y - 0.5f, 0, src.h));

                    // Collection the contribution
                    v += m_ * src.m_data[y * src.s + x];
                    m += m_;
                }

            CHECK_NEZ (m);

            dst.m_data[d_y * dst.s + d_x] = uint8_t (v / m);
        }

    return dst;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
buffer<T>::operator[] (point<2,size_t> p) const
{
    return begin ()[p.y * s + p.x];
}


//-----------------------------------------------------------------------------
template <typename T>
T&
buffer<T>::operator[] (point<2,size_t> p)
{
    return begin ()[p.y * s + p.x];
}


//-----------------------------------------------------------------------------
template <typename T>
T
buffer<T>::operator[] (size_t idx) const
{
    return begin ()[idx];
}


//-----------------------------------------------------------------------------
template <typename T>
T&
buffer<T>::operator[] (size_t idx)
{
    return begin ()[idx];
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T*
buffer<T>::data (void)
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <typename T>
T*
buffer<T>::begin (void)
{
    return m_data.get ();
}


//-----------------------------------------------------------------------------
template <typename T>
T*
buffer<T>::end (void)
{
    return begin () + h * s;
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
buffer<T>::data (void) const
{
    return begin ();
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
buffer<T>::begin (void) const
{
    return m_data.get ();
}


//-----------------------------------------------------------------------------
template <typename T>
const T*
buffer<T>::end (void) const
{
    return begin () + h * s;
}


//-----------------------------------------------------------------------------
template struct util::image::buffer<uint8_t>;
template struct util::image::buffer<float>;
template struct util::image::buffer<double>;

template util::image::buffer<uint8_t> util::image::buffer<uint8_t>::alloc (void) const;
template util::image::buffer<uint8_t> util::image::buffer<uint8_t>::clone (void) const;
template util::image::buffer<uint8_t> util::image::buffer<float>::clone (void) const;
template util::image::buffer<uint8_t> util::image::buffer<double>::clone (void) const;

template util::image::buffer<float> util::image::buffer<float>::clone (void) const;
