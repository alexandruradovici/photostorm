
#include "quotes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "data.h"
#include "misc.h"

#ifndef PHOTOSTORM_WEBSITE

#include "ps_system.h"

#endif

_ps_quote * ps_quote_create (const char *text, const char *autor, const char *keywords)
{
	_ps_quote *quote = NULL;
	if (text!=NULL && autor!=NULL && keywords!=NULL)
	{
		quote = (_ps_quote*)malloc (sizeof(_ps_quote));
		quote->quote = strdup (text);
		quote->author = strdup (autor);
		quote->keywords = strdup (keywords);
	}
	return quote;
}

_ps_quote * ps_get_quote_nr (const char *quote_file, int qnr)
{
	_ps_quote *result = NULL;
	int nr=0;
	gzFile f = gzopen (quote_file, "rb");
	if (f!=NULL)
	{
		int psq=1;
		while (!gzeof(f) && psq==1)
		{
			char id[4];
			gzread (f,&id,3);
			id[3]='\0';
			if (strcmp(id,"PSQ")==0)
			{
				unsigned short n;
				gzread (f,&n,2);
				char * date_nefolosite;
				date_nefolosite = (char*)malloc (n-5);
				gzread (f, date_nefolosite, n-5);
				free (date_nefolosite);
				while (!gzeof(f) && nr<qnr)
				{
					char text[10000];
					char autor[100];
					char keywords[100];
					gzread (f,&n,2);
					gzread (f,text,n);
					text[n]='\0';
					gzread (f,&n,2);
					gzread (f,autor,n);
					autor[n]='\0';
					gzread (f,&n,2);
					gzread (f,keywords,n);
					keywords[n]='\0';
					nr++;
					if (nr==qnr) result = ps_quote_create (text, autor, keywords); 
				}
			} else psq=0;
		}
		gzclose (f);
	}
	return result;
}

_ps_quote * ps_get_quote (const char *quote_file, const char *keys, int r_anyway)
{
	_ps_quote *result = NULL;
	int lista[10000];
	int nr_q=0;
	int nr=0;
	gzFile f = gzopen (quote_file, "rb");
	if (f!=NULL)
	{
		int psq=1;
		while (!gzeof(f) && psq==1)
		{
			char id[4];
			gzread (f,&id,3);
			id[3]='\0';
			if (strcmp(id,"PSQ")==0)
			{
				unsigned short n;
				gzread (f,&n,2);
				char * date_nefolosite;
				date_nefolosite = (char*)malloc (n-5);
				gzread (f, date_nefolosite, n-5);
				free (date_nefolosite);
				while (!gzeof(f))
				{
					char text[10000];
					char autor[100];
					char keywords[100];
					gzread (f,&n,2);
					gzread (f,text,n);
					text[n]='\0';
					gzread (f,&n,2);
					gzread (f,autor,n);
					autor[n]='\0';
					gzread (f,&n,2);
					gzread (f,keywords,n);
					keywords[n]='\0';
					nr++;
					if (search_keywords (strdup (keys), keywords))
					{
						nr_q++;
						lista[nr_q]=nr;
					}
				}
			} else psq=0;
		}
		gzclose (f);
	}
	if (nr_q>0)
	{
		int i = 1+(ps_random (nr_q));
		result = ps_get_quote_nr (quote_file, lista[i]);
	}
	else if (r_anyway && nr>0)
	{
		int i = 1+(ps_random (nr));
		result = ps_get_quote_nr (quote_file, i);
	}
	return result;
}

#ifndef PHOTOSTORM_WEBSITE

_ps_quote * ps_get_quote_online (const char *server, int port, const char *keywords, int r_anyway)
{
	_ps_quote *result = NULL;
	if (server!=NULL)
	{
		int sock = socket_connect (server, port);
		if (sock != -1)
		{
			char buf[MAX_BUF];
			sprintf (buf, "Agent: %s\nAccept: %s\nReturn-anyway: %d\n\n\n", ps_name_inet, ps_accept_inet, r_anyway);
			ps_send (sock, buf, strlen (buf), 0);
			FILE *f = fdopen (sock, "r");
			if (f!=NULL)
			{
				if (!feof(f)) 
				{
					/*char *srv_data = readline (f);
					if (srv_data!=NULL && strcmp (srv_data, "PSQ_SERVER")==0)
					{*/
						if (!feof(f)) 
						{
							result = (_ps_quote*)malloc (sizeof(_ps_quote));
							result->quote = NULL;
							result->author = NULL;
							result->keywords = NULL;
							result->quote = readline (f);
							if (!feof(f)) result->author = readline (f);
						}
					/*}*/
				}
				fclose (f);
			}
			socket_close (sock);
		}
	}
	return result;
}

#endif

void ps_quote_free (_ps_quote *quote)
{
	if (quote!=NULL)
	{
		if (quote->quote!=NULL) free (quote->quote);
		if (quote->author!=NULL) free (quote->author);
		if (quote->keywords!=NULL) free (quote->keywords);
		free (quote);
	}
}
