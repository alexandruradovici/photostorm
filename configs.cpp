
#include "data.h"

#include "configs.h"
#include "misc.h"

#ifndef PHOTOSTORM_WEBSITE

#include "ps_system.h"
#include "plugins.h"

#endif

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef PHOTOSTORM_PROGRAM

#include "ps_dialogs.h"
#include "messages.h"

#ifndef PHOTOSTORM_WEBSITE
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "ps_widgets.h"
#include "design.h"
#include "lang/translate.h"

#endif

#include "photo.h"

#endif

static _ps_config config;

#ifdef PHOTOSTORM_PROGRAM

static GtkWidget *config_window;
static GtkWidget *pagina_plugin;

#ifndef PHOTOSTORM_WEBSITE

_ps_plugin * ps_plugins_list_get_selected_plugin (GtkWidget *window);
const char * ps_lang_list_get_language (GtkWidget *window);

void cb_ps_show_quotes_lib (GtkWidget *buton, gpointer data)
{
	gtk_widget_show_all (ps_create_quotes_lib_dialog ((GtkWindow*)config_window));
}

void cb_ps_edit_messages (GtkWidget *buton, gpointer data)
{
	gtk_widget_show_all (ps_messages_edit_dialog ((GtkWindow*)config_window));
}

void cb_ps_config_okey (GtkWidget *buton, gpointer data)
{
	GtkWidget *window = (GtkWidget*)data;
	if (window!=NULL)
	{
		// wallpaper
		GtkWidget *wallpaper = (GtkWidget*)ps_get_widget_from (window, "wallpaper");
		if (wallpaper!=NULL)
		{
			config.wallpaper.cint = ps_option_menu_get_index ((GtkWidget*)ps_get_widget_from (wallpaper, "interval"));
			config.wallpaper.dtype = ps_option_menu_get_index ((GtkWidget*)ps_get_widget_from (wallpaper, "display_type"));
			//config.wallpaper.gray = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (wallpaper, "bw"));
			config.wallpaper.dinfo = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (wallpaper, "display_info"));
			strcpy (config.wallpaper.format, ps_edit_get_text ((GtkWidget*)ps_get_widget_from (wallpaper, "info_format")));
			config.wallpaper.border = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (wallpaper, "draw_info_border"));
			config.wallpaper.pos.a = ps_position_get_a ((GtkWidget*)ps_get_widget_from (wallpaper, "info_position"));
			config.wallpaper.pos.x = ps_position_get_x ((GtkWidget*)ps_get_widget_from (wallpaper, "info_position"));
			config.wallpaper.pos.y = ps_position_get_y ((GtkWidget*)ps_get_widget_from (wallpaper, "info_position"));
			_ps_color *col = ps_color_select_get_color ((GtkWidget*)ps_get_widget_from (wallpaper, "info_color"));
			if (col!=NULL)
			{
				config.wallpaper.fcolor.red = col->red;
				config.wallpaper.fcolor.green = col->green;
				config.wallpaper.fcolor.blue = col->blue;
				config.wallpaper.fcolor.alpha = col->alpha;
				free (col);
			}
			config.wallpaper.fsize = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (wallpaper, "info_text"));
		}
		// calendar
		GtkWidget *calendar = (GtkWidget*)ps_get_widget_from (window, "calendar");
		if (calendar!=NULL)
		{
			config.calendar.disp = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (calendar, "display_calendar"));
			config.calendar.border = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (calendar, "draw_calendar_border"));
			config.calendar.pos.a = ps_position_get_a ((GtkWidget*)ps_get_widget_from (calendar, "calendar_position"));
			config.calendar.pos.x = ps_position_get_x ((GtkWidget*)ps_get_widget_from (calendar, "calendar_position"));
			config.calendar.pos.y = ps_position_get_y ((GtkWidget*)ps_get_widget_from (calendar, "calendar_position"));
			_ps_color *col = ps_color_select_get_color ((GtkWidget*)ps_get_widget_from (calendar, "marking_color"));
			if (col!=NULL)
			{
				config.calendar.mcolor.red = col->red;
				config.calendar.mcolor.green = col->green;
				config.calendar.mcolor.blue = col->blue;
				config.calendar.mcolor.alpha = col->alpha;
				free (col);
			}
			col = ps_color_select_get_color ((GtkWidget*)ps_get_widget_from (calendar, "calendar_color"));
			if (col!=NULL)
			{
				config.calendar.fcolor.red = col->red;
				config.calendar.fcolor.green = col->green;
				config.calendar.fcolor.blue = col->blue;
				config.calendar.fcolor.alpha = col->alpha;
				free (col);
			}
			config.calendar.cmfsize = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (calendar, "big_text"));
			config.calendar.omfsize = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (calendar, "small_text"));
			config.calendar.mark_date = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (calendar, "mark_date"));
			config.calendar.p_month = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (calendar, "prev_number"));
			config.calendar.n_month = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (calendar, "next_number"));
		}
		// quotes
		GtkWidget *quotes = (GtkWidget*)ps_get_widget_from (window, "quotes");
		if (quotes!=NULL)
		{
			config.quotes.disp = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (quotes, "display_quotes"));
			config.quotes.border = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (quotes, "draw_quotes_border"));
			config.quotes.pos.a = ps_position_get_a ((GtkWidget*)ps_get_widget_from (quotes, "quotes_position"));
			config.quotes.pos.x = ps_position_get_x ((GtkWidget*)ps_get_widget_from (quotes, "quotes_position"));
			config.quotes.pos.y = ps_position_get_y ((GtkWidget*)ps_get_widget_from (quotes, "quotes_position"));
			_ps_color *col = ps_color_select_get_color ((GtkWidget*)ps_get_widget_from (quotes, "quotes_color"));
			if (col!=NULL)
			{
				config.quotes.fcolor.red = col->red;
				config.quotes.fcolor.green = col->green;
				config.quotes.fcolor.blue = col->blue;
				config.quotes.fcolor.alpha = col->alpha;
				free (col);
			}
			config.quotes.fsize = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (quotes, "quotes_text"));
			config.quotes.inet = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (quotes, "get_quotes_online"));
			strcpy (config.quotes.server, ps_server_data_get_server ((GtkWidget*)ps_get_widget_from (quotes, "quotes_server")));
			config.quotes.port = ps_server_data_get_port ((GtkWidget*)ps_get_widget_from (quotes, "quotes_server"));
			strcpy (config.quotes.keywords, ps_edit_get_text ((GtkWidget*)ps_get_widget_from (quotes, "qkeywords")));
			config.quotes.only_keywords = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (quotes, "only_keywords"));
		}
		// messages
		GtkWidget *messages = (GtkWidget*)ps_get_widget_from (window, "messages");
		if (messages!=NULL)
		{	
			config.messages.disp = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (messages, "display_messages"));
			config.messages.border = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (messages, "draw_messages_border"));
			config.messages.pos.a = ps_position_get_a ((GtkWidget*)ps_get_widget_from (messages, "messages_position"));
			config.messages.pos.x = ps_position_get_x ((GtkWidget*)ps_get_widget_from (messages, "messages_position"));
			config.messages.pos.y = ps_position_get_y ((GtkWidget*)ps_get_widget_from (messages, "messages_position"));
			_ps_color *col = ps_color_select_get_color ((GtkWidget*)ps_get_widget_from (messages, "time_color"));
			if (col!=NULL)
			{
				config.messages.tcolor.red = col->red;
				config.messages.tcolor.green = col->green;
				config.messages.tcolor.blue = col->blue;
				config.messages.tcolor.alpha = col->alpha;
				free (col);
			}
			col = ps_color_select_get_color ((GtkWidget*)ps_get_widget_from (messages, "message_color"));
			if (col!=NULL)
			{
				config.messages.mcolor.red = col->red;
				config.messages.mcolor.green = col->green;
				config.messages.mcolor.blue = col->blue;
				config.messages.mcolor.alpha = col->alpha;
				free (col);
			}
			config.messages.fsize = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (messages, "messages_text"));
		}
		// email
		GtkWidget *email = (GtkWidget*)ps_get_widget_from (window, "email");
		if (email!=NULL)
		{	
			config.email.disp = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (email, "display_email"));
			config.email.border = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (email, "draw_email_border"));
			config.email.pos.a = ps_position_get_a ((GtkWidget*)ps_get_widget_from (email, "email_position"));
			config.email.pos.x = ps_position_get_x ((GtkWidget*)ps_get_widget_from (email, "email_position"));
			config.email.pos.y = ps_position_get_y ((GtkWidget*)ps_get_widget_from (email, "email_position"));
			config.email.max_nr = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (email, "number_emails"));
			_ps_color *col = ps_color_select_get_color ((GtkWidget*)ps_get_widget_from (email, "sender_color"));
			if (col!=NULL)
			{
				config.email.fcolor.red = col->red;
				config.email.fcolor.green = col->green;
				config.email.fcolor.blue = col->blue;
				config.email.fcolor.alpha = col->alpha;
				free (col);
			}
			col = ps_color_select_get_color ((GtkWidget*)ps_get_widget_from (email, "subject_color"));
			if (col!=NULL)
			{
				config.email.scolor.red = col->red;
				config.email.scolor.green = col->green;
				config.email.scolor.blue = col->blue;
				config.email.scolor.alpha = col->alpha;
				free (col);
			}
			config.email.fsize = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (email, "emails_text"));
			strcpy (config.email.server, ps_server_data_get_server ((GtkWidget*)ps_get_widget_from (email, "email_server")));
			config.email.port = ps_server_data_get_port ((GtkWidget*)ps_get_widget_from (email, "email_server"));
			strcpy (config.email.username, ps_server_data_get_username ((GtkWidget*)ps_get_widget_from (email, "email_server")));
			strcpy (config.email.password, ps_server_data_get_password ((GtkWidget*)ps_get_widget_from (email, "email_server")));
			config.email.update = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (email, "update_time"));
		}
		// language
		GtkWidget *language = (GtkWidget*)ps_get_widget_from (window, "language");
		if (language!=NULL)
		{
			config.lang.enabled = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (language, "enabled"));
			strcpy (config.lang.file, ps_lang_list_get_language (language));
		}
		// system
		GtkWidget *system = (GtkWidget*)ps_get_widget_from (window, "system");
		if (system!=NULL)
		{
			strcpy (config.system.shared_albums, ps_location_edit_get_location ((GtkWidget*)ps_get_widget_from (system, "shared_albums_loc")));
			strcpy (config.system.albums, ps_location_edit_get_location ((GtkWidget*)ps_get_widget_from (system, "albums_loc")));
			#ifndef WIN32
			strcpy (config.system.libs, ps_location_edit_get_location ((GtkWidget*)ps_get_widget_from (system, "libs_loc")));
			#endif
			strcpy (config.system.shared_plugins, ps_location_edit_get_location ((GtkWidget*)ps_get_widget_from (system, "shared_plugins_loc")));
			strcpy (config.system.plugins, ps_location_edit_get_location ((GtkWidget*)ps_get_widget_from (system, "plugins_loc")));
			config.system.scr_size_autodetect = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (system, "scr_autodetect"));
			config.system.scr_width = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (system, "scr_w"));
			config.system.scr_height = (int)ps_number_edit_get_number ((GtkWidget*)ps_get_widget_from (system, "scr_h"));
		}
		ps_save_config (ps_get_config_filename(), config);
		if ((GtkWindow*)gtk_window_get_transient_for ((GtkWindow*)window)!=NULL)
		{
			ps_wallpaper_update (false);
			gtk_widget_destroy (window);
		}
		else 
		{
			gtk_widget_hide_all (window);
			ps_wallpaper_update (true);
		}
	}
}

void cb_ps_config_close (GtkWidget *buton, gpointer data)
{
	GtkWidget *window = (GtkWidget*)data;
	if ((GtkWindow*)gtk_window_get_transient_for ((GtkWindow*)window)==NULL) gtk_main_quit ();
	if (window!=NULL) gtk_widget_destroy (window);
}

