
#include <windows.h>

#include "../../ps_types.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int loaded = 0;

static void *(*send_command) (unsigned long cmd, void *sent_data) = NULL;

static unsigned short applet_init (void *(*ps_cb_send_command_to_photostorm_function) (unsigned long cmd, void *sent_data))
{
	if (loaded) return 0;
	send_command = ps_cb_send_command_to_photostorm_function;
//	send_command (PS_PLUGIN_CMD_UPDATE_WALLPAPER, NULL);
	loaded = 1;
	printf ("Plugin is initialized for the applet\n");
	return 0;
}

static unsigned short init (void *(*ps_cb_send_command_to_photostorm_function) (unsigned long cmd, void *sent_data))
{
	if (loaded) return 0;
	send_command = ps_cb_send_command_to_photostorm_function;
//	send_command (PS_PLUGIN_CMD_UPDATE_WALLPAPER, NULL);
	loaded = 1;
	printf ("Plugin is initialized\n");
	return 0;
}

static void process_image (_ps_photo *date_poza)
{
	// here comed the image processing part
	// this function will be called when the wallpaper is beeing refreshed
}

static void config ()
{
	printf ("Plugin config function\n");
}	

static void about ()
{
	printf ("Plugin about function\n");
}	

static unsigned short unload ()
{
	if (loaded==0) return 0;
	send_command = NULL;
	loaded = 0;
	printf ("Plugin is unloaded\n");
	return 0;
}	

__declspec (dllexport) _ps_plugin ps_plugin_info =
{
	NULL, //void *id;
	NULL, //char *filename;
	1, //unsigned short version;
	0, //int enabled;
	"Demo Plugin", //char *name;
	"PhotoStorm Team", //char *author;
	"Sample plugin. Use this one as a sample for new plugins.", //char *description;
	PS_PLUGIN_RUN_BEFORE_FILTERS, //char run;
	applet_init, //void (*init) (...);
	init, //void (*init) (..);
	config, //void (*config) (void);
	process_image, //void (*process_image) (_ps_photo *);
	about, //void (*about) (void);
	unload //void (*unload) (void);
};




