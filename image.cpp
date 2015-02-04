/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */


#include "image.hpp"

#include "debug.hpp"
#include "except.hpp"

#include <fstream>


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>::buffer (size_t _w, size_t _h, size_t _s):
    w (_w),
    h (_h),
    s (_s),
    data (std::make_unique<T[]> (_w * _s))
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
    data (std::move (_data))
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
    std::fill (data.get (), data.get () + w * s, v);
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

    std::transform (data.get (),
                    data.get () + w * s,
                    out.data.get (),
                    [] (auto v) {
        return rescale<T,U> (v);
    });

    return out;
}


//-----------------------------------------------------------------------------
template <typename T>
util::image::buffer<T>
util::image::buffer<T>::downsample (unsigned factor)
{
    CHECK_GT (factor, 1);

    size_t w_ = w / factor;
    size_t h_ = h / factor;
    size_t s_ = w_;

    CHECK_NEQ (w_, 0);
    CHECK_NEQ (h_, 0);

    buffer out { w_, h_, s_, std::make_unique<T[]> (w_ * h_) };

    // Do a really shitty nearest neighbour average in src-space.
    for (size_t y = 0; y < out.h; ++y)
        for (size_t x = 0; x < out.w; ++x) {
            size_t src00 = ((y * factor) + 0) * s + (x * factor) + 0;
            size_t src01 = ((y * factor) + 0) * s + (x * factor) + 1;
            size_t src10 = ((y * factor) + 1) * s + (x * factor) + 0;
            size_t src11 = ((y * factor) + 1) * s + (x * factor) + 1;

            size_t dst_offset = y * out.s + x;
            out.data[dst_offset] = (src00 + src01 + src10 + src11) / 4;
        }

    return out;
}


//-----------------------------------------------------------------------------
static void
write_netpbm (const uint8_t *restrict pixels,
              size_t width,
              size_t height,
              size_t stride,
              const boost::filesystem::path &path,
              const char* MAGIC) {
    CHECK (pixels);
    CHECK_GT (width, 0);
    CHECK_GE (stride, width);
    CHECK_GT (height, 0);

    // Establish an output stream
    std::ofstream output (path.string ());
    if (!output.good ())
        throw util::output_error ("Unable to open output file");

    // Write the PPM header.
    output << MAGIC  << "\n"
           << width  << "\n"
           << height << "\n"
           << (size_t)std::numeric_limits<uint8_t>::max () << "\n";

    // Write the data rows
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x)
            output << pixels[y * stride + x];
    }
}


//-----------------------------------------------------------------------------
void
util::pgm::write (const uint8_t *restrict pixels,
                  size_t width,
                  size_t height,
                  size_t stride,
                  const boost::filesystem::path &path) {
    // TODO: We should switch between P2 (ascii) and P5 (binary)
    static const char MAGIC[] = "P5";
    write_netpbm (pixels, width, height, stride, path, MAGIC);
}


//-----------------------------------------------------------------------------
void
util::ppm::write (const uint8_t *restrict pixels,
                  size_t width,
                  size_t height,
                  size_t stride,
                  const boost::filesystem::path &path) {
    // TODO: We should switch between P3 (ascii) and P6 (binary)
    static const char MAGIC[] = "P6";
    write_netpbm (pixels, width, height, stride, path, MAGIC);
}


//-----------------------------------------------------------------------------
template struct util::image::buffer<uint8_t>;
template util::image::buffer<uint8_t> util::image::buffer<uint8_t>::alloc (void) const;
