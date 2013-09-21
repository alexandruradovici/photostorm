
#include "ps_dialogs.h"
#include "data.h"
#include "design.h"
#include "configs.h"
#include "misc.h"
#include "lang/translate.h"
#include "ps_system.h"
#include "messages.h"
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

char * open_dialog_path = NULL;

/*struct _ps_dialog_result
{
	GtkWidget *dialog;
	GtkWidget *parent;
	_cb_ps_open_dialog_result *functie;
};*/

char *last_path="";

void cb_ps_question_dialog_result (GtkButton *buton, gpointer data)
{
	_ps_question_dialog *dialog = (_ps_question_dialog*)data;
	if (buton == ps_get_widget_from (dialog->window, "ps_result_yes")) dialog->functie (dialog->id, PS_BUTTON_YES);
	if (buton == ps_get_widget_from (dialog->window, "ps_result_no")) dialog->functie (dialog->id, PS_BUTTON_NO);
	gtk_widget_destroy (dialog->window);
	free (dialog);
}

void cb_ps_dialog_close (GtkButton *buton, gpointer data)
{	
	gtk_widget_destroy ((GtkWidget*)data);
}

void cb_ps_wizard_goto_next_page (GtkButton *buton, gpointer data)
{
	if (data!=NULL)
	{
		GtkWidget *window = ((_ps_wizard*)(data))->window;
		GtkNotebook *notes = (GtkNotebook*)ps_get_widget_from (window, "ps_wizard_notes");
		if (notes!=NULL)
		{
			int n=gtk_notebook_get_n_pages ((GtkNotebook*)notes);
			int i=gtk_notebook_get_current_page ((GtkNotebook*)notes);
			int b=1;
			if (i==0) b = 0;
			if (i==n-1) b = 3;
			int jump=0;
			if ((jump=((_ps_wizard*)(data))->actionf ((_ps_wizard*)data, i, b)))
			{	
				i=i+jump;
				if (i==n)
				{
					gtk_widget_destroy (window);
				} 
				else
				if (i>-1 && i<n)
				{
					if (i>0) gtk_widget_set_sensitive ((GtkWidget*)ps_get_widget_from (window, "ps_wizard_bback"), TRUE);
					if (i==0) gtk_label_set_text ((GtkLabel*)ps_get_widget_from(buton, "str"), ((_ps_wizard*)data)->b_begin);
					if ((i-1)==0) gtk_label_set_text ((GtkLabel*)ps_get_widget_from(buton, "str"), ((_ps_wizard*)data)->b_cont);
					if (i==(n-1)) 
					{
						gtk_image_set_from_stock ((GtkImage*)ps_get_widget_from (buton, "img"), "gtk-ok", GTK_ICON_SIZE_BUTTON);
						gtk_label_set_text ((GtkLabel*)ps_get_widget_from(buton, "str"), ((_ps_wizard*)data)->b_done);
					}
					else
					{
						gtk_image_set_from_stock ((GtkImage*)ps_get_widget_from (buton, "img"), "gtk-go-forward", GTK_ICON_SIZE_BUTTON);
						gtk_label_set_text ((GtkLabel*)ps_get_widget_from(buton, "str"), ((_ps_wizard*)data)->b_cont);
					}
					gtk_notebook_set_current_page ((GtkNotebook*)notes, i);
				}
			}
		}
	}
}

void cb_ps_wizard_goto_previous_page (GtkButton *buton, gpointer data)
{
	if (data!=NULL)
	{
		GtkWidget *window = ((_ps_wizard*)data)->window;
		GtkNotebook *notes = (GtkNotebook*)ps_get_widget_from (window, "ps_wizard_notes");
		if (notes!=NULL)
		{
			int n=gtk_notebook_get_n_pages ((GtkNotebook*)notes);
			int i=gtk_notebook_get_current_page ((GtkNotebook*)notes);
			int b = 2;
			int jump=0;
			if ((jump=((_ps_wizard*)(data))->actionf ((_ps_wizard*)data, i, b)))
			{
				i=i+jump;
				if (i>-1)
				{
					if (i==0) gtk_widget_set_sensitive ((GtkWidget*)buton, FALSE);
					if (i<(n-1)) 
					{
						GtkWidget *b=(GtkWidget*)ps_get_widget_from(window,"ps_wizard_bcont");
						if (i==0) gtk_label_set_text ((GtkLabel*)ps_get_widget_from(b, "str"), ((_ps_wizard*)data)->b_begin);
						else gtk_label_set_text ((GtkLabel*)ps_get_widget_from(b, "str"), ((_ps_wizard*)data)->b_cont);
						gtk_image_set_from_stock ((GtkImage*)ps_get_widget_from (b, "img"), "gtk-go-forward", GTK_ICON_SIZE_BUTTON);
					}
					gtk_notebook_set_current_page ((GtkNotebook*)notes, i);
				}
			}
		}
	}
}

/*void cb_ps_send_and_close_open_dialog (GtkButton *buton, gpointer data)
{
	_ps_dialog_result *dialog_result = (_ps_dialog_result*)data;
	if (dialog_result!=NULL)
	{
		//if (open_dialog_path!=NULL) free (open_dialog_path);
		open_dialog_path = (char*)gtk_file_selection_get_filename ((GtkFileSelection*)dialog_result->dialog);
		dialog_result->functie (dialog_result->parent, (char*)gtk_file_selection_get_filename ((GtkFileSelection*)dialog_result->dialog));
		gtk_widget_destroy (dialog_result->dialog);
		free (dialog_result);
	}
}

void cb_ps_close_open_dialog (GtkButton *buton, gpointer data)
{
	_ps_dialog_result *dialog_result = (_ps_dialog_result*)data;
	if (dialog_result!=NULL)
	{
		//if (open_dialog_path!=NULL) free (open_dialog_path);
		open_dialog_path = (char*)gtk_file_selection_get_filename ((GtkFileSelection*)dialog_result->dialog);
		gtk_widget_destroy (dialog_result->dialog);
		free (dialog_result);
	}
}*/

void cb_ps_wizard_exit (GtkButton *buton, gpointer data)
{
	gtk_widget_destroy (((_ps_wizard*)(data))->window);
	free ((_ps_wizard*)data);
}

void cb_ps_close_album_properties_dialog (GtkWidget *button, gpointer data)
{
	_ps_album_properties *props = (_ps_album_properties*)data;
	if (props!=NULL)
	{
		if (ps_get_widget_from (props->window, "ps_album_properties_b_ok") == button)
		{
			GtkWidget *nume = (GtkWidget*)ps_get_widget_from (props->window, "ps_album_properties_nume_edit");
			GtkWidget *autor = (GtkWidget*)ps_get_widget_from (props->window, "ps_album_properties_autor_edit");
			GtkWidget *categoria = (GtkWidget*)ps_get_widget_from (props->window, "ps_album_properties_categoria_edit");
			GtkWidget *descriere = (GtkWidget*)ps_get_widget_from (props->window, "ps_album_properties_descriere_edit");
			_ps_album *date_album = props->date_album;
			if (date_album->nume!=NULL) free (date_album->nume);
			if (date_album->autor!=NULL) free (date_album->autor);
			if (date_album->categoria!=NULL) free (date_album->categoria);
			if (date_album->descriere!=NULL) free (date_album->descriere);
			date_album->nume = ps_get_text_from_entry ((GtkEditable*)nume);
			date_album->autor = ps_get_text_from_entry ((GtkEditable*)autor);
			date_album->categoria = ps_get_trimmed_text_from_entry ((GtkEditable*)((GtkCombo*)(categoria))->entry);
			date_album->descriere = ps_get_text_from_text_view ((GtkTextView*)descriere);
			if (date_album!=NULL)
			{
				if (props->tip_album==0) 
				{	
					ps_write_album_from_memory (ps_get_current_album (), date_album);
					char *albumul = ps_get_current_album ();
					ps_clear_albums_list ();
					_ps_config *config = ps_get_config ();
					ps_load_albums_from_path (config->system.shared_albums);
					ps_load_albums_from_path (config->system.albums);	
					//printf ("(%s)\n", albumul);
					selecteaza_album (albumul);
					if (albumul!=NULL) free (albumul);
					//ps_list_album_from_file (ps_get_current_album ());
				}
			}
		}
		if ((GtkWidget*)props->window!=NULL) gtk_widget_destroy ((GtkWidget*)props->window);
		ps_album_properties_free (props);
	}
}

void cb_ps_close_photo_properties_dialog (GtkWidget *button, gpointer data)
{
	_ps_photo_properties *props = (_ps_photo_properties*)data;
	if (props!=NULL)
	{
		if (ps_get_widget_from (props->window, "ps_photo_properties_b_ok") == button)
		{
			GtkWidget *nume = (GtkWidget*)ps_get_widget_from (props->window, "ps_photo_properties_nume_edit");
			GtkWidget *autor = (GtkWidget*)ps_get_widget_from (props->window, "ps_photo_properties_autor_edit");
			GtkWidget *descriere = (GtkWidget*)ps_get_widget_from (props->window, "ps_photo_properties_descriere_edit");
			GtkWidget *disp = (GtkWidget*)ps_get_widget_from (props->window, "photo_disp_menu");
			_ps_photo *date_poza = props->date_poza;
			if (date_poza->nume!=NULL) free (date_poza->nume);
			if (date_poza->autor!=NULL) free (date_poza->autor);
			if (date_poza->descriere!=NULL) free (date_poza->descriere);
			date_poza->nume = ps_get_text_from_entry ((GtkEditable*)nume);
			date_poza->autor = ps_get_text_from_entry ((GtkEditable*)autor);
			date_poza->descriere = ps_get_text_from_text_view ((GtkTextView*)descriere);
			strcpy (date_poza->optiuni, "A");
			sprintf (date_poza->optiuni, "%c", 'A'+ps_option_menu_get_index (disp));
			if (date_poza!=NULL)
			{
				if (props->tip_album==0) 
				{	
					ps_modify_photo_nr_from_album_file (date_poza, ps_get_current_album (), props->photo_nr);  
					ps_list_album_from_file (ps_get_current_album ());
				}
			}
		}
		if ((GtkWidget*)props->window!=NULL) gtk_widget_destroy ((GtkWidget*)props->window);
		ps_photo_properties_free (props);
	}
}

void ps_refresh_messages_list (GtkWidget *window);

