/**************************************************************
*		File: main.cpp                                         *
*		ID: PS_main_cpp                                        *
*		Author: Alexandru Radovici                             *
*		Date: February 5, 2004                                 *
*		Modified: February 6, 2004                             *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This is the main source file of PhotoStorm.   *
**************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "data.h"
#include "album.h"
#include "design.h"
#include "configs.h"
#include "ps_system.h"
#include "ps_dialogs.h"
#include "lang/translate.h"
#include "messages.h"
#include "plugins.h"

#ifdef LINUX
#include <signal.h>
#endif

void segfault_handler (int sig)
{
	printf (__("\"Segmentation Fault\"\nYou have just found a bug in PhotoStorm.\nPlease send an email to\n  bugs@photostorm.net\nor\n  photostorm@gmail.com\nto report this bug.\nThank You\n"));
	exit (1);
}

void quit_handler (int sig)
{
	ps_save_system_config ();
	ps_save_plugins_cache ();
	unload_plugins ();
	printf (__("Good Bye."));
	exit (0);
}

void forcequit_handler (int sig)
{
	printf (__("PhotoStorm was forced to quit. Good Bye."));
	exit (0);
}

int main(int argc, char *argv[])
{
    
	#ifdef LINUX
	signal (SIGSEGV, segfault_handler);
	signal (SIGTERM, quit_handler);
	signal (SIGKILL, forcequit_handler);
	#endif
	
    #ifdef WIN32
    freopen (NULL, "w", stderr);
    #endif
	
	#ifdef LINUX
	freopen ("/dev/null", "w", stderr);
	#endif

	char t[1000];
	sprintf (t,"%s/.photostorm", ps_get_user_dir());
	#ifdef WIN32
	mkdir (t);
	#else
	mkdir (t, S_IRUSR | S_IWUSR | S_IXUSR);
	#endif
	
	srand (time(0));

	gtk_init (&argc, &argv);
	g_thread_init (NULL);
	ps_config_default ();
	ps_load_config_default ();
	
	_ps_config *config = ps_get_config ();
	char temp[1000];
	sprintf (temp, "%s%clanguages%c%s", config->system.libs, ps_dir_sep, ps_dir_sep, config->lang.file);
	ps_set_language (temp);
	
	printf (__("Welcome to %s version %s"),ps_name,ps_version);
	printf ("\n");
	
	//printf ("libs: %s\n", config->system.libs);
	
	GtkWidget *main_window = ps_create_design();
	ps_clear_albums_list ();
	ps_load_albums_from_path (config->system.shared_albums);
	if (strcmp (config->system.shared_albums, config->system.albums)!=0) ps_load_albums_from_path (config->system.albums);	
	ps_load_albums_cache ();
	
	load_plugins (config->system.shared_plugins);
	if (strcmp (config->system.shared_plugins, config->system.plugins)!=0) load_plugins (config->system.plugins);
	ps_load_plugins_cache ();
	
	ps_load_messages ();
	
	int start_photostorm = true;
	int update_wallpaper = false;
	int new_wallpaper = false;
	int messages_editor = false;
	int configure = false;
	int start_gtk = true;
	
	for (int i=0;i<argc;i++)
	{
		if (strcmp(argv[i], "--configure")==0)
		{
			start_photostorm = false;
			configure = true;
		}
		else
		if (strcmp(argv[i], "--update_wallpaper")==0)
		{
			start_photostorm = false;
			update_wallpaper = true;
		}
		else
		if (strcmp(argv[i], "--new_wallpaper")==0)
		{
			start_photostorm = false;
			new_wallpaper = true;
		}
		else
		if (strcmp(argv[i], "--messages_editor")==0)
		{
			start_photostorm = false;
			messages_editor = true;
		}
		else
		if (i==1)
		{
			start_photostorm = false;
			start_gtk = false;
			import_album (argv[i]);
		}
	}
	
	if (start_photostorm) 
    {
        gtk_widget_show_all (main_window);
		if (!selecteaza_album (config->system.selected_album)) selecteaza_primul_album ();
    }
	else 
	if (configure) ps_configuration (NULL);
	else 
	if (update_wallpaper) start_gtk = ps_wallpaper_update (true);
	else
	if (new_wallpaper) start_gtk = ps_wallpaper_new (true);
	else
	if (messages_editor) 
	{
		gtk_widget_show_all (ps_messages_edit_dialog (NULL));
	}
	//ps_configuration (NULL);
	//ps_wallpaper_new (true);
	if (start_gtk) gtk_main();
	ps_save_system_config ();
	ps_save_plugins_cache ();
	unload_plugins ();
	return 0;
}
