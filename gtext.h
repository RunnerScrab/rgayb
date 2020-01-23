#ifndef GAYTEXT_H_
#define GAYTEXT_H_

#include "colorwheel.h"
#include "charvector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct GayText
{
	size_t longest_line;
	size_t lines;
	union rgb* color_table;
};

typedef enum {GAY = 0, SUPER_GAY, MEGA_GAY} TextGayness;

void GayText_FreeColorTable(struct GayText* tm);
void GayText_PopulateColorTable(struct GayText* tm, TextGayness gayness);
int GayText_AllocateColorTable(struct GayText* tm, size_t length);
void GayText_ScanTextFile(struct GayText* tm, const char* text, const size_t textlen);
int GayText_MakeTextGay(struct GayText* tm, const char* text, const size_t textlen,
			cv_t* out, TextGayness gayness);
//Change tabs to 3 spaces and fill blank lines with spaces
void GayText_PrepareText(struct GayText* tm, const char* text, size_t textlen, cv_t* out);

#endif