void cb_ps_close_message_edit_dialog (GtkWidget *button, gpointer data)
{
	_ps_message_edit * message_edit_dialog_data = (_ps_message_edit*)data;
	if (message_edit_dialog_data!=NULL)
	{
		GtkWidget *window = message_edit_dialog_data->window; 
		if (window!=NULL)
		{
			GtkWidget *b_msg_ok = (GtkWidget*)ps_get_widget_from (window, "b_msg_ok");
			if (button == b_msg_ok)
			{
				_ps_message *message = message_edit_dialog_data->message;
				GtkWidget *calendar = (GtkWidget*)ps_get_widget_from (window, "calendar");
				GtkWidget *d_day = (GtkWidget*)ps_get_widget_from (window, "d_day");
				GtkWidget *d_week = (GtkWidget*)ps_get_widget_from (window, "d_week");
				GtkWidget *d_month = (GtkWidget*)ps_get_widget_from (window, "d_month");
				GtkWidget *d_year = (GtkWidget*)ps_get_widget_from (window, "d_year");
				GtkWidget *m_time_edit = (GtkWidget*)ps_get_widget_from (window, "m_time_edit");
				GtkWidget *m_text = (GtkWidget*)ps_get_widget_from (window, "m_text");
				unsigned int year, month, date, wday, hour, minute;
				gtk_calendar_get_date ((GtkCalendar*)calendar, &year, &month, &date);
				time_t tmp;
				time (&tmp);
				tm *mytime = localtime (&tmp);
				mytime->tm_year = year-1900;
				mytime->tm_mon = month;
				mytime->tm_mday = date;
				tmp = mktime (mytime);
				mytime = localtime (&tmp);
				wday = mytime->tm_wday;
				if (wday==0) wday=7;
				month=month+1;
				hour = ps_time_edit_get_hour (m_time_edit);
				minute = ps_time_edit_get_minute (m_time_edit);
				char * message_text = ps_edit_get_text (m_text);
				int d_day_state = ps_check_button_get_value (d_day);
				int d_week_state = ps_check_button_get_value (d_week);
				int d_month_state = ps_check_button_get_value (d_month);
				int d_year_state = ps_check_button_get_value (d_year);
				if (d_day_state==TRUE) 
				{ 
					date=0; 
					wday=0; 
				}
				if (d_week_state==FALSE && date!=0) wday=0;
				if (d_week_state==TRUE) date = 0;
				if (d_month_state==TRUE) month=0;
				if (d_year_state==TRUE) year=0;
				_ps_message *message_new = create_message (date, month, year, wday, hour, minute, message_text);
				if (message_text!=NULL) free (message_text);
				GList *ps_messages_list = get_messages_list ();
				if (message!=NULL && strcmp(strtrim(message_new->message_text),"")!=0)
				{
					ps_messages_list = g_list_remove (ps_messages_list, message);
					free (message);
				}
				if (strcmp(strtrim(message_new->message_text),"")!=0)
				{
					ps_messages_list = g_list_append (ps_messages_list, message_new);
					ps_messages_list = g_list_sort (ps_messages_list, (GCompareFunc)messages_list_time_comparator);
					set_messages_list (ps_messages_list);
				}
				ps_refresh_messages_list ((GtkWidget*)gtk_window_get_transient_for ((GtkWindow*)window));
			}
			gtk_widget_destroy (window);
		}
		free (message_edit_dialog_data);
	}
}

void cb_ps_close_messages_edit_dialog_and_save_messages (GtkWidget *button, gpointer data)
{
	ps_save_messages ();
	GtkWindow *parent = gtk_window_get_transient_for ((GtkWindow*)data);
	if (parent==NULL) ps_wallpaper_update (true);
	else 
	{
		ps_wallpaper_update (false);
		if ((GtkWidget*)data!=NULL) gtk_widget_destroy ((GtkWidget*)data);
	}
}

void cb_ps_close_messages_edit_dialog (GtkWidget *button, gpointer data)
{
	if ((GtkWidget*)data!=NULL) gtk_widget_destroy ((GtkWidget*)data);
	GList *ps_messages_list = get_messages_list ();
	g_list_free (ps_messages_list);
	set_messages_list (NULL);
	ps_load_messages ();
}

char * ps_get_trimmed_text_from_entry (GtkEditable *edit)
{
	char *result=NULL;
	if (edit!=NULL)
	{
		char *s_ = gtk_editable_get_chars (edit, 0, -1);
		if (s_!=NULL) 
		{	
			char *s = strtrim (s_);
			if (s!=NULL) result = s;
			free (s_);
		}
	}
	return result;
}

char * ps_get_text_from_entry (GtkEditable *edit)
{
	char *result=NULL;
	if (edit!=NULL)	result = gtk_editable_get_chars (edit, 0, -1);
	return result;
}

char * ps_get_text_from_text_view (GtkTextView *textview)
{
	char * result = NULL;
	GtkTextBuffer *buf = gtk_text_view_get_buffer (textview);
	if (buf!=NULL)
	{
		GtkTextIter buf_start;
		gtk_text_buffer_get_start_iter (buf, &buf_start);
		GtkTextIter buf_end;
		gtk_text_buffer_get_end_iter (buf, &buf_end);
		result = gtk_text_buffer_get_text (buf, &buf_start, &buf_end, TRUE);
	}
	return result;
}

/*GtkWidget * ps_create_open_dialog (char *calea, GtkWidget *parent, _cb_ps_open_dialog_result functie)
{
	if (calea==NULL) calea = open_dialog_path;
	GtkWidget *dialog;
	dialog = gtk_file_selection_new (__("Select Files"));
	if (calea!=NULL) gtk_file_selection_set_filename ((GtkFileSelection*)dialog, calea);
	gtk_file_selection_hide_fileop_buttons ((GtkFileSelection*)dialog);
	gtk_window_set_modal ((GtkWindow*)dialog, TRUE);
	GtkWidget *buton_ok = ((GtkFileSelection*)dialog)->ok_button;
	gtk_button_set_label ((GtkButton*)buton_ok, __("Open"));
	GtkWidget *buton_cancel = ((GtkFileSelection*)dialog)->cancel_button;
	gtk_button_set_label ((GtkButton*)buton_cancel, __("Cancel"));
	_ps_dialog_result *dialog_result = (_ps_dialog_result*)malloc (sizeof (_ps_dialog_result));
	dialog_result->dialog = dialog;
	dialog_result->parent=parent;
	dialog_result->functie = functie;
	g_signal_connect (buton_ok, "clicked", (GCallback)cb_ps_send_and_close_open_dialog, dialog_result);
	g_signal_connect (buton_cancel, "clicked", (GCallback)cb_ps_close_open_dialog, dialog_result);
	return dialog;
}*/

