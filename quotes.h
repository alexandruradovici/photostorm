
#ifndef _QUOTES__H
#define _QUOTES__H

#include "data.h"

struct _ps_quote
{
	char *quote;
	char *author;
	char *keywords;
};

_ps_quote * ps_quote_create (const char *text, const char *autor, const char *keywords);
_ps_quote * ps_get_quote_nr (const char *quote_file, int qnr);

#ifndef PHOTOSTORM_WEBSITE

_ps_quote * ps_get_quote_online (const char *server, int port, const char *keywords, int r_anyway);

#endif
_ps_quote * ps_get_quote (const char *quote_file, const char *keys, int r_anyway);
void ps_quote_free (_ps_quote *quote);

#endif
