
#ifndef _MISC__H
#define _MISC__H

#include <stdio.h>
#include <stdlib.h>

#include "photo.h"

const char * config_path (const char *file);
const char * album_path (const char *album);
const char * pict_path (const char *pict);
char * quotes_path (const char *quotes);
char * font_path (const char *font);
char * help_path (const char *dfile);
const char *generate_security_code (int n);
char * strtrim (const char *s_);
char * ps_strfilter_filename (char *filename);
char * ps_replace_str (char *s, unsigned long lmax);
char * extract_path (const char *str);
char * extract_file_name (const char *str);
int search_keywords (char *keys, const char *keywords);
char *readline (FILE *f);
int ps_strtoint (const char *str);
const char * ps_inttostr (int nr);
_ps_color * ps_strtocolor (const char *str);
const char * ps_colortostr (_ps_color color);
_ps_color * ps_pack_color (int r, int g, int b, int alpha);;

#endif