GtkWidget * ps_create_wizard_dialog (_ps_wizard *date)
{
	GtkWidget *window=NULL;
	GtkAccelGroup *accel_group = gtk_accel_group_new ();
	if (date!=NULL)
	{
		if (date->dx<0 || date->dx > 1600) date->dx=450;
		if (date->dy<0 || date->dy > 1200) date->dy=250;
		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		date->window=window;
		gtk_window_set_title ((GtkWindow*)window, date->name);
		gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
		//gtk_widget_set_size_request (window, date->dx, date->dy);
		gtk_window_set_resizable ((GtkWindow*)window, FALSE);
		gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
		gtk_window_set_modal ((GtkWindow*)window, TRUE);
		gtk_window_set_transient_for ((GtkWindow*)window, (GtkWindow*)date->ps_main_window);
		/*pict=ps_create_pixbuf_from_file (pict_path ("picts/photostorm.png"));
		if (pict!=NULL) 
		{	
			gtk_window_set_icon ((GtkWindow*)window, pict);
			gdk_pixbuf_unref (pict);
		}*/
		gtk_container_set_border_width ((GtkContainer*)window, 7);
			
		GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
		gtk_container_add ((GtkContainer*)window, vbox);		
	
		GtkWidget *hbox = gtk_hbox_new (FALSE, 2);
		GtkWidget *border_imagine = gtk_viewport_new (NULL, NULL);
		gtk_widget_set_size_request (border_imagine, -1, 240);
		gtk_viewport_set_shadow_type ((GtkViewport*)border_imagine, GTK_SHADOW_IN);
		int nr_jpg = ps_random (2)+1;
		char temp[100];
		sprintf (temp, "logos/wizard_%d.jpg", nr_jpg);
		GtkWidget *imagine = ps_create_image_from_file (pict_path (temp));
		gtk_container_add ((GtkContainer*)border_imagine, imagine);
		gtk_box_pack_start ((GtkBox*)hbox, border_imagine, FALSE, TRUE, 2);
		
		GtkWidget *pagini = gtk_notebook_new ();
		ps_hookup_object (window, pagini, "ps_wizard_notes");
	
		gtk_notebook_set_show_tabs ((GtkNotebook*)pagini, FALSE);
		gtk_notebook_set_show_border ((GtkNotebook*)pagini, FALSE);
		gtk_box_pack_start ((GtkBox*)hbox, pagini, TRUE, TRUE, 2);
		
		gtk_box_pack_start ((GtkBox*)vbox, hbox, TRUE, TRUE, 2);
	
		GtkWidget *separator = gtk_separator_menu_item_new ();
		gtk_box_pack_start ((GtkBox*)vbox, separator, FALSE, TRUE, 0);
	
		GtkSizeGroup *grup = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkWidget *hbox_jos = gtk_hbox_new (FALSE, 10);
		GtkWidget *bcont = ps_create_button_with_image_from_stock ("gtk-go-forward", date->b_begin, 1);
		gtk_size_group_add_widget (grup, bcont);
		ps_hookup_object (window, bcont, "ps_wizard_bcont");
		gtk_box_pack_end ((GtkBox*)hbox_jos, bcont, FALSE, FALSE, 2);
		g_signal_connect (bcont, "clicked", (GCallback)cb_ps_wizard_goto_next_page, date);
		gtk_widget_add_accelerator (bcont, "clicked", accel_group, GDK_Return, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	
		GtkWidget *bback = ps_create_button_with_image_from_stock ("gtk-go-back", date->b_back, 1);
		gtk_size_group_add_widget (grup, bback);
		ps_hookup_object (window, bback, "ps_wizard_bback");
		gtk_box_pack_end ((GtkBox*)hbox_jos, bback, FALSE, FALSE, 2);
		gtk_widget_set_sensitive (bback, FALSE);
		g_signal_connect (bback, "clicked", (GCallback)cb_ps_wizard_goto_previous_page, date);
	
		GtkWidget *bcancel = ps_create_button_with_image_from_stock ("gtk-cancel", date->b_cancel, 1);
		gtk_size_group_add_widget (grup, bcancel);
		gtk_box_pack_start ((GtkBox*)hbox_jos, bcancel, FALSE, FALSE, 2);
		g_signal_connect (bcancel, "clicked", (GCallback)cb_ps_wizard_exit, date);
		gtk_widget_add_accelerator (bcancel, "clicked", accel_group, GDK_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	
		gtk_box_pack_start ((GtkBox*)vbox, hbox_jos, FALSE, FALSE, 2);
		
		GTK_WIDGET_SET_FLAGS (bcont, GTK_CAN_DEFAULT);
		gtk_widget_grab_default (bcont);
		gtk_window_add_accel_group ((GtkWindow*)window, accel_group);
	}
	return window;
}

_ps_wizard * ps_create_wizard (char *title, char *bbegin, char *bcont, char *bback, char *bdone, char *bcancel, int (*functie) (_ps_wizard *data, int page, int button), int dx, int dy)
{
	_ps_wizard *wizard=NULL;
	if (title && bbegin && bcont && bback && bdone && bcancel && functie)
	{
		wizard = (_ps_wizard*)malloc (sizeof (_ps_wizard));
		if (wizard!=NULL)
		{
			wizard->name = strdup (title);
			wizard->b_begin = strdup (bbegin);
			wizard->b_cont = strdup (bcont);
			wizard->b_back = strdup (bback);
			wizard->b_cancel = strdup (bcancel);
			wizard->b_done = strdup (bdone);
			wizard->actionf = functie;
			wizard->path = strdup (last_path);
			wizard->dx=dx;
			wizard->dy=dy;
			wizard->ps_main_window = ps_get_main_window ();
		}
	}
	return wizard;
}

void ps_add_wizard_page (_ps_wizard *wizard, GtkWidget *page)
{
	if (wizard!=NULL && page!=NULL)
	{
		GtkNotebook *notes = (GtkNotebook*)ps_get_widget_from (((_ps_wizard*)wizard)->window, "ps_wizard_notes");
		if (notes!=NULL)
		{
			gtk_notebook_append_page (notes, page, NULL);
		}
	}
}

void ps_question_dialog (char *poza, int stock, char *text, GtkWidget *parent, int id, _cb_ps_question_dialog_result *functie)
{
	_ps_question_dialog *dialog = (_ps_question_dialog*)malloc (sizeof(_ps_question_dialog));
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkAccelGroup *accel_group = gtk_accel_group_new ();
	dialog->window = window;
	dialog->id = id;
	dialog->functie = functie;
	gtk_window_set_title ((GtkWindow*)window, __("Question"));
	gtk_container_set_border_width ((GtkContainer*)window, 5);
	gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
	gtk_window_set_resizable ((GtkWindow*)window, FALSE);
	gtk_window_set_modal ((GtkWindow*)window, TRUE);
	gtk_window_set_transient_for ((GtkWindow*)window, (GtkWindow*)parent);
	GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add ((GtkContainer*)window, vbox);
	
	GtkWidget *intrebare;
	if (stock) intrebare = ps_create_mascot_header_with_image_from_stock (poza, text);
		else intrebare = ps_create_mascot_header (poza, text);
	ps_hookup_object (window, intrebare, "intrebare");
	gtk_box_pack_start ((GtkBox*)vbox, intrebare, TRUE, TRUE, 2);
	
	GtkWidget *separator = gtk_separator_menu_item_new ();
	gtk_box_pack_start ((GtkBox*)vbox, separator, FALSE, TRUE, 0);
	
	GtkSizeGroup *grup = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	GtkWidget *hbox_jos = gtk_hbox_new (FALSE, 10);
	GtkWidget *byes = ps_create_button_with_image_from_stock ("gtk-yes", __("Yes"), 1);
	gtk_size_group_add_widget (grup, byes);
	ps_hookup_object (window, byes, "ps_result_yes");
	gtk_box_pack_end ((GtkBox*)hbox_jos, byes, FALSE, FALSE, 2);
	g_signal_connect (byes, "clicked", (GCallback)cb_ps_question_dialog_result, dialog);
	gtk_widget_add_accelerator (byes, "clicked", accel_group, GDK_Return, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	
	GtkWidget *bno = ps_create_button_with_image_from_stock ("gtk-no", __("No"), 1);
	gtk_size_group_add_widget (grup, bno);
	ps_hookup_object (window, bno, "ps_result_no");
	gtk_box_pack_end ((GtkBox*)hbox_jos, bno, FALSE, FALSE, 2);
	g_signal_connect (bno, "clicked", (GCallback)cb_ps_question_dialog_result, dialog);
	gtk_widget_add_accelerator (bno, "clicked", accel_group, GDK_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_box_pack_start ((GtkBox*)vbox, hbox_jos, FALSE, FALSE, 2);
	gtk_window_add_accel_group ((GtkWindow*)window, accel_group);
	gtk_widget_show_all (window);
}

void ps_information_dialog (char *poza, int stock, char *text, GtkWidget *parent)
{
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkAccelGroup *accel_group = gtk_accel_group_new ();
	gtk_window_set_title ((GtkWindow*)window, __("Information"));
	gtk_container_set_border_width ((GtkContainer*)window, 5);
	gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
	gtk_window_set_resizable ((GtkWindow*)window, FALSE);
	gtk_window_set_modal ((GtkWindow*)window, TRUE);
	gtk_window_set_transient_for ((GtkWindow*)window, (GtkWindow*)parent);
	GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add ((GtkContainer*)window, vbox);
	
	GtkWidget *intrebare;
	if (stock) intrebare = ps_create_mascot_header_with_image_from_stock (poza, text);
		else intrebare = ps_create_mascot_header (poza, text);
	gtk_box_pack_start ((GtkBox*)vbox, intrebare, TRUE, TRUE, 2);
	
	GtkWidget *separator = gtk_separator_menu_item_new ();
	gtk_box_pack_start ((GtkBox*)vbox, separator, FALSE, TRUE, 0);
	
	GtkSizeGroup *grup = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	GtkWidget *hbox_jos = gtk_hbox_new (FALSE, 10);
	GtkWidget *bokey = ps_create_button_with_image_from_stock ("gtk-ok", __("Okey"), 1);
	gtk_size_group_add_widget (grup, bokey);
	ps_hookup_object (window, bokey, "ps_information_close");
	gtk_box_pack_end ((GtkBox*)hbox_jos, bokey, FALSE, FALSE, 2);
	g_signal_connect (bokey, "clicked", (GCallback)cb_ps_dialog_close, window);
	gtk_widget_add_accelerator (bokey, "clicked", accel_group, GDK_Return, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (bokey, "clicked", accel_group, GDK_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	
	/*GtkWidget *bno = ps_create_button_with_image_from_stock ("gtk-no", __("No"), 1);
	gtk_size_group_add_widget (grup, bno);
	ps_hookup_object (window, bno, "ps_question_no");
	gtk_box_pack_end ((GtkBox*)hbox_jos, bno, FALSE, FALSE, 2);
	g_signal_connect (bno, "clicked", (GCallback)cb_ps_dialog_close, window);*/
	gtk_box_pack_start ((GtkBox*)vbox, hbox_jos, FALSE, FALSE, 2);
	gtk_window_add_accel_group ((GtkWindow*)window, accel_group);
	gtk_widget_show_all (window);
}

static gint ps_progress_update (gpointer data)
{
	gboolean result = FALSE;
	_ps_progress *progress = (_ps_progress*)data;
	if (progress!=NULL)
	{
		GtkWidget *window = progress->window;
		if (GTK_IS_WINDOW(window))
		{
			GtkWidget *info = (GtkWidget*)ps_get_widget_from (window, "ps_progress_text");
			char *str = (char*)gtk_label_get_text ((GtkLabel*)info);
			if ((GtkLabel*)info!=NULL && str!=NULL && progress->text!=NULL)
			{
				if (strcmp (str, progress->text)!=0) gtk_label_set_text ((GtkLabel*)info, progress->text);
			}
			GtkWidget *bara = (GtkWidget*)ps_get_widget_from (window, "ps_progress_bar");
			gtk_progress_set_percentage ((GtkProgress*)bara, (double)progress->position/100);
			result = !progress->close;
			if (progress->close==TRUE) 
			{	
				gtk_widget_destroy (window);
				if (progress->quit) gtk_main_quit ();
				ps_progress_free (progress);
			}
		}
		else result = FALSE;
	}
	return result;
}

gboolean cb_ps_progress_dialog_close_event (GtkWidget *widget, GdkEvent *ev, gpointer data)
{
	gboolean result = TRUE;
	_ps_progress *progress = (_ps_progress*)data;
	if (progress!=NULL)
	{
		if (progress->quit) result = FALSE;
	}
	return result;
}

_ps_progress * ps_progress_dialog (GtkWidget *parent, void *data)
{
	_ps_progress *progress = (_ps_progress*)malloc (sizeof (_ps_progress));
	if (progress!=NULL)
	{
		GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title ((GtkWindow*)window, __("Working..."));
		gtk_window_set_modal ((GtkWindow*)window, TRUE);
		gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
		gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
		gtk_container_set_border_width ((GtkContainer*)window, 10);
		gtk_widget_set_size_request (window, 240, 120);
		gtk_window_set_transient_for ((GtkWindow*)window, (GtkWindow*)parent);
		GtkWidget *vbox;
		vbox= gtk_vbox_new (FALSE, FALSE);
		gtk_box_pack_start ((GtkBox*)vbox, ps_create_mascot_header_with_image_from_stock ("gtk-dialog-info", __("Please Wait...")), FALSE, FALSE, 2);
		/*GtkWidget *contur_bara;
		contur_bara = gtk_viewport_new (NULL, NULL);
		gtk_viewport_set_shadow_type ((GtkViewport*)contur_bara, GTK_SHADOW_NONE);*/
		GtkWidget *bara;
		bara = gtk_progress_bar_new ();
		//gtk_container_add ((GtkContainer*)contur_bara, bara);
		gtk_box_pack_start ((GtkBox*)vbox, bara, FALSE, FALSE, 2);
		ps_hookup_object (window, bara, "ps_progress_bar");
		GtkWidget *info;
		info = gtk_label_new ("");
		ps_hookup_object (window, info, "ps_progress_text");
		gtk_box_pack_start ((GtkBox*)vbox, info, TRUE, TRUE, 2);
		gtk_container_add ((GtkContainer*)window, vbox);
		gtk_widget_show_all (window);
		progress->parent = parent;
		progress->window = window;
		progress->text = strdup ("");
		progress->position = 0;
		progress->data = data;
		progress->close = FALSE;
		g_signal_connect (window, "destroy-event", (GCallback)cb_ps_progress_dialog_close_event, progress);
		gtk_timeout_add (100, (GtkFunction)ps_progress_update, progress);
	}
	return progress;
}

void ps_progress_set_text (_ps_progress *progress, const char *str)
{
	if (progress!=NULL)
	{
		if (progress->text!=NULL) free (progress->text);
		progress->text = strdup (str);
	}
}

void ps_progress_set_progress (_ps_progress *progress, int p)
{
	if (p>=0 && p<=100 && progress!=NULL)
	{
		progress->position = p;
	}
}

void ps_progress_dialog_close (_ps_progress *progress)
{
	if (progress!=NULL)
	{
		progress->close = TRUE;
	}
}

GtkWidget * ps_album_properties_dialog (_ps_album_properties *props)
{
	GtkWidget *window = NULL;
	GtkAccelGroup *accel_group = gtk_accel_group_new ();
	if (props->date_album!=NULL)
	{
		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title ((GtkWindow*)window, __("Album Properties"));
		gtk_window_set_skip_pager_hint ((GtkWindow*)window, TRUE);
		gtk_window_set_skip_taskbar_hint ((GtkWindow*)window, TRUE);
		gtk_window_set_modal ((GtkWindow*)window, TRUE);
		gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
		gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
		gtk_container_set_border_width ((GtkContainer*)window, 5);
		GtkWidget *hbox = gtk_hbox_new (FALSE, 2);
		GtkWidget *vbox = gtk_vbox_new (FALSE, 2);
		GtkWidget *vbox_margine = gtk_vbox_new (FALSE, 2);
		/*GtkWidget *imagine = gtk_image_new_from_pixbuf (ps_create_pixbuf_from_memory (
								  		   				props->date_poza->date_preview, 
										   				props->date_poza->n_poza_preview));
		gtk_misc_set_alignment ((GtkMisc*)imagine, 0.0, 0.0);*/
		
		GtkWidget *imagine = gtk_image_new_from_file (pict_path ("char/add_photo.png"));
		gtk_misc_set_alignment ((GtkMisc*)imagine, 0.0, 0.0);
		gtk_box_pack_start ((GtkBox*)vbox_margine, imagine, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)vbox_margine, ps_create_album_props_label (props->date_album), FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)hbox, vbox_margine, FALSE, FALSE, 5);
		gtk_box_pack_start ((GtkBox*)hbox, vbox, TRUE, TRUE, 2);
		gtk_container_add ((GtkContainer*)window, hbox);
		//gtk_widget_set_size_request (window, 240, 120);
		
		
		GtkSizeGroup *label = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkSizeGroup *edit = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkWidget *frame_info = gtk_frame_new (__("Album Information"));
		GtkWidget *table = gtk_table_new (4, 2, FALSE);
		gtk_table_set_row_spacings ((GtkTable*)table, 5);
		gtk_table_set_col_spacings ((GtkTable*)table, 5);
		gtk_container_add ((GtkContainer*)frame_info, table);
		gtk_box_pack_start ((GtkBox*)vbox, frame_info, TRUE, TRUE, 2);
			
		GtkWidget *nume_album = gtk_label_new (__("Title"));
		gtk_misc_set_alignment ((GtkMisc*)nume_album, 0, 0);
		gtk_size_group_add_widget (label, nume_album);
		gtk_table_attach ((GtkTable*)table, nume_album, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 2, 2);
		GtkWidget *autor_album = gtk_label_new (__("Author"));
		gtk_misc_set_alignment ((GtkMisc*)autor_album, 0, 0);
		gtk_size_group_add_widget (label, autor_album);
		gtk_table_attach ((GtkTable*)table, autor_album, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 2, 2);
		GtkWidget *categoria_album = gtk_label_new (__("Cathegory"));
		gtk_misc_set_alignment ((GtkMisc*)categoria_album, 0, 0);
		gtk_size_group_add_widget (label, categoria_album);
		gtk_table_attach ((GtkTable*)table, categoria_album, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 2, 2);
		GtkWidget *descriere_album = gtk_label_new (__("Description"));
		gtk_misc_set_alignment ((GtkMisc*)descriere_album, 0, 0);
		gtk_size_group_add_widget (label, descriere_album);
		gtk_table_attach ((GtkTable*)table, descriere_album, 0, 1, 3, 4, GTK_FILL, GTK_FILL, 2, 2);
		/*GtkWidget *optiuni_poza = gtk_label_new (__("Options"));
		gtk_misc_set_alignment ((GtkMisc*)optiuni_poza, 0.0, 0.0);
		gtk_size_group_add_widget (label, optiuni_poza);
		gtk_table_attach ((GtkTable*)table, optiuni_poza, 0, 1, 3, 4, GTK_FILL, GTK_FILL, 2, 2);*/
			
		GtkWidget *nume_album_edit = gtk_entry_new_with_max_length (255);
		ps_hookup_object (window, nume_album_edit, "ps_album_properties_nume_edit");
		gtk_size_group_add_widget (edit, nume_album_edit);
		gtk_table_attach ((GtkTable*)table, nume_album_edit, 1, 2, 0, 1, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_FILL), 2, 2);
		GtkWidget *autor_album_edit = gtk_entry_new_with_max_length (255);
		ps_hookup_object (window, autor_album_edit, "ps_album_properties_autor_edit");
		gtk_size_group_add_widget (edit, autor_album_edit);
		gtk_table_attach ((GtkTable*)table, autor_album_edit, 1, 2, 1, 2, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_FILL), 2, 2);
		GtkWidget *categoria_album_edit = gtk_combo_new ();
		gtk_combo_set_value_in_list ((GtkCombo*)categoria_album_edit, FALSE, FALSE);
		gtk_combo_set_popdown_strings ((GtkCombo*)categoria_album_edit, ps_get_albums_categories ());
		ps_hookup_object (window, categoria_album_edit, "ps_album_properties_categoria_edit");
		gtk_size_group_add_widget (edit, categoria_album_edit);
		gtk_table_attach ((GtkTable*)table, categoria_album_edit, 1, 2, 2, 3, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_FILL), 2, 2);
		GtkWidget *descriere_album_edit_scroll = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)descriere_album_edit_scroll, GTK_SHADOW_IN);
		gtk_scrolled_window_set_policy ((GtkScrolledWindow*)descriere_album_edit_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		GtkWidget *descriere_album_edit = gtk_text_view_new ();
		gtk_widget_set_size_request (descriere_album_edit, 300, 100);
		gtk_text_view_set_wrap_mode ((GtkTextView*)descriere_album_edit, GTK_WRAP_WORD);
		gtk_container_add ((GtkContainer*)descriere_album_edit_scroll, descriere_album_edit);
		ps_hookup_object (window, descriere_album_edit, "ps_album_properties_descriere_edit");
		gtk_size_group_add_widget (edit, descriere_album_edit_scroll);
		gtk_table_attach ((GtkTable*)table, descriere_album_edit_scroll, 1, 2, 3, 4, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), 2, 2);
		
		GtkWidget *d_table = gtk_table_new (2, 2, TRUE);
		gtk_table_set_row_spacings ((GtkTable*)d_table, 5);
		gtk_table_set_col_spacings ((GtkTable*)d_table, 5);
		
		/*GSList *display_edit;
		GtkWidget *d_def = ps_create_radio_button_with_image_from_stock ("gtk-dialog-info", __("Use default option"), 1);
		gtk_table_attach ((GtkTable*)d_table, d_def, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 2, 2);
		ps_hookup_object (window, d_def, "ps_photo_properties_display_def");
		GtkWidget *d_stretch = ps_create_radio_button_with_image_from_stock ("gtk-dialog-info", __("Resized to fullscreen"), 1);
		gtk_table_attach ((GtkTable*)d_table, d_stretch, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 2, 2);
		ps_hookup_object (window, d_stretch, "ps_photo_properties_display_stretch");
		GtkWidget *d_center = ps_create_radio_button_with_image_from_stock ("gtk-dialog-info", __("Centered"), 1);
		gtk_table_attach ((GtkTable*)d_table, d_center, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 2, 2);
		ps_hookup_object (window, d_center, "ps_photo_properties_display_center");
		GtkWidget *d_tile = ps_create_radio_button_with_image_from_stock ("gtk-dialog-info", __("Tiled"), 1);
		gtk_table_attach ((GtkTable*)d_table, d_tile, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 2, 2);
		ps_hookup_object (window, d_tile, "ps_photo_properties_display_tile");
		display_edit = gtk_radio_button_get_group ((GtkRadioButton*)d_def);
		gtk_radio_button_set_group ((GtkRadioButton*)d_stretch, display_edit);
		display_edit = gtk_radio_button_get_group ((GtkRadioButton*)d_stretch);
		gtk_radio_button_set_group ((GtkRadioButton*)d_center, display_edit);
		display_edit = gtk_radio_button_get_group ((GtkRadioButton*)d_center);
		gtk_radio_button_set_group ((GtkRadioButton*)d_tile, display_edit);
		GtkWidget *d_frame = gtk_frame_new (__("Display the photo"));
		gtk_container_add ((GtkContainer*)d_frame, d_table);
		gtk_box_pack_start ((GtkBox*)vbox, d_frame, FALSE, FALSE, 2);*/
		
		GtkSizeGroup *butoane = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkWidget *b_box = gtk_hbox_new (FALSE, 2);
		GtkWidget *b_ok = ps_create_button_with_image_from_stock ("gtk-ok", __("Okey"), 1);
		ps_hookup_object (window, b_ok, "ps_album_properties_b_ok");
		gtk_widget_add_accelerator (b_ok, "clicked", accel_group, GDK_Return, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
		GtkWidget *b_cancel = ps_create_button_with_image_from_stock ("gtk-cancel", __("Cancel"), 1);
		ps_hookup_object (window, b_ok, "ps_album_properties_b_cancel");
		gtk_widget_add_accelerator (b_cancel, "clicked", accel_group, GDK_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
		gtk_box_pack_end ((GtkBox*)b_box, b_ok, FALSE, FALSE, 2);
		gtk_size_group_add_widget (butoane, b_ok);
		gtk_box_pack_end ((GtkBox*)b_box, b_cancel, FALSE, FALSE, 2);
		gtk_size_group_add_widget (butoane, b_cancel);
		
		props->window = window;
		
		g_signal_connect (b_ok, "clicked", (GCallback)cb_ps_close_album_properties_dialog, props);
		g_signal_connect (b_cancel, "clicked", (GCallback)cb_ps_close_album_properties_dialog, props);
		
		gtk_box_pack_start ((GtkBox*)vbox, b_box, FALSE, FALSE, 2);
		
		// completarea cu date
		gtk_entry_set_text ((GtkEntry*)nume_album_edit, props->date_album->nume);
		gtk_entry_set_text ((GtkEntry*)autor_album_edit, props->date_album->autor);
		gtk_entry_set_text ((GtkEntry*)((GtkCombo*)categoria_album_edit)->entry, props->date_album->categoria);
		GtkTextBuffer *buf = gtk_text_view_get_buffer ((GtkTextView*)descriere_album_edit);
		gtk_text_buffer_set_text (buf, props->date_album->descriere, strlen (props->date_album->descriere));
		
		gtk_widget_show_all (window);
	}
	gtk_window_add_accel_group ((GtkWindow*)window, accel_group);
	return window;
}

GtkWidget * ps_photo_properties_dialog (_ps_photo_properties *props)
{
	GtkWidget *window = NULL;
	GtkAccelGroup *accel_group = gtk_accel_group_new ();
	if (props->date_poza!=NULL)
	{
		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title ((GtkWindow*)window, __("Photo Properties"));
		gtk_window_set_skip_pager_hint ((GtkWindow*)window, TRUE);
		gtk_window_set_skip_taskbar_hint ((GtkWindow*)window, TRUE);
		gtk_window_set_modal ((GtkWindow*)window, TRUE);
		gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
		gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
		gtk_container_set_border_width ((GtkContainer*)window, 5);
		GtkWidget *hbox = gtk_hbox_new (FALSE, 2);
		GtkWidget *vbox = gtk_vbox_new (FALSE, 2);
		GtkWidget *vbox_margine = gtk_vbox_new (FALSE, 2);
		GtkWidget *imagine = gtk_image_new_from_pixbuf (ps_create_pixbuf_from_memory (
								  		   				props->date_poza->date_preview, 
										   				props->date_poza->n_poza_preview));
		gtk_misc_set_alignment ((GtkMisc*)imagine, 0.0, 0.0);
		gtk_box_pack_start ((GtkBox*)vbox_margine, imagine, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)vbox_margine, ps_create_photo_props_label (props->date_poza), FALSE, FALSE, 2);
		/*gtk_box_pack_start ((GtkBox*)hbox, gtk_image_new_from_pixbuf (
										   ps_create_mascot_header_with_image_from_memory (
								  		   props->date_poza->date_preview, 
										   props->date_poza->n_poza_preview, 
										   "Edit the information for this photo."), 
										   FALSE, FALSE, 2);*/
		gtk_box_pack_start ((GtkBox*)hbox, vbox_margine, FALSE, FALSE, 5);
		gtk_box_pack_start ((GtkBox*)hbox, vbox, TRUE, TRUE, 2);
		gtk_container_add ((GtkContainer*)window, hbox);
		//gtk_widget_set_size_request (window, 240, 120);
		
		
		GtkSizeGroup *label = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkSizeGroup *edit = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkWidget *frame_info = gtk_frame_new (__("Photo Information"));
		GtkWidget *table = gtk_table_new (3, 2, FALSE);
		gtk_table_set_row_spacings ((GtkTable*)table, 5);
		gtk_table_set_col_spacings ((GtkTable*)table, 5);
		gtk_container_add ((GtkContainer*)frame_info, table);
		gtk_box_pack_start ((GtkBox*)vbox, frame_info, TRUE, TRUE, 2);
			
		GtkWidget *nume_poza = gtk_label_new (__("Title"));
		gtk_misc_set_alignment ((GtkMisc*)nume_poza, 0, 0);
		gtk_size_group_add_widget (label, nume_poza);
		gtk_table_attach ((GtkTable*)table, nume_poza, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 2, 2);
		GtkWidget *autor_poza = gtk_label_new (__("Author"));
		gtk_misc_set_alignment ((GtkMisc*)autor_poza, 0, 0);
		gtk_size_group_add_widget (label, autor_poza);
		gtk_table_attach ((GtkTable*)table, autor_poza, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 2, 2);
		GtkWidget *descriere_poza = gtk_label_new (__("Description"));
		gtk_misc_set_alignment ((GtkMisc*)descriere_poza, 0, 0);
		gtk_size_group_add_widget (label, descriere_poza);
		gtk_table_attach ((GtkTable*)table, descriere_poza, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 2, 2);
		/*GtkWidget *optiuni_poza = gtk_label_new (__("Options"));
		gtk_misc_set_alignment ((GtkMisc*)optiuni_poza, 0.0, 0.0);
		gtk_size_group_add_widget (label, optiuni_poza);
		gtk_table_attach ((GtkTable*)table, optiuni_poza, 0, 1, 3, 4, GTK_FILL, GTK_FILL, 2, 2);*/
			
		GtkWidget *nume_poza_edit = gtk_entry_new_with_max_length (255);
		ps_hookup_object (window, nume_poza_edit, "ps_photo_properties_nume_edit");
		gtk_size_group_add_widget (edit, nume_poza_edit);
		gtk_table_attach ((GtkTable*)table, nume_poza_edit, 1, 2, 0, 1, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_FILL), 2, 2);
		GtkWidget *autor_poza_edit = gtk_entry_new_with_max_length (255);
		ps_hookup_object (window, autor_poza_edit, "ps_photo_properties_autor_edit");
		gtk_size_group_add_widget (edit, autor_poza_edit);
		gtk_table_attach ((GtkTable*)table, autor_poza_edit, 1, 2, 1, 2, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_FILL), 2, 2);
		GtkWidget *descriere_poza_edit_scroll = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)descriere_poza_edit_scroll, GTK_SHADOW_IN);
		gtk_scrolled_window_set_policy ((GtkScrolledWindow*)descriere_poza_edit_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		GtkWidget *descriere_poza_edit = gtk_text_view_new ();
		gtk_text_view_set_wrap_mode ((GtkTextView*)descriere_poza_edit, GTK_WRAP_WORD);
		gtk_container_add ((GtkContainer*)descriere_poza_edit_scroll, descriere_poza_edit);
		ps_hookup_object (window, descriere_poza_edit, "ps_photo_properties_descriere_edit");
		gtk_size_group_add_widget (edit, descriere_poza_edit_scroll);
		gtk_table_attach ((GtkTable*)table, descriere_poza_edit_scroll, 1, 2, 2, 3, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), 2, 2);
		
		/*GtkWidget *d_table = gtk_table_new (2, 2, TRUE);
		gtk_table_set_row_spacings ((GtkTable*)d_table, 5);
		gtk_table_set_col_spacings ((GtkTable*)d_table, 5);
		
		GSList *display_edit;
		GtkWidget *d_def = ps_create_radio_button_with_image_from_stock ("gtk-dialog-info", __("Use default option"), 1);
		gtk_table_attach ((GtkTable*)d_table, d_def, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 2, 2);
		ps_hookup_object (window, d_def, "ps_photo_properties_display_def");
		GtkWidget *d_stretch = ps_create_radio_button_with_image_from_stock ("gtk-dialog-info", __("Resized to fullscreen"), 1);
		gtk_table_attach ((GtkTable*)d_table, d_stretch, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 2, 2);
		ps_hookup_object (window, d_stretch, "ps_photo_properties_display_stretch");
		GtkWidget *d_center = ps_create_radio_button_with_image_from_stock ("gtk-dialog-info", __("Centered"), 1);
		gtk_table_attach ((GtkTable*)d_table, d_center, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 2, 2);
		ps_hookup_object (window, d_center, "ps_photo_properties_display_center");
		GtkWidget *d_tile = ps_create_radio_button_with_image_from_stock ("gtk-dialog-info", __("Tiled"), 1);
		gtk_table_attach ((GtkTable*)d_table, d_tile, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 2, 2);
		ps_hookup_object (window, d_tile, "ps_photo_properties_display_tile");
		display_edit = gtk_radio_button_get_group ((GtkRadioButton*)d_def);
		gtk_radio_button_set_group ((GtkRadioButton*)d_stretch, display_edit);
		display_edit = gtk_radio_button_get_group ((GtkRadioButton*)d_stretch);
		gtk_radio_button_set_group ((GtkRadioButton*)d_center, display_edit);
		display_edit = gtk_radio_button_get_group ((GtkRadioButton*)d_center);
		gtk_radio_button_set_group ((GtkRadioButton*)d_tile, display_edit);*/
		GtkWidget *d_frame = gtk_frame_new (__("Display the photo"));
		const char *photo_disp[8] = {__("System setting"), __("Proportional Resize"), __("Shrink Only"), __("Enlarge Only"), __("Stretch"), __("Center"), __("Tile")};
		GtkWidget *photo_disp_menu = ps_create_option_menu (__("The photo will be displayed"), photo_disp, 8, 1);
		ps_hookup_object (window, photo_disp_menu, "photo_disp_menu");
		gtk_container_add ((GtkContainer*)d_frame, photo_disp_menu);
		gtk_box_pack_start ((GtkBox*)vbox, d_frame, FALSE, FALSE, 2);
		
		GtkSizeGroup *butoane = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkWidget *b_box = gtk_hbox_new (FALSE, 2);
		GtkWidget *b_ok = ps_create_button_with_image_from_stock ("gtk-ok", __("Okey"), 1);
		ps_hookup_object (window, b_ok, "ps_photo_properties_b_ok");
		gtk_widget_add_accelerator (b_ok, "clicked", accel_group, GDK_Return, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
		GtkWidget *b_cancel = ps_create_button_with_image_from_stock ("gtk-cancel", __("Cancel"), 1);
		ps_hookup_object (window, b_ok, "ps_photo_properties_b_cancel");
		gtk_widget_add_accelerator (b_cancel, "clicked", accel_group, GDK_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
		gtk_box_pack_end ((GtkBox*)b_box, b_ok, FALSE, FALSE, 2);
		gtk_size_group_add_widget (butoane, b_ok);
		gtk_box_pack_end ((GtkBox*)b_box, b_cancel, FALSE, FALSE, 2);
		gtk_size_group_add_widget (butoane, b_cancel);
		
		props->window = window;
		
		g_signal_connect (b_ok, "clicked", (GCallback)cb_ps_close_photo_properties_dialog, props);
		g_signal_connect (b_cancel, "clicked", (GCallback)cb_ps_close_photo_properties_dialog, props);
		
		gtk_box_pack_start ((GtkBox*)vbox, b_box, FALSE, FALSE, 2);
		
		// completarea cu date
		gtk_entry_set_text ((GtkEntry*)nume_poza_edit, props->date_poza->nume);
		gtk_entry_set_text ((GtkEntry*)autor_poza_edit, props->date_poza->autor);
		GtkTextBuffer *buf = gtk_text_view_get_buffer ((GtkTextView*)descriere_poza_edit);
		gtk_text_buffer_set_text (buf, props->date_poza->descriere, strlen (props->date_poza->descriere));
		if (strlen (props->date_poza->optiuni)>0)
		{
			if (props->date_poza->optiuni[0]-'A'>=0 && props->date_poza->optiuni[0]-'A'<=8) ps_option_menu_set_index (photo_disp_menu, props->date_poza->optiuni[0]-'A');
				else ps_option_menu_set_index (photo_disp_menu, 0);
		}
		else ps_option_menu_set_index (photo_disp_menu, 0);
		
		gtk_widget_show_all (window);
	}
	gtk_window_add_accel_group ((GtkWindow*)window, accel_group);
	return window;
}

