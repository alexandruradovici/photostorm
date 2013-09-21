
#include <string.h>
#include <stdlib.h>

#include <panel-applet.h>
#include <gtk/gtk.h>
#include <time.h>

#include "configs.h"
#include "ps_linux.h"
#include "album.h"
#include "misc.h"
#include "photo.h"

static _ps_config *config;
static time_t ps_timestamp = 0;
static time_t ps_last_wallpaper_change = 0;
static time_t ps_last_email_update = 0;
GtkWidget *textul;

static int photostorm_running = false;

static const char photostorm_applet_menu_xml [] =
   "<popup name=\"button3\">\n"
   "   <menuitem name=\"Start PhotoStorm\" "
   "             verb=\"Start_photostorm\" "
   "           _label=\"_Start PhotoStorm\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk_go-forward\"/>\n"
   "   <menuitem name=\"Wallapper Change\" "
   "             verb=\"New_wallpaper\" "
   "           _label=\"_Make New Wallpaper\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-new\"/>\n"
   "   <menuitem name=\"Wallapper Update\" "
   "             verb=\"Update_wallpaper\" "
   "           _label=\"_Update Wallpaper\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-refresh\"/>\n"
   "   <menuitem name=\"Properties Item\" "
   "             verb=\"Configuration\" "
   "           _label=\"_Configuration\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-preferences\"/>\n"
   "   <menuitem name=\"About Item\" "
   "             verb=\"About\" "
   "           _label=\"_About...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gnome-stock-about\"/>\n"
   "</popup>\n";

void * start_cmd (void *data)
{
	photostorm_running = true;
	system ((const char*)data);
	photostorm_running = false;
	g_thread_exit (0);
	return (0);
}

void system__ (const char *cmd)
{
	if (!photostorm_running)
	{
		GThread *firul;
		firul = g_thread_create (start_cmd, (void*)cmd, TRUE, NULL);
	}
}

void ps_applet_load_config ()
{
	config = ps_load_config (ps_get_config_filename());
	_ps_album *date_album = ps_load_album_heather (config_path ("photostorm.cwa"));
	if (date_album!=NULL)
	{
		ps_last_wallpaper_change = date_album->data;
		ps_album_free (date_album);
	} else ps_last_wallpaper_change = 0;
}

static void display_start_photostorm (BonoboUIComponent *uic, void *applet, const char *data) 
{
	system__ ("photostorm");
	ps_applet_load_config ();
}

static void display_new_wallpaper (BonoboUIComponent *uic, void *applet, const char *data) 
{
	system__ ("photostorm --new_wallpaper");
	ps_applet_load_config ();
}

static void display_update_wallpaper (BonoboUIComponent *uic, void *applet, const char *data) 
{
	system__ ("photostorm --update_wallpaper");
	ps_applet_load_config ();
}
   
static void display_configuration_dialog (BonoboUIComponent *uic, void *applet, const char *data) 
{
	//char t[1000];
	//sprintf (t, "%s/photostorm --configure", config->system.libs);
	system__ ("photostorm --configure");
	ps_applet_load_config ();
}

void cb_ps_about_close (GtkWidget *window, GdkEventButton *ev_b, gpointer data)
{
	if (ev_b->button==1) if ((GtkWidget*)data!=NULL) gtk_widget_destroy ((GtkWidget*)data);
}

static void display_about_dialog (BonoboUIComponent *uic, void *applet, const char *data) 
{
	GtkWidget *about_window = gtk_window_new (GTK_WINDOW_POPUP);
	gtk_window_set_decorated ((GtkWindow*)about_window, FALSE);
	gtk_window_set_position ((GtkWindow*)about_window, GTK_WIN_POS_CENTER);
	GtkWidget *pslogo_ev = gtk_event_box_new ();
	GtkWidget *photostorm_logo = gtk_image_new_from_file (pict_path ("logos/splash_screen.jpg"));
	gtk_container_add ((GtkContainer*)pslogo_ev, photostorm_logo);
	gtk_container_add ((GtkContainer*)about_window, pslogo_ev);
	g_signal_connect (pslogo_ev, "button-release-event", (GCallback)cb_ps_about_close, about_window);
	gtk_widget_show_all (about_window);
}
   
