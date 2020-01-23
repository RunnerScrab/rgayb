#include "gtext.h"

int GayText_AllocateColorTable(struct GayText* tm, size_t length)
{
	tm->color_table = (union rgb*) malloc(sizeof(union rgb) * length);
	memset(tm->color_table, 0, sizeof(union rgb) * length);
	return tm->color_table ? 0 : -1;
}

void GayText_FreeColorTable(struct GayText* tm)
{
	free(tm->color_table);
}

void GayText_ScanTextFile(struct GayText* tm, const char* text, const size_t textlen)
{
	size_t idx = 0;
	tm->lines = 1;
	while(idx < textlen)
	{
		char* p = strchr(&text[idx], '\n');
		if(p)
		{
			size_t linelen = (p - &text[idx]);
			if(tm->longest_line < linelen)
			{
				tm->longest_line = linelen;
			}
			++tm->lines;
			idx += linelen + 1;
		}
		else
		{
			if(tm->lines == 1)
			{
				tm->longest_line = textlen;
			}
			break;
		}
	}
}

static int CreateANSIResetCode(char* output, size_t output_maxlen)
{
	return snprintf(output, output_maxlen, "\x1b[0m");
}

static int CreateANSI24BitCode(union rgb* color, TextGayness gayness, char* output, size_t output_maxlen)
{
	switch(gayness)
	{
	case GAY:
		return snprintf(output, output_maxlen, "\x1b[%d;2;%d;%d;%dm",
				38, color->components.r, color->components.g, color->components.b);
	case SUPER_GAY:
		return snprintf(output, output_maxlen, "\x1b[%d;2;%d;%d;%dm\x1b[%d;2;%d;%d;%dm",
				48, color->components.r, color->components.g, color->components.b,
				38,
				0,0,0);
	case MEGA_GAY:
		return snprintf(output, output_maxlen, "\x1b[%d;2;%d;%d;%dm\x1b[%d;2;%d;%d;%dm",
				48, color->components.r, color->components.g, color->components.b,
				38,
				(unsigned char)(255-color->components.r), (unsigned char)(255-color->components.g),
				(unsigned char)(255-color->components.b));

	default:
		return -1;
	}
}

void GayText_PopulateColorTable(struct GayText* tm, TextGayness gayness)
{
	union rgb* ct = tm->color_table;
	size_t idx = 0, len = tm->longest_line;
	for(; idx < len; ++idx)
	{
		hsv_to_rgb(&ct[idx], len, idx);
	}
}

int GayText_MakeTextGay(struct GayText* tm, const char* text, const size_t textlen,
			cv_t* output, TextGayness gayness)
{
	size_t idx = 0, lineidx = 0;
	size_t longest_line = tm->longest_line;
	char code[128] = {0};
	int codelen = 0;
	GayText_AllocateColorTable(tm, tm->longest_line);
	GayText_PopulateColorTable(tm, gayness);
	union rgb* ct = tm->color_table;
	for(lineidx = 1; idx < textlen; ++idx, ++lineidx)
	{
		char ch = text[idx];
		if(ch == '\n')
		{
			lineidx = 0;
			codelen = CreateANSIResetCode(code, 128);
		}
		else
		{
			codelen = CreateANSI24BitCode(&ct[gayness == GAY ? (idx % longest_line) :
							 (lineidx % longest_line)], gayness, code, 128);
		}
		if(codelen < 0)
		{
			return -1;
		}
		cv_append(output, code, codelen);
		cv_push(output, ch);
	}
	//Reset terminal colors back to default
	codelen = CreateANSIResetCode(code, 128);
	cv_append(output, code, codelen);
	//We need a printing character following the reset code for it to actually
	//take effect, apparently
	cv_push(output, '\n');
	cv_push(output, 0);
	GayText_FreeColorTable(tm);
	return 0;
}

void GayText_PrepareText(struct GayText* tm, const char* text, size_t textlen, cv_t* out)
{
	size_t idx = 0, lineidx = 0;
	size_t longest_line = tm->longest_line;
	for(; idx < textlen; ++idx)
	{
		char ch = text[idx];
		switch(ch)
		{
		case '\n':

			for(; lineidx < longest_line; ++lineidx)
			{
				cv_push(out, ' ');
			}

			cv_push(out, '\n');
			lineidx = 0;
			break;
		case '\t':
		{
			size_t tabidx = 0;
			for(; tabidx < 3; ++tabidx, ++lineidx)
			{
				cv_push(out, ' ');
			}
		}
		break;
		default:
			cv_push(out, ch);
			++lineidx;
			break;
		}

	}
}