void ps_select_color_dialog (GtkWidget *parent, _ps_color *color)
{
	if (color!=NULL)
	{
		GtkWidget * color_select = gtk_color_selection_dialog_new (__("Select a color"));
		gtk_window_set_transient_for ((GtkWindow*)color_select, (GtkWindow*)parent);
		gtk_window_set_modal ((GtkWindow*)color_select, TRUE);
		GtkColorSelection *sel = (GtkColorSelection*)((GtkColorSelectionDialog*)color_select)->colorsel;
		if (color!=NULL)
		gtk_color_selection_set_has_opacity_control ((GtkColorSelection*)sel, TRUE);
		GdkColor col;
		col.red = (int)(color->red*65535/255);
		col.green = (int)(color->green*65535/255);
		col.blue = (int)(color->blue*65535/255);
		long alpha = 65535-(int)(color->alpha*65535/127);
		gtk_color_selection_set_previous_color(sel, &col);
		gtk_color_selection_set_previous_alpha(sel, alpha);
		gtk_color_selection_set_current_color(sel, &col);
		gtk_color_selection_set_current_alpha (sel, alpha);
		gint response = gtk_dialog_run ((GtkDialog*)color_select);
		if (response==GTK_RESPONSE_OK)
		{	
			gtk_color_selection_get_current_color (sel, &col);
			alpha = gtk_color_selection_get_current_alpha (sel);
			color->red = (int)(col.red*255/65535);
			color->green = (int)(col.green*255/65535);
			color->blue = (int)(col.blue*255/65535);
			color->alpha = 127-(int)(alpha*127/65535);
		}
		gtk_widget_destroy (color_select);
	}
}


