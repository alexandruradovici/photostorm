
#ifndef _PLUGINS__H
#define _PLUGINS__H

#include "data.h"
#include "ps_types.h"
#include <gtk/gtk.h>

GList *get_ps_plugins_list ();

//void ps_plugin_free (_ps_plugin *date_plugin);
void load_plugins (const char *calea);
int plugin_init (_ps_plugin *plugin);
int plugin_uninit (_ps_plugin *plugin);
void unload_plugins ();

#ifdef PHOTOSTORM_APPLET
void system__ (const char *cmd);
#endif

#endif