GtkWidget * ps_config_wallpaper ()
{
	GtkWidget *pagina = gtk_vbox_new (FALSE, 2);
	GtkWidget *upd_f = gtk_frame_new (__("Wallpaper Update"));
	ps_hookup_object (pagina, upd_f, "upd_f");
	gtk_container_set_border_width ((GtkContainer*)upd_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, upd_f, FALSE, FALSE, 2);
	GtkWidget *vbox_upd = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_upd, "vbox_upd");
	gtk_container_add ((GtkContainer*)upd_f, vbox_upd);
	const char *intervals[6] = {__("Never"), __("Hour"), __("Day"), __("Week"), __("Month"), __("Year")};
	GtkWidget *interval = ps_create_option_menu ("Change wallpaper every", intervals, 6, 1);
	ps_hookup_object (pagina, interval, "interval");
	ps_option_menu_set_index (interval, config.wallpaper.cint);
	gtk_box_pack_start ((GtkBox*)vbox_upd, interval, FALSE, FALSE, 2);
	const char *display_types[6] = {__("Proportional Resize"), __("Shrink Only"), __("Enlarge Only"), __("Stretch"), __("Center"), __("Tile")};
	GtkWidget *display_type = ps_create_option_menu ("Wallpaper display type", display_types, 6, 1);
	ps_hookup_object (pagina, display_type, "display_type");
	ps_option_menu_set_index (display_type, config.wallpaper.dtype);
	gtk_box_pack_start ((GtkBox*)vbox_upd, display_type, FALSE, FALSE, 2);
	/*GtkWidget *bw = ps_create_check_button_with_image_from_stock ("", __("Display wallpaper in grayscale"), 1);
	ps_hookup_object (pagina, bw, "bw");
	ps_check_button_set_value (bw, config.wallpaper.gray);
	gtk_box_pack_start ((GtkBox*)vbox_upd, bw, FALSE, FALSE, 2);*/
	
	GtkWidget *info_f = gtk_frame_new (__("Wallpaper Information"));
	ps_hookup_object (pagina, info_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)info_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, info_f, FALSE, FALSE, 2);
	GtkWidget *vbox_info = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_info, "vbox_info");
	gtk_container_add ((GtkContainer*)info_f, vbox_info);
	GtkWidget *display_info = ps_create_check_button_with_image_from_stock ("", __("Display photo's information"), 1);
	ps_hookup_object (pagina, display_info, "display_info");
	ps_check_button_set_value (display_info, config.wallpaper.dinfo);
	gtk_box_pack_start ((GtkBox*)vbox_info, display_info, FALSE, FALSE, 2);
	GtkWidget *info_format = ps_create_edit (__("Photo information format"), 100, 30, 1, '\0');
	ps_hookup_object (pagina, info_format, "info_format");
	ps_edit_set_text (info_format, config.wallpaper.format);
	gtk_box_pack_start ((GtkBox*)vbox_info, info_format, FALSE, FALSE, 2);
	GtkWidget *info_format_help = gtk_label_new (__("(\"%n\"-name, \"%a\"-author, \"\\n\"-new line)"));
	gtk_misc_set_alignment ((GtkMisc*)info_format_help, 0.0, 0.0);
	ps_hookup_object (pagina, info_format_help, "info_format_help");
	gtk_box_pack_start ((GtkBox*)vbox_info, info_format_help, FALSE, FALSE, 0);
	GtkWidget *draw_info_border = ps_create_check_button_with_image_from_stock ("", __("Draw a border around the photo's information"), 1);
	ps_hookup_object (pagina, draw_info_border, "draw_info_border");
	ps_check_button_set_value (draw_info_border, config.wallpaper.border);
	gtk_box_pack_start ((GtkBox*)vbox_info, draw_info_border, FALSE, FALSE, 2);
	GtkWidget *info_position = ps_create_position_widget ("Display information at", 800, 30);
	ps_hookup_object (pagina, info_position, "info_position");
	ps_position_set_values (info_position, config.wallpaper.pos.a, config.wallpaper.pos.x, config.wallpaper.pos.y); 
	gtk_box_pack_start ((GtkBox*)vbox_info, info_position, FALSE, FALSE, 2);
	//_ps_color *color = ps_pack_color (255, 255, 255, 0);
	//printf ("config:fcolor:alpha: %d\n", config.wallpaper.fcolor.alpha);
	GtkWidget *info_color = ps_create_color_select ("Information color", &config.wallpaper.fcolor);
	ps_hookup_object (pagina, info_color, "info_color");
	//ps_color_select_set_color (info_color, &config.wallpaper.fcolor);
	gtk_box_pack_start ((GtkBox*)vbox_info, info_color, FALSE, FALSE, 2);
	GtkWidget *info_text = ps_create_number_edit ("Information text size", 0, 100, 1, 1);
	ps_hookup_object (pagina, info_text, "info_text");
	ps_number_edit_set_number (info_text, config.wallpaper.fsize);
	gtk_box_pack_start ((GtkBox*)vbox_info, info_text, FALSE, FALSE, 2);
	return pagina;
}

GtkWidget * ps_config_calendar ()
{
	GtkWidget *pagina = gtk_vbox_new (FALSE, 2);
	GtkWidget *info_f = gtk_frame_new (__("Calendar Display"));
	ps_hookup_object (pagina, info_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)info_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, info_f, FALSE, FALSE, 2);
	GtkWidget *vbox_info = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_info, "vbox_info");
	gtk_container_add ((GtkContainer*)info_f, vbox_info);
	GtkWidget *display_calendar = ps_create_check_button_with_image_from_stock ("", __("Display calendar"), 1);
	ps_hookup_object (pagina, display_calendar, "display_calendar");
	ps_check_button_set_value (display_calendar, config.calendar.disp);
	gtk_box_pack_start ((GtkBox*)vbox_info, display_calendar, FALSE, FALSE, 2);
	GtkWidget *draw_calendar_border = ps_create_check_button_with_image_from_stock ("", __("Draw a border around the calendar"), 1);
	ps_hookup_object (pagina, draw_calendar_border, "draw_calendar_border");
	ps_check_button_set_value (draw_calendar_border, config.calendar.border);
	gtk_box_pack_start ((GtkBox*)vbox_info, draw_calendar_border, FALSE, FALSE, 2);
	GtkWidget *calendar_position = ps_create_position_widget ("Display calendar at", 400, 20);
	ps_hookup_object (pagina, calendar_position, "calendar_position");
	ps_position_set_values (calendar_position, config.calendar.pos.a, config.calendar.pos.x, config.calendar.pos.y); 
	gtk_box_pack_start ((GtkBox*)vbox_info, calendar_position, FALSE, FALSE, 2);
	//_ps_color *col_mark = ps_pack_color (255, 255, 0, 30);
	GtkWidget *marking_color = ps_create_color_select ("Marking color", &config.calendar.mcolor);
	ps_hookup_object (pagina, marking_color, "marking_color");
	gtk_box_pack_start ((GtkBox*)vbox_info, marking_color, FALSE, FALSE, 2);
	//_ps_color *color = ps_pack_color (255, 255, 255, 30);
	GtkWidget *calendar_color = ps_create_color_select ("Calendar color", &config.calendar.fcolor);
	ps_hookup_object (pagina, calendar_color, "calendar_color");
	gtk_box_pack_start ((GtkBox*)vbox_info, calendar_color, FALSE, FALSE, 2);
	/*GtkWidget *hbox_textsize = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (pagina, hbox_textsize, "hbox_textsize");
	gtk_box_pack_start ((GtkBox*)vbox_info, hbox_textsize, FALSE, FALSE, 2);*/
	GtkWidget *big_text = ps_create_number_edit ("Current month size", 0, 100, 1, 1);
	ps_hookup_object (pagina, big_text, "big_text");
	ps_number_edit_set_number (big_text, config.calendar.cmfsize);
	gtk_box_pack_start ((GtkBox*)vbox_info, big_text, FALSE, FALSE, 2);
	GtkWidget *small_text = ps_create_number_edit ("Other months text size", 0, 100, 1, 1);
	ps_hookup_object (pagina, small_text, "small_text");
	ps_number_edit_set_number (small_text, config.calendar.omfsize);
	gtk_box_pack_start ((GtkBox*)vbox_info, small_text, FALSE, FALSE, 2);
	
	GtkWidget *data_f = gtk_frame_new (__("Calendar Format"));
	ps_hookup_object (pagina, data_f, "data_f");
	gtk_container_set_border_width ((GtkContainer*)data_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, data_f, FALSE, FALSE, 2);
	GtkWidget *vbox_data = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_data, "vbox_data");
	gtk_container_add ((GtkContainer*)data_f, vbox_data);
	GtkWidget *mark_date = ps_create_check_button_with_image_from_stock ("gtk-ok", __("Mark current date"), 1);
	ps_hookup_object (pagina, mark_date, "mark_date");
	ps_check_button_set_value (mark_date, config.calendar.mark_date);
	gtk_box_pack_start ((GtkBox*)vbox_data, mark_date, FALSE, FALSE, 2);
	GtkWidget *prev_number = ps_create_number_edit ("Number of past months", 0, 6, 1, 1);
	ps_hookup_object (pagina, prev_number, "prev_number");
	ps_number_edit_set_number (prev_number, config.calendar.p_month);
	gtk_box_pack_start ((GtkBox*)vbox_data, prev_number, FALSE, FALSE, 2);
	GtkWidget *next_number = ps_create_number_edit ("Number of future months", 0, 6, 1, 1);
	ps_hookup_object (pagina, next_number, "next_number");
	ps_number_edit_set_number (next_number, config.calendar.n_month); 
	gtk_box_pack_start ((GtkBox*)vbox_data, next_number, FALSE, FALSE, 2);
	/*GtkWidget *quotes_server = ps_server_data ("Quotes server", "www.photostorm.net", "18200", NULL, NULL);
	ps_hookup_object (pagina, quotes_server, "quotes_server");
	gtk_box_pack_start ((GtkBox*)vbox_qdata, quotes_server, FALSE, FALSE, 2);
	GtkWidget *qkeywords = ps_create_edit ("Use keywords (blank for none)", 1000, -1, 1, '\0');
	ps_hookup_object (pagina, qkeywords, "qkeywords");
	gtk_box_pack_start ((GtkBox*)vbox_qdata, qkeywords, FALSE, FALSE, 2);*/
	return pagina;
}

GtkWidget * ps_config_quotes ()
{
	GtkWidget *pagina = gtk_vbox_new (FALSE, 2);
	GtkWidget *info_f = gtk_frame_new (__("Quotes Display"));
	ps_hookup_object (pagina, info_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)info_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, info_f, FALSE, FALSE, 2);
	GtkWidget *vbox_info = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_info, "vbox_info");
	gtk_container_add ((GtkContainer*)info_f, vbox_info);
	GtkWidget *display_quotes = ps_create_check_button_with_image_from_stock ("", __("Display quotes"), 1);
	ps_hookup_object (pagina, display_quotes, "display_quotes");
	ps_check_button_set_value (display_quotes, config.quotes.disp);
	gtk_box_pack_start ((GtkBox*)vbox_info, display_quotes, FALSE, FALSE, 2);
	GtkWidget *draw_quotes_border = ps_create_check_button_with_image_from_stock ("", __("Draw a border around the quote"), 1);
	ps_hookup_object (pagina, draw_quotes_border, "draw_quotes_border");
	ps_check_button_set_value (draw_quotes_border, config.quotes.border);
	gtk_box_pack_start ((GtkBox*)vbox_info, draw_quotes_border, FALSE, FALSE, 2);
	GtkWidget *quotes_position = ps_create_position_widget ("Display quotes at", 600, 500);
	ps_hookup_object (pagina, quotes_position, "quotes_position");
	ps_position_set_values (quotes_position, config.quotes.pos.a, config.quotes.pos.x, config.quotes.pos.y);
	gtk_box_pack_start ((GtkBox*)vbox_info, quotes_position, FALSE, FALSE, 2);
	//_ps_color *color = ps_pack_color (255, 255, 255, 0);
	GtkWidget *quotes_color = ps_create_color_select ("Text color", &config.quotes.fcolor);
	ps_hookup_object (pagina, quotes_color, "quotes_color");
	gtk_box_pack_start ((GtkBox*)vbox_info, quotes_color, FALSE, FALSE, 2);
	GtkWidget *quotes_text = ps_create_number_edit ("Text size", 0, 100, 1, 1);
	ps_hookup_object (pagina, quotes_text, "quotes_text");
	ps_number_edit_set_number (quotes_text, config.quotes.fsize);
	gtk_box_pack_start ((GtkBox*)vbox_info, quotes_text, FALSE, FALSE, 2);
	
	GtkWidget *qdata_f = gtk_frame_new (__("Quotes Selection"));
	ps_hookup_object (pagina, qdata_f, "qdata_f");
	gtk_container_set_border_width ((GtkContainer*)qdata_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, qdata_f, FALSE, FALSE, 2);
	GtkWidget *vbox_qdata = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_qdata, "vbox_qdata");
	gtk_container_add ((GtkContainer*)qdata_f, vbox_qdata);
	GtkWidget *get_quotes_online = ps_create_check_button_with_image (pict_path ("picts/website24.png"), __("Try to get quotes from a quotes server"), 1);
	ps_hookup_object (pagina, get_quotes_online, "get_quotes_online");
	ps_check_button_set_value (get_quotes_online, config.quotes.inet);
	gtk_box_pack_start ((GtkBox*)vbox_qdata, get_quotes_online, FALSE, FALSE, 2);
	GtkWidget *quotes_server = ps_server_data ("Quotes server", "www.photostorm.net", 18200, NULL, NULL);
	ps_hookup_object (pagina, quotes_server, "quotes_server");
	ps_server_data_set_server (quotes_server, config.quotes.server);
	ps_server_data_set_port (quotes_server, config.quotes.port);
	gtk_box_pack_start ((GtkBox*)vbox_qdata, quotes_server, FALSE, FALSE, 2);
	GtkWidget *qkeywords = ps_create_edit ("Use keywords (blank for none)", 1000, -1, 1, '\0');
	ps_hookup_object (pagina, qkeywords, "qkeywords");
	ps_edit_set_text (qkeywords, config.quotes.keywords);
	gtk_box_pack_start ((GtkBox*)vbox_qdata, qkeywords, FALSE, FALSE, 2);
	GtkWidget *only_keywords = ps_create_check_button_with_image_from_stock ("gtk-dialog-warning", __("Use only quotes that match the keywords"), 1);
	ps_hookup_object (pagina, only_keywords, "only_keywords");	
	ps_check_button_set_value (only_keywords, config.quotes.only_keywords);
	gtk_box_pack_start ((GtkBox*)vbox_qdata, only_keywords, FALSE, FALSE, 2);
	GtkWidget *quotes_lib_button = gtk_button_new_with_label (__("View Quotes Library"));
	ps_hookup_object (pagina, quotes_lib_button, "quotes_lib_button");
	gtk_box_pack_start ((GtkBox*)vbox_qdata, quotes_lib_button, FALSE, FALSE, 2);
	g_signal_connect (quotes_lib_button, "clicked", (GCallback)cb_ps_show_quotes_lib, NULL);
	return pagina;
}