void ps_wizard_free (_ps_wizard *wizard)
{
	if (wizard!=NULL)
	{
		if (wizard->name!=NULL) free (wizard->name);
		if (wizard->b_begin!=NULL) free (wizard->b_begin);
		if (wizard->b_cont!=NULL) free (wizard->b_cont);
		if (wizard->b_back!=NULL) free (wizard->b_back);
		if (wizard->b_cancel!=NULL) free (wizard->b_cancel);
		if (wizard->b_done!=NULL) free (wizard->b_done);
		if (wizard->window!=NULL) g_object_unref (wizard->window);
		free (wizard);
	}
}

void ps_progress_free (_ps_progress *progress)
{
	if (progress!=NULL)
	{
		if (progress->text!=NULL) free (progress->text);
		free (progress);
	}
}

void ps_album_properties_free (_ps_album_properties *props)
{
	if (props!=NULL)
	{
		if (props->date_album!=NULL) ps_album_free (props->date_album);
		free (props);
	}
}

void ps_photo_properties_free (_ps_photo_properties *props)
{
	if (props!=NULL)
	{
		if (props->date_poza!=NULL) ps_photo_free (props->date_poza);
		free (props);
	}
}

// Dialogs

static char *last_open_path = NULL;

void cb_ps_open_dialog_set_preview (GtkFileChooser *fdialog, gpointer data)
{
	GtkWidget* preview = (GtkWidget*)data;
	if (fdialog!=NULL && data!=NULL)
	{
		char *filename = gtk_file_chooser_get_filename (fdialog);
		if (filename!=NULL)
		{
			ps_preview_set_image (preview, filename);
		}
		else ps_preview_set_image (preview, "");
		if (filename!=NULL) g_free (filename);
	}
}

