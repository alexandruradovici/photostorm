/**************************************************************
*		File: translate.h                                      *
*		ID: PS_translate_h                                     *
*		Author: Alexandru Radovici                             *
*		Date: February 5, 2004                                 *
*		Modified: February 6, 2004                             *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This file is header of "translate.cpp".       *
*		What's New:                                            *
*			* Modified "__(...)" function                      *
*			* Added "__to_lang(...)" function                  *
**************************************************************/

#ifndef _TRANSLATE__H
#define _TRANSLATE__H

//language table
#define ps_lang_eng 0
#define ps_lang_ro 0
#define ps_lang_default 0

struct _ps_lang
{
   char *nume;
   char *autor; 
   char *versiune;
   char *data;
   char *font;
   char *file;
};

// functions
void ps_set_language (const char *flanguage);
char * __(const char *s);
char * __to_lang(const char *s, int lang);
_ps_lang * ps_lang_new_from_file (const char *filename);
_ps_lang * ps_lang_new (const char *nume, const char *autor, const char *versiunea, const char *data);
void ps_lang_repair (_ps_lang *lang);
void ps_lang_free (_ps_lang *lang);

#endif