GtkWidget * ps_config_messages ()
{
	GtkWidget *pagina = gtk_vbox_new (FALSE, 2);
	GtkWidget *info_f = gtk_frame_new (__("Messages Display"));
	ps_hookup_object (pagina, info_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)info_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, info_f, FALSE, FALSE, 2);
	GtkWidget *vbox_info = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_info, "vbox_info");
	gtk_container_add ((GtkContainer*)info_f, vbox_info);
	GtkWidget *display_messages = ps_create_check_button_with_image_from_stock ("", __("Display messages"), 1);
	ps_hookup_object (pagina, display_messages, "display_messages");
	ps_check_button_set_value (display_messages, config.messages.disp);
	gtk_box_pack_start ((GtkBox*)vbox_info, display_messages, FALSE, FALSE, 2);
	GtkWidget *draw_messages_border = ps_create_check_button_with_image_from_stock ("", __("Draw a border around the messages"), 1);
	ps_hookup_object (pagina, draw_messages_border, "draw_messages_border");
	ps_check_button_set_value (draw_messages_border, config.messages.border);
	gtk_box_pack_start ((GtkBox*)vbox_info, draw_messages_border, FALSE, FALSE, 2);
	GtkWidget *messages_position = ps_create_position_widget ("Display messages at", 100, 400);
	ps_hookup_object (pagina, messages_position, "messages_position");
	ps_position_set_values (messages_position, config.messages.pos.a, config.messages.pos.x, config.messages.pos.y);
	gtk_box_pack_start ((GtkBox*)vbox_info, messages_position, FALSE, FALSE, 2);
	GtkWidget *time_color = ps_create_color_select ("Time color", &config.messages.tcolor);
	ps_hookup_object (pagina, time_color, "time_color");
	gtk_box_pack_start ((GtkBox*)vbox_info, time_color, FALSE, FALSE, 2);
	GtkWidget *message_color = ps_create_color_select ("Message color", &config.messages.mcolor);
	ps_hookup_object (pagina, message_color, "message_color");
	gtk_box_pack_start ((GtkBox*)vbox_info, message_color, FALSE, FALSE, 2);
	GtkWidget *messages_text = ps_create_number_edit ("Text size", 0, 100, 1, 1);
	ps_hookup_object (pagina, messages_text, "messages_text");
	ps_number_edit_set_number (messages_text, config.messages.fsize);
	gtk_box_pack_start ((GtkBox*)vbox_info, messages_text, FALSE, FALSE, 2);
	/*
	GtkWidget *messagesdata_f = gtk_frame_new (__("Messages Editor"));
	ps_hookup_object (pagina, messagesdata_f, "messagesdata_f");
	gtk_container_set_border_width ((GtkContainer*)messagesdata_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, messagesdata_f, FALSE, FALSE, 2);
	GtkWidget *vbox_messagesdata = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_messagesdata, "vbox_messagesdata");
	gtk_container_add ((GtkContainer*)messagesdata_f, vbox_messagesdata);
	GtkWidget *hbox_messagesdata = gtk_hbox_new (FALSE, 2);
	gtk_box_pack_start ((GtkBox*)vbox_messagesdata, hbox_messagesdata, FALSE, FALSE, 2);
	ps_hookup_object (pagina, hbox_messagesdata, "hbox_messages_data");
	GtkWidget *calendar = gtk_calendar_new ();
	gtk_box_pack_start ((GtkBox*)hbox_messagesdata, calendar, FALSE, FALSE, 2);
	
	GtkWidget * hbox_message = gtk_hbox_new (FALSE, 0);
	ps_hookup_object (pagina, hbox_message, "hbox_message");
	gtk_box_pack_start ((GtkBox*)vbox_messagesdata, hbox_message, FALSE, FALSE, 2);
	GtkWidget *message_time_edit = ps_create_time_edit (__("Time"), 0, 0);
	ps_hookup_object (pagina, message_time_edit, "message_time_edit");
	gtk_box_pack_start ((GtkBox*)hbox_message, message_time_edit, FALSE, FALSE, 2);
	GtkWidget *message_text = ps_create_edit (__("Message"), 250, 30, 1, 0);
	ps_hookup_object (pagina, message_text, "message_text");
	gtk_box_pack_start ((GtkBox*)hbox_message, message_text, FALSE, FALSE, 2);
	*/
	GtkWidget * b_add_message = ps_create_button_with_image_from_stock ("gtk-index", __("Edit Messages"), 1); 
	ps_hookup_object (pagina, b_add_message, "b_add_message");
	gtk_box_pack_start ((GtkBox*)pagina, b_add_message, FALSE, FALSE, 2);
	g_signal_connect (b_add_message, "clicked", (GCallback)cb_ps_edit_messages, NULL);
	
	return pagina;
}

GtkTreeModel * ps_model_lista_plugins ()
{
	GtkTreeStore *store = gtk_tree_store_new (ps_c_plugin_numar, G_TYPE_STRING, G_TYPE_BOOLEAN, 
											  G_TYPE_STRING, G_TYPE_STRING);
	return (GtkTreeModel*) store;
}

void cb_ps_plugins_list_check (GtkCellRendererToggle *cell, gchar *spath, gpointer data)
{
	//printf ("Toggled\n");
	GtkTreeModel *model = (GtkTreeModel*)data;
	GtkTreeIter pos;
	GtkTreePath *path = gtk_tree_path_new_from_string (spath);
	gtk_tree_model_get_iter (model, &pos, path);
	gboolean check_box;
	char* filename;
	gtk_tree_model_get (model, &pos, ps_c_plugin_enabled, &check_box, -1);
	gtk_tree_model_get (model, &pos, ps_c_plugin_file, &filename, -1);
	//printf ("check_box: %d\n", (check_box)?1:0);
	_ps_plugin *date_plugin = NULL;
	if (filename!=NULL)
	{
		GList *ps_plugins_list = get_ps_plugins_list ();
		int nr = g_list_length (ps_plugins_list);
		int gasit = 0;
		for (int j=0;j<nr && !gasit;j++)
		{
			_ps_plugin *plugin = (_ps_plugin*)g_list_nth_data (ps_plugins_list, j);
			if (plugin!=NULL && plugin->filename!=NULL)
			{
				if (strcmp (plugin->filename, filename)==0)
				{
					date_plugin = plugin;
					gasit = 1;
				}
			}
		}
	}
	if (date_plugin!=NULL)
	{
		if (check_box == FALSE) 
		{
			if (plugin_init (date_plugin) == 0) check_box = TRUE;
		}
		else
		if (check_box == TRUE) 
		{
			if (plugin_uninit (date_plugin) == 0) check_box = FALSE;
		}
		date_plugin->enabled = (check_box==TRUE)?1:0;
		GtkWidget *l_status_data = (GtkWidget*)ps_get_widget_from (pagina_plugin, "l_status_data");
		if (date_plugin->enabled==1) gtk_label_set_text ((GtkLabel*)l_status_data, __("Runs"));
			else gtk_label_set_text ((GtkLabel*)l_status_data, __("Stopped"));
	}
	//else check_box = FALSE;
	gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_plugin_enabled, check_box, 
												    //ps_c_albume_culoare_font, s,
													-1);
	gtk_tree_path_free (path);
}

GtkWidget * ps_create_config_plugins_list ()
{
	GtkWidget * lista_plugins = gtk_tree_view_new_with_model (ps_model_lista_plugins());
	GtkCellRenderer * render;
	GtkTreeViewColumn * col;
	
	// Enabled
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Use"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_plugin_enabled);
	gtk_tree_view_column_set_resizable (col, TRUE);
    render = gtk_cell_renderer_toggle_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_signal_connect (render, "toggled", (GCallback)cb_ps_plugins_list_check, gtk_tree_view_get_model ((GtkTreeView*)lista_plugins));
	g_object_set ((GObject*)render, "xalign", 0.5, "yalign", 0.5, NULL);
	gtk_tree_view_column_set_attributes (col, render, "active", ps_c_plugin_enabled, NULL);
    gtk_tree_view_append_column ((GtkTreeView*)lista_plugins, col);
	
	// Name
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Name"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_plugin_nume);
	gtk_tree_view_column_set_resizable (col, TRUE);
    render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 0.5, "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_plugin_nume, NULL);
    gtk_tree_view_append_column ((GtkTreeView*)lista_plugins, col);
	
	// Author
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Author"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_plugin_autor);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 1.0, "style", PANGO_STYLE_ITALIC, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_plugin_autor, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_plugins, col);
	
	// File
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("File"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_plugin_file);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 1.0, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_plugin_file, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_plugins, col);
	
	gtk_widget_set_size_request (lista_plugins, 100, 200);
	gtk_tree_view_set_headers_visible ((GtkTreeView*)lista_plugins, TRUE);
	gtk_tree_view_set_rules_hint ((GtkTreeView*)lista_plugins, TRUE);
	
	/*
    sel = gtk_tree_view_get_selection ((GtkTreeView*)lista_poze);
	g_signal_connect (lista_poze, "row-activated", (GCallback)cb_ps_dublu_click_picture, NULL);
	gtk_tree_selection_set_mode (sel, GTK_SELECTION_SINGLE);
	g_signal_connect (sel, "changed", (GCallback)cb_ps_selected_picture, NULL);
	
	g_signal_connect (lista_poze, "button-release-event", (GCallback)cb_ps_meniu_lista_poze, NULL);
	*/
	
	return lista_plugins;
}

void adauga_plugin (GtkWidget *window, _ps_plugin *date_plugin)
{
    if (date_plugin==NULL) return;
	GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget_from (window, "plugins_list"));
	GtkTreeIter pos;
	
	gtk_tree_store_append ((GtkTreeStore*)model,&pos,NULL);
	gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_plugin_file, date_plugin->filename,
                        ps_c_plugin_nume, date_plugin->name, 
						ps_c_plugin_autor, date_plugin->author,
						ps_c_plugin_enabled, date_plugin->enabled,
						-1);
}