char *ps_open_dialog_single_file (GtkWidget *parent, const char *title, const char *filename, int show_preview)
{
	char *res=NULL;
	GtkWidget* open = gtk_file_chooser_dialog_new (title, (GtkWindow*)parent, GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	if (show_preview)
	{
		GtkWidget* preview = ps_preview_create();
		gtk_widget_set_size_request (preview, 160, 120);
		ps_hookup_object (open, preview, "preview");
		gtk_file_chooser_set_preview_widget ((GtkFileChooser*)open, preview);
		g_signal_connect (open, "update-preview", (GCallback)cb_ps_open_dialog_set_preview, preview);
	}
	if (filename!=NULL) gtk_file_chooser_set_filename ((GtkFileChooser*)open, filename);
		else if (last_open_path!=NULL) gtk_file_chooser_set_current_folder ((GtkFileChooser*)open, last_open_path);
	int response = gtk_dialog_run ((GtkDialog*)open);
	if (response==GTK_RESPONSE_ACCEPT)
	{
		res = gtk_file_chooser_get_filename ((GtkFileChooser*)open);
	}
	if (last_open_path!=NULL) free (last_open_path);
	last_open_path = gtk_file_chooser_get_current_folder ((GtkFileChooser*)open);
	gtk_widget_destroy (open);
	return res;
}

static char *last_save_path = NULL;

char *ps_save_dialog_single_file (GtkWidget *parent, const char *title, const char *filename, int show_preview)
{
	char *res=NULL;
	GtkWidget* save = gtk_file_chooser_dialog_new (title, (GtkWindow*)parent, GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
	if (show_preview)
	{
		GtkWidget* preview = ps_preview_create();
		ps_hookup_object (save, preview, "preview");
		gtk_file_chooser_set_preview_widget ((GtkFileChooser*)save, preview);
		g_signal_connect (save, "update-preview", (GCallback)cb_ps_open_dialog_set_preview, preview);
	}
	if (filename!=NULL) gtk_file_chooser_set_current_name ((GtkFileChooser*)save, filename);
	gtk_file_chooser_set_current_folder ((GtkFileChooser*)save, last_save_path);
	int response = gtk_dialog_run ((GtkDialog*)save);
	if (response==GTK_RESPONSE_ACCEPT)
	{
		res = gtk_file_chooser_get_filename ((GtkFileChooser*)save);
	}
	if (last_save_path!=NULL) free (last_save_path);
	last_save_path = gtk_file_chooser_get_current_folder ((GtkFileChooser*)save);
	gtk_widget_destroy (save);
	return res;
}

static char *last_folder_path = NULL;

char *ps_select_folder_dialog (GtkWidget *parent, const char *title, const char *path)
{
	char *res=NULL;
	GtkWidget* sfolder = gtk_file_chooser_dialog_new (title, (GtkWindow*)parent, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, NULL);
	if (path!=NULL) gtk_file_chooser_set_current_folder ((GtkFileChooser*)sfolder, path);
		else if (last_folder_path!=NULL) gtk_file_chooser_set_current_folder ((GtkFileChooser*)sfolder, last_folder_path);
	int response = gtk_dialog_run ((GtkDialog*)sfolder);
	if (response==GTK_RESPONSE_ACCEPT)
	{
		res = gtk_file_chooser_get_filename ((GtkFileChooser*)sfolder);
	}
	if (last_folder_path!=NULL) free (last_folder_path);
	last_folder_path = gtk_file_chooser_get_current_folder ((GtkFileChooser*)sfolder);
	gtk_widget_destroy (sfolder);
	return res;
}

GtkWidget * ps_create_quotes_list ()
{
	GtkTreeStore *store = gtk_tree_store_new (ps_c_quotes_list_numar,G_TYPE_INT, G_TYPE_STRING,
											  G_TYPE_STRING, G_TYPE_STRING);
	
	GtkWidget * quotes_list = gtk_tree_view_new_with_model ((GtkTreeModel*)store);
	GtkCellRenderer * render;
	GtkTreeViewColumn * col;
	
	// Author
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Author"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_quotes_list_autor);
	gtk_tree_view_column_set_resizable (col, TRUE);
	gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_fixed_width (col, 400);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 0.0, "style", PANGO_STYLE_ITALIC, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_quotes_list_autor, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)quotes_list, col);
	
	// Keywords
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Keywords"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_quotes_list_keywords);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 0.0, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_quotes_list_keywords, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)quotes_list, col);
	
	//gtk_widget_set_size_request (quotes_list, 100, 300);
	gtk_tree_view_set_headers_visible ((GtkTreeView*)quotes_list, TRUE);
	gtk_tree_view_set_rules_hint ((GtkTreeView*)quotes_list, TRUE);
	
	//g_signal_connect (lista_poze, "button-release-event", (GCallback)cb_ps_meniu_lista_poze, NULL);
	
	return quotes_list;
}

void cb_ps_quotes_lib_list_selected (GtkTreeSelection *sel, gpointer user_data)
{
	GtkTreeView * quotes_list;
	GtkTreeModel * model;
	GtkTreeIter pos;
	gchar * date;
	
	GtkWidget *window = (GtkWidget*)user_data;
	if (window==NULL) return;
	
	quotes_list = gtk_tree_selection_get_tree_view (sel);
	if (gtk_tree_selection_get_selected (sel, &model, &pos))
	{
		gtk_tree_model_get (model, &pos, ps_c_quotes_list_quotetext, &date, -1);
		if (date!=NULL)
		{
			GtkWidget *quote_text = (GtkWidget*)ps_get_widget_from (window, "quote_text");
			GtkTextBuffer *buffer = gtk_text_buffer_new (NULL);
			gtk_text_buffer_set_text (buffer, date, strlen (date));
			gtk_text_view_set_buffer ((GtkTextView*)quote_text, buffer);
			g_free (date);
		}
	}
}

void adauga_citat (_ps_quote *quote, int id, GtkWidget *quotes_lib)
{
	if (quote!=NULL)
	{
			char temp[1000];
			strcpy (temp, "");
			int linie_noua=0;
			for (unsigned int i=0;i<strlen(quote->author);i++)
			{
				sprintf (&temp[strlen(temp)], "%c", quote->author[i]);
				if ((i+1)%(400/8)==0) linie_noua=1;
				if (quote->author[i]==' ' && linie_noua)
				{
					sprintf (&temp[strlen(temp)], "\n"); 
					linie_noua=0;
				}
			}
			GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget_from (quotes_lib, "quotes_list"));
			GtkTreeIter pos;
			gtk_tree_store_append ((GtkTreeStore*)model,&pos,NULL);
			gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_quotes_list_id, id,
            		            ps_c_quotes_list_autor, temp, 
								ps_c_quotes_list_keywords, quote->keywords,
								ps_c_quotes_list_quotetext, quote->quote,
								-1);
	}
}

GtkWidget *ps_create_quotes_lib_dialog (GtkWindow *parent)
{
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_position ((GtkWindow*)window, GTK_WIN_POS_CENTER);
	gtk_window_set_title ((GtkWindow*)window, __("Quotes Library"));
	gtk_widget_set_size_request (window, 600, 400);
	gtk_container_set_border_width ((GtkContainer*)window, 7);
	gtk_window_set_transient_for ((GtkWindow*)window, parent);
	gtk_window_set_modal ((GtkWindow*)window, true);
	GtkWidget *vbox = gtk_vbox_new (FALSE, 2);
	gtk_container_add ((GtkContainer*)window, vbox);
	
	GtkWidget *header = ps_create_mascot_header (pict_path ("char/add_photo.png"), __("Welcome to the Quotes Library. You can read here all the quotes that PhotoStorm has found in its database."));
	ps_hookup_object (window, header, "header");
	gtk_box_pack_start ((GtkBox*)vbox, header, FALSE, FALSE, 2);
	
	GtkWidget *panou = gtk_vpaned_new ();
	gtk_box_pack_start ((GtkBox*)vbox, panou, TRUE, TRUE, 2);
	
	GtkWidget *quotes_list = ps_create_quotes_list ();
	ps_hookup_object (window, quotes_list, "quotes_list");	
	GtkWidget *quotes_list_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)quotes_list_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)quotes_list_scroll, GTK_SHADOW_IN);
	ps_hookup_object (window, quotes_list_scroll, "quotes_list_scroll");
	gtk_container_add ((GtkContainer*)quotes_list_scroll, quotes_list);
	gtk_paned_pack1 ((GtkPaned*)panou, quotes_list_scroll, TRUE, TRUE);
	
	GtkWidget *quote_text = gtk_text_view_new ();
	gtk_text_view_set_wrap_mode ((GtkTextView*)quote_text, GTK_WRAP_WORD);
	gtk_text_view_set_editable ((GtkTextView*)quote_text, FALSE);
	ps_hookup_object (window, quote_text, "quote_text");
	GtkWidget *quotes_text_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)quotes_text_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)quotes_text_scroll, GTK_SHADOW_IN);
	ps_hookup_object (window, quotes_text_scroll, "quotes_text_scroll");
	gtk_container_add ((GtkContainer*)quotes_text_scroll, quote_text);
	gtk_paned_pack2 ((GtkPaned*)panou, quotes_text_scroll, TRUE, TRUE);
	gtk_widget_set_size_request (quotes_list_scroll, 100, 200);
	
	_ps_quote *quote = NULL;
	int n = 1 ;
	while ((quote = ps_get_quote_nr (quotes_path ("quotes1"), n)))
	{
		adauga_citat (quote, n, window);
		ps_quote_free (quote);
		n++;
	}
	
	GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)quotes_list);
	//g_signal_connect (lista_poze, "row-activated", (GCallback)cb_ps_dublu_click_picture, NULL);
	gtk_tree_selection_set_mode (sel, GTK_SELECTION_SINGLE);
	g_signal_connect (sel, "changed", (GCallback)cb_ps_quotes_lib_list_selected, window);
	
	GtkTreeIter pos;
	gtk_tree_model_get_iter_first (gtk_tree_view_get_model ((GtkTreeView*)quotes_list), &pos);
	gtk_tree_selection_select_iter (sel, &pos);
	
	return window;
}

