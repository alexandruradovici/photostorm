
#include "misc.h"
#include "data.h"
#include "photo.h"
#include "ps_system.h"
#include "configs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char * config_path (const char *file)
{
	char *result;
	if (file==NULL) result=strdup ("");
	else
	{
		char t[10000];
		sprintf (t, "%s%c.photostorm%c%s", ps_get_user_dir (), ps_dir_sep, ps_dir_sep, file);
		result = strdup (t);
	}
	return result;
}

const char * album_path (const char *album)
{
	char *result;
	_ps_config *config = ps_get_config ();
	if (album==NULL && config!=NULL) result=strdup ("");
	else
	{
		char t[10000];
		sprintf (t, "%s%c%s", config->system.albums, ps_dir_sep, album);
		result = strdup (t);
	}
	return result;
}

const char * pict_path (const char *pict)
{
	char *result;
	_ps_config *config = ps_get_config ();
	if (pict==NULL && config!=NULL) result=strdup ("");
	else
	{
		char t[10000];
		sprintf (t, "%s%cpics%c%s", config->system.libs, ps_dir_sep, ps_dir_sep, pict);
		result = strdup (t);
	}
	return result;
}

char * font_path (const char *font)
{
	char *result;
	_ps_config *config = ps_get_config ();
	if (font==NULL && config!=NULL) result=strdup ("");
	else
	{
		char t[10000];
		sprintf (t, "%s%cfonts%c%s.ttf", config->system.libs, ps_dir_sep, ps_dir_sep, font);
		result = strdup (t);
	}
	return result;
}

char * quotes_path (const char *quotes)
{
	char *result = strdup ("");
	_ps_config *config = ps_get_config ();
	if (quotes==NULL && config!=NULL) result=strdup ("");
	else
	{
		char t[10000];
		sprintf (t, "%s%cQuotes%c%s.psq", config->system.libs, ps_dir_sep, ps_dir_sep, quotes);
		result = strdup (t);
	}
	return result;
}

/*char * album_path (char *album)
{
	char *result;
	if (album==NULL) result=strdup ("");
	else
	{
		if (ps_albums_path!=NULL) result=(char*)malloc (strlen(ps_albums_path)+strlen(album)+10);
			else result=(char*)malloc (strlen(album)+10);
		if (result!=NULL)
		{
			if (ps_albums_path!=NULL) sprintf ((char*)result,"%s/%s",ps_albums_path,(char*)album);
				else sprintf ((char*)result,"./%s",(char*)album);
		}
	}
	return result;
}*/

/*char * pict_path (char *pict)
{
	char *result;
	if (pict==NULL) result=strdup ("");
	else
	{
		if (ps_pics_path!=NULL) result=(char*)malloc (strlen(ps_pics_path)+strlen(pict)+10);
			else result=(char*)malloc (strlen(pict)+10);
		if (result!=NULL)
		{
			if (ps_pics_path!=NULL) sprintf ((char*)result,"%s/%s",ps_pics_path,(char*)pict);
				else sprintf ((char*)result,"./%s",(char*)pict);
		}
	}
	return result;
}

char * font_path (char *font)
{
	char *result;
	if (font==NULL) result=strdup ("");
	else
	{
		if (ps_font_path!=NULL) result=(char*)malloc (strlen(ps_font_path)+strlen(font)+10);
			else result=(char*)malloc (strlen(font)+10);
		if (result!=NULL)
		{
			if (ps_font_path!=NULL) sprintf ((char*)result,"%s/%s.ttf",ps_font_path,(char*)font);
				else sprintf ((char*)result,"./%s.ttf",(char*)font);
		}
	}
	return result;
}*/

char * strtrim (const char *s_)
{
	char * result=NULL;
	if (s_!=NULL)
	{
		const char *s=s_;
		unsigned int i=0;
		unsigned int l=0;
		for (i=0;i<strlen(s_) && s_[i]==' ';i++) s++;
		if (i<strlen(s_))
		{
			const char *temp = s;
			for (i=strlen (temp)-1;i>=0 && temp[i]==' ';i--) l++;
			result = (char*)malloc (strlen (temp)-l+1);
			if (result!=NULL)
			{
				strncpy (result, temp, strlen(temp)-l);
				result [strlen(temp)-l]='\0';
			}
		}
	}
	return result;
}

char * ps_strfilter_filename (char *filename)
{
	char * result = NULL;
	if (filename!=NULL)
	{
		result = strtrim (filename);
		for (unsigned int i=0;i<strlen (result);i++) 
			if (result[i]==' ' || result[i]=='\\' || result[i]=='/') result[i]='_';
	}
	return result;
}

char * ps_replace_str (char *s, unsigned long lmax)
{
	char *result=NULL;
	if (s!=NULL)
	{
		if (strlen(s)>lmax) s[lmax]='\0';
		result=(char*)malloc (strlen(s)+1);
		strcpy (result, s);
		result[strlen(s)]='\0';
	}
	return result;
}

char * extract_path (const char *str)
{
	char * result = strdup ("");
	if (str!=NULL)
	{
		char temp[10000];
		strcpy (temp,"");
		int gata = 0;
		int i=strlen(str);
		while (!gata && i>0)
		{
			i--;
			if (str[i]=='\\' || str[i]=='/') gata=1;
		}
		strncpy (temp, str, i);
		temp[i]='\0';
		result = strdup (temp);
		//printf ("calea: %s\n", result);
	}
	return result;
}