_ps_plugin * ps_plugins_list_get_selected_plugin (GtkWidget *window)
{
    if (window==NULL) return NULL;
    char *file = NULL;
	
	_ps_plugin * date_plugin = NULL;
        
    GtkTreeSelection * sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget_from (window, "plugins_list"));
	if (sel!=NULL)
	{
		GtkTreeModel * model;
		GtkTreeIter pos;
		if (gtk_tree_selection_get_selected (sel, &model, &pos))
		{
		    gtk_tree_model_get (model, &pos, ps_c_plugin_file, &file, -1);
		}
	}
	if (file!=NULL)
	{
		GList *ps_plugins_list = get_ps_plugins_list ();
		int nr = g_list_length (ps_plugins_list);
		int gasit = 0;
		for (int j=0;j<nr && !gasit;j++)
		{
			_ps_plugin *plugin = (_ps_plugin*)g_list_nth_data (ps_plugins_list, j);
			if (plugin!=NULL && plugin->filename!=NULL)
			{
				if (strcmp (plugin->filename, file)==0)
				{
					date_plugin = plugin;
					gasit = 1;
				}
			}
		}
		free (file);
	}
	return date_plugin;
}



void cb_ps_selected_plugin (GtkTreeSelection *sel, gpointer user_data)
{
	GtkWidget *pagina = (GtkWidget*)user_data;
	_ps_plugin * date_plugin = ps_plugins_list_get_selected_plugin (pagina);
	if (date_plugin!=NULL)
	{
		GtkWidget *b_plugin_config = (GtkWidget*)ps_get_widget_from (pagina, "b_plugin_config");
		if (date_plugin->config!=NULL && b_plugin_config!=NULL)
		{
			gtk_widget_set_sensitive (b_plugin_config, TRUE);
		}
		else
		{
			gtk_widget_set_sensitive (b_plugin_config, FALSE);
		}
		GtkWidget *b_plugin_about = (GtkWidget*)ps_get_widget_from (pagina, "b_plugin_about");
		if (date_plugin->about!=NULL && b_plugin_about!=NULL)
		{
			gtk_widget_set_sensitive (b_plugin_about, TRUE);
		}
		else
		{
			gtk_widget_set_sensitive (b_plugin_about, FALSE);
		}
		GtkWidget *l_desc_data = (GtkWidget*)ps_get_widget_from (pagina, "l_desc_data");
		gtk_label_set_text ((GtkLabel*)l_desc_data, date_plugin->description);
		GtkWidget *l_run_data = (GtkWidget*)ps_get_widget_from (pagina, "l_run_data");
		switch (date_plugin->run)
		{
			case PS_PLUGIN_RUN_BEFORE_FILTERS:
				gtk_label_set_text ((GtkLabel*)l_run_data, __("Before Filters"));
				break;
			case PS_PLUGIN_RUN_AFTER_FILTERS:
				gtk_label_set_text ((GtkLabel*)l_run_data, __("After Filters"));
				break;
			default: 
				gtk_label_set_text ((GtkLabel*)l_run_data, __("Unknown"));
				break;
		}
		GtkWidget *l_status_data = (GtkWidget*)ps_get_widget_from (pagina, "l_status_data");
		if (date_plugin->enabled==1) gtk_label_set_text ((GtkLabel*)l_status_data, __("Runs"));
			else gtk_label_set_text ((GtkLabel*)l_status_data, __("Stopped"));
	}
}

void cb_ps_b_plugin_config (GtkWidget *buton, gpointer user_data)
{
	GtkWidget *pagina = (GtkWidget*)user_data;
	_ps_plugin * date_plugin = ps_plugins_list_get_selected_plugin (pagina);
	if (date_plugin!=NULL && date_plugin->config!=NULL)
	{
		date_plugin->config ();
	}
}

void cb_ps_b_plugin_about (GtkWidget *buton, gpointer user_data)
{
	GtkWidget *pagina = (GtkWidget*)user_data;
	_ps_plugin * date_plugin = ps_plugins_list_get_selected_plugin (pagina);
	if (date_plugin!=NULL && date_plugin->about!=NULL)
	{
		date_plugin->about ();
	}
}

GtkWidget * ps_config_plugins ()
{
	GtkWidget *pagina = gtk_vbox_new (FALSE, 2);
	GtkWidget *scr_f = gtk_frame_new (__("Available Plugins"));
	ps_hookup_object (pagina, scr_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)scr_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, scr_f, FALSE, FALSE, 2);
	GtkWidget *vbox_scr = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_scr, "vbox_scr");
	gtk_container_add ((GtkContainer*)scr_f, vbox_scr);
	GtkWidget *plugins_list = ps_create_config_plugins_list ();
	ps_hookup_object (pagina, plugins_list, "plugins_list");	
	GtkWidget *plugins_list_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)plugins_list_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)plugins_list_scroll, GTK_SHADOW_IN);
	ps_hookup_object (pagina, plugins_list_scroll, "plugins_list_scroll_scroll");
	gtk_container_add ((GtkContainer*)plugins_list_scroll, plugins_list);
	gtk_box_pack_start ((GtkBox*)vbox_scr, plugins_list_scroll, FALSE, FALSE, 2);
	GtkWidget *hbox_buttons = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (pagina, hbox_buttons, "hbox_buttons");
	gtk_box_pack_start ((GtkBox*)vbox_scr, hbox_buttons, FALSE, FALSE, 2);
	GtkSizeGroup *size_plugin_buttons = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	GtkWidget *b_plugin_config = ps_create_button_with_image_from_stock ("gtk-preferences", __("Configure"), 1);
	ps_hookup_object (pagina, b_plugin_config, "b_plugin_config");
	gtk_size_group_add_widget (size_plugin_buttons, b_plugin_config);
	gtk_widget_set_sensitive (b_plugin_config, FALSE);
	gtk_box_pack_start ((GtkBox*)hbox_buttons, b_plugin_config, FALSE, FALSE, 2);
	g_signal_connect (b_plugin_config, "clicked", (GCallback)cb_ps_b_plugin_config, pagina);
	GtkWidget *b_plugin_about = ps_create_button_with_image_from_stock ("", __("About"), 1);
	ps_hookup_object (pagina, b_plugin_about, "b_plugin_about");
	gtk_size_group_add_widget (size_plugin_buttons, b_plugin_about);
	gtk_widget_set_sensitive (b_plugin_about, FALSE);
	gtk_box_pack_start ((GtkBox*)hbox_buttons, b_plugin_about, FALSE, FALSE, 2);
	g_signal_connect (b_plugin_about, "clicked", (GCallback)cb_ps_b_plugin_about, pagina);
	GList *ps_plugins_list = get_ps_plugins_list ();
	int nr = g_list_length (ps_plugins_list);
	for (int j=0;j<nr;j++)
	{
		_ps_plugin *date_plugin = (_ps_plugin*)g_list_nth_data (ps_plugins_list, j);
		adauga_plugin (pagina, date_plugin);
	}
	GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)plugins_list);
	g_signal_connect (sel, "changed", (GCallback)cb_ps_selected_plugin, pagina);
	GtkWidget * hbox_desc = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (pagina, hbox_desc, "hbox_desc");
	gtk_box_pack_start ((GtkBox*)vbox_scr, hbox_desc, TRUE, FALSE, 2);
	GtkWidget * l_desc = gtk_label_new ("");
	gtk_label_set_markup ((GtkLabel*)l_desc, __("<b>Description:</b>"));
	ps_hookup_object (pagina, l_desc, "l_desc");
	gtk_box_pack_start ((GtkBox*)hbox_desc, l_desc, FALSE, FALSE, 2);
	GtkWidget * l_desc_data = gtk_label_new ("");
	gtk_label_set_line_wrap ((GtkLabel*)l_desc_data, TRUE);
	ps_hookup_object (pagina, l_desc_data, "l_desc_data");
	gtk_box_pack_start ((GtkBox*)hbox_desc, l_desc_data, FALSE, FALSE, 2);
	
	GtkWidget * hbox_run = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (pagina, hbox_run, "hbox_run");
	gtk_box_pack_start ((GtkBox*)vbox_scr, hbox_run, TRUE, FALSE, 2);
	GtkWidget * l_run = gtk_label_new ("");
	gtk_label_set_markup ((GtkLabel*)l_run, __("<b>Runs:</b>"));
	ps_hookup_object (pagina, l_run, "l_run");
	gtk_box_pack_start ((GtkBox*)hbox_run, l_run, FALSE, FALSE, 2);
	GtkWidget * l_run_data = gtk_label_new ("");
	ps_hookup_object (pagina, l_run_data, "l_run_data");
	gtk_box_pack_start ((GtkBox*)hbox_run, l_run_data, FALSE, FALSE, 2);
	
	GtkWidget * hbox_status = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (pagina, hbox_status, "hbox_status");
	gtk_box_pack_start ((GtkBox*)vbox_scr, hbox_status, TRUE, FALSE, 2);
	GtkWidget * l_status = gtk_label_new ("");
	gtk_label_set_markup ((GtkLabel*)l_status, __("<b>Status:</b>"));
	ps_hookup_object (pagina, l_status, "l_status");
	gtk_box_pack_start ((GtkBox*)hbox_status, l_status, FALSE, FALSE, 2);
	GtkWidget * l_status_data = gtk_label_new ("");
	ps_hookup_object (pagina, l_status_data, "l_status_data");
	gtk_box_pack_start ((GtkBox*)hbox_status, l_status_data, FALSE, FALSE, 2);
	
	GtkWidget *info_plugins = ps_create_mascot_header_with_image_from_stock ("gtk-dialog-warning", __("Using plugins might be dangerous. PhotoStorm has no control over the plugin's code and is NOT RESPONSIBLE for any damage caused directly or indirectly by the plugins."));
	ps_hookup_object (pagina, info_plugins, "info_plugins");
	gtk_box_pack_start ((GtkBox*)vbox_scr, info_plugins, FALSE, FALSE, 2);
	pagina_plugin = pagina;
	return pagina;
}

GtkWidget * ps_config_email ()
{
	GtkWidget *pagina = gtk_vbox_new (FALSE, 2);
	GtkWidget *info_f = gtk_frame_new (__("Email Display"));
	ps_hookup_object (pagina, info_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)info_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, info_f, FALSE, FALSE, 2);
	GtkWidget *vbox_info = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_info, "vbox_info");
	gtk_container_add ((GtkContainer*)info_f, vbox_info);
	GtkWidget *display_email = ps_create_check_button_with_image_from_stock ("", __("Display email titles"), 1);
	ps_hookup_object (pagina, display_email, "display_email");
	ps_check_button_set_value (display_email, config.email.disp);
	gtk_box_pack_start ((GtkBox*)vbox_info, display_email, FALSE, FALSE, 2);
	GtkWidget *draw_email_border = ps_create_check_button_with_image_from_stock ("", __("Draw a border around the email titles"), 1);
	ps_hookup_object (pagina, draw_email_border, "draw_email_border");
	ps_check_button_set_value (draw_email_border, config.email.border);
	gtk_box_pack_start ((GtkBox*)vbox_info, draw_email_border, FALSE, FALSE, 2);
	GtkWidget *email_position = ps_create_position_widget ("Display email titles at", 100, 400);
	ps_hookup_object (pagina, email_position, "email_position");
	ps_position_set_values (email_position, config.email.pos.a, config.email.pos.x, config.email.pos.y);
	gtk_box_pack_start ((GtkBox*)vbox_info, email_position, FALSE, FALSE, 2);
	GtkWidget *number_emails = ps_create_number_edit ("Maximum number of emails to display", 1, 30, 1, 1);
	ps_hookup_object (pagina, number_emails, "number_emails");
	ps_number_edit_set_number (number_emails, config.email.max_nr);
	gtk_box_pack_start ((GtkBox*)vbox_info, number_emails, FALSE, FALSE, 2);
	//_ps_color *col_f = ps_pack_color (255, 255, 0, 0);
	GtkWidget *sender_color = ps_create_color_select ("Sender color", &config.email.fcolor);
	ps_hookup_object (pagina, sender_color, "sender_color");
	gtk_box_pack_start ((GtkBox*)vbox_info, sender_color, FALSE, FALSE, 2);
	//_ps_color *col_s = ps_pack_color (255, 255, 255, 0);
	GtkWidget *subject_color = ps_create_color_select ("Subject color", &config.email.scolor);
	ps_hookup_object (pagina, subject_color, "subject_color");
	gtk_box_pack_start ((GtkBox*)vbox_info, subject_color, FALSE, FALSE, 2);
	GtkWidget *emails_text = ps_create_number_edit ("Text size", 0, 100, 1, 1);
	ps_hookup_object (pagina, emails_text, "emails_text");
	ps_number_edit_set_number (emails_text, config.email.fsize);
	gtk_box_pack_start ((GtkBox*)vbox_info, emails_text, FALSE, FALSE, 2);
	
	GtkWidget *emaildata_f = gtk_frame_new (__("Email Account Information (POP3)"));
	ps_hookup_object (pagina, emaildata_f, "emaildata_f");
	gtk_container_set_border_width ((GtkContainer*)emaildata_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, emaildata_f, FALSE, FALSE, 2);
	GtkWidget *vbox_emaildata = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_emaildata, "vbox_emaildata");
	gtk_container_add ((GtkContainer*)emaildata_f, vbox_emaildata);
	GtkWidget *email_server = ps_server_data ("Email server", "lug.orizont.net", 110, "linuxeyes", "linuxmusic");
	ps_hookup_object (pagina, email_server, "email_server");
	ps_server_data_set_server (email_server, config.email.server);
	ps_server_data_set_port (email_server, config.email.port);
	ps_server_data_set_username (email_server, config.email.username);
	ps_server_data_set_password (email_server, config.email.password);
	gtk_box_pack_start ((GtkBox*)vbox_emaildata, email_server, FALSE, FALSE, 2);
	GtkWidget *update_time = ps_create_number_edit ("Update email information interval (in minutes)", 1, 10000, 1, 1);
	ps_hookup_object (pagina, update_time, "update_time");
	ps_number_edit_set_number (update_time, config.email.update);
	gtk_box_pack_start ((GtkBox*)vbox_emaildata, update_time, FALSE, FALSE, 2);
	return pagina;
}