GtkWidget * ps_message_edit_dialog (GtkWindow *parent, _ps_message *message)
{
	
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkAccelGroup *accel_group = gtk_accel_group_new ();
	if (parent!=NULL) gtk_window_set_transient_for ((GtkWindow*)window, parent);
	gtk_window_set_title ((GtkWindow*)window, __("Edit Message"));
	gtk_window_set_skip_pager_hint ((GtkWindow*)window, TRUE);
	gtk_window_set_skip_taskbar_hint ((GtkWindow*)window, TRUE);
	gtk_window_set_modal ((GtkWindow*)window, TRUE);
	gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
	gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_container_set_border_width ((GtkContainer*)window, 5);
	GtkWidget *vbox = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, vbox, "vbox");
	gtk_container_add ((GtkContainer*)window, vbox);
	GtkWidget *hbox = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (window, hbox, "hbox");
	gtk_box_pack_start ((GtkBox*)vbox, hbox, FALSE, FALSE, 2);
	/*GtkWidget *vbox_2 = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, vbox_2, "vbox_2");
	gtk_box_pack_start ((GtkBox*)hbox, vbox_2, FALSE, FALSE, 2);*/
	GtkWidget *calendar = gtk_calendar_new ();
	ps_hookup_object (window, calendar, "calendar");
	gtk_calendar_display_options ((GtkCalendar*)calendar, (GtkCalendarDisplayOptions)(GTK_CALENDAR_SHOW_HEADING | GTK_CALENDAR_SHOW_DAY_NAMES | GTK_CALENDAR_WEEK_START_MONDAY | GTK_CALENDAR_SHOW_WEEK_NUMBERS));
	gtk_box_pack_start ((GtkBox*)hbox, calendar, FALSE, FALSE, 2);
	
	/*GtkWidget *vbox_2 = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, vbox_2, "vbox_2");
	gtk_box_pack_start ((GtkBox*)hbox, vbox_2, FALSE, FALSE, 2);*/
	
	GtkWidget *f_message = gtk_frame_new (__("Message"));
	ps_hookup_object (window, f_message, "f_message");
	gtk_container_set_border_width ((GtkContainer*)f_message, 2);
	gtk_box_pack_start ((GtkBox*)vbox, f_message, FALSE, FALSE, 2);
	GtkWidget *vbox_message = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (window, vbox_message, "hbox_message");
	gtk_container_add ((GtkContainer*)f_message, vbox_message);
	GtkWidget *m_time_edit = ps_create_time_edit (__("Time"), 0, 0);
	ps_hookup_object (window, m_time_edit, "m_time_edit");
	gtk_box_pack_start ((GtkBox*)vbox_message, m_time_edit, FALSE, FALSE, 2);
	GtkWidget *m_text = ps_create_edit (__("Message Text"), 250, 30, 1, '\0');
	ps_hookup_object (window, m_text, "m_text");
	gtk_box_pack_start ((GtkBox*)vbox_message, m_text, FALSE, FALSE, 2);
	
	GtkWidget *hbox_buttons = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (window, hbox_buttons, "hbox_buttons");
	gtk_box_pack_start ((GtkBox*)vbox, hbox_buttons, FALSE, FALSE, 2);
	GtkSizeGroup *size_group_buttons = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	GtkWidget *b_msg_ok = ps_create_button_with_image_from_stock ("gtk-apply", __("Okey"), 1);
	ps_hookup_object (window, b_msg_ok, "b_msg_ok");
	gtk_widget_add_accelerator (b_msg_ok, "clicked", accel_group, GDK_Return, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_size_group_add_widget (size_group_buttons, b_msg_ok);
	gtk_box_pack_end ((GtkBox*)hbox_buttons, b_msg_ok, FALSE, FALSE, 2);
	GtkWidget *b_msg_cancel = ps_create_button_with_image_from_stock ("gtk-cancel", __("Cancel"), 1);
	ps_hookup_object (window, b_msg_cancel, "b_msg_cancel");
	gtk_widget_add_accelerator (b_msg_cancel, "clicked", accel_group, GDK_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_size_group_add_widget (size_group_buttons, b_msg_cancel);
	gtk_box_pack_end ((GtkBox*)hbox_buttons, b_msg_cancel, FALSE, FALSE, 2);
	gtk_window_add_accel_group ((GtkWindow*)window, accel_group);
	
	_ps_message_edit *message_edit_dialog_data = (_ps_message_edit*)malloc (sizeof (_ps_message_edit));
	message_edit_dialog_data->window = window;
	message_edit_dialog_data->message = message;
	
	g_signal_connect (b_msg_ok, "clicked", (GCallback)cb_ps_close_message_edit_dialog, message_edit_dialog_data);
	g_signal_connect (b_msg_cancel, "clicked", (GCallback)cb_ps_close_message_edit_dialog, message_edit_dialog_data);
	
	GtkWidget *f_disp = gtk_frame_new (__("Display"));
	ps_hookup_object (window, f_disp, "f_disp");
	gtk_container_set_border_width ((GtkContainer*)f_disp, 2);
	gtk_box_pack_start ((GtkBox*)hbox, f_disp, TRUE, TRUE, 2);
	GtkWidget *vbox_disp = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, vbox_disp, "vbox_disp");
	gtk_container_add ((GtkContainer*)f_disp, vbox_disp);
	
	/*GtkWidget *d_hour = ps_create_check_button_with_image_from_stock ("gtk-dialog-info", __("Every Hour"), 1);
	ps_hookup_object (window, d_hour, "d_hour");
	gtk_box_pack_start ((GtkBox*)vbox_disp, d_hour, FALSE, FALSE, 2);*/
	GtkWidget *d_day = ps_create_check_button_with_image_from_stock ("gtk-dialog-info", __("Every Day"), 1);
	ps_hookup_object (window, d_day, "d_day");
	gtk_box_pack_start ((GtkBox*)vbox_disp, d_day, FALSE, FALSE, 2);
	GtkWidget *d_week = ps_create_check_button_with_image_from_stock ("gtk-dialog-info", __("Every Week"), 1);
	ps_hookup_object (window, d_week, "d_week");
	gtk_box_pack_start ((GtkBox*)vbox_disp, d_week, FALSE, FALSE, 2);
	GtkWidget *d_month = ps_create_check_button_with_image_from_stock ("gtk-dialog-info", __("Every Month"), 1);
	ps_hookup_object (window, d_month, "d_month");
	gtk_box_pack_start ((GtkBox*)vbox_disp, d_month, FALSE, FALSE, 2);
	GtkWidget *d_year = ps_create_check_button_with_image_from_stock ("gtk-dialog-info", __("Every Year"), 1);
	ps_hookup_object (window, d_year, "d_year");
	gtk_box_pack_start ((GtkBox*)vbox_disp, d_year, FALSE, FALSE, 2);
	time_t tmp;
	time (&tmp);
	tm *mytime = localtime (&tmp);
	unsigned int date = mytime->tm_mday;
	unsigned int month = mytime->tm_mon;
	unsigned int year = mytime->tm_year+1900;
	int h = mytime->tm_hour;
	int m = mytime->tm_min;
	if (message!=NULL)
	{
		if (message->date!=0) date = message->date;
		if (message->month!=0) month = message->month-1;
		if (message->year!=0) year = message->year;
		if (message->wday!=0) ps_check_button_set_value (d_week, TRUE);
		if (message->wday==0 && message->date==0) ps_check_button_set_value (d_day, TRUE);
		if (message->month==0) ps_check_button_set_value (d_month, TRUE);
		if (message->year==0) ps_check_button_set_value (d_year, TRUE);
		h = message->hour;
		m = message->minute;
		ps_edit_set_text (m_text, message->message_text);
	}
	gtk_calendar_select_day ((GtkCalendar*)calendar, date);
	gtk_calendar_select_month ((GtkCalendar*)calendar, month, year);
	ps_time_edit_set_time (m_time_edit, h, m);
	
	/*const char *disp_types[] = {__("Once"), __("Hourly"), __("Daily"), __("Weekly"), __("Monthly"), __("Yearly")};
	GtkWidget *m_disp = ps_create_option_menu (__("Display Type"), disp_types, 6, 1);
	ps_hookup_object (window, m_disp, "m_disp");
	gtk_box_pack_start ((GtkBox*)vbox_disp, m_disp, FALSE, FALSE, 2);*/
	return window;
}

GtkWidget * ps_create_messages_list ()
{
	GtkTreeStore *store = gtk_tree_store_new (ps_c_messages_list_numar,G_TYPE_STRING, G_TYPE_STRING,
											  G_TYPE_STRING);
	
	GtkWidget * messages_list = gtk_tree_view_new_with_model ((GtkTreeModel*)store);
	GtkCellRenderer * render;
	GtkTreeViewColumn * col;
	
	// Time
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Time"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_messages_list_time);
	gtk_tree_view_column_set_resizable (col, FALSE);
	gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_fixed_width (col, 120);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 1.0, "yalign", 0.0, "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_messages_list_time, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)messages_list, col);
	
	// Message Text
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Message Text"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_messages_list_message_text);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 0.0, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_messages_list_message_text, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)messages_list, col);
	
	gtk_widget_set_size_request (messages_list, 450, 200);
	gtk_tree_view_set_headers_visible ((GtkTreeView*)messages_list, TRUE);
	gtk_tree_view_set_headers_clickable ((GtkTreeView*)messages_list, FALSE);
	gtk_tree_view_set_rules_hint ((GtkTreeView*)messages_list, TRUE);
	
	//g_signal_connect (lista_poze, "button-release-event", (GCallback)cb_ps_meniu_lista_poze, NULL);
	
	return messages_list;
}

const char *ps_get_selected_message_id (GtkWidget *window)
{
	if (window==NULL) return NULL;
	const char *id;
	GtkTreeSelection * sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget_from (window, "messages_list"));
	if (sel!=NULL)
	{
		GtkTreeModel * model;
		GtkTreeIter pos;
		if (gtk_tree_selection_get_selected (sel, &model, &pos))
		{
		    gtk_tree_model_get (model, &pos, ps_c_messages_list_id, &id, -1);
		}
	}
	return id;
}