char * extract_file_name (const char *str)
{
	char * result = strdup ("");
	if (str!=NULL)
	{
		char temp[10000];
		strcpy (temp,"");
		int gata = 0;
		int i=strlen(str);
		while (!gata && i>0)
		{
			i--;
			if (str[i]=='\\' || str[i]=='/') gata=1;
		}
		strncpy (temp, &str[i]+1, strlen(str)-i);
		temp[i]='\0';
		result = strdup (temp);
		//printf ("Fisier: %s\n", result);
	}
	return result;
}

int search_keywords (char *keys, const char *keywords)
{
	int result = 0;
	if (keywords!=NULL)
	{
		char *key = strtok (keys, ", ");
		while (key!=NULL && result==0)
		{
			if (strstr (keywords, key)!=NULL) result = 1;
			key = strtok (NULL, ", ");
		}
	}
	return result;
}

char *readline (FILE *f)
{
	char *line = NULL;
	if (!feof(f))
	{
		char v;
		int n;
		fread (&v, 1, 1, f);
		do
		{
			if (v!='\n')
			{
				if (line!=NULL) line = (char*)realloc (line, strlen (line)+2);
				else 
				{	
					line = (char*)malloc (2);
					line[0]='\0';
				}
				n = strlen(line);
				line [n] = v;
				line [n+1] = '\0';
			}
			fread (&v, 1, 1, f);
		} while (!feof(f) && v!='\n');
	}
	return line;
}

int ps_strtoint (const char *str)
{
	int nr=0;
	if (strcmp (str,"yes")==0) nr=1;
	else
	if (strcmp (str,"no")==0) nr=0;
	else
	sscanf (str, "%d", &nr);
	return nr;
}

const char * ps_inttostr (int nr)
{
	char temp[100];
	sprintf (temp, "%d", nr);
	return strdup (temp);
}

_ps_color * ps_strtocolor (const char *str)
{
	_ps_color *color = ps_pack_color (255, 255, 255, 0);
	if (strcmp (str,"white")==0)
	{
		color->red=255;
		color->green=255;
		color->blue=255;
		color->alpha=0;
	}
	else
	if (strcmp (str,"black")==0)
	{
		color->red=0;
		color->green=0;
		color->blue=0;
		color->alpha=0;
	}
	else
	if (strcmp (str,"red")==0)
	{
		color->red=255;
		color->green=0;
		color->blue=0;
		color->alpha=0;
	}
	else
	if (strcmp (str,"green")==0)
	{
		color->red=0;
		color->green=255;
		color->blue=0;
		color->alpha=0;
	}
	else
	if (strcmp (str,"blue")==0)
	{
		color->red=255;
		color->green=255;
		color->blue=255;
		color->alpha=0;
	}
	else
	if (strcmp (str,"yellow")==0)
	{
		color->red=255;
		color->green=255;
		color->blue=0;
		color->alpha=0;
	}
	else
	if (strcmp (str,"magenta")==0)
	{
		color->red=255;
		color->green=0;
		color->blue=255;
		color->alpha=0;
	}
	else
	if (strcmp (str,"cyan")==0)
	{
		color->red=0;
		color->green=255;
		color->blue=255;
		color->alpha=0;
	}
	else
	{
		int red, green, blue, alpha;
		sscanf (str, "%d,%d,%d,%d", &red, &green, &blue, &alpha);
		alpha = 127-alpha*127/255;
		color->red=red;
		color->green=green;
		color->blue=blue;
		color->alpha=alpha;
	}
	return color;
}

const char * ps_colortostr (_ps_color color)
{
	char temp[100];
	if (color.red==255 && color.green==255 && color.blue==255 && color.alpha==0)
		strcpy (temp, "white");
	else
	if (color.red==0 && color.green==0 && color.blue==0 && color.alpha==0)
		strcpy (temp, "black");
	else
	if (color.red==255 && color.green==0 && color.blue==0 && color.alpha==0)
		strcpy (temp, "red");
	else
	if (color.red==0 && color.green==255 && color.blue==0 && color.alpha==0)
		strcpy (temp, "green");
	else
	if (color.red==0 && color.green==0 && color.blue==255 && color.alpha==0)
		strcpy (temp, "blue");
	else
	if (color.red==255 && color.green==255 && color.blue==0 && color.alpha==0)
		strcpy (temp, "yellow");
	else
	if (color.red==0 && color.green==255 && color.blue==255 && color.alpha==0)
		strcpy (temp, "cyan");
	else
	if (color.red==255 && color.green==0 && color.blue==255 && color.alpha==0)
		strcpy (temp, "magenta");
	else
		sprintf (temp, "%d,%d,%d,%d", color.red, color.green, color.blue, (int)(255-color.alpha*255/127));
	return strdup (temp);
}

_ps_color * ps_pack_color (int r, int g, int b, int alpha)
{
	_ps_color *result = (_ps_color*)malloc (sizeof(_ps_color));
	if (result!=NULL)
	{
		result->red = r;
		result->green = g;
		result->blue = b;
		result->alpha = alpha;
	}
	return result;
}