GtkTreeModel * ps_model_lista_lang ()
{
	GtkTreeStore *store = gtk_tree_store_new (ps_c_lang_numar,G_TYPE_STRING, G_TYPE_STRING,
											  G_TYPE_STRING, G_TYPE_STRING, 
                                              G_TYPE_STRING, G_TYPE_STRING);
	return (GtkTreeModel*) store;
}

GtkWidget * ps_create_config_lang_list ()
{
	GtkWidget * lista_lang = gtk_tree_view_new_with_model (ps_model_lista_lang());
	GtkCellRenderer * render;
	GtkTreeViewColumn * col;
	
	// Language
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Language"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_lang_nume);
	gtk_tree_view_column_set_resizable (col, TRUE);
    render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 0.5, "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_lang_nume, NULL);
    gtk_tree_view_append_column ((GtkTreeView*)lista_lang, col);
	
	// Author
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Author"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_lang_autor);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 1.0, "style", PANGO_STYLE_ITALIC, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_lang_autor, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_lang, col);
	
	// Version
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Version"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_lang_versiune);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.5, "yalign", 1.0, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_lang_versiune, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_lang, col);
	
	// Date
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Date"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_lang_data);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.5, "yalign", 1.0, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_lang_data, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_lang, col);
	
	// Status
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Status"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_lang_tip);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 1.0, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_lang_tip, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_lang, col);
	
	gtk_widget_set_size_request (lista_lang, 100, 200);
	gtk_tree_view_set_headers_visible ((GtkTreeView*)lista_lang, TRUE);
	gtk_tree_view_set_rules_hint ((GtkTreeView*)lista_lang, TRUE);
	
	/*
    sel = gtk_tree_view_get_selection ((GtkTreeView*)lista_poze);
	g_signal_connect (lista_poze, "row-activated", (GCallback)cb_ps_dublu_click_picture, NULL);
	gtk_tree_selection_set_mode (sel, GTK_SELECTION_SINGLE);
	g_signal_connect (sel, "changed", (GCallback)cb_ps_selected_picture, NULL);
	
	g_signal_connect (lista_poze, "button-release-event", (GCallback)cb_ps_meniu_lista_poze, NULL);
	*/
	
	return lista_lang;
}

void adauga_limba (GtkWidget *window, _ps_lang *lang)
{
    if (lang==NULL) return;
    ps_lang_repair (lang);
	GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget_from (window, "lang_list"));
	GtkTreeIter pos;
	
	char tip[1000];
	if (strcmp (lang->versiune, ps_version)==0) strcpy (tip, __("Good"));
	  else if (strcmp (lang->versiune, ps_version)>0) strcpy (tip, __("Too New"));
		 else strcpy (tip, __("Too Old"));
	if (strcmp (lang->versiune, "")==0) strcpy (tip, __("Incomplete"));
	
	gtk_tree_store_append ((GtkTreeStore*)model,&pos,NULL);
	gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_lang_file, lang->file,
                        ps_c_lang_nume, lang->nume, 
						ps_c_lang_autor, lang->autor,
						ps_c_lang_versiune, lang->versiune,
						ps_c_lang_data, lang->data,
						ps_c_lang_tip, tip,
						-1);
}

const char * ps_lang_list_get_language (GtkWidget *window)
{
    if (window==NULL) return strdup ("");
    char *file = NULL;
        
    GtkTreeSelection * sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget_from (window, "lang_list"));
	if (sel!=NULL)
	{
		GtkTreeModel * model;
		GtkTreeIter pos;
		if (gtk_tree_selection_get_selected (sel, &model, &pos))
		{
		    gtk_tree_model_get (model, &pos, ps_c_lang_file, &file, -1);
		}
	}
	//printf ("lang file: =%s=\n", file);
	return (file!=NULL)?file:strdup ("");
}

#ifdef PHOTOSTORM_PROGRAM
#ifndef PHOTOSTORM_WEBSITE

void scan_languages (GtkWidget *window, const char *calea)
{
	if (calea!=NULL)
	{
		struct dirent *namelist;
		/*//int n=scandir (calea, &namelist, 0, alphasort);
		DIR *dir = opendir (calea);
		if (dir!=NULL)
		{
			do
			{
				struct dirent *t;
				namelist = readdir (dir);
				printf ("namelist[%d]->d_name: %s\n",n, namelist[n]->d_name);
			} while (namelist[n-1]!=NULL);
			n--;
			closedir (dir);
		}*/
		DIR *dir = opendir (calea);
		if (dir==NULL) printf (__("Error loading languages from %s\n"), calea);
		else
		{
			int tot = 0;
			do
			{
				namelist = readdir (dir);
				if (namelist!=NULL)
				{
				//char *ext=NULL;
				//if (rindex (namelist[i]->d_name, '.')!=NULL) ext=strdup(rindex (namelist[i]->d_name, '.'));
				//if (ext!=NULL)
				//{
					//if (strcasecmp (ext,".psa")==0)
					//{
						char *temp;
						int t=1;
						if (calea[strlen(calea)]!=ps_dir_sep) t=2;
						temp=(char*)malloc (strlen (calea)+strlen(namelist->d_name)+t);
						if (temp!=NULL)
						{
							strcpy (temp, calea);
							if (t==2) strcat (temp, ps_dir_sep_str);
							strcat (temp, namelist->d_name);
							_ps_lang *lang = ps_lang_new_from_file (temp);
							if (lang!=NULL)
							{
							       adauga_limba (window, lang);
							       ps_lang_free (lang);
                            }
							free (temp);
						}
				//	}
				//	free (ext);
				//free (namelist);
				} else tot = 1;
			} while (!tot);
			closedir (dir);
		}
	}
}

void selecteaza_limba (GtkWidget *window, const char *lang_file)
{
    GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget_from (window, "lang_list"));
	GtkTreeIter pos,s_pos;
	GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget_from (window, "lang_list"));
	int gasit=0;
	if (gtk_tree_model_get_iter_first (model, &s_pos))
	do
	{
		gchar *s;
		gtk_tree_model_get (model, &s_pos, ps_c_lang_file, &s, -1);
		if (s!=NULL)
		{
			if (strcmp (s,(char*)lang_file)==0) 
			{	
				gasit=1;
				pos=s_pos;
				gtk_tree_selection_select_iter (sel, &pos);
			}
			g_free (s);
		}
	} while (gtk_tree_model_iter_next (model, &s_pos) && !gasit);
}

#endif
#endif

GtkWidget * ps_config_language ()
{
	GtkWidget *pagina = gtk_vbox_new (FALSE, 2);
	GtkWidget *info_f = gtk_frame_new (__("Language Options"));
	ps_hookup_object (pagina, info_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)info_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, info_f, FALSE, FALSE, 2);
	GtkWidget *vbox_info = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_info, "vbox_info");
	gtk_container_add ((GtkContainer*)info_f, vbox_info);
	GtkWidget *lang_support = ps_create_check_button_with_image_from_stock ("", __("Enable multilanguage support"), 1);
	ps_hookup_object (pagina, lang_support, "enabled");
	ps_check_button_set_value (lang_support, config.lang.enabled);
	gtk_box_pack_start ((GtkBox*)vbox_info, lang_support, FALSE, FALSE, 2);
	//GtkWidget *lang_list = ps_create_option_menu (__("Language"), langs_list, nr_langs); 
	
	GtkWidget *scr_f = gtk_frame_new (__("Available Languages"));
	ps_hookup_object (pagina, scr_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)scr_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, scr_f, FALSE, FALSE, 2);
	GtkWidget *vbox_scr = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_scr, "vbox_scr");
	gtk_container_add ((GtkContainer*)scr_f, vbox_scr);
	GtkWidget *lang_list = ps_create_config_lang_list ();
	ps_hookup_object (pagina, lang_list, "lang_list");	
	GtkWidget *lang_list_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)lang_list_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)lang_list_scroll, GTK_SHADOW_IN);
	ps_hookup_object (pagina, lang_list_scroll, "lang_list_scroll_scroll");
	gtk_container_add ((GtkContainer*)lang_list_scroll, lang_list);
	gtk_box_pack_start ((GtkBox*)vbox_scr, lang_list_scroll, FALSE, FALSE, 2);
	char temp[10000];
	sprintf (temp, "%s%clanguages", config.system.libs, ps_dir_sep);
	_ps_lang *eng = ps_lang_new ("English", "Default Language", ps_version, "30/31/2004");
	adauga_limba (pagina, eng);
	ps_lang_free (eng);
	scan_languages (pagina, temp);
	selecteaza_limba (pagina, config.lang.file);
	GtkWidget *info_restart = ps_create_mascot_header_with_image_from_stock ("gtk-dialog-warning", __("PhotoStorm has to be restarted so that the language settings will take effect."));
	ps_hookup_object (pagina, info_restart, "info_restart");
	gtk_box_pack_start ((GtkBox*)vbox_scr, info_restart, FALSE, FALSE, 2);
	return pagina;
}

