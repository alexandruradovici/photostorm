#include "plugins.h"
#include "data.h"
#include "ps_system.h"
#include "photo.h"
#ifndef PHOTOSTORM_APPLET
#include "ps_dialogs.h"
#include "design.h"
#endif
#ifdef WIN32
#include <winbase.h>
#else
#include <dlfcn.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <dirent.h>

static GList *ps_plugins_list = NULL;

GList *get_ps_plugins_list ()
{
	return ps_plugins_list;
}

void * ps_command_from_plugin (unsigned long ps_cmd, void *sent_data)
{
	switch (ps_cmd)
	{
		case PS_PLUGIN_CMD_UPDATE_WALLPAPER:
			#ifndef PHOTOSTORM_APPLET
			ps_wallpaper_update (false);
			#else
			system__ ("photostorm --update_wallpaper");
			#endif
			break;
		case PS_PLUGIN_CMD_SHOW_INFORMATION_DIALOG:
			if ((char*)sent_data!=NULL)
			{
				#ifndef PHOTOSTORM_APPLET
				ps_information_dialog ("gtk-dialog-info", true, (char*)sent_data, ps_get_main_window ());
				#endif
			}
			break;
		default:
			break;
	}
	return NULL;
}

_ps_plugin * load_plugin (const char *filename)
{
    #ifdef WIN32
    SetErrorMode (SEM_FAILCRITICALERRORS);
    #endif
	_ps_plugin *date_plugin = NULL;
	#ifdef WIN32
	HINSTANCE__ *h = LoadLibrary (filename);
	#else
	void *h = dlopen(filename, RTLD_NOW);
	#endif
	if (h!=NULL)
	{
        #ifdef WIN32
        date_plugin = (_ps_plugin*)GetProcAddress (h, "ps_plugin_info");
        #else
		date_plugin = (_ps_plugin*)dlsym (h, "ps_plugin_info");
		#endif
		if (date_plugin!=NULL)
		{
			date_plugin->handle = h;
			date_plugin->filename = strdup (filename);
			date_plugin->enabled = 0;
		}
		#ifdef WIN32
		else FreeLibrary (h);
		#else
		else dlclose (h);
		#endif
	}
	return date_plugin;
}

int plugin_init (_ps_plugin *plugin)
{
	if (plugin!=NULL && plugin->init!=NULL) return plugin->init (ps_command_from_plugin);
		else return 2;
}

int plugin_uninit (_ps_plugin *plugin)
{
	if (plugin!=NULL && plugin->unload!=NULL) return plugin->unload ();
		else return 2;
}

void unload_plugin (_ps_plugin *date_plugin)
{
	if (date_plugin!=NULL)
	{
	    #ifdef WIN32
	    FreeLibrary ((HINSTANCE__*)date_plugin->handle);
	    #else
		dlclose (date_plugin->handle);
		#endif
	}
}

void add_plugin (const char *filename)
{
	_ps_plugin *date_plugin = load_plugin (filename);
	if (date_plugin!=NULL)
	{
		ps_plugins_list = g_list_append (ps_plugins_list, date_plugin);
	}
}

void load_plugins (const char *calea)
{
	if (calea!=NULL)
	{
		struct dirent *namelist;
		DIR *dir = opendir (calea);
		if (dir==NULL);
		else
		{
			int tot = 0;
			do
			{
				namelist = readdir (dir);
				if (namelist!=NULL)
				{
					
						char *temp;
						int t=1;
						if (calea[strlen(calea)]!='/') t=2;
						temp=(char*)malloc (strlen (calea)+strlen(namelist->d_name)+t);
						if (temp!=NULL)
						{
							strcpy (temp, calea);
							if (t==2) strcat (temp, ps_dir_sep_str);
							strcat (temp, namelist->d_name);
							add_plugin (temp);
							free (temp);
						}
				} else tot = 1;
			} while (!tot);
			closedir (dir);
		}
	}
}

void unload_plugins ()
{
	GList *ps_plugins_list = get_ps_plugins_list ();
	int nr = g_list_length (ps_plugins_list);
	int gasit = 0;
	for (int j=0;j<nr && !gasit;j++)
	{
		_ps_plugin *plugin = (_ps_plugin*)g_list_nth_data (ps_plugins_list, j);
		if (plugin!=NULL && plugin->enabled==1) plugin_uninit (plugin);
		unload_plugin (plugin);
	}
	g_list_free (ps_plugins_list);
	ps_plugins_list = NULL;
}
