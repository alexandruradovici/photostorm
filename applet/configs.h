
#ifndef _CONFIGS__H
#define _CONFIGS__H

#include "data.h"

#ifdef PHOTOSTORM_PROGRAM

#ifndef PHOTOSTORM_WEBSITE

#include <gtk/gtk.h>

#endif

#endif

#include "photo.h"

#ifndef PHOTOSTORM_WEBSITE

#define ps_load_config_default() ps_load_config (ps_get_config_filename());
#define ps_save_system_config() ps_save_config (ps_get_config_filename(), *ps_get_config ());

#endif


struct _ps_position
{
	short a;
	int x;
	int y;
};

struct _ps_config_wallpaper
{
	short cint;
	short dtype;
	short gray;
	short dinfo;
	char format[1000];
	short border;
	_ps_position pos;
	_ps_color fcolor;
	short fsize;
};

struct _ps_config_calendar
{
	short disp;
	short border;
	_ps_position pos;
	_ps_color mcolor;
	_ps_color fcolor;
	short cmfsize;
	short omfsize;
	short mark_date;
	short p_month;
	short n_month;
};

struct _ps_config_quotes
{
	short disp;
	short border;
	_ps_position pos;
	_ps_color fcolor;
	short fsize;
	short inet;
	char server[255];
	short port;
	char keywords[255];
	short only_keywords;
};

struct _ps_config_email
{
	short disp;
	short border;
	_ps_position pos;
	short max_nr;
	_ps_color fcolor;
	_ps_color scolor;
	short fsize;
	char server [255];
	short port;
	char username [255];
	char password [255];
	short update;
};

struct _ps_config_lang
{
	short enabled;
};

struct _ps_config_system
{
	char shared_albums[1000];
	char albums[1000];
	char libs[1000];
	int scr_width;
	int scr_height;
};

struct _ps_config
{
	_ps_config_wallpaper wallpaper;
	_ps_config_calendar calendar;
	_ps_config_quotes quotes;
	_ps_config_email email;
	_ps_config_lang lang;
	_ps_config_system system;
};

#define _PS_CONFIG

#ifdef PHOTOSTORM_PROGRAM

#ifndef PHOTOSTORM_WEBSITE

void ps_configuration (GtkWidget *parent);

#endif

#endif

_ps_config * ps_load_config (const char *file);
#ifndef PHOTOSTORM_WEBSITE
void ps_config_default ();
char * ps_get_config_filename ();
char * ps_get_config_path ();
#endif
_ps_config * ps_get_config ();
void ps_save_config (const char *file, _ps_config config);


#endif