GtkWidget * ps_config_system ()
{
	GtkWidget *pagina = gtk_vbox_new (FALSE, 2);
	GtkWidget *info_f = gtk_frame_new (__("System Paths"));
	ps_hookup_object (pagina, info_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)info_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, info_f, FALSE, FALSE, 2);
	GtkWidget *vbox_info = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_info, "vbox_info");
	gtk_container_add ((GtkContainer*)info_f, vbox_info);	
	GtkWidget *shared_albums_loc = ps_create_location_edit (__("Shared albums location"), 1000, -1, 1);
	ps_hookup_object (pagina, shared_albums_loc, "shared_albums_loc");
	ps_location_edit_set_location (shared_albums_loc, config.system.shared_albums);
	gtk_box_pack_start ((GtkBox*)vbox_info, shared_albums_loc, TRUE, FALSE, 2); 
	GtkWidget *albums_loc = ps_create_location_edit (__("Personal albums location"), 1000, -1, 1);
	ps_hookup_object (pagina, albums_loc, "albums_loc");
	ps_location_edit_set_location (albums_loc, config.system.albums);
	gtk_box_pack_start ((GtkBox*)vbox_info, albums_loc, TRUE, FALSE, 2); 
	GtkWidget *libs_loc = ps_create_location_edit (__("PhotoStorm libraries location"), 1000, -1, 1);
	ps_hookup_object (pagina, libs_loc, "libs_loc");
	ps_location_edit_set_location (libs_loc, config.system.libs);
	gtk_box_pack_start ((GtkBox*)vbox_info, libs_loc, TRUE, FALSE, 2);
	GtkWidget *shared_plugins_loc = ps_create_location_edit (__("Shared plugins location"), 1000, -1, 1);
	ps_hookup_object (pagina, shared_plugins_loc, "shared_plugins_loc");
	ps_location_edit_set_location (shared_plugins_loc, config.system.shared_plugins);
	gtk_box_pack_start ((GtkBox*)vbox_info, shared_plugins_loc, TRUE, FALSE, 2); 
	GtkWidget *plugins_loc = ps_create_location_edit (__("Personal plugins location"), 1000, -1, 1);
	ps_hookup_object (pagina, plugins_loc, "plugins_loc");
	ps_location_edit_set_location (plugins_loc, config.system.plugins);
	gtk_box_pack_start ((GtkBox*)vbox_info, plugins_loc, TRUE, FALSE, 2); 	
	
	GtkWidget *scr_f = gtk_frame_new (__("Screen Information"));
	ps_hookup_object (pagina, scr_f, "info_f");
	gtk_container_set_border_width ((GtkContainer*)scr_f, 5);
	gtk_box_pack_start ((GtkBox*)pagina, scr_f, FALSE, FALSE, 2);
	GtkWidget *vbox_scr = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (pagina, vbox_scr, "vbox_scr");
	gtk_container_add ((GtkContainer*)scr_f, vbox_scr);
	GtkWidget *scr_autodetect = ps_create_check_button_with_image_from_stock ("", __("Try to autodetect screen size (Win32 only)"), 1);
	ps_hookup_object (pagina, scr_autodetect, "scr_autodetect");
	ps_check_button_set_value (scr_autodetect, config.system.scr_size_autodetect);
	gtk_box_pack_start ((GtkBox*)vbox_scr, scr_autodetect, FALSE, FALSE, 2);
	GtkWidget *scr_w = ps_create_number_edit (__("Screen Width"), 100, 10000, 1, 1);	
	ps_hookup_object (pagina, scr_w, "scr_w");
	ps_number_edit_set_number (scr_w, config.system.scr_width);
	gtk_box_pack_start ((GtkBox*)vbox_scr, scr_w, FALSE, FALSE, 2);
	GtkWidget *scr_h = ps_create_number_edit (__("Screen Height"), 100, 10000, 1, 1);	
	ps_hookup_object (pagina, scr_h, "scr_h");
	ps_number_edit_set_number (scr_h, config.system.scr_height);
	gtk_box_pack_start ((GtkBox*)vbox_scr, scr_h, FALSE, FALSE, 2); 
	/*GtkWidget *lang_list = ps_create_option_menu (__("Language"), ps_lang_table, 4, 1); 
	ps_hookup_object (pagina, lang_list, "lang_list");
	gtk_box_pack_start ((GtkBox*)vbox_scr, lang_list, FALSE, FALSE, 2);*/
	return pagina;
}

