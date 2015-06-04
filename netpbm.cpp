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

#include "netpbm.hpp"

#include "io.hpp"
#include "except.hpp"
#include <fstream>


///////////////////////////////////////////////////////////////////////////////
// HACK: This does not support the full header structure with any robustness.
// In particular it will abort when it sees a comment. If you want better
// support use waif, or port its implementation.
util::image::buffer<uint8_t>
util::pgm::read (const boost::filesystem::path &path)
{
    util::mapped_file raw (path);

    std::ifstream cooked (path.string (), std::ios::binary);
    char magic[2];
    size_t width, height, scale;
    char space;

    cooked >> magic[0] >> magic[1] >> width >> height >> scale >> space;

    if (magic[0] != 'P' && magic[1] != '5')
        throw std::runtime_error ("invalid header magic");

    if (width == 0 || height == 0 || scale == 0)
        throw std::runtime_error ("zero width, height, or scale");

    size_t expected = width * height;
    size_t remain   = raw.size () - cooked.tellg ();
    if (expected != remain)
        throw std::runtime_error ("expected data size mismatch");

    util::image::buffer<uint8_t> out (width, height);

    CHECK_EQ (out.w, out.s);
    std::copy (raw.begin () + cooked.tellg () - 1, raw.end (), out.begin ());

    return out;
}

//-----------------------------------------------------------------------------
static void
write_netpbm (const uint8_t *restrict pixels,
              size_t components,
              size_t width,
              size_t height,
              size_t stride,
              const boost::filesystem::path &path,
              const char* MAGIC) {
    CHECK (pixels);
    CHECK_GT (components, 0);
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
            for (size_t c = 0; c < components; ++c)
                output << pixels[y * stride + x * components + c];
    }
}


//-----------------------------------------------------------------------------
void
util::pgm::write (const util::image::buffer<uint8_t> &src,
                  const boost::filesystem::path &path)
{
    write (src.begin (), src.w, src.h, src.s, path);
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
    write_netpbm (pixels, 1, width, height, stride, path, MAGIC);
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
    write_netpbm (pixels, 3, width, height, stride, path, MAGIC);
}