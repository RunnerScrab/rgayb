// Copyright (c) 2020, kmeow
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the rgayb nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL KMEOW BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "colorwheel.h"

#ifndef max
#define max(a, b) (a > b ? a : b)
#endif

#define min3(a, b, c) (a < b ? ((a < c) ? a : c) : ((b < c) ? b : c))

#ifdef USESSE2
static inline __m128 _mm_min3_ps(__m128 a, __m128 b, __m128 c)
{
	return _mm_min_ps(_mm_min_ps(a, b), c);
}

static inline __m128 _mm_mod_ps(__m128 a, __m128 b)
{
	//Vector modulus division
	__m128 ftimes = _mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_div_ps(a, b)));
	return _mm_sub_ps(a, _mm_mul_ps(ftimes, b));
}
#else

static inline float hsvf(float n, float h)
{
	float k = fmodf((n + h), 6.f);
	return 255.f * (1.f - max(min3(k, 4.f - k, 1.f), 0.f));
}

#endif

void hsv_to_rgb(union rgb* v, int max_steps, int step)
{
#ifdef USESSE2
	//On average ~3.4 times faster than the ordinary scalar version
	//Performs calculations for r, g, and b channels simultaneously
	float angle = 360.f * ((float)step / (float)max_steps);
	float h = ((float)angle) / 60.f;

	__m128 vh = _mm_set_ps1(h);

	//The vector variables below (non const, non static) are
	//actually put in .data by gcc, even with no optimization
	//flags.  Not what I would expect, but w/e
	__m128 vn = _mm_set_ps(5.f, 3.f, 1.f, 0.f);
	__m128 vone = _mm_set_ps1(1.f);
	__m128 vzero = _mm_set_ps1(0.f);
	__m128 vfour = _mm_set_ps1(4.f);
	__m128 vff = _mm_set_ps1(255.f);

	__m128 vk = _mm_mod_ps(_mm_add_ps(vh, vn), _mm_set_ps1(6.f));

	__m128 scale = _mm_max_ps(vzero,
				_mm_min3_ps(vk, _mm_sub_ps(vfour, vk), vone));
	float fresults[4];

	_mm_store_ps(fresults, _mm_mul_ps(vff, _mm_sub_ps(vone, scale)));

	v->components.r = (unsigned char) fresults[3];
	v->components.g = (unsigned char) fresults[2];
	v->components.b = (unsigned char) fresults[1];
#else
	float angle = 360.f * ((float)step/(float)max_steps);
	float h = ((float)angle) / 60.f;
	v->components.r = (unsigned char) hsvf(5.f, h);
	v->components.g = (unsigned char) hsvf(3.f, h);
	v->components.b = (unsigned char) hsvf(1.f, h);
#endif
}
