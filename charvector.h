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

#ifndef CHARVECTOR_H_
#define CHARVECTOR_H_
#include <stdlib.h>
#include <stdarg.h>

typedef char el_t;

typedef struct
{
	el_t* data;
	size_t length, capacity;
} cv_t; // Short for "Char Vector Type"

//cv_sprintf notwithstanding, cv_t has C++ std::vector<char>-like semantics and should be
//used that way

void cv_swap(cv_t* a, cv_t* b);
void cv_copy(cv_t* dest, cv_t* source);
int cv_init(cv_t* cv, size_t startsize);
void cv_destroy(cv_t* cv);
int cv_push(cv_t* cv, el_t newel);
int cv_resize(cv_t* cv, size_t newsize);
int cv_appendstr(cv_t* cv, el_t* data);
int cv_appendcv(cv_t* dest, cv_t* src);
int cv_append(cv_t* cv, el_t* data, size_t len);
void cv_clear(cv_t* cv);

el_t cv_at(cv_t* cv, size_t idx);
size_t cv_len(cv_t* cv);

void cv_sprintf(cv_t* pcv, const char* fmt, ...);

#endif