void ps_refresh_messages_list (GtkWidget *window)
{
	if (window!=NULL)
	{
		GtkWidget *calendar = (GtkWidget*)ps_get_widget_from (window, "calendar");
		gtk_calendar_clear_marks ((GtkCalendar*)calendar);
		GtkWidget *messages_list = (GtkWidget*)ps_get_widget_from (window, "messages_list");
		if (calendar!=NULL && messages_list!=NULL)
		{
			GtkTreeModel * lista_mesaje = gtk_tree_view_get_model ((GtkTreeView*)messages_list);
			gtk_tree_store_clear ((GtkTreeStore*)lista_mesaje);
			guint year, month, day;
			gtk_calendar_get_date ((GtkCalendar*)calendar, &year, &month, &day);
			time_t tmp;
			time (&tmp);
			tm *mytime = localtime (&tmp);
			mytime->tm_year = year-1900;
			mytime->tm_mon = month;
			mytime->tm_mday = day;
			tmp = mktime (mytime);
			mytime = localtime (&tmp);
			GList *ps_messages_list = get_messages_list ();
			int nr_msgs = g_list_length (ps_messages_list);
			char *l_time = NULL;
			for (int i=0;i<nr_msgs;i++)
			{
				_ps_message *message = (_ps_message*)g_list_nth_data (ps_messages_list, i);
				if (message->date!=0 && message->month==(month+1) && message->year==year) gtk_calendar_mark_day ((GtkCalendar*)calendar, message->date);
				int wday = mytime->tm_wday;
				if (wday==0) wday=7;
				if (message!=NULL && 
					(message->year==0 || (message->year==(mytime->tm_year+1900))) &&
					(message->month==0 || (message->month==(mytime->tm_mon+1))) &&
					(message->date==0 || (message->date==mytime->tm_mday)) &&
					(message->wday==0 || (message->wday==wday))
			   	)
				{
					char temp[100];
					sprintf (temp, "%.2d:%.2d", message->hour, message->minute);
					if (l_time==NULL || (l_time!=NULL && strcmp (l_time, temp)!=0))
					{
						if (l_time!=NULL) free (l_time);
						l_time = strdup (temp);
					}
					else strcpy (temp, "");
					GtkTreeIter pos;
					gtk_tree_store_append ((GtkTreeStore*)lista_mesaje,&pos,NULL);
					gtk_tree_store_set ((GtkTreeStore*)lista_mesaje, &pos, ps_c_messages_list_id, message->id,
            				            ps_c_messages_list_time, temp, 
										ps_c_messages_list_message_text, message->message_text,
										-1);
				}
			}
			if (l_time!=NULL) free (l_time);
		}
	}
}

void cb_ps_calendar_changed (GtkCalendar *calendar, gpointer user_data)
{
	ps_refresh_messages_list ((GtkWidget*)user_data);
}

void cb_ps_add_message (GtkWidget *buton, gpointer user_data)
{
	if ((GtkWidget*)user_data!=NULL)
	{
		//printf ("selected message text: %s\n", ((_ps_message*)g_list_nth_data (g_list_find_custom (get_messages_list(), ps_get_selected_message_id((GtkWidget*)user_data), (GCompareFunc)messages_list_message_to_id_comparator), 0))->message_text);
		gtk_widget_show_all (ps_message_edit_dialog ((GtkWindow*)user_data, NULL));
	}
	//if ((_ps_message*)user_data!=NULL) ps_message_edit_dialog (NULL, (_ps_message*)user_data);
}

void cb_ps_edit_message (GtkWidget *buton, gpointer user_data)
{
	if ((GtkWidget*)user_data!=NULL)
	{
		//printf ("selected message text: %s\n", ((_ps_message*)g_list_nth_data (g_list_find_custom (get_messages_list(), ps_get_selected_message_id((GtkWidget*)user_data), (GCompareFunc)messages_list_message_to_id_comparator), 0))->message_text);
		gtk_widget_show_all (ps_message_edit_dialog ((GtkWindow*)user_data, (_ps_message*)g_list_nth_data (g_list_find_custom (get_messages_list(), ps_get_selected_message_id((GtkWidget*)user_data), (GCompareFunc)messages_list_message_to_id_comparator), 0)));
	}
	//if ((_ps_message*)user_data!=NULL) ps_message_edit_dialog (NULL, (_ps_message*)user_data);
}

void cb_ps_delete_message (GtkWidget *buton, gpointer user_data)
{
	if ((GtkWidget*)user_data!=NULL)
	{
		//printf ("selected message text: %s\n", ((_ps_message*)g_list_nth_data (g_list_find_custom (get_messages_list(), ps_get_selected_message_id((GtkWidget*)user_data), (GCompareFunc)messages_list_message_to_id_comparator), 0))->message_text);
		GList *ps_messages_list = get_messages_list ();
		_ps_message *message = (_ps_message*)g_list_nth_data (g_list_find_custom (get_messages_list(), ps_get_selected_message_id((GtkWidget*)user_data), (GCompareFunc)messages_list_message_to_id_comparator), 0);
		ps_messages_list = g_list_remove (ps_messages_list, message);
		set_messages_list (ps_messages_list);
		ps_refresh_messages_list ((GtkWidget*)user_data);
	}
	//if ((_ps_message*)user_data!=NULL) ps_message_edit_dialog (NULL, (_ps_message*)user_data);
}

GtkWidget * ps_messages_edit_dialog (GtkWindow *parent)
{
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkAccelGroup *accel_group = gtk_accel_group_new ();
	if (parent!=NULL) gtk_window_set_transient_for ((GtkWindow*)window, parent);
	gtk_window_set_title ((GtkWindow*)window, __("Messages Editor"));
	gtk_window_set_skip_pager_hint ((GtkWindow*)window, TRUE);
	gtk_window_set_skip_taskbar_hint ((GtkWindow*)window, TRUE);
	gtk_window_set_modal ((GtkWindow*)window, TRUE);
	gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
	gtk_window_set_type_hint ((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_container_set_border_width ((GtkContainer*)window, 5);
	GtkWidget *vbox = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, vbox, "vbox");
	gtk_container_add ((GtkContainer*)window, vbox);
	GtkWidget *hbox = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (window, hbox, "hbox");
	gtk_box_pack_start ((GtkBox*)vbox, hbox, FALSE, FALSE, 2);
	/*GtkWidget *vbox_2 = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, vbox_2, "vbox_2");
	gtk_box_pack_start ((GtkBox*)hbox, vbox_2, FALSE, FALSE, 2);*/
	GtkWidget *calendar = gtk_calendar_new ();
	ps_hookup_object (window, calendar, "calendar");
	gtk_calendar_display_options ((GtkCalendar*)calendar, (GtkCalendarDisplayOptions)(GTK_CALENDAR_SHOW_HEADING | GTK_CALENDAR_SHOW_DAY_NAMES | GTK_CALENDAR_WEEK_START_MONDAY | GTK_CALENDAR_SHOW_WEEK_NUMBERS));
	gtk_box_pack_start ((GtkBox*)hbox, calendar, FALSE, FALSE, 2);
	g_signal_connect (calendar, "day_selected", (GCallback)cb_ps_calendar_changed, window);
	GtkWidget *messages_img = gtk_image_new_from_file (pict_path ("logos/messages.jpg"));
	ps_hookup_object (window, messages_img, "messages_img");
	gtk_box_pack_start ((GtkBox*)hbox, messages_img, TRUE, TRUE, 2);
	
	/*GtkWidget *vbox_2 = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (window, vbox_2, "vbox_2");
	gtk_box_pack_start ((GtkBox*)hbox, vbox_2, FALSE, FALSE, 2);*/
	
	GtkWidget *messages_list = ps_create_messages_list ();
	ps_hookup_object (window, messages_list, "messages_list");	
	GtkWidget *messages_list_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)messages_list_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)messages_list_scroll, GTK_SHADOW_IN);
	ps_hookup_object (window, messages_list_scroll, "messages_list_scroll");
	gtk_container_add ((GtkContainer*)messages_list_scroll, messages_list);
	gtk_box_pack_start ((GtkBox*)vbox, messages_list_scroll, TRUE, TRUE, 2);
	//gtk_paned_pack1 ((GtkPaned*)panou, messages_list_scroll, TRUE, TRUE);
	
	GtkWidget *hbox_msg_buttons = gtk_hbox_new (TRUE, 2);
	ps_hookup_object (window, hbox_msg_buttons, "hbox_msg_buttons");
	gtk_box_pack_start ((GtkBox*)vbox, hbox_msg_buttons, FALSE, FALSE, 2);
	GtkSizeGroup *size_group_msg_buttons = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	GtkWidget *b_msg_add = ps_create_button_with_image_from_stock ("gtk-add", __("Add"), 1);
	ps_hookup_object (window, b_msg_add, "b_msg_add");
	gtk_widget_add_accelerator (b_msg_add, "clicked", accel_group, GDK_Insert, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_size_group_add_widget (size_group_msg_buttons, b_msg_add);
	gtk_box_pack_start ((GtkBox*)hbox_msg_buttons, b_msg_add, FALSE, FALSE, 2);
	g_signal_connect (b_msg_add, "clicked", (GCallback)cb_ps_add_message, window);
	GtkWidget *b_msg_edit = ps_create_button_with_image_from_stock ("gtk-index", __("Edit"), 1);
	ps_hookup_object (window, b_msg_edit, "b_msg_edit");
	gtk_widget_add_accelerator (b_msg_edit, "clicked", accel_group, GDK_F2, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_size_group_add_widget (size_group_msg_buttons, b_msg_edit);
	gtk_box_pack_start ((GtkBox*)hbox_msg_buttons, b_msg_edit, FALSE, FALSE, 2);
	g_signal_connect (b_msg_edit, "clicked", (GCallback)cb_ps_edit_message, window);
	GtkWidget *b_msg_delete = ps_create_button_with_image_from_stock ("gtk-delete", __("Delete"), 1);
	ps_hookup_object (window, b_msg_delete, "b_msg_delete");
	gtk_widget_add_accelerator (b_msg_delete, "clicked", accel_group, GDK_Delete, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_size_group_add_widget (size_group_msg_buttons, b_msg_delete);
	gtk_box_pack_start ((GtkBox*)hbox_msg_buttons, b_msg_delete, FALSE, FALSE, 2);
	g_signal_connect (b_msg_delete, "clicked", (GCallback)cb_ps_delete_message, window);
	
	GtkWidget *separator = gtk_separator_menu_item_new ();
	ps_hookup_object (window, separator, "separator");
	gtk_box_pack_start ((GtkBox*)vbox, separator, FALSE, TRUE, 0);
	
	GtkWidget *hbox_buttons = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (window, hbox_buttons, "hbox_buttons");
	gtk_box_pack_start ((GtkBox*)vbox, hbox_buttons, FALSE, FALSE, 2);
	GtkSizeGroup *size_group_buttons = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	GtkWidget *b_msg_ok = ps_create_button_with_image_from_stock ("gtk-apply", __("Okey"), 1);
	ps_hookup_object (window, b_msg_ok, "b_msg_ok");
	gtk_widget_add_accelerator (b_msg_ok, "clicked", accel_group, GDK_Return, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_size_group_add_widget (size_group_buttons, b_msg_ok);
	gtk_box_pack_end ((GtkBox*)hbox_buttons, b_msg_ok, FALSE, FALSE, 2);
	GtkWidget *b_msg_cancel = ps_create_button_with_image_from_stock ("gtk-cancel", __("Cancel"), 1);
	ps_hookup_object (window, b_msg_ok, "b_msg_ok");
	gtk_widget_add_accelerator (b_msg_cancel, "clicked", accel_group, GDK_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_size_group_add_widget (size_group_buttons, b_msg_cancel);
	gtk_box_pack_end ((GtkBox*)hbox_buttons, b_msg_cancel, FALSE, FALSE, 2);
	gtk_window_add_accel_group ((GtkWindow*)window, accel_group);
	
	g_signal_connect (b_msg_ok, "clicked", (GCallback)cb_ps_close_messages_edit_dialog_and_save_messages, window);
	g_signal_connect (b_msg_cancel, "clicked", (GCallback)cb_ps_close_messages_edit_dialog, window);
	
	g_signal_connect (window, "destroy", (GCallback)cb_ps_close_messages_edit_dialog, NULL);
	if (parent==NULL) g_signal_connect (window, "destroy", (GCallback)gtk_main_quit, NULL);
	
	ps_refresh_messages_list (window);
	
	return window;
}
