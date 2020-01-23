#include "gtext.h"

void GayText_ScanTextFile(struct GayText* tm, const char* text, const size_t textlen)
{
	size_t idx = 0;
	tm->lines = 1;
	while(idx < textlen)
	{
		char* p = strchr(&text[idx], '\n');
		if(p)
		{
			size_t linelen = p - &text[idx];
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

int GayText_MakeTextGay(struct GayText* tm, const char* text, const size_t textlen,
			cv_t* out, TextGayness gayness)
{
	size_t idx = 0, lineidx = 0;
	size_t longest_line = tm->longest_line;
	char code[128] = {0};
	int codelen = 0;
	cv_t output;
	cv_init(&output, textlen << 1);

	for(; idx < textlen; ++idx, ++lineidx)
	{
		union rgb color;
		char ch = text[idx];
		hsv_to_rgb(&color, longest_line,
			gayness == GAY ? (idx % longest_line) :
			lineidx);

		if(ch == '\n')
		{
			lineidx = 0;
			codelen = CreateANSIResetCode(code, 128);
		}
		else
		{
			codelen = CreateANSI24BitCode(&color, gayness, code, 128);
		}
		if(codelen < 0)
		{
			return -1;
		}
		cv_append(&output, code, codelen);
		cv_push(&output, ch);
	}
	//Reset terminal colors back to default
	codelen = CreateANSIResetCode(code, 128);
	cv_append(&output, code, codelen);
	//We need a printing character following the reset code for it to actually
	//take effect, apparently
	cv_push(&output, '\n');
	cv_push(&output, 0);
	cv_copy(out, &output);
	cv_destroy(&output);
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
			for(; tabidx < 3; ++tabidx)
			{
				cv_push(out, ' ');
			}
			lineidx += tabidx;
		}
		break;
		default:
			cv_push(out, ch);
			++lineidx;
			break;
		}

	}
}
