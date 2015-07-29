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

#ifdef __UTIL_MATHS_FAST_IPP
#error
#endif
#define __UTIL_MATHS_FAST_IPP
    

namespace util { namespace maths { namespace fast {
    ///////////////////////////////////////////////////////////////////////////
    constexpr float
    pow2 (float p)
    {
        float offset = (p < 0) ? 1.0f : 0.0f;
        float clipp = (p < -126) ? -126.0f : p;
        int32_t w = static_cast<int32_t> (clipp);
        float z = clipp - w + offset;
        union { uint32_t i; float f; } v = { static_cast<uint32_t> ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

        return v.f;

        //float clipp = (p < -126) ? -126.0f : p;
        //union { uint32_t i; float f; } v = { static_cast<uint32_t> ( (1 << 23) * (clipp + 126.94269504f) ) };
        //return v.f;
    }


    constexpr float
    log2 (float x)
    {
        union { float f; uint32_t i; } vx = { x };
        union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
        float y = vx.i;
        y *= 1.1920928955078125e-7f;

        return y - 124.22551499f
            - 1.498030302f * mx.f 
            - 1.72587999f / (0.3520887068f + mx.f);


        //union { float f; uint32_t i; } vx = { x };
        //float y = vx.i;
        //y *= 1.1920928955078125e-7f;
        //return y - 126.94269504f;
    }


    constexpr float
    log (float x)
    {
        return 0.69314718f * log2 (x);
    }


    constexpr float
    pow (float a, float b)
    {
        return pow2 (b * log2 (a));
    }


    constexpr float
    sqrt (float x)
    {
        //return pow (x, 0.5f);
        union { float f; int32_t i; } u  = { x };

        int32_t v = u.i;
        v  -= 1 << 23;
        v >>= 1;
        v  += 1 << 29;

        u.i = v;
        return u.f;
    }


    constexpr float
    invsqrt (float x)
    {
        union
        {
            float x;
            int32_t i;
        } u = { x };

        u.i = 0x5f3759df - (u.i >> 1);

        // refine estimate. repeat as necessary.
        u.x = u.x * (1.5f - x * 0.5f * u.x * u.x);
        return u.x;
    }
} } }
