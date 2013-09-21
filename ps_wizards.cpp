
#include "data.h"

#include "ps_wizards.h"
#include "lang/translate.h"
#include "misc.h"
#include "ps_dialogs.h"
#include "design.h"
#include "configs.h"
#include "string.h"
#include "ps_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h> 
#include <dirent.h>

_ps_wizard *ps_wizard_add_photo;

int ps_function_create_album_wizard (_ps_wizard *wizard, int page, int buton)
{
	int result=0;
	if (buton==0) result=1;
	if (buton==1)
	{
		result=1;
		if (page==1) 
		{
			char *nume = ps_get_trimmed_text_from_entry ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_nume"));
			if (nume!=NULL)
			{
				if (strlen (nume)==0) result = 0;
				else result = 1;
				free (nume);
			}
			else result = 0;
			//printf ("nume: (%s)\n", nume);
			if (result==0) ps_information_dialog ("gtk-dialog-info", 1, __("Your new album has no name! You <b>have to</b> give a <b>name</b> to your album."), wizard->window);
		}
		if (page==2)
		{
			char *categoria = ps_get_trimmed_text_from_entry ((GtkEditable*)((GtkCombo*)(ps_get_widget_from (wizard->window, "ps_wizard_create_album_categoria")))->entry);
			if (categoria!=NULL) 
			{				
				result = 1;
				free (categoria);
			}
			else result = 0;
			//printf ("categoria: (%s)", categoria);
			if (result==0) ps_information_dialog ("gtk-dialog-info", 1, __("You have not specified any cathegory for your album. You <b>have to</b> place your album in a <b>cathegory</b>."), wizard->window);
		}
		if (page==5)
		{
			char *parola1 = ps_get_text_from_entry ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_parola1"));
			//printf ("parola1: (%s)\n", parola1);
			if (parola1!=NULL && strlen (parola1)>0)
			{
				result = 1;
				free (parola1);
			} 
			else result = 2;
		}
		if (page==6)
		{
			char *parola1 = ps_get_text_from_entry ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_parola1"));
			char *parola2 = ps_get_text_from_entry ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_parola2"));
			if (parola1!=NULL && parola2!=NULL) 
			{	
				//printf ("parola1: (%s)\n", parola1);
				//printf ("parola2: (%s)\n", parola2);
				if (strcmp (parola1, parola2)==0) result = 1;
				else result = 0;
			}
			else result = 0;
			if (result==0) ps_information_dialog ("gtk-dialog-info", 1, __("The <b>passwords</b> that you have entered <b>do not match</b>. Please make sure that you have spelled them correctly."), wizard->window);				
			if (parola1!=NULL) free (parola1);
			if (parola2!=NULL) free (parola2);	
		}
	}
	if (buton==3)
	{
		char *nume = ps_get_trimmed_text_from_entry ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_nume"));
		char *categoria = ps_get_trimmed_text_from_entry ((GtkEditable*)((GtkCombo*)(ps_get_widget_from (wizard->window, "ps_wizard_create_album_categoria")))->entry);
		char *autor = ps_get_text_from_entry ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_autor"));
		char *descriere = ps_get_text_from_text_view ((GtkTextView*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_descriere"));
		char *parola = ps_get_text_from_entry ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_parola1"));
		int e=0;
		if (nume!=NULL && categoria!=NULL)
		{
			char *albumul = (char*)malloc (strlen(categoria)+3+strlen(nume)+3+1);
			if (albumul!=NULL)
			{
				sprintf (&albumul[0], "%s___%s.psa", ps_strfilter_filename(categoria), ps_strfilter_filename(nume));
				e = ps_create_new_album (nume, autor, categoria, descriere, NULL, parola, album_path (albumul)); 
				if (e!=0) 
				{
					ps_clear_albums_list ();
					_ps_config *config = ps_get_config ();
					ps_clear_albums_list ();
					ps_load_albums_from_path (config->system.shared_albums);
					ps_load_albums_from_path (config->system.albums);	
					//printf ("%s\n", album_path(albumul));
					selecteaza_album (album_path(albumul));
					//printf ("%s\n", ps_get_current_album());
					GtkToggleButton *add_poze = (GtkToggleButton*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_add_poze_yes");					
					if (gtk_toggle_button_get_active (add_poze)==TRUE)
					{
						ps_add_photo_wizard ();
					}
				}
				free (albumul);
			}
		}
		if (e==0) 
		{
			ps_information_dialog ("gtk-dialog-info", 1, __("We are sorry, but the album couldn't be created. Please try to change the name or the cathegory."), wizard->window);
			result = 0;
		}
		else result = 1;
	}
	if (buton==2) 
	{
		if (page==7)
		{
			char *parola = ps_get_text_from_entry ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_create_album_parola1"));
			if (parola!=NULL) 
			{	
				result = -2;
				free (parola);
			}
			else result = -1;
		} 
		else result = -1;
	}
	return result;
}

int ps_function_add_photo_wizard (_ps_wizard *wizard, int page, int buton)
{
	int result=0;
	if (buton==0) result=1;
	if (buton==1)
	{
		result=1;
		if (page==1)  // alegerea nr de poze
		{
			GtkToggleButton *nr_poze = (GtkToggleButton*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nr_poze");
			gboolean n_poze = gtk_toggle_button_get_active (nr_poze);
			if (n_poze==FALSE) result=2;
		}
		if (page==2)
		{
			GtkEditable *filename = (GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nume_fisier");
			if (filename!=NULL)
			{
				char *poza = gtk_editable_get_chars (filename, 0, -1);
				if (poza!=NULL)
				{
					_ps_photo *date_poza = ps_create_photo (poza);
					if (date_poza!=NULL)
					{
						GtkEntry *nume_poza = (GtkEntry*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nume_edit");
						gtk_entry_set_text (nume_poza, date_poza->nume);
						ps_photo_free (date_poza);
						result=2;
					} else result=0;
					free (poza);
				} else result=0;
			} else result=0;
			if (result==0) ps_information_dialog ("gtk-dialog-info", 1, __("We are very sorry, but the file you have selected is <b>not a photo</b> or its format is <b>not supported</b> by Photostorm."), wizard->window);
		}
		if (page==3)
		{
			char *location = ps_location_edit_get_location ((GtkWidget*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_photos_location"));
			if (location!=NULL) result = 2;
		}
		if (page==5)
		{
		}
	}
	if (buton==3)
	{
		GtkToggleButton *nr_poze = (GtkToggleButton*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nr_poze");
		gboolean n_poze = gtk_toggle_button_get_active (nr_poze);
		if (n_poze==TRUE)
		{
			char *poza = gtk_editable_get_chars ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nume_fisier"), 0, -1);
			if (poza!=NULL)
			{
				char *nume = gtk_editable_get_chars ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nume_edit"), 0, -1);
				char *autor = gtk_editable_get_chars ((GtkEditable*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_autor_edit"), 0, -1);
				GtkTextBuffer *buf = gtk_text_view_get_buffer ((GtkTextView*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_descriere_edit"));
				GtkTextIter buf_start;
				gtk_text_buffer_get_start_iter (buf, &buf_start);
				GtkTextIter buf_end;
				gtk_text_buffer_get_end_iter (buf, &buf_end);
				char *descriere = gtk_text_buffer_get_text (buf, &buf_start, &buf_end, TRUE);
				if (nume==NULL) nume=strdup (poza);
				if (autor==NULL) autor=strdup ("");
				if (descriere==NULL) descriere=strdup ("");
				_ps_photo *date_poza = ps_create_photo_with_data (poza, nume, autor, descriere, "", "");
				if (date_poza!=NULL)
				{
					// mai trebuie verificat
					char *albumul = ps_get_current_album ();
					if (albumul!=NULL)
					{
						if (!ps_add_photo_to_album_from_memory (date_poza, albumul))
							ps_information_dialog ("gtk-dialog-info", 1, __("We are very sorry, but there has been an <b>error</b> while trying to add your photo to the album. Please try it agaian."), wizard->ps_main_window);
						ps_photo_free (date_poza);
						ps_list_album_from_file (albumul);
						free (albumul);
					}
					result=1;
				}	
			}
		}
		else
		{
			char *albumul = ps_get_current_album ();
			if (albumul==NULL) result = 0;
			char *calea = ps_location_edit_get_location ((GtkWidget*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_photos_location"));
			if (albumul!=NULL && calea!=NULL)
			{
				struct dirent *namelist;
				DIR *dir = opendir (calea);
				if (dir==NULL) printf (__("Error loading photos from %s\n"), calea);
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
								ps_add_photo_to_album_from_file (temp, albumul);
								free (temp);
							}
						} else tot = 1;
					} while (!tot);
					closedir (dir);
				}
			}
			ps_list_album_from_file (albumul);
			free (albumul);
			result = 1;
		} // end of else nr_photos == TRUE
		if (result == 1)
		{
			GtkToggleButton *add_poza = (GtkToggleButton*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_add_one_more");
			gboolean one_more_photo = gtk_toggle_button_get_active (add_poza);
			if (one_more_photo==TRUE)
			{
				result=-3;
			}
		}
	}
	if (buton==2) 
	{
		result=-1;
		if (page==3)  // selectarea pozelor multiple
		{
			GtkToggleButton *nr_poze = (GtkToggleButton*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nr_poze");
			gboolean n_poze = gtk_toggle_button_get_active (nr_poze);
			if (n_poze==FALSE) result=-2;
		}
		if (page==4)  // introducerea informatiilor despre mai multe poze
		{
			GtkToggleButton *nr_poze = (GtkToggleButton*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nr_poze");
			gboolean n_poze = gtk_toggle_button_get_active (nr_poze);
			if (n_poze==TRUE) result=-2;
		}
		if (page==5)
		{
			GtkToggleButton *nr_poze = (GtkToggleButton*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nr_poze");
			gboolean n_poze = gtk_toggle_button_get_active (nr_poze);
			if (n_poze==FALSE) result=-2;
		}
	}
	return result;
}

void cb_ps_add_picture_wizard_show_preview (GtkEditable *textbox, gpointer data)
{
	char *text = gtk_editable_get_chars (textbox, 0, -1);
	if (text!=NULL && ((GtkImage*)data)!=NULL)
	{
		GdkPixbuf *poza;
		GdkPixbuf *poza_preview;
		poza = gdk_pixbuf_new_from_file (text, NULL);
		if (poza!=NULL)
		{
			int dx = 380, dy = 120;
			int pdx = gdk_pixbuf_get_width (poza);
			int pdy = gdk_pixbuf_get_height (poza);
			if (pdx>dx || pdy>dy)
			if (pdx<pdy)
			{
				poza_preview = gdk_pixbuf_scale_simple (poza, dx, (int)(pdy*(double)dx/pdx)+1, GDK_INTERP_BILINEAR);
			} else
			{
				poza_preview = gdk_pixbuf_scale_simple (poza, (int)(pdx*(double)dy/pdy)+1, dy, GDK_INTERP_BILINEAR);
			}
			gtk_image_set_from_pixbuf ((GtkImage*)data, poza_preview);
			g_object_unref (poza);
			g_object_unref (poza_preview);
		} else gtk_image_set_from_file ((GtkImage*)data, NULL);
		//gtk_image_set_from_file ((GtkImage*)data, text);
		free (text);
	}
}

void cb_ps_show_open_dialog (GtkButton *button, gpointer data)
{
	_ps_wizard *wizard = (_ps_wizard*)data;
	if (wizard!=NULL)
	{
		char *filename = ps_open_dialog_single_file (wizard->window, __("Select a photo to add"), NULL, true);
		if (filename!=NULL)
		{
			GtkEntry *nume_fisier = (GtkEntry*)ps_get_widget_from (wizard->window, "ps_wizard_add_photo_nume_fisier");
			if (nume_fisier!=NULL) gtk_entry_set_text (nume_fisier, filename);
			free (filename);
		}
	}
}

void ps_add_photo_wizard ()
{
	_ps_wizard *wizard=NULL;
	//if (wizard!=NULL)
	//{
		wizard = ps_create_wizard (__("Add photos to your album"), __("Begin"), \
								   __("Continue"), __("Go Back"), __("Done"), __("Cancel"), 
								   ps_function_add_photo_wizard, 450, 300);
		ps_create_wizard_dialog (wizard);
		// pagina 1
		GtkWidget *pagina1 = ps_create_mascot_header (pict_path ("logos/add_photos.png"), 
				   __("You have chosen to add some photos to your album. This wizard will guide you through the process. Just press <b>Begin</b> to start."));
		
		// pagina 2
		GtkWidget *pagina2 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina2 = ps_create_mascot_header (pict_path ("logos/nr_of_photos.png"),
				   __("How many photos would you like to add?"));
		gtk_box_pack_start ((GtkBox*)pagina2, hpagina2, FALSE, FALSE, 2);
		GSList *grup_nr_poze;
		GtkWidget *p1 = ps_create_radio_button_with_image (pict_path ("logos/add_one_photo.png"), __("Just <b>one</b> photo."), 2);
		ps_hookup_object (wizard->window, p1, "ps_wizard_add_photo_nr_poze");
		GtkWidget *pn = ps_create_radio_button_with_image (pict_path ("logos/add_more_photos.png"), __("<b>More than one</b> photos."), 2);
		grup_nr_poze = gtk_radio_button_get_group ((GtkRadioButton*)p1);
		gtk_radio_button_set_group ((GtkRadioButton*)pn, grup_nr_poze);
		GtkWidget *hbox_pagina2 = gtk_hbox_new (TRUE, 20);
		gtk_box_pack_start ((GtkBox*)hbox_pagina2, p1, TRUE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)hbox_pagina2, pn, TRUE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)pagina2, hbox_pagina2, TRUE, TRUE, 2);
		
		// pagina 3
		GtkWidget *pagina3 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina3 = ps_create_mascot_header (pict_path ("logos/photo_filename.png"),
				   __("Please enter the filename of the photo that you would like to add"));
		gtk_box_pack_start ((GtkBox*)pagina3, hpagina3, FALSE, FALSE, 2);
		GtkWidget *cadran_img_pagina3 = gtk_viewport_new (NULL, NULL);
		gtk_viewport_set_shadow_type ((GtkViewport*)cadran_img_pagina3, GTK_SHADOW_IN);
		GtkWidget *img_pagina3 = gtk_image_new_from_file (pict_path ("pics/photostorm.png"));
		gtk_container_add ((GtkContainer*)cadran_img_pagina3, img_pagina3);
		gtk_box_pack_start ((GtkBox*)pagina3, cadran_img_pagina3, TRUE, TRUE, 2);
		
		GtkWidget *hbox_pagina3 = gtk_hbox_new (FALSE, 7);
		GtkWidget *l_file = gtk_label_new (__("Filename"));
		GtkWidget *nume_fisier = gtk_entry_new_with_max_length (2000);
		g_signal_connect (nume_fisier, "changed", (GCallback)cb_ps_add_picture_wizard_show_preview, img_pagina3);
		ps_hookup_object (wizard->window, nume_fisier, "ps_wizard_add_photo_nume_fisier");
		GtkWidget *b_browse_pagina3 = ps_create_button_with_image_from_stock ("gtk-open", __("Browse"), 1);
		g_signal_connect (b_browse_pagina3, "clicked", (GCallback)cb_ps_show_open_dialog, wizard);
		gtk_box_pack_start ((GtkBox*)hbox_pagina3, l_file, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)hbox_pagina3, nume_fisier, TRUE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)hbox_pagina3, b_browse_pagina3, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)pagina3, hbox_pagina3, FALSE, FALSE, 2);
		
		// pagina 4
		GtkWidget *pagina4 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina4 = ps_create_mascot_header (pict_path ("logos/add_photos_folder.jpg"),
				   __("Please select a folder to load the photos from"));
		gtk_box_pack_start ((GtkBox*)pagina4, hpagina4, FALSE, FALSE, 2);
		GtkWidget *photos_location = ps_create_location_edit (__("Select the photos location"), 1000, -1, 1);
		ps_hookup_object (wizard->window, photos_location, "ps_wizard_add_photo_photos_location");
		gtk_box_pack_start ((GtkBox*)pagina4, photos_location, FALSE, TRUE, 2);
		
		// pagina 5
		GtkWidget *pagina5 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina5 = ps_create_mascot_header (pict_path ("logos/add_photo_info.jpg"),
				   __("You may add some information about the picture that you would like to add to the album."));
		gtk_box_pack_start ((GtkBox*)pagina5, hpagina5, FALSE, FALSE, 2);
		GtkSizeGroup *label_p5 = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkSizeGroup *edit_p5 = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
		GtkWidget *t_pagina5 = gtk_table_new (3, 2, FALSE);
		gtk_table_set_row_spacings ((GtkTable*)t_pagina5, 5);
		gtk_table_set_col_spacings ((GtkTable*)t_pagina5, 5);
		gtk_box_pack_start ((GtkBox*)pagina5, t_pagina5, TRUE, TRUE, 2);
		
		GtkWidget *nume_poza_p5 = gtk_label_new (__("Title"));
		gtk_label_set_justify ((GtkLabel*)nume_poza_p5, GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment ((GtkMisc*)nume_poza_p5, 0, 0);
		gtk_size_group_add_widget (label_p5, nume_poza_p5);
		gtk_table_attach ((GtkTable*)t_pagina5, nume_poza_p5, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 2, 2);
		GtkWidget *autor_poza_p5 = gtk_label_new (__("Author"));
		gtk_label_set_justify ((GtkLabel*)autor_poza_p5, GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment ((GtkMisc*)autor_poza_p5, 0, 0);
		gtk_size_group_add_widget (label_p5, autor_poza_p5);
		gtk_table_attach ((GtkTable*)t_pagina5, autor_poza_p5, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 2, 2);
		GtkWidget *descriere_poza_p5 = gtk_label_new (__("Description"));
		gtk_label_set_justify ((GtkLabel*)descriere_poza_p5, GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment ((GtkMisc*)descriere_poza_p5, 0, 0);
		gtk_size_group_add_widget (label_p5, descriere_poza_p5);
		gtk_table_attach ((GtkTable*)t_pagina5, descriere_poza_p5, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 2, 2);
		
		GtkWidget *nume_poza_edit_p5 = gtk_entry_new_with_max_length (255);
		ps_hookup_object (wizard->window, nume_poza_edit_p5, "ps_wizard_add_photo_nume_edit");
		gtk_size_group_add_widget (edit_p5, nume_poza_edit_p5);
		gtk_table_attach ((GtkTable*)t_pagina5, nume_poza_edit_p5, 1, 2, 0, 1, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_FILL), 2, 2);
		GtkWidget *autor_poza_edit_p5 = gtk_entry_new_with_max_length (255);
		ps_hookup_object (wizard->window, autor_poza_edit_p5, "ps_wizard_add_photo_autor_edit");
		gtk_size_group_add_widget (edit_p5, autor_poza_edit_p5);
		gtk_table_attach ((GtkTable*)t_pagina5, autor_poza_edit_p5, 1, 2, 1, 2, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_FILL), 2, 2);
		GtkWidget *descriere_poza_edit_scroll_p5 = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)descriere_poza_edit_scroll_p5, GTK_SHADOW_IN);
		gtk_scrolled_window_set_policy ((GtkScrolledWindow*)descriere_poza_edit_scroll_p5, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		GtkWidget *descriere_poza_edit_p5 = gtk_text_view_new ();
		gtk_text_view_set_wrap_mode ((GtkTextView*)descriere_poza_edit_p5, GTK_WRAP_WORD);
		gtk_container_add ((GtkContainer*)descriere_poza_edit_scroll_p5, descriere_poza_edit_p5);
		ps_hookup_object (wizard->window, descriere_poza_edit_p5, "ps_wizard_add_photo_descriere_edit");
		gtk_size_group_add_widget (edit_p5, descriere_poza_edit_scroll_p5);
		gtk_table_attach ((GtkTable*)t_pagina5, descriere_poza_edit_scroll_p5, 1, 2, 2, 3, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), 2, 2);
		
		// pagina 6
		GtkWidget *pagina6 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina6 = ps_create_mascot_header (pict_path ("logos/add_photo_finished.jpg"),
				   __("Photostorm has now enough information to add your photo to the album. Please press <b>Done</b> to complete."));
		gtk_box_pack_start ((GtkBox*)pagina6, hpagina6, TRUE, TRUE, 2);
		GtkWidget *p6_add = ps_create_check_button_with_image_from_stock ("gtk-refresh", __("I would like to <b>add one more photo</b> to my album."), 1);
		ps_hookup_object (wizard->window, p6_add, "ps_wizard_add_photo_add_one_more");
		gtk_box_pack_end ((GtkBox*)pagina6, p6_add, FALSE, FALSE, 2);
		
		// adaugarea paginiilor
		ps_add_wizard_page (wizard, pagina1);
		ps_add_wizard_page (wizard, pagina2);
		ps_add_wizard_page (wizard, pagina3);
		ps_add_wizard_page (wizard, pagina4);
		ps_add_wizard_page (wizard, pagina5);
		ps_add_wizard_page (wizard, pagina6);
		gtk_widget_show_all (wizard->window);
	//}
	ps_wizard_add_photo = wizard;
}

void ps_create_album_wizard ()
{
	_ps_wizard *wizard=NULL;
	//if (wizard!=NULL)
	//{
		wizard = ps_create_wizard (__("Create a new album"), __("Begin"), \
								   __("Continue"), __("Go Back"), __("Done"), __("Cancel"), 
								   ps_function_create_album_wizard, 450, 300);
		ps_create_wizard_dialog (wizard);
		// pagina 1
		GtkWidget *pagina1 = ps_create_mascot_header (pict_path ("logos/create_album.jpg"), 
 				   __("You have chosen to create a new album. This wizard will guide you through the process. Just press <b>Begin</b> to start."));
		
		// pagina 2
		GtkWidget *pagina2 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina2 = ps_create_mascot_header (pict_path ("logos/create_album_name.jpg"),
				   __("Please select a name for your album. It can be anything. (eg: <i>My trip to Canada</i>)"));
		gtk_box_pack_start ((GtkBox*)pagina2, hpagina2, FALSE, FALSE, 2);
		GtkWidget *hbox_pagina2 = gtk_hbox_new (FALSE, 2);
		GtkWidget *nume_album = gtk_label_new (__("Name"));
		GtkWidget *nume_album_edit = gtk_entry_new_with_max_length (255);
		ps_hookup_object (wizard->window, nume_album_edit, "ps_wizard_create_album_nume");
		gtk_box_pack_start ((GtkBox*)hbox_pagina2, nume_album, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)hbox_pagina2, nume_album_edit, TRUE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)pagina2, hbox_pagina2, TRUE, TRUE, 2);
		
		// pagina 3
		GtkWidget *pagina3 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina3 = ps_create_mascot_header (pict_path ("logos/create_album_category.jpg"),
				   __("Please select a category for your album. (eg: <i>My trips</i>)"));
		gtk_box_pack_start ((GtkBox*)pagina3, hpagina3, FALSE, FALSE, 2);
		GtkWidget *hbox_pagina3 = gtk_hbox_new (FALSE, 2);
		GtkWidget *categoria_album = gtk_label_new (__("Cathegory"));
		GtkWidget *categoria_album_edit = gtk_combo_new ();
		gtk_combo_set_value_in_list ((GtkCombo*)categoria_album_edit, FALSE, FALSE);
		gtk_combo_set_popdown_strings ((GtkCombo*)categoria_album_edit, ps_get_albums_categories ());
		ps_hookup_object (wizard->window, categoria_album_edit, "ps_wizard_create_album_categoria");
		gtk_box_pack_start ((GtkBox*)hbox_pagina3, categoria_album, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)hbox_pagina3, categoria_album_edit, TRUE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)pagina3, hbox_pagina3, TRUE, TRUE, 2);

		// pagina 4
		GtkWidget *pagina4 = gtk_vbox_new (FALSE, 7);
		GtkWidget *hpagina4 = ps_create_mascot_header (pict_path ("logos/create_album_author_name.jpg"),
				   __("Please enter your name. As you are creating the album, you are its author."));
		gtk_box_pack_start ((GtkBox*)pagina4, hpagina4, FALSE, FALSE, 2);
		GtkWidget *hbox_pagina4 = gtk_hbox_new (FALSE, 2);
		GtkWidget *autor_album = gtk_label_new (__("Author"));
		GtkWidget *autor_album_edit = gtk_entry_new_with_max_length (255);
		gtk_entry_set_text ((GtkEntry*)autor_album_edit, ps_get_username ());
		ps_hookup_object (wizard->window, autor_album_edit, "ps_wizard_create_album_autor");
		gtk_box_pack_start ((GtkBox*)hbox_pagina4, autor_album, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)hbox_pagina4, autor_album_edit, TRUE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)pagina4, hbox_pagina4, TRUE, TRUE, 2);
		
		// pagina 5
		GtkWidget *pagina5 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina5 = ps_create_mascot_header (pict_path ("logos/create_album_description.jpg"),
				   __("Please enter a description for your new album."));
		gtk_box_pack_start ((GtkBox*)pagina5, hpagina5, FALSE, FALSE, 2);
		GtkWidget *descriere_album = gtk_label_new (__("Description"));
		gtk_misc_set_alignment ((GtkMisc*)descriere_album, 0.0, 0.0);
		GtkWidget *descriere_album_edit_scroll = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)descriere_album_edit_scroll, GTK_SHADOW_IN);
		gtk_scrolled_window_set_policy ((GtkScrolledWindow*)descriere_album_edit_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		GtkWidget *descriere_album_edit = gtk_text_view_new ();
		gtk_text_view_set_wrap_mode ((GtkTextView*)descriere_album_edit, GTK_WRAP_WORD);
		gtk_container_add ((GtkContainer*)descriere_album_edit_scroll, descriere_album_edit);
		ps_hookup_object (wizard->window, descriere_album_edit, "ps_wizard_create_album_descriere");
		gtk_box_pack_start ((GtkBox*)pagina5, descriere_album, FALSE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)pagina5, descriere_album_edit_scroll, TRUE, TRUE, 2);
		
		// pagina 6
		GtkWidget *pagina6 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina6 = ps_create_mascot_header (pict_path ("logos/create_album_password.jpg"),
				   __("You may protect your album with a password. If you don't want to, just click <b>Continue</b>."));
		gtk_box_pack_start ((GtkBox*)pagina6, hpagina6, FALSE, FALSE, 2);
		GtkWidget *hbox_pagina6 = gtk_hbox_new (FALSE, 2);
		GtkWidget *parola1_album = gtk_label_new (__("Password"));
		GtkWidget *parola1_album_edit = gtk_entry_new_with_max_length (255);
		gtk_entry_set_invisible_char ((GtkEntry*)parola1_album_edit, '*');
		gtk_entry_set_visibility ((GtkEntry*)parola1_album_edit, FALSE);
		ps_hookup_object (wizard->window, parola1_album_edit, "ps_wizard_create_album_parola1");
		gtk_box_pack_start ((GtkBox*)hbox_pagina6, parola1_album, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)hbox_pagina6, parola1_album_edit, TRUE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)pagina6, hbox_pagina6, TRUE, TRUE, 2);
		
		// pagina 7
		GtkWidget *pagina7 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina7 = ps_create_mascot_header (pict_path ("logos/create_album_password.jpg"),
				   __("Please type the password one more time in order to be sure that you haven't misspeled it."));
		gtk_box_pack_start ((GtkBox*)pagina7, hpagina7, FALSE, FALSE, 2);
		GtkWidget *hbox_pagina7 = gtk_hbox_new (FALSE, 2);
		GtkWidget *parola2_album = gtk_label_new (__("Retype Password"));
		GtkWidget *parola2_album_edit = gtk_entry_new_with_max_length (255);
		gtk_entry_set_invisible_char ((GtkEntry*)parola2_album_edit, '*');
		gtk_entry_set_visibility ((GtkEntry*)parola2_album_edit, FALSE);
		ps_hookup_object (wizard->window, parola2_album_edit, "ps_wizard_create_album_parola2");
		gtk_box_pack_start ((GtkBox*)hbox_pagina7, parola2_album, FALSE, FALSE, 2);
		gtk_box_pack_start ((GtkBox*)hbox_pagina7, parola2_album_edit, TRUE, TRUE, 2);
		gtk_box_pack_start ((GtkBox*)pagina7, hbox_pagina7, TRUE, TRUE, 2);
		
		// pagina 8
		GtkWidget *pagina8 = gtk_vbox_new (FALSE, 7);		
		GtkWidget *hpagina8 = ps_create_mascot_header (pict_path ("logos/add_photos.png"),
				   __("Would you like to add some photos to your new album?"));
		gtk_box_pack_start ((GtkBox*)pagina8, hpagina8, FALSE, FALSE, 2);
		GtkWidget *vbox_pagina8 = gtk_vbox_new (FALSE, 2);
		GSList *add_poze;
		GtkWidget *p8_yes = ps_create_radio_button_with_image_from_stock ("gtk-yes", __("<b>Yes</b>, that would be great."), 1);
		ps_hookup_object (wizard->window, p8_yes, "ps_wizard_create_album_add_poze_yes");
		gtk_box_pack_start ((GtkBox*)vbox_pagina8, p8_yes, FALSE, FALSE, 2);
		GtkWidget *p8_no = ps_create_radio_button_with_image_from_stock ("gtk-no", __("<b>No</b>, thank you, I will add them later."), 1);
		gtk_box_pack_start ((GtkBox*)vbox_pagina8, p8_no, FALSE, FALSE, 2);
		add_poze = gtk_radio_button_get_group ((GtkRadioButton*)p8_yes);
		gtk_radio_button_set_group ((GtkRadioButton*)p8_no, add_poze);
		gtk_box_pack_start ((GtkBox*)pagina8, vbox_pagina8, FALSE, FALSE, 2);
		
		// pagina 9
		GtkWidget *pagina9 = gtk_vbox_new (FALSE, 7);
		GtkWidget *hpagina9 = ps_create_mascot_header (pict_path ("logos/create_album_finished.jpg"),
				   __("Congratulations. Photostorm has now all the information it needs to create your new album. Press <b>Done</b> to finish."));
		gtk_box_pack_start ((GtkBox*)pagina9, hpagina9, TRUE, TRUE, 2);
		
		// adaugarea paginiilor
		ps_add_wizard_page (wizard, pagina1);
		ps_add_wizard_page (wizard, pagina2);
		ps_add_wizard_page (wizard, pagina3);
		ps_add_wizard_page (wizard, pagina4);
		ps_add_wizard_page (wizard, pagina5);
		ps_add_wizard_page (wizard, pagina6);
		ps_add_wizard_page (wizard, pagina7);
		ps_add_wizard_page (wizard, pagina8);
		ps_add_wizard_page (wizard, pagina9);
		gtk_widget_show_all (wizard->window);
	//}
	ps_wizard_add_photo = wizard;
}