void ps_configuration (GtkWidget *parent)
{
	GtkAccelGroup * accel_group = gtk_accel_group_new ();
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_position ((GtkWindow*)window, GTK_WIN_POS_CENTER);
	gtk_window_set_modal ((GtkWindow*)window, TRUE);
	if ((GtkWindow*)parent!=NULL) gtk_window_set_transient_for ((GtkWindow*)window, (GtkWindow*)parent);
	gtk_container_set_border_width ((GtkContainer*)window, 7);
	gtk_window_set_title ((GtkWindow*)window, __("Photostorm Configuration"));
	
	GtkWidget *vbox = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, vbox, "vbox");
	
	GtkWidget *hbox = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (window, hbox, "hbox");
	gtk_box_pack_start ((GtkBox*)vbox, hbox, TRUE, TRUE, 2);
	
	// config image
	GtkWidget *config_image = gtk_image_new_from_file (pict_path ("logos/config.jpg"));
	ps_hookup_object (window, config_image, "config_image");
	gtk_box_pack_start ((GtkBox*)hbox, config_image, FALSE, FALSE, 2);
	
	// crearea notebook-ului
	GtkWidget *pagini = gtk_notebook_new ();
	ps_hookup_object (window, pagini, "pagini");
	gtk_box_pack_start ((GtkBox*)hbox, pagini, TRUE, TRUE, 2);
	gtk_notebook_set_show_border ((GtkNotebook*)pagini, FALSE);
	
	// pagina 1 - "Wallpaper"
	GtkWidget *pagina1 = ps_config_wallpaper ();
	ps_hookup_object (window, pagina1, "wallpaper");
	
	// pagina 2 - "Calendar"
	GtkWidget *pagina2 = ps_config_calendar ();
	ps_hookup_object (window, pagina2, "calendar");
	
	// pagina 3 - "Quotes"
	GtkWidget *pagina3 = ps_config_quotes ();
	ps_hookup_object (window, pagina3, "quotes");
	
	// pagina 4 - "Messages"
	GtkWidget *pagina4 = ps_config_messages ();//ps_create_mascot_header_with_image_from_stock ("", __("This function has not been yet implemented.")); //gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, pagina4, "messages");
	
	// pagina 5 - "E-mail"
	GtkWidget *pagina5 = ps_config_email ();
	ps_hookup_object (window, pagina5, "email");
	
	// pagina 6 - "Plugins"
	GtkWidget *pagina6 = ps_config_plugins ();//ps_create_mascot_header_with_image_from_stock ("", __("This function has not been yet implemented.")); //gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, pagina6, "plugins");
	
	// pagina 7 - "Language"
	GtkWidget *pagina7 = ps_config_language ();
	ps_hookup_object (window, pagina7, "language");
	
	// pagina 8 - "System"
	GtkWidget *pagina8 = ps_config_system ();
	ps_hookup_object (window, pagina8, "system");
	
	// adaugarea paginilor
	gtk_notebook_append_page ((GtkNotebook*)pagini, pagina1, gtk_label_new (__("Wallpaper")));
	gtk_notebook_append_page ((GtkNotebook*)pagini, pagina2, gtk_label_new (__("Calendar")));
	gtk_notebook_append_page ((GtkNotebook*)pagini, pagina3, gtk_label_new (__("Quotes")));
	gtk_notebook_append_page ((GtkNotebook*)pagini, pagina4, gtk_label_new (__("Messages")));
	gtk_notebook_append_page ((GtkNotebook*)pagini, pagina5, gtk_label_new (__("Email")));
	gtk_notebook_append_page ((GtkNotebook*)pagini, pagina6, gtk_label_new (__("Plugins")));
	gtk_notebook_append_page ((GtkNotebook*)pagini, pagina7, gtk_label_new (__("Language")));
	gtk_notebook_append_page ((GtkNotebook*)pagini, pagina8, gtk_label_new (__("System")));
	
	// butoane
	GtkSizeGroup *butoane = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	GtkWidget *buton_help = ps_create_button_with_image_from_stock ("gtk-help", __("Help"), 1);
	ps_hookup_object (window, buton_help, "b_help");
	gtk_size_group_add_widget (butoane, buton_help);
	gtk_widget_add_accelerator (buton_help, "clicked", accel_group, GDK_F1, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	g_signal_connect (buton_help, "clicked", (GCallback)cb_ps_open_browser, help_path ("index.html"));
	GtkWidget *buton_ok = ps_create_button_with_image_from_stock ("gtk-apply", __("Okey"), 1);
	ps_hookup_object (window, buton_ok, "b_ok");
	gtk_widget_add_accelerator (buton_ok, "clicked", accel_group, GDK_Return, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	g_signal_connect (buton_ok, "clicked", (GCallback)cb_ps_config_okey, window);
	gtk_size_group_add_widget (butoane, buton_ok);
	GtkWidget *buton_cancel = ps_create_button_with_image_from_stock ("gtk-cancel", __("Cancel"), 1);
	ps_hookup_object (window, buton_cancel, "b_cancel");
	gtk_widget_add_accelerator (buton_cancel, "clicked", accel_group, GDK_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	g_signal_connect (buton_cancel, "clicked", (GCallback)cb_ps_config_close, window);
	gtk_size_group_add_widget (butoane, buton_cancel);
	
	GtkWidget *hbox2 = gtk_hbox_new (FALSE, 2); 
	ps_hookup_object (window, vbox, "hbox2");
	gtk_box_pack_start ((GtkBox*)hbox2, buton_help, FALSE, FALSE, 2);
	gtk_box_pack_end ((GtkBox*)hbox2, buton_ok, FALSE, FALSE, 2);
	gtk_box_pack_end ((GtkBox*)hbox2, buton_cancel, FALSE, FALSE, 2);
	
	gtk_box_pack_start ((GtkBox*)vbox, hbox2, FALSE, FALSE, 2);
	
	gtk_container_add ((GtkContainer*)window, vbox);
	
	g_signal_connect (window, "destroy", (GCallback)cb_ps_config_close, window);
	
	config_window = window;
	
	gtk_window_add_accel_group ((GtkWindow*)window, accel_group);
	
	gtk_widget_show_all (window);	
}

#endif

#endif

void ps_set_value (_ps_config *config, const char *var, const char *value)
{
	if (config!=NULL && var!=NULL && value!=NULL)
	{
		//printf ("%s\n", var);
		// wallpaper
		if (strcmp(var,"wallpaper_interval")==0) config->wallpaper.cint = ps_strtoint (value);
		if (strcmp(var,"wallpaper_display_type")==0) config->wallpaper.dtype = ps_strtoint (value);
		//if (strcmp(var,"wallpaper_grayscale")==0) config->wallpaper.gray = ps_strtoint (value);
		if (strcmp(var,"wallpaper_display_info")==0) config->wallpaper.dinfo = ps_strtoint (value);
		if (strcmp(var,"wallpaper_info_format")==0) strcpy (config->wallpaper.format, value);
		if (strcmp(var,"wallpaper_border")==0) config->wallpaper.border = ps_strtoint (value);
		if (strcmp(var,"wallpaper_info_x")==0) config->wallpaper.pos.x = ps_strtoint (value);
		if (strcmp(var,"wallpaper_info_y")==0) config->wallpaper.pos.y = ps_strtoint (value);
		if (strcmp(var,"wallpaper_info_autoposition")==0) config->wallpaper.pos.a = ps_strtoint (value);
		if (strcmp(var,"wallpaper_font_color")==0) 
		{
			_ps_color *col = ps_strtocolor (value);
			if (col!=NULL)
			{
				config->wallpaper.fcolor.red = col->red;
				config->wallpaper.fcolor.green = col->green;
				config->wallpaper.fcolor.blue = col->blue;
				config->wallpaper.fcolor.alpha = col->alpha;
				free (col);
			}
		}
		if (strcmp(var,"wallpaper_font_size")==0) config->wallpaper.fsize = ps_strtoint (value);
		// calendar
		if (strcmp(var,"calendar_display")==0) config->calendar.disp= ps_strtoint (value);
		if (strcmp(var,"calendar_border")==0) config->calendar.border = ps_strtoint (value);
		if (strcmp(var,"calendar_position_x")==0) config->calendar.pos.x = ps_strtoint (value);
		if (strcmp(var,"calendar_position_y")==0) config->calendar.pos.y = ps_strtoint (value);
		if (strcmp(var,"calendar_autoposition")==0) config->calendar.pos.a = ps_strtoint (value);
		if (strcmp(var,"calendar_mark_color")==0) 
		{
			_ps_color *col = ps_strtocolor (value);
			if (col!=NULL)
			{
				config->calendar.mcolor.red = col->red;
				config->calendar.mcolor.green = col->green;
				config->calendar.mcolor.blue = col->blue;
				config->calendar.mcolor.alpha = col->alpha;
				free (col);
			}
		}
		if (strcmp(var,"calendar_font_color")==0) 
		{
			_ps_color *col = ps_strtocolor (value);
			if (col!=NULL)
			{
				config->calendar.fcolor.red = col->red;
				config->calendar.fcolor.green = col->green;
				config->calendar.fcolor.blue = col->blue;
				config->calendar.fcolor.alpha = col->alpha;
				free (col);
			}
		}
		if (strcmp(var,"calendar_current_month_font_size")==0) config->calendar.cmfsize = ps_strtoint (value);
		if (strcmp(var,"calendar_other_months_font_size")==0) config->calendar.omfsize = ps_strtoint (value);
		if (strcmp(var,"calendar_mark_date")==0) config->calendar.mark_date = ps_strtoint (value);
		if (strcmp(var,"calendar_past_months")==0) config->calendar.p_month = ps_strtoint (value);
		if (strcmp(var,"calendar_future_months")==0) config->calendar.n_month = ps_strtoint (value);
		// quotes
		if (strcmp(var,"quotes_display")==0) config->quotes.disp= ps_strtoint (value);
		if (strcmp(var,"quotes_border")==0) config->quotes.border = ps_strtoint (value);
		if (strcmp(var,"quotes_position_x")==0) config->quotes.pos.x = ps_strtoint (value);
		if (strcmp(var,"quotes_position_y")==0) config->quotes.pos.y = ps_strtoint (value);
		if (strcmp(var,"quotes_autoposition")==0) config->quotes.pos.a = ps_strtoint (value);
		if (strcmp(var,"quotes_font_color")==0) 
		{
			_ps_color *col = ps_strtocolor (value);
			if (col!=NULL)
			{
				config->quotes.fcolor.red = col->red;
				config->quotes.fcolor.green = col->green;
				config->quotes.fcolor.blue = col->blue;
				config->quotes.fcolor.alpha = col->alpha;
				free (col);
			}
		}
		if (strcmp(var,"quotes_font_size")==0) config->quotes.fsize = ps_strtoint (value);
		if (strcmp(var,"quotes_get_quotes_online")==0) config->quotes.inet = ps_strtoint (value);
		if (strcmp(var,"quotes_server_name")==0) strcpy (config->quotes.server,value);
		if (strcmp(var,"quotes_server_port")==0) config->quotes.port = ps_strtoint (value);
		if (strcmp(var,"quotes_keywords")==0) strcpy (config->quotes.keywords, value);
		if (strcmp(var,"quotes_use_only_keywords")==0) config->quotes.only_keywords = ps_strtoint (value);
		// messages
		if (strcmp(var,"messages_display")==0) config->messages.disp= ps_strtoint (value);
		if (strcmp(var,"messages_border")==0) config->messages.border = ps_strtoint (value);
		if (strcmp(var,"messages_position_x")==0) config->messages.pos.x = ps_strtoint (value);
		if (strcmp(var,"messages_position_y")==0) config->messages.pos.y = ps_strtoint (value);
		if (strcmp(var,"messages_autoposition")==0) config->messages.pos.a = ps_strtoint (value);
		if (strcmp(var,"messages_time_color")==0)
		{
			_ps_color *col = ps_strtocolor (value);
			if (col!=NULL)
			{
				config->messages.tcolor.red = col->red;
				config->messages.tcolor.green = col->green;
				config->messages.tcolor.blue = col->blue;
				config->messages.tcolor.alpha = col->alpha;
				free (col);
			}
		}
		if (strcmp(var,"messages_message_color")==0)
		{
			_ps_color *col = ps_strtocolor (value);
			if (col!=NULL)
			{
				config->messages.mcolor.red = col->red;
				config->messages.mcolor.green = col->green;
				config->messages.mcolor.blue = col->blue;
				config->messages.mcolor.alpha = col->alpha;
				free (col);
			}
		}
		if (strcmp(var,"messages_font_size")==0) config->messages.fsize = ps_strtoint (value);
		// email
		if (strcmp(var,"email_display")==0) config->email.disp= ps_strtoint (value);
		if (strcmp(var,"email_border")==0) config->email.border = ps_strtoint (value);
		if (strcmp(var,"email_position_x")==0) config->email.pos.x = ps_strtoint (value);
		if (strcmp(var,"email_position_y")==0) config->email.pos.y = ps_strtoint (value);
		if (strcmp(var,"email_autoposition")==0) config->email.pos.a = ps_strtoint (value);
		if (strcmp(var,"email_sender_color")==0)
		{
			_ps_color *col = ps_strtocolor (value);
			if (col!=NULL)
			{
				config->email.fcolor.red = col->red;
				config->email.fcolor.green = col->green;
				config->email.fcolor.blue = col->blue;
				config->email.fcolor.alpha = col->alpha;
				free (col);
			}
		}
		if (strcmp(var,"email_subject_color")==0)
		{
			_ps_color *col = ps_strtocolor (value);
			if (col!=NULL)
			{
				config->email.scolor.red = col->red;
				config->email.scolor.green = col->green;
				config->email.scolor.blue = col->blue;
				config->email.scolor.alpha = col->alpha;
				free (col);
			}
		}
		if (strcmp(var,"email_maximum_number")==0) config->email.max_nr = ps_strtoint (value);
		if (strcmp(var,"email_font_size")==0) config->email.fsize = ps_strtoint (value);
		if (strcmp(var,"email_server_name")==0) strcpy (config->email.server,value);
		if (strcmp(var,"email_server_port")==0) config->email.port = ps_strtoint (value);
		if (strcmp(var,"email_username")==0) strcpy (config->email.username, value);
		if (strcmp(var,"email_password")==0) strcpy (config->email.password, value);
		if (strcmp(var,"email_update_interval")==0) config->email.update = ps_strtoint(value);
		// language
		if (strcmp(var,"language_enabled")==0) config->lang.enabled = ps_strtoint (value);
		if (strcmp(var,"language_file")==0) strcpy (config->lang.file,value);
		// system
		if (strcmp(var,"system_shared_albums_path")==0) strcpy (config->system.shared_albums, value);
		if (strcmp(var,"system_albums_path")==0) strcpy (config->system.albums, value);
		#ifndef WIN32
		if (strcmp(var,"system_libs_path")==0) strcpy (config->system.libs, value);
		#endif
		if (strcmp(var,"system_shared_plugins_path")==0) strcpy (config->system.shared_plugins, value);
		if (strcmp(var,"system_plugins_path")==0) strcpy (config->system.plugins, value);
		if (strcmp(var,"system_screen_size_autodetect")==0) config->system.scr_size_autodetect = ps_strtoint (value);
		if (strcmp(var,"system_screen_width")==0) config->system.scr_width = ps_strtoint (value);
		if (strcmp(var,"system_screen_height")==0) config->system.scr_height = ps_strtoint (value);
		if (strcmp(var,"system_selected_album")==0) strcpy (config->system.selected_album, value);
	}
}

#ifndef PHOTOSTORM_WEBSITE

void ps_config_default ()
{
			config.wallpaper.cint = 2;
			config.wallpaper.dtype = 0;
			//config.wallpaper.gray = ps_check_button_get_value ((GtkWidget*)ps_get_widget_from (wallpaper, "bw"));
			config.wallpaper.dinfo = 1;
			strcpy (config.wallpaper.format, "%n\\n(C) %a");
			config.wallpaper.border = 0;
			config.wallpaper.pos.a = 1;
			config.wallpaper.pos.x = 0;
			config.wallpaper.pos.y = 0;
			_ps_color *col = ps_pack_color (255, 255, 255, 0);
			if (col!=NULL)
			{
				config.wallpaper.fcolor.red = col->red;
				config.wallpaper.fcolor.green = col->green;
				config.wallpaper.fcolor.blue = col->blue;
				config.wallpaper.fcolor.alpha = col->alpha;
				free (col);
			}
			config.wallpaper.fsize = 0;
		// calendar
			config.calendar.disp = 1;
			config.calendar.border = 0;
			config.calendar.pos.a = 1;
			config.calendar.pos.x = 0;
			config.calendar.pos.y = 0;
			col = ps_pack_color (255, 255, 0, 0);
			if (col!=NULL)
			{
				config.calendar.mcolor.red = col->red;
				config.calendar.mcolor.green = col->green;
				config.calendar.mcolor.blue = col->blue;
				config.calendar.mcolor.alpha = col->alpha;
				free (col);
			}
			col = ps_pack_color (255, 255, 255, 0);
			if (col!=NULL)
			{
				config.calendar.fcolor.red = col->red;
				config.calendar.fcolor.green = col->green;
				config.calendar.fcolor.blue = col->blue;
				config.calendar.fcolor.alpha = col->alpha;
				free (col);
			}
			config.calendar.cmfsize = 0;
			config.calendar.omfsize = 0;
			config.calendar.mark_date = 1;
			config.calendar.p_month = 0;
			config.calendar.n_month = 0;
		// quotes
			config.quotes.disp = 1;
			config.quotes.border = 0;
			config.quotes.pos.a = 1;
			config.quotes.pos.x = 0;
			config.quotes.pos.y = 0;
			col = ps_pack_color (255, 255, 255, 0);
			if (col!=NULL)
			{
				config.quotes.fcolor.red = col->red;
				config.quotes.fcolor.green = col->green;
				config.quotes.fcolor.blue = col->blue;
				config.quotes.fcolor.alpha = col->alpha;
				free (col);
			}
			config.quotes.fsize = 0;
			config.quotes.inet = 0;
			strcpy (config.quotes.server, "www.photostorm.net");
			config.quotes.port = 8201;
			strcpy (config.quotes.keywords, "");
			config.quotes.only_keywords = 0;
		// messages
			config.messages.disp = 0;
			config.messages.border = 0;
			config.messages.pos.a = 1;
			config.messages.pos.x = 0;
			config.messages.pos.y = 0;
			config.messages.max_nr = 10;
			col = ps_pack_color (255, 255, 0, 0);
			if (col!=NULL)
			{
				config.messages.tcolor.red = col->red;
				config.messages.tcolor.green = col->green;
				config.messages.tcolor.blue = col->blue;
				config.messages.tcolor.alpha = col->alpha;
				free (col);
			}
			col = ps_pack_color (255, 255, 255, 0);
			if (col!=NULL)
			{
				config.messages.mcolor.red = col->red;
				config.messages.mcolor.green = col->green;
				config.messages.mcolor.blue = col->blue;
				config.messages.mcolor.alpha = col->alpha;
				free (col);
			}
			config.messages.fsize = 0;
		// email
			config.email.disp = 0;
			config.email.border = 0;
			config.email.pos.a = 1;
			config.email.pos.x = 0;
			config.email.pos.y = 0;
			config.email.max_nr = 10;
			col = ps_pack_color (255, 255, 0, 0);
			if (col!=NULL)
			{
				config.email.fcolor.red = col->red;
				config.email.fcolor.green = col->green;
				config.email.fcolor.blue = col->blue;
				config.email.fcolor.alpha = col->alpha;
				free (col);
			}
			col = ps_pack_color (255, 255, 255, 0);
			if (col!=NULL)
			{
				config.email.scolor.red = col->red;
				config.email.scolor.green = col->green;
				config.email.scolor.blue = col->blue;
				config.email.scolor.alpha = col->alpha;
				free (col);
			}
			config.email.fsize = 0;
			strcpy (config.email.server, "");
			config.email.port = 110;
			strcpy (config.email.username, "");
			strcpy (config.email.password, "");
			config.email.update = 10;
		// language
			config.lang.enabled = 0;
			strcpy (config.lang.file, "");
		// system
		    #ifdef WIN32
		    sprintf (config.system.shared_albums, "%s\\Albums", ps_get_install_dir());
			sprintf (config.system.albums, "%s\\My PhotoStorm Albums", ps_get_user_docs_dir());
			sprintf (config.system.libs, "%s\\", ps_get_install_dir ());
			config.system.scr_size_autodetect = 1;
			LPDEVMODE lpDevMode = (LPDEVMODE)malloc (sizeof(DEVMODE));
			lpDevMode->dmSize=sizeof (DEVMODE);
			lpDevMode->dmDriverExtra = 0;
			if (EnumDisplaySettings (NULL, ENUM_CURRENT_SETTINGS, lpDevMode))
			{
			   config.system.scr_width = lpDevMode->dmPelsWidth;
			   config.system.scr_height = lpDevMode->dmPelsHeight;
			}
			else
			{
			   config.system.scr_width = 1024;
			   config.system.scr_height = 768;
		    }
			//printf ("install_dir %s\n", ps_get_install_dir ());
		    #else
			strcpy (config.system.shared_albums, "/usr/local/lib/photostorm/Albums/");
			strcpy (config.system.albums, "");
			sprintf (config.system.albums, "%s/PhotoStorm Albums", ps_get_user_dir());
			strcpy (config.system.libs, "/usr/local/lib/photostorm/");
			strcpy (config.system.shared_plugins, "/usr/local/lib/photostorm/Plugins/");
			strcpy (config.system.plugins, "");
			sprintf (config.system.plugins, "%s/.photostorm/Plugins", ps_get_user_dir());
			config.system.scr_width = 800;
			config.system.scr_height = 600;
			#endif
			#ifdef WIN32
			mkdir (config.system.albums);
			#else
			mkdir (config.system.albums, S_IRWXU);
			#endif
			strcpy (config.system.selected_album, "");
}

#endif

_ps_config * ps_load_config (const char *file)
{
	//_ps_config *result = (_ps_config*)malloc (sizeof(_ps_config));
	FILE *f;
	f = fopen (file, "r");
	if (f!=NULL)
	{
		char *data;
		char *head = strdup ("");
		while (!feof(f))
		{
			char *data1 = readline (f);
			data = strtrim (data1);
			if (data1!=NULL) free (data1);
			if (data!=NULL)
			{
				if (data[0]=='[' && strlen (data)>1)
				{
					if (head!=NULL) free (head);
					head = strtok (&data[1],"]");
					if (head==NULL) head = strdup ("");
						else head = strdup (head);
				}
				else
				if (data[0]=='#');
				else
				{
					char *var = strtrim (strtok (data, "="));
					//strtok (NULL, "\"");
					char *val = strtrim (strtok (NULL, "\0"));
					if (var!=NULL && val!=NULL)
					{
						char *head_var = (char*)malloc (strlen(head)+strlen(var)+3);
						//printf ("%s=%s\n", var, val);
						sprintf (head_var, "%s_%s", head, var);
						ps_set_value (&config, head_var, val);
					}
					if (val!=NULL) free (val);
					if (var!=NULL) free (var);
				}
				free (data);
			}
		}
	} 
#ifdef PHOTOSTORM_WEBSITE	
	else return NULL;
#endif
	return &config;
}

_ps_config * ps_get_config ()
{
	return &config;
}

void ps_save_config (const char *file, _ps_config config)
{
	FILE *f = fopen (file, "w");
	if (f!=NULL)
	{
		//wallpaper
		fprintf (f, "[wallpaper]\n");
		fprintf (f, "interval = %d\n", config.wallpaper.cint);
		fprintf (f, "display_type = %d\n", config.wallpaper.dtype);
		//fprintf (f, "grayscale = %s\n", (config.wallpaper.gray)?"yes":"no");
		fprintf (f, "display_info = %s\n", (config.wallpaper.dinfo)?"yes":"no");
		fprintf (f, "info_format = %s\n", config.wallpaper.format);
		fprintf (f, "border = %s\n", (config.wallpaper.border)?"yes":"no");
		fprintf (f, "info_autoposition = %s\n", (config.wallpaper.pos.a)?"yes":"no");
		fprintf (f, "info_x = %d\n", config.wallpaper.pos.x);
		fprintf (f, "info_y = %d\n", config.wallpaper.pos.y);
		fprintf (f, "font_color = %s\n", ps_colortostr (config.wallpaper.fcolor));
		fprintf (f, "font_size = %d\n", config.wallpaper.fsize);
		fprintf (f,"\n");
		// calendar
		fprintf (f, "[calendar]\n");
		fprintf (f, "display = %s\n", (config.calendar.disp)?"yes":"no");
		fprintf (f, "border = %s\n", (config.calendar.border)?"yes":"no");
		fprintf (f, "autoposition = %s\n", (config.calendar.pos.a)?"yes":"no");
		fprintf (f, "position_x = %d\n", config.calendar.pos.x);
		fprintf (f, "position_y = %d\n", config.calendar.pos.y);
		fprintf (f, "mark_color = %s\n", ps_colortostr (config.calendar.mcolor));
		fprintf (f, "font_color = %s\n", ps_colortostr (config.calendar.fcolor));
		fprintf (f, "current_month_font_size = %d\n", config.calendar.cmfsize);
		fprintf (f, "other_months_font_size = %d\n", config.calendar.omfsize);
		fprintf (f, "mark_date = %s\n", (config.calendar.mark_date)?"yes":"no");
		fprintf (f, "past_months = %d\n", config.calendar.p_month);
		fprintf (f, "future_months = %d\n", config.calendar.n_month);
		fprintf (f, "\n");
		// quotes
		fprintf (f, "[quotes]\n");
		fprintf (f, "display = %s\n", (config.quotes.disp)?"yes":"no");
		fprintf (f, "border = %s\n", (config.quotes.border)?"yes":"no");
		fprintf (f, "autoposition = %s\n", (config.quotes.pos.a)?"yes":"no");
		fprintf (f, "position_x = %d\n", config.quotes.pos.x);
		fprintf (f, "position_y = %d\n", config.quotes.pos.y);
		fprintf (f, "font_color = %s\n", ps_colortostr (config.quotes.fcolor));
		fprintf (f, "font_size = %d\n", config.quotes.fsize);
		fprintf (f, "get_quotes_online = %s\n", (config.quotes.inet)?"yes":"no");
		fprintf (f, "server_name = %s\n", config.quotes.server);
		fprintf (f, "server_port = %d\n", config.quotes.port);
		fprintf (f, "keywords = %s\n", config.quotes.keywords);
		fprintf (f, "use_only_keywords = %s\n", (config.quotes.only_keywords)?"yes":"no");
		fprintf (f, "\n");
		// messages
		fprintf (f, "[messages]\n");
		fprintf (f, "display = %s\n", (config.messages.disp)?"yes":"no");
		fprintf (f, "border = %s\n", (config.messages.border)?"yes":"no");
		fprintf (f, "autoposition = %s\n", (config.messages.pos.a)?"yes":"no");
		fprintf (f, "position_x = %d\n", config.messages.pos.x);
		fprintf (f, "position_y = %d\n", config.messages.pos.y);
		fprintf (f, "maximum_number = %d\n", config.messages.max_nr);
		fprintf (f, "time_color = %s\n", ps_colortostr (config.messages.tcolor));
		fprintf (f, "message_color = %s\n", ps_colortostr (config.messages.mcolor));
		fprintf (f, "font_size = %d\n", config.messages.fsize);
		fprintf (f, "\n");
		// email
		fprintf (f, "[email]\n");
		fprintf (f, "display = %s\n", (config.email.disp)?"yes":"no");
		fprintf (f, "border = %s\n", (config.email.border)?"yes":"no");
		fprintf (f, "autoposition = %s\n", (config.email.pos.a)?"yes":"no");
		fprintf (f, "position_x = %d\n", config.email.pos.x);
		fprintf (f, "position_y = %d\n", config.email.pos.y);
		fprintf (f, "maximum_number = %d\n", config.email.max_nr);
		fprintf (f, "sender_color = %s\n", ps_colortostr (config.email.fcolor));
		fprintf (f, "subject_color = %s\n", ps_colortostr (config.email.scolor));
		fprintf (f, "font_size = %d\n", config.email.fsize);
		fprintf (f, "server_name = %s\n", config.email.server);
		fprintf (f, "server_port = %d\n", config.email.port);
		fprintf (f, "username = %s\n", config.email.username);
		fprintf (f, "password = %s\n", config.email.password);
		fprintf (f, "update_interval = %d\n", config.email.update);
		fprintf (f, "\n");
		// language
		fprintf (f, "[language]\n");
		fprintf (f, "enabled = %s\n", (config.lang.enabled)?"yes":"no");
		fprintf (f, "file = %s\n", config.lang.file);
		fprintf (f, "\n");
		// system
		//fprintf (f, "##### DO NOT EDIT DIRECTLY THIS FILE UNLESS YOU KNOW VERY WELL WHAT YOU ARE DOING!!!\n");
		fprintf (f, "[system]\n");
		fprintf (f, "shared_albums_path = %s\n", config.system.shared_albums);
		fprintf (f, "albums_path = %s\n", config.system.albums);
		fprintf (f, "### Do NOT EDIT the following lines unless you know very well what you are doing!!!\n");
		fprintf (f, "libs_path = %s\n", config.system.libs);
		fprintf (f, "shared_plugins_path = %s\n", config.system.shared_plugins);
		fprintf (f, "plugins_path = %s\n", config.system.plugins);
		fprintf (f, "screen_size_autodetect = %s\n", (config.system.scr_size_autodetect)?"yes":"no");
		fprintf (f, "screen_width = %d\n", config.system.scr_width);
		fprintf (f, "screen_height = %d\n", config.system.scr_height);
		fprintf (f, "selected_album = %s\n", config.system.selected_album);
		fclose (f);
	}
}

#ifndef PHOTOSTORM_WEBSITE

char * ps_get_config_filename ()
{
	char temp[10000];
	strcpy (temp, ps_get_user_dir());
	sprintf (temp,"%s%c%s%c%s", ps_get_user_dir(),ps_dir_sep,".photostorm",ps_dir_sep,"photostorm.cfg");
	return strdup (temp);
}

char * ps_get_albums_cache_filename ()
{
	char temp[10000];
	strcpy (temp, ps_get_user_dir());
	sprintf (temp,"%s%c%s%c%s", ps_get_user_dir(),ps_dir_sep,".photostorm",ps_dir_sep,"ps_albums.cache");
	return strdup (temp);
}

char * ps_get_plugins_cache_filename ()
{
	char temp[10000];
	strcpy (temp, ps_get_user_dir());
	sprintf (temp,"%s%c%s%c%s", ps_get_user_dir(),ps_dir_sep,".photostorm",ps_dir_sep,"ps_plugins.cache");
	return strdup (temp);
}

char * ps_get_messages_filename ()
{
	char temp[10000];
	strcpy (temp, ps_get_user_dir());
	sprintf (temp,"%s%c%s%c%s", ps_get_user_dir(),ps_dir_sep,".photostorm",ps_dir_sep,"messages.data");
	return strdup (temp);
}

char * ps_get_config_path ()
{
	char temp[10000];
	strcpy (temp, ps_get_user_dir());
	sprintf (temp,"%s%c%s%c", ps_get_user_dir(),ps_dir_sep,".photostorm",ps_dir_sep);
	return strdup (temp);
}

#endif

#ifndef PHOTOSTORM_APPLET

void load_albums_cache (const char *album_file)
{
	FILE *f = fopen (album_file, "r");
	if (f!=NULL)
	{
		while (!feof(f))
		{
			char *data1 = readline (f);
			char *data = strtrim (data1);
			char *a = strtok (data, " ");
			if (data!=NULL && a!=NULL)
			{
				int activ;
				sscanf (a, "%d", &activ);
				char *album_name = strtok (NULL, "\0");
				ps_set_album (album_name, activ);
				free (data);
			}
			if (data1!=NULL) free (data1);
		}
		fclose (f);
	}
}

void save_albums_cache (const char *albums_file)
{
	FILE *f = fopen (albums_file, "w");
	if (f!=NULL)
	{
		fprintf (f, "# THIS FILE AUTOMATICLY GENERATED BY PHTOOSTORM. DO NOT EDIT IT !!!\n");
		ps_write_albums_info (f);
		fclose (f);
	}
}

#endif

void load_plugins_cache (const char *plugins_file)
{
	FILE *f = fopen (plugins_file, "r");
	if (f!=NULL)
	{
		while (!feof(f))
		{
			char *data1 = readline (f);
			char *data = strtrim (data1);
			char *a = strtok (data, " ");
			if (data!=NULL && a!=NULL)
			{
				int activ;
				sscanf (a, "%d", &activ);
				char *plugin_filename = strtok (NULL, "\0");
				if (plugin_filename!=NULL)
				{
					GList *ps_plugins_list = get_ps_plugins_list ();
					int nr = g_list_length (ps_plugins_list);
					int gasit = 0;
					for (int j=0;j<nr && !gasit;j++)
					{
						_ps_plugin *plugin = (_ps_plugin*)g_list_nth_data (ps_plugins_list, j);
						if (plugin!=NULL && plugin->filename!=NULL)
						{
							if (strcmp (plugin->filename, plugin_filename)==0)
							{
								plugin->enabled = (activ!=0)?1:0;
								gasit = 1;
								if (plugin->enabled==1)
								{
									plugin_init (plugin);
								}
								else plugin->enabled = 0;
							}
						}
					}
				}
				free (data);
			}
			if (data1!=NULL) free (data1);
		}
		fclose (f);
	}
}

void save_plugins_cache (const char *plugins_file)
{
	FILE *f = fopen (plugins_file, "w");
	if (f!=NULL)
	{
		fprintf (f, "# THIS FILE AUTOMATICLY GENERATED BY PHTOOSTORM. DO NOT EDIT IT !!!\n");
		GList *ps_plugins_list = get_ps_plugins_list ();
		int nr = g_list_length (ps_plugins_list);
		int gasit = 0;
		for (int j=0;j<nr && !gasit;j++)
		{
			_ps_plugin *plugin = (_ps_plugin*)g_list_nth_data (ps_plugins_list, j);
			if (plugin!=NULL && plugin->filename!=NULL)
			{
				fprintf (f, "%d %s\n", plugin->enabled, plugin->filename);
			}
		}
		fclose (f);
	}
}
