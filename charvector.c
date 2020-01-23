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

#include "charvector.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

void cv_sprintf(cv_t* pcv, const char* fmt, ...)
{
	va_list arglist, cpyarglist;
	va_copy(cpyarglist, arglist);
	va_start(arglist, fmt);

	size_t bwritten = vsnprintf(pcv->data, pcv->capacity, fmt, arglist);
	va_end(arglist);
	va_start(cpyarglist, fmt);
	if(bwritten >= pcv->capacity)
	{
		//Our buffer wasn't large enough. Resize it!
		pcv->capacity = bwritten + 1;
		pcv->length = pcv->capacity;
		pcv->data = (el_t*) realloc(pcv->data,
					sizeof(el_t) * pcv->capacity);
		memset(pcv->data, 0, sizeof(el_t) * pcv->capacity);
		vsnprintf(pcv->data, pcv->capacity, fmt, cpyarglist);
	}
	va_end(cpyarglist);
}

int cv_append(cv_t* cv, el_t* data, size_t len)
{
	if((cv->length + len) > cv->capacity)
	{
		cv->capacity = max((cv->capacity << 1),(cv->capacity + len));
		cv->data = (el_t*) realloc(cv->data,
					sizeof(el_t) * cv->capacity);
		if(!cv->data)
		{
			return -1;
		}
	}
	memcpy(&(cv->data[cv->length]), data, len * sizeof(el_t));
	cv->length = cv->length + len;
	return cv->length;
}

void cv_clear(cv_t* cv)
{
	memset(cv->data, 0, sizeof(el_t) * cv->capacity);
	cv->length = 0;
}

int cv_appendcv(cv_t* dest, cv_t* src)
{
	return cv_append(dest, src->data, src->length);
}

int cv_appendstr(cv_t* cv, el_t* data)
{
	return cv_append(cv, data, strlen(data));
}


int cv_init(cv_t* cv, size_t startsize)
{
	if(startsize > 0)
	{
		cv->length = 0;
		cv->capacity = startsize << 1;
		cv->data = (el_t*) malloc(sizeof(el_t) * cv->capacity);
		memset(cv->data, 0, sizeof(el_t) * cv->capacity);
	}
	return (startsize > 0 && cv->data) ? 0 : -1;
}

int cv_resize(cv_t* cv, size_t newcap)
{
	if(!cv->data || newcap != cv->capacity)
	{
		cv->data = (el_t*) realloc(cv->data, sizeof(el_t) * newcap);
		cv->capacity = newcap;
	}
	return cv->data ? 0 : -1;
}

void cv_swap(cv_t* a, cv_t* b)
{
	cv_t temp;
	temp.data = a->data;
	temp.length = a->length;
	temp.capacity = a->capacity;

	a->data = b->data;
	a->length = b->length;
	a->capacity= b->capacity;

	b->data = temp.data;
	b->length = temp.length;
	b->capacity = temp.capacity;
}

void cv_copy(cv_t* dest, cv_t* source)
{
	dest->length = source->length;
	dest->capacity = source->capacity;
	if(dest->data)
		free(dest->data);
	dest->data = (el_t*) malloc(sizeof(el_t) * dest->capacity);
	memcpy(dest->data, source->data, dest->capacity);
}

void cv_destroy(cv_t* cv)
{
	cv->length = 0;
	cv->capacity = 0;
	free(cv->data);
	cv->data = 0;
}

int cv_push(cv_t* cv, el_t newel)
{
	if(cv->length >= cv->capacity)
	{
		cv->capacity = cv->capacity << 1;
		cv->data = (el_t*) realloc(cv->data, sizeof(el_t) * cv->capacity);
	}

	if(cv->data)
	{
		cv->data[cv->length] = newel;
		++cv->length;
		return 0;
	}
	else
	{
		return -1;
	}
}

inline static void swap_elements(el_t* a, el_t* b)
{
	el_t t = *b;
	*b = *a;
	*a = t;
}


el_t cv_at(cv_t* cv, size_t idx)
{
	return cv->data[idx];
}

size_t cv_len(cv_t* cv)
{
	return cv->length;
}