static const BonoboUIVerb photostorm_applect_menu_verbs [] = {
		BONOBO_UI_VERB ("Start_photostorm", display_start_photostorm),
		BONOBO_UI_VERB ("New_wallpaper", display_new_wallpaper),
		BONOBO_UI_VERB ("Update_wallpaper", display_update_wallpaper),
        BONOBO_UI_VERB ("Configuration", display_configuration_dialog),
        BONOBO_UI_VERB ("About", display_about_dialog),
        BONOBO_UI_VERB_END
};

gboolean cb_photostorm_server (gpointer data)
{
	if (ps_timestamp == 0) time(&ps_timestamp);
	time_t ts;
	time (&ts);
	
	tm t1;
	localtime_r (&ps_last_wallpaper_change, &t1);
	tm t2;
	localtime_r (&ts, &t2);
	
	int w_new=0;
	int w_update=0;
	
	if (ps_last_wallpaper_change>0)
	{
		switch (config->wallpaper.cint)
		{
			case 0:
				break;
			case 1:
				if (t1.tm_hour != t2.tm_hour) w_new=1;
				break;
			case 2:
				if (t1.tm_wday != t2.tm_wday) w_new=1;
				break;
			case 3:
				if (t1.tm_wday > t2.tm_wday) w_new=1;
				break;
			case 4:
				if (t1.tm_mon != t2.tm_mon) w_new=1;
				break;
			case 5:
				if (t1.tm_year != t2.tm_year) w_new=1;
				break;
		}
		if (!w_new)
		{
			if (config->email.disp && abs (ts-ps_last_wallpaper_change)>(config->email.update?config->email.update:10)*60) w_update=1;
			if (config->calendar.disp && (t1.tm_mon!=t2.tm_mon || t1.tm_mday!=t2.tm_mday || t1.tm_year!=t2.tm_year)) w_update=1;
		}
	}
	if (w_new)
	{
		system__ ("photostorm --new_wallpaper");
		ps_applet_load_config ();
	}
	if (w_update)
	{
		system__ ("photostorm --update_wallpaper");
		ps_applet_load_config ();
	}
	/*if (abs (ts-ps_timestamp) > 10)
	{
		ps_timestamp = ts;
		char t[1000];
		sprintf (t, "%s/photostorm --update_wallpaper", config->system.libs);
		system (t);
	}*/
	return TRUE;
}

static gboolean photostorm_applet_fill (PanelApplet *applet, const gchar *iid, gpointer data)
{
	if (strcmp (iid, "OAFIID:PhotoStorm") != 0)
		return FALSE;
	ps_config_default ();
	ps_applet_load_config ();
	//textul = gtk_label_new (config->system.libs);
	char t[1000];
	sprintf (t, "%s/pics/picts/photostorm.png", config->system.libs);
	GtkWidget *pics = gtk_image_new_from_file (t);
	gtk_container_add (GTK_CONTAINER (applet), pics);

	gtk_widget_show_all (GTK_WIDGET (applet));
	
	panel_applet_setup_menu (PANEL_APPLET (applet),
                         photostorm_applet_menu_xml,
                         photostorm_applect_menu_verbs,
                         applet);
	gtk_timeout_add (1000, (GtkFunction)cb_photostorm_server, NULL);
	//panel_applet_setup_menu ();
	
	/*GtkWidget *label = gtk_label_new ("Test text");
	gtk_container_add ((GtkContainer*)applet, label);
	gtk_widget_show_all (GTK_WIDGET (applet));*/

	return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY ("OAFIID:PhotoStorm_Factory",
                             PANEL_TYPE_APPLET,
                             "PhotoStorm",
                             "0.1",
                             photostorm_applet_fill,
                             NULL);
