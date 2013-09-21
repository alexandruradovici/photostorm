
#ifndef _PS_TYPES__H
#define _PS_TYPES__H

#include "data.h"
#include <time.h>

// plugin run
#define PS_PLUGIN_RUN_BEFORE_FILTERS 0
#define PS_PLUGIN_RUN_AFTER_FILTERS 1

// plugin commands
#define PS_PLUGIN_CMD_UPDATE_WALLPAPER 1
#define PS_PLUGIN_CMD_SHOW_INFORMATION_DIALOG 2

typedef struct _ps_album
{
	char *nume;
	char *autor;
	char *categoria;
	char *descriere;
	char *metadata;
	time_t data;
	char *parola;
	long n_antet;
	long n_poze;
	unsigned char nr_poze;
	//char *antet;
	char *date;
};

struct _ps_photo
{
	char *nume;
	char *autor;
	char *descriere;
	char *optiuni;
	char *metadata;
	int n_poza_preview;
	char *date_preview;
	unsigned long n_poza;
	char *date;
};

struct _ps_color
{
	int red;
	int green;
	int blue;
	int alpha;
};

struct _ps_plugin
{
	void *handle;
	char *filename;
	unsigned short plugin_version;
	unsigned short enabled;
	char *name;
	char *author;
	char *description;
	char run;
	unsigned short (*applet_init) (void *(*cb_ps_send_command_to_photostorm_function)(unsigned long ps_cmd, void *sent_data));
	unsigned short (*init) (void *(*cb_ps_send_command_to_photostorm_function)(unsigned long ps_cmd, void *sent_data));
	void (*config) (void);
	void (*process_image) (_ps_photo *picture);
	void (*about) (void);
	unsigned short (*unload) (void);
};

struct _ps_message
{
	char id[45];
	int date;
	int month;
	int year;
	int wday;
	int hour;
	int minute;
	char message_text[250];
};

#endif
