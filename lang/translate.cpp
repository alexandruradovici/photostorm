/**************************************************************
*		File: translate.cpp                                    *
*		ID: PS_translate_cpp                                   *
*		Author: Alexandru Radovici                             *
*		Date: February 5, 2004                                 *
*		Modified: February 6, 2004                             *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This file contains the main translation       *
*               functions PhotoStorm.                         *
*		What's New:                                            *
*			* Modified "__(...)" function                      *
*			* Added "__to_lang(...)" function                  *
*			* Repaired id check (id[3]='\0')                   *
**************************************************************/

#include "translate.h"
#include "../data.h"
#include "../configs.h"
#include "../misc.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static char* current_language = NULL;

void ps_set_language (const char *flanguage)
{
	if (current_language!=NULL) free (current_language);
	current_language = strdup (flanguage);
	//printf ("current_language =%s=\n", current_language);
}

char * __tf (int lang)
{
	/*char *result;
	if (ps_path!=NULL) result=(char*)malloc (strlen(ps_path)+21);
		else result=(char*)malloc (21);
	if (result!=NULL)
	{
		if (ps_path!=NULL) sprintf ((char*)result,"%s/languages/%s.psl",ps_path,ps_lang_table[lang],lang);
			else sprintf ((char*)result,"./languages/%s.psl",ps_lang_table[lang]);
	}
	return result;*/
	return current_language!=NULL?current_language:strdup ("");
	//return strdup ("");
}

char * __(const char *s)
{
    _ps_config *config = ps_get_config ();
    if (!config->lang.enabled) return strdup (s);
	return __to_lang (s,config->lang.enabled);
}

char * __to_lang(const char *s, int lang)
{	
	char *result=strdup(s);
	int found=0;
	if (lang!=0)
	{
		gzFile f;
		f=gzopen ((char*)__tf(lang),"rb");
		if (f!=NULL)
		{
			if (!gzeof(f))
			{
				char id[4];
				gzread (f,&id,3);
				id[3]='\0';
				//printf ("%s\n", id);
				//fprintf (stderr, "*\n");
				if (strcmp(id,"PSL")==0)
				{
					unsigned short n;
					gzread (f,&n,2);
					char * date_nefolosite;
					date_nefolosite = (char*)malloc (n-5);
					gzread (f, date_nefolosite, n-5);
					free (date_nefolosite);
					char original [32600];
					char tradus [32600];
					while (!gzeof(f) && !found)
					{
						gzread (f,&n,2);
						gzread (f,&original,n);
						original[n]='\0';
						gzread (f,&n,2);
						gzread (f,&tradus,n);
						tradus[n]='\0';
						//printf ("[%s] = [%s]\n", original, tradus);
						if (strcasecmp (original,(char*)s)==0)
						{
							result=strdup (tradus);
							found=1;
						}
					}
				} else lang=0;
			}
			gzclose (f);
		}
		else lang=0;
	}
	if (lang==0 || !found) result=strdup (s);
	return result;
}

_ps_lang * ps_lang_new (const char *nume, const char *autor, const char *versiunea, const char *data)
{
    _ps_lang *lang = (_ps_lang*)malloc (sizeof (_ps_lang));
    lang->file = strdup ("");
    lang->nume = strdup (nume);
    lang->autor = strdup (autor);
    lang->versiune = strdup (versiunea);
    lang->data = strdup (data);
    lang->font = strdup ("");
    return lang;
}

_ps_lang * ps_lang_new_from_file (const char *filename)
{
    _ps_lang *lang = NULL;
    if (filename!=NULL)
    {
        gzFile f;
		f=gzopen (filename,"rb");
		if (f!=NULL)
		{
			if (!gzeof(f))
			{
				char id[4];
				gzread (f,&id,3);
				id[3]='\0';
				if (strcmp(id,"PSL")==0)
				{
				    lang = (_ps_lang*)malloc (sizeof (_ps_lang));
				    lang->file = extract_file_name (filename);
				    lang->nume = NULL;
				    lang->autor = NULL;
				    lang->versiune = NULL;
				    lang->data = NULL;
				    lang->font = NULL;
				    
				    unsigned short n;
					gzread (f,&n,2);
					char * date;
					date = (char*)malloc (n-5);
					gzread (f, date, n-5);
					
					char atribut [10000];
					char valoare [10000];
					int pos=0;
					int nr = n-5;
					while (pos<nr)
					{
                         memcpy (&n, &date[pos], 2);
                         pos=pos+2;
                         memcpy (&atribut, &date[pos], n);
                         atribut[n]='\0';
                         pos = pos + n;
                         
                         memcpy (&n, &date[pos], 2);
                         pos=pos+2;
                         memcpy (&valoare, &date[pos], n);
                         valoare[n]='\0';
                         pos = pos + n;
                         
                         //printf ("[%s] = [%s]\n", atribut, valoare);
                         
                         if (strcasecmp ("lang", atribut)==0) lang->nume=strdup (valoare);
                         if (strcasecmp ("authors", atribut)==0) lang->autor=strdup (valoare);
                         if (strcasecmp ("version", atribut)==0) lang->versiune=strdup (valoare);
                         if (strcasecmp ("date", atribut)==0) lang->data=strdup (valoare);
                         if (strcasecmp ("font", atribut)==0) lang->font=strdup (valoare);
                    }
					
                    free (date);
				}
			}
			gzclose (f);
		}
    }
    return lang;
}

void ps_lang_repair (_ps_lang *lang)
{
    if (lang->nume==NULL) lang->nume = strdup ("");
    if (lang->autor==NULL) lang->autor = strdup ("");
    if (lang->versiune==NULL) lang->versiune = strdup ("");
    if (lang->data==NULL) lang->data = strdup ("");
    if (lang->font==NULL) lang->font = strdup ("");
}

void ps_lang_free (_ps_lang *lang)
{
    if (lang!=NULL)
    {
        if (lang->nume!=NULL) free (lang->nume);
        if (lang->autor!=NULL) free (lang->autor);
        if (lang->versiune!=NULL) free (lang->versiune);
        if (lang->data!=NULL) free (lang->data);
        if (lang->font!=NULL) free (lang->font);
        free (lang);
    }
}
