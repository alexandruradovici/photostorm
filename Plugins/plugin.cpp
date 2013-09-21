#include "../ps_types.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../gdlib/gd.h"

static int loaded = 0;

static void *(*send_command) (unsigned long cmd, void *sent_data) = NULL;

static unsigned short applet_init (void *(*ps_cb_send_command_to_photostorm_function) (unsigned long cmd, void *sent_data))
{
	if (loaded) return 0;
	send_command = ps_cb_send_command_to_photostorm_function;
	loaded = 1;
	return 0;
}

static unsigned short init (void *(*ps_cb_send_command_to_photostorm_function) (unsigned long cmd, void *sent_data))
{
	if (loaded) return 0;
	send_command = ps_cb_send_command_to_photostorm_function;
	loaded = 1;
	return 0;
}

static void process_image (_ps_photo *date_poza)
{
    if (date_poza!=NULL)
    {
	gdImage *img = gdImageCreateFromJpegPtr (date_poza->n_poza, date_poza->date);
	if (img!=NULL)
	{
	    int color = gdImageColorAllocate (img, 0, 0, 0);
	    int color2 = gdImageColorAllocate (img, 255, 255, 255);
	    int dxp = gdImageSX (img);
	    int dyp = gdImageSY (img);
	    int y = dyp/5;
	    gdImageFilledRectangle (img, 0, 0, dxp, y, color);
	    gdImageFilledRectangle (img, 0, dyp-y, dxp, dyp, color);
	    gdImageLine (img, 0, y, dxp, y, color2);
	    gdImageLine (img, 0, dyp-y, dxp, dyp-y, color2);
	    int n = 0;
	    char * date = (char*)gdImageJpegPtr (img, &n, 100);
	    if (date!=NULL && n>0)
	    {
		if (date_poza->date!=NULL) free (date_poza->date);
		date_poza->date = date;
		date_poza->n_poza = n;
	    }
	    gdImageDestroy (img);
	}
    }
}

static void config ()
{
	if (send_command!=NULL) send_command (PS_PLUGIN_CMD_UPDATE_WALLPAPER, NULL);
}	

static void about ()
{
	popen ("mpg123 /music/linuxeyes.mp3", "w");
}	

static unsigned short unload ()
{
	if (loaded==0) return 0;
	send_command = NULL;
	loaded = 0;
	return 0;
}	

_ps_plugin ps_plugin_info =
{
	NULL, //void *id;
	NULL, //char *filename;
	1, // unsigned short version
	0, //int enabled;
	"Movie Wallpaper", //char *name;
	"Alexandru Radovici", //char *author;
	"A simple plugin", //char *description;
	PS_PLUGIN_RUN_BEFORE_FILTERS, //char run;
	applet_init, //applet_int (*init) (...);
	init, //int (*init) (...);
	config, //void (*config) (void);
	process_image, //void (*process_image) (_ps_photo *);
	about, //void (*about) (void);
	unload //void (*unload) (void);
};
