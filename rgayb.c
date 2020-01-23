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
#include "charvector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "gtext.h"


int main(int argc, char** argv)
{
	TextGayness gayness = GAY;
	FILE* fp = argc > 1 ? fopen(argv[1], "rb") : 0;
	if(!fp)
	{
		const char* errormsg = "Too few arguments.\n";
		write(2, errormsg, strlen(errormsg));
		return -1;
	}

	if(argc > 2)
	{
		if(strstr(argv[2], "super"))
		{
			gayness = SUPER_GAY;
		}
		else if(strstr(argv[2], "mega"))
		{
			gayness = MEGA_GAY;
		}
	}

	fseek(fp, 0, SEEK_END);
	size_t filelen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* text = malloc(sizeof(char) * (filelen + 1));
	if(!text)
	{
		const char* errormsg = "Couldn't allocate enough memory.\n";
		write(2, errormsg, strlen(errormsg));
		free(text);
		return -1;
	}

	if(fread(text, sizeof(char), filelen, fp) != filelen)
	{
		free(text);
		const char* errormsg = "Error reading file.\n";
		write(2, errormsg, strlen(errormsg));
		return -1;
	}
	text[filelen + 1] = 0;

	struct GayText tm;
	memset(&tm, 0, sizeof(struct GayText));


	GayText_ScanTextFile(&tm, text, filelen + 1);
	cv_t processed_text;
	cv_init(&processed_text, filelen);
	//Replace tabs and blank lines with spaces to even out with
	//the longest lines
	GayText_PrepareText(&tm, text, filelen, &processed_text);

	cv_t newgaytext;
	cv_init(&newgaytext, filelen << 1);

	GayText_MakeTextGay(&tm, processed_text.data, processed_text.length, &newgaytext, gayness);
	printf("%s", newgaytext.data);

	cv_destroy(&newgaytext);
	cv_destroy(&processed_text);
	fclose(fp);
	free(text);
	return 0;
}
