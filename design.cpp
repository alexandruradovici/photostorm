/**************************************************************
*		File: design.cpp                                       *
*		ID: PS_design_cpp                                      *
*		Author: Alexandru Radovici                             *
*		Date: February 6, 2004                                 *
*		Modified: never                                        *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This is the interface source file of          *
*               PhotoStorm.                                   *
**************************************************************/

#include "design.h"
#include "ps_dialogs.h"
#include "data.h"
#include "lang/translate.h"
#include "misc.h"
#include "ps_system.h"
#include "configs.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget * ps_main_window;

enum
{
	ps_c_albume_nume,
	ps_c_albume_activ,
	
	ps_c_albume_vizibil,
	ps_c_albume_grosime_font,
	ps_c_albume_stil_font,
	ps_c_albume_culoare_font,
	ps_c_albume_poza,
	ps_c_albume_fisier,
	ps_c_albume_numar
};

enum
{
	ps_c_poze_poza,
	ps_c_poze_nume,
	ps_c_poze_autor,
	ps_c_poze_descriere,
	ps_c_poze_optiuni,
	ps_c_poze_activa,
	
	ps_c_poze_dx,
	ps_c_poze_dy,
	ps_c_poze_grosime_font,
	ps_c_poze_stil_font,
	ps_c_poze_culoare_font,
	ps_c_poze_date,
	ps_c_poze_nr,
	ps_c_poze_numar
};

GtkWidget * ps_get_main_window ()
{
	return ps_main_window;
}

// callbacks

gboolean cb_ps_set_wallpaper (gpointer data)
{
	ps_set_wallpaper ((char*)data);
	return TRUE;
}

void cb_ps_main_buton_wallpaper (GtkWidget *buton, gpointer data)
{
	char *wallpaper_file = ps_open_dialog_single_file (ps_get_main_window (), __("Select an image to transform into a wallpaper"), NULL, 1);
	if (wallpaper_file!=NULL)
	{
		_ps_photo *date_poza = ps_create_photo (wallpaper_file);
		if (date_poza != NULL)
		{
			ps_photo_wallpaper (date_poza);
		}
		free (wallpaper_file);
	}
}

void cb_ps_configuration (GtkWidget *buton, gpointer data)
{
	ps_configuration (ps_get_main_window ());
}

void * start_cmd (void *data)
{
	system ((const char*)data);
	g_thread_exit (0);
	return (0);
}

void system__ (const char *cmd)
{
	GThread *firul;
	firul = g_thread_create (start_cmd, (void*)cmd, TRUE, NULL);
}

void cb_ps_open_browser (GtkWidget *buton, gpointer data)
{
	char temp[10000];
	#ifdef LINUX
	sprintf (temp, "mozilla %s", (char*)data);
	#endif
	#ifdef WIN32
	sprintf (temp, "explorer.exe %s", (char*)data);
	#endif
	system__ (temp);
}

void cb_ps_dialogs_result (int dialog, int buton)
{
	if (dialog == PS_ALBUM_DELETE_DIALOG)
	{
		if (buton == PS_BUTTON_YES)
		{
			sterge_album (ps_get_current_album ());
		}
	}
	
	if (dialog == PS_PHOTO_DELETE_DIALOG)
	{
		if (buton == PS_BUTTON_YES)
		{
			GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget("ps_main_lista_poze"));
			GtkTreeModel * model;
			GtkTreeIter pos;
			if (gtk_tree_selection_get_selected (sel, &model, &pos))
			{
				int nr;
				gtk_tree_model_get (model, &pos, ps_c_poze_nr, &nr, -1);
				ps_delete_photo_nr_from_album_file (ps_get_current_album(), nr);
				ps_list_album_from_file (ps_get_current_album());
			}
		}
	}
}

/*void cb_ps_album_selectat (GtkTreeSelection *sel, gpointer data)
{
	GtkTreeIter pos;
	GtkTreeModel *model;
	if (gtk_tree_selection_get_selected (sel, &model, &pos))
	{
		
	}
}*/

void cb_ps_selected_picture (GtkTreeSelection *sel, gpointer user_data)
{
	GtkTreeView * lista_poze;
	GtkTreeModel * model;
	GtkTreeIter pos;
	GdkPixbuf * poza, * poza_preview;
	gchar * date;
	int nr;
	int dx,dy;
	int pdx,pdy;
	lista_poze = gtk_tree_selection_get_tree_view (sel);
	if (gtk_tree_selection_get_selected (sel, &model, &pos))
	{
		gtk_tree_model_get (model, &pos, ps_c_poze_date, &date, ps_c_poze_nr, &nr, -1);
		gtk_window_get_size ((GtkWindow*)ps_get_main_window (), &dx, &dy);
		dx=dx-10-gtk_paned_get_position ((GtkPaned*)ps_get_widget ("ps_main_hpanou"));
		dy=gtk_paned_get_position ((GtkPaned*)ps_get_widget ("ps_main_vpanou"));
		_ps_photo * date_poza = ps_load_photo_nr_from_album_memory (ps_get_current_album_data(), nr);
		if (date_poza!=NULL) 
		{	
			poza = ps_create_pixbuf_from_memory (date_poza->date, date_poza->n_poza);
			ps_photo_free (date_poza);
		}
		else poza=NULL;
		pdx = gdk_pixbuf_get_width (poza);
		pdy = gdk_pixbuf_get_height (poza);
		poza_preview = gdk_pixbuf_scale_simple (poza, dx, dy, GDK_INTERP_BILINEAR);
		g_object_unref (poza);
		/*if (pdx>dx || pdy>dy)
			if (pdx<pdy)
			{
				printf ("%f\n",(double)dy/pdy);
			} else
			{
				poza=gdk_pixbuf_scale_simple (poza, (int)(pdx*(double)dy/pdy)+1, dy, GDK_INTERP_BILINEAR);
			}*/
		//ps_main_imagine = gtk_image_new_from_pixbuf (NULL);
		gtk_image_set_from_pixbuf ((GtkImage*)ps_get_widget ("ps_main_imagine"), poza_preview);
		g_object_unref (poza_preview);
	}
}

void cb_ps_selected_album (GtkTreeSelection *sel, gpointer user_data)
{
	GtkTreeView * lista_albume;
	GtkTreeModel * model;
	GtkTreeIter pos;
	gchar * albumul;
	lista_albume = gtk_tree_selection_get_tree_view (sel);
	if (gtk_tree_selection_get_selected (sel, &model, &pos))
	{
		gtk_tree_model_get (model, &pos, ps_c_albume_fisier, &albumul, -1);
		if (albumul == NULL) albumul = strdup ("");
		ps_list_album_from_file (albumul);
		_ps_config *config = ps_get_config();
		strcpy (config->system.selected_album, albumul);
		free (albumul);
	}
}

void cb_ps_dublu_click_picture (GtkTreeView *lista_poze, GtkTreePath *path_lista, GtkTreeViewColumn *col)
{
	GtkTreeModel * model = gtk_tree_view_get_model (lista_poze);
	int nr=-1;
	if (model!=NULL)
	{
		GtkTreeIter pos;
		if (gtk_tree_model_get_iter (model, &pos, path_lista))
		{
			gtk_tree_model_get (model, &pos, ps_c_poze_nr, &nr, -1);
			//ps_photo_wallpaper (ps_load_photo_nr_from_album_memory (ps_get_current_album_data(), nr));
			ps_photo_wallpaper (ps_load_photo_nr_from_album_file (ps_get_current_album(), nr));
		}
	}
}

void cb_ps_main_album_check (GtkCellRendererToggle *cell, gchar *spath, gpointer data)
{
	GtkTreeModel *model = (GtkTreeModel*)data;
	GtkTreeIter pos;
	GtkTreePath *path = gtk_tree_path_new_from_string (spath);
	gtk_tree_model_get_iter (model, &pos, path);
	gboolean check_box;
	gtk_tree_model_get (model, &pos, ps_c_albume_activ, &check_box, -1);
	check_box = !check_box;
	char s[10];
	int stil_font;
	if (check_box) 
	{
		strcpy (s,"#000000");
		stil_font=PANGO_STYLE_NORMAL;
	}
	else 
	{
		strcpy (s,"#000000");
		stil_font=PANGO_STYLE_ITALIC;
	}
	gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_albume_activ, check_box, 
												    //ps_c_albume_culoare_font, s,
													ps_c_albume_stil_font, stil_font,
													-1);
	gtk_tree_path_free (path);
}

void cb_ps_pozitie_main_menu_lista_albume (GtkMenu *menu, gint &x, gint &y, gboolean *push_in, gpointer *data)
{
	if (ps_fisier_album_curent_din_lista_albume()!=NULL)
	{
		ps_menu_activeaza ("ps_main_menu_lista_albume_sterge");
		ps_menu_activeaza ("ps_main_menu_lista_albume_adauga");
		ps_menu_activeaza ("ps_main_menu_lista_albume_propietati");
	}
	else
	{			
		ps_menu_dezactiveaza ("ps_main_menu_lista_albume_sterge");
		ps_menu_dezactiveaza ("ps_main_menu_lista_albume_adauga");
		ps_menu_dezactiveaza ("ps_main_menu_lista_albume_propietati");
	}
	
	/*GtkMenuItem *item = (GtkMenuItem*)ps_get_widget("ps_main_menu_lista_albume_autor");
	if (item!=NULL)
	{
		GtkWidget *label = GTK_BIN(item)->child;
		char *s = (char*)gtk_label_get_text ((GtkLabel*)label);
		if (s!=NULL)
		{
			if (strcmp(s,__("The Author..."))!=0) ps_menu_activeaza ("ps_main_menu_lista_poze_autor");
				else ps_menu_dezactiveaza ("ps_main_menu_lista_poze_autor");
		}
		else
		{
			gtk_label_set_text ((GtkLabel*)label, __("The Author..."));
			ps_menu_dezactiveaza ("ps_main_menu_lista_poze_autor");
		}
	}*/
	/*gint xw, yw;
	gtk_window_get_position ((GtkWindow*)ps_main_window, &xw, &yw);
	xw=xw+(gint)ps_main_buton_poze->allocation.x;
	yw=yw+(gint)ps_main_buton_poze->allocation.y;
	memcpy (&x, &xw, 4);
	memcpy (&y, &yw, 4);
	printf ("%d, %d\n",x,y);*/
}

void cb_ps_pozitie_main_menu_lista_poze (GtkMenu *menu, gint &x, gint &y, gboolean *push_in, gpointer *data)
{
	GtkTreeSelection *sel;
	sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget ("ps_main_lista_poze"));
	if (sel!=NULL)
	{
		GtkTreeModel *model;
		GtkTreeIter pos;
		if (gtk_tree_selection_get_selected (sel, &model, &pos)) 
		{
			ps_menu_activeaza ("ps_main_menu_lista_poze_wallpaper");
			ps_menu_activeaza ("ps_main_menu_lista_poze_export");
			ps_menu_activeaza ("ps_main_menu_lista_poze_sterge");
			ps_menu_activeaza ("ps_main_menu_lista_poze_propietati");
		}
		else
		{			
			ps_menu_dezactiveaza ("ps_main_menu_lista_poze_wallpaper");
			ps_menu_dezactiveaza ("ps_main_menu_lista_poze_export");
			ps_menu_dezactiveaza ("ps_main_menu_lista_poze_sterge");
			ps_menu_dezactiveaza ("ps_main_menu_lista_poze_propietati");
			GtkMenuItem *item = (GtkMenuItem*)ps_get_widget ("ps_main_menu_lista_poze_autor");
			if (item!=NULL)
			{
				GtkWidget *label = GTK_BIN(item)->child;
				gtk_label_set_text ((GtkLabel*)label, __("The Author..."));
			}
		}
	} 
	if (ps_fisier_album_curent_din_lista_albume()!=NULL)
	{
		ps_menu_activeaza ("ps_main_menu_lista_poze_adauga");
	}
	else
	{			
		ps_menu_dezactiveaza ("ps_main_menu_lista_poze_wallpaper");
		ps_menu_dezactiveaza ("ps_main_menu_lista_poze_export");
		ps_menu_dezactiveaza ("ps_main_menu_lista_poze_adauga");
		ps_menu_dezactiveaza ("ps_main_menu_lista_poze_sterge");
		ps_menu_dezactiveaza ("ps_main_menu_lista_poze_propietati");
	}
	
	GtkMenuItem *item = (GtkMenuItem*)ps_get_widget("ps_main_menu_lista_poze_autor");
	if (item!=NULL)
	{
		GtkWidget *label = GTK_BIN(item)->child;
		char *s = (char*)gtk_label_get_text ((GtkLabel*)label);
		if (s!=NULL)
		{
			/*if (strcmp(s,__("The Author..."))!=0) ps_menu_activeaza ("ps_main_menu_lista_poze_autor");
				else*/ ps_menu_dezactiveaza ("ps_main_menu_lista_poze_autor");
		}
		else
		{
			gtk_label_set_text ((GtkLabel*)label, __("The Author..."));
			ps_menu_dezactiveaza ("ps_main_menu_lista_poze_autor");
		}
	}
	/*gint xw, yw;
	gtk_window_get_position ((GtkWindow*)ps_main_window, &xw, &yw);
	xw=xw+(gint)ps_main_buton_poze->allocation.x;
	yw=yw+(gint)ps_main_buton_poze->allocation.y;
	memcpy (&x, &xw, 4);
	memcpy (&y, &yw, 4);
	printf ("%d, %d\n",x,y);*/
}

void cb_ps_main_buton_albume (GtkButton *buton, gpointer data)
{
	gtk_menu_popup ((GtkMenu*)ps_get_widget ("ps_main_menu_lista_albume"), NULL, NULL, (GtkMenuPositionFunc)cb_ps_pozitie_main_menu_lista_albume, NULL, 0, gtk_get_current_event_time ());
}

void cb_ps_main_buton_poze (GtkButton *buton, gpointer data)
{
	gtk_menu_popup ((GtkMenu*)ps_get_widget ("ps_main_menu_lista_poze"), NULL, NULL, (GtkMenuPositionFunc)cb_ps_pozitie_main_menu_lista_poze, NULL, 0, gtk_get_current_event_time ());
}

void cb_ps_main_menu (GtkMenuItem *item, gpointer user_data)
{
	char *data = (char*)user_data;
	if (data!=NULL)
	{
		if (strcmp(data,"ps_lista_albume_nou")==0) ps_create_album_wizard();
		if (strcmp(data,"ps_lista_albume_import")==0)
		{
			/*GtkWidget *files = gtk_file_selection_new (__("Select an album to import"));
			if (gtk_dialog_run ((GtkDialog*)files)==GTK_RESPONSE_OK)
			{
				char *albumul = strdup(gtk_file_selection_get_filename ((GtkFileSelection*)files));
			}
			gtk_widget_destroy (files);*/
			char *albumul = ps_open_dialog_single_file (ps_get_main_window(), __("Select an album to import"), NULL, false);
			if (albumul!=NULL) 
			{
				import_album (albumul);
				_ps_config *config = ps_get_config ();
				ps_clear_albums_list ();
				ps_load_albums_from_path (config->system.shared_albums);
				ps_load_albums_from_path (config->system.albums);	
				selecteaza_album (albumul);
				free (albumul);
			}
		}
		if (strcmp(data,"ps_lista_albume_sterge")==0)
		{
			ps_question_dialog ("gtk-dialog-warning", 1, 
								__("Are you sure that you want to <b>delete</b> this album? All the pictures in the album will be deleted. This action is not undoable!"), 
								ps_get_main_window (), PS_ALBUM_DELETE_DIALOG,
								cb_ps_dialogs_result);
		}
		
		if (strcmp(data,"ps_lista_poze_wallpaper")==0)
		{
			GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget("ps_main_lista_poze"));
			GtkTreeModel * model;
			GtkTreeIter pos;
			if (gtk_tree_selection_get_selected (sel, &model, &pos))
			{
				int nr;
				gtk_tree_model_get (model, &pos, ps_c_poze_nr, &nr, -1);
				_ps_photo *date_poza = ps_load_photo_nr_from_album_file (ps_get_current_album (), nr);
				ps_photo_wallpaper (date_poza);
			}
		}
		
		if (strcmp(data,"ps_lista_poze_export")==0)
		{
			/*GtkWidget *files = gtk_file_selection_new (__("Select an album to import"));
			if (gtk_dialog_run ((GtkDialog*)files)==GTK_RESPONSE_OK)
			{
				char *albumul = strdup(gtk_file_selection_get_filename ((GtkFileSelection*)files));
			}
			gtk_widget_destroy (files);*/
			GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget("ps_main_lista_poze"));
			GtkTreeModel * model;
			GtkTreeIter pos;
			if (gtk_tree_selection_get_selected (sel, &model, &pos))
			{
				int nr;
				gtk_tree_model_get (model, &pos, ps_c_poze_nr, &nr, -1);
				_ps_photo *date_poza = ps_load_photo_nr_from_album_file (ps_get_current_album (), nr);
				if (date_poza!=NULL)
				{
					char temp[1000];
					sprintf (temp, "%s___%s.jpg", ps_strfilter_filename (date_poza->autor), ps_strfilter_filename (date_poza->nume));
					char *poza = ps_save_dialog_single_file (ps_get_main_window(), __("Where do you want to save the picture?"), temp, false);
					if (poza!=NULL)
					{
						int n=0;
						char *export_data = ps_create_photo_export_data (date_poza->date, date_poza->n_poza, n);
						if (export_data!=NULL && n>0) 
						{
							FILE *f = fopen (poza, "wb");
							if (f!=NULL)
							{
								fwrite (export_data, n, 1, f); 
								fclose (f);
							}
							else ps_information_dialog ("gtk-dialog-info", 1, "We are sorry, but the picture could <b>not</b> be <b>exported</b>. Please try to choose a different location for it.", ps_get_main_window());
							if (export_data!=NULL) free (export_data);
						}
					}
					ps_photo_free (date_poza);
				}
			}
		}
		
		if (strcmp(data,"ps_lista_poze_adauga")==0) ps_add_photo_wizard();
		if (strcmp(data,"ps_lista_poze_sterge_poza_nr")==0)
		{
			/*GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget("ps_main_lista_poze"));
			GtkTreeModel * model;
			GtkTreeIter pos;
			if (gtk_tree_selection_get_selected (sel, &model, &pos))
			{
				int nr;
				gtk_tree_model_get (model, &pos, ps_c_poze_nr, &nr, -1);
				ps_sterge_poza_nr_din_album_fisier (album_path("test"), nr);
				ps_afiseaza_album_din_fisier (album_path("test"));
			}*/
			ps_question_dialog ("gtk-dialog-warning", 1, 
								__("Are you sure that you want to <b>delete</b> this picture? This action is not undoable!"), 
								ps_get_main_window (), PS_PHOTO_DELETE_DIALOG, 
								cb_ps_dialogs_result);
		}
		
		if (strcmp(data,"ps_lista_albume_propietati")==0)
		{
			_ps_album *date_album = ps_load_album (ps_get_current_album ());
			if (date_album!=NULL)
			{
				_ps_album_properties *props = (_ps_album_properties*)malloc (sizeof(_ps_album_properties));
				props->parent = ps_get_main_window ();
				props->tip_album = 0;
				props->date_album = date_album;
				ps_album_properties_dialog (props);
			}
		}
		
		if (strcmp(data,"ps_lista_poze_propietati")==0)
		{
			GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget("ps_main_lista_poze"));
			GtkTreeModel * model;
			GtkTreeIter pos;
			if (gtk_tree_selection_get_selected (sel, &model, &pos))
			{
				int nr;
				gtk_tree_model_get (model, &pos, ps_c_poze_nr, &nr, -1);
				_ps_photo *date_poza = ps_load_photo_nr_from_album_file (ps_get_current_album (), nr);
				_ps_photo_properties *props = (_ps_photo_properties*)malloc (sizeof(_ps_photo_properties));
				props->parent = ps_get_main_window ();
				props->tip_album = 0;
				props->photo_nr = nr;
				props->date_poza = date_poza;
				ps_photo_properties_dialog (props);
			}
		}
	}
}

gboolean cb_ps_meniu_lista_albume (GtkWidget *lista_albume, GdkEventButton *buton, gpointer data)
{
	gboolean result=FALSE;
	if (buton->button==3)
	{
		GtkTreeSelection *sel =	gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
		if (sel!=NULL)
		{
			GtkTreeModel *model;
			GtkTreeIter pos;
			char *fisier;
			if (gtk_tree_selection_get_selected (sel, &model, &pos))
			{
				gtk_tree_model_get (model, &pos, ps_c_albume_fisier, &fisier, -1);
				if (fisier!=NULL)
				{
				}
			}
		}
		gtk_menu_popup ((GtkMenu*)ps_get_widget ("ps_main_menu_lista_albume"), NULL, NULL, (GtkMenuPositionFunc)cb_ps_pozitie_main_menu_lista_albume, NULL, buton->button, gtk_get_current_event_time ());
	}
	return result;
}
	
gboolean cb_ps_meniu_lista_poze (GtkWidget *lista_poze, GdkEventButton *buton, gpointer data)
{
	gboolean result=FALSE;
	int afiseaza=0;
	if (buton->button==3)
	{
		GtkTreeSelection *sel =	gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget ("ps_main_lista_poze"));
		if (sel!=NULL)
		{
			GtkTreeModel *model;
			GtkTreeIter pos;
			char *autor;
			int se_stie_autorul=1;
			if (gtk_tree_selection_get_selected (sel, &model, &pos))
			{
				gtk_tree_model_get (model, &pos, ps_c_poze_autor, &autor, -1);
				if (autor!=NULL)
				{
					if (strcmp(autor,"")!=0)
					{
						GtkMenuItem *item = (GtkMenuItem*)ps_get_widget ("ps_main_menu_lista_poze_autor");
						GtkWidget *label = GTK_BIN(item)->child;
						char *text_autor = (char*)malloc (strlen (__("About"))+strlen (autor)+5);
						sprintf (text_autor, "%s %s...", __("About"), autor);
						if (label!=NULL) gtk_label_set_text ((GtkLabel*)label, text_autor);
						free (text_autor);
					} else se_stie_autorul=0;
				} else se_stie_autorul=0;
				if (!se_stie_autorul)
				{
					GtkMenuItem *item = (GtkMenuItem*)ps_get_widget ("ps_main_menu_lista_poze_autor");
					GtkWidget *label = GTK_BIN(item)->child;
					gtk_label_set_text ((GtkLabel*)label, __("The Author..."));
				}
				afiseaza=1;				
			}
			else 
			if (ps_fisier_album_curent_din_lista_albume()!=NULL)
			{
				GtkMenuItem *item = (GtkMenuItem*)ps_get_widget ("ps_main_menu_lista_poze_autor");
				GtkWidget *label = GTK_BIN(item)->child;
				gtk_label_set_text ((GtkLabel*)label, __("The Author..."));
				afiseaza = 1;
			}
		} 
		if (afiseaza==1) gtk_menu_popup ((GtkMenu*)ps_get_widget ("ps_main_menu_lista_poze"), NULL, NULL, (GtkMenuPositionFunc)cb_ps_pozitie_main_menu_lista_poze, NULL, buton->button, gtk_get_current_event_time ());
	}
	return result;
}

void cb_ps_main_exit (GtkObject *object, gpointer data)
{
	ps_save_albums_cache ();
	gtk_main_quit ();
}

/*** asta vine mutate de aici ***/
void adauga_categorie (const char *categoria)
{
	if (categoria!=NULL)
	{
		GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget("ps_main_lista_albume"));
		GtkTreeIter pos;
		gtk_tree_store_append ((GtkTreeStore*)model,&pos,NULL);
		gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_albume_nume, categoria, 
							ps_c_albume_activ, TRUE, 
		
							ps_c_albume_fisier, NULL,
							ps_c_albume_vizibil, FALSE,
							ps_c_albume_grosime_font, PANGO_WEIGHT_BOLD,
							ps_c_albume_stil_font, PANGO_STYLE_NORMAL,
							ps_c_albume_poza, gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/collection.png")),24,24,GDK_INTERP_BILINEAR),
							//ps_c_albume_culoare_font, "#000000",
							-1);
	}
}

void adauga_album (const char *categoria, const char *albumul, const char *fisierul, int activ)
{
	if (categoria!=NULL && albumul!=NULL && fisierul!=NULL)
	{
		GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
		GtkTreeIter pos,s_pos;
		int gasit=0;
		if (gtk_tree_model_get_iter_first (model, &s_pos))
		do
		{
			gchar *s;
			gtk_tree_model_get (model, &s_pos, ps_c_albume_nume, &s, -1);
			if (s!=NULL)
			{
				if (strcmp (s,(char*)categoria)==0) 
				{	
					gasit=1;
					pos=s_pos;
				}
			}
		} while (gtk_tree_model_iter_next (model, &s_pos) && !gasit);
		if (!gasit) 
		{
			adauga_categorie (categoria);
			gasit=0;
			if (gtk_tree_model_get_iter_first (model, &s_pos))
			do
			{
				gchar *s;
				gtk_tree_model_get (model, &s_pos, ps_c_albume_nume, &s, -1);
				if (s!=NULL)
				{
					if (strcmp (s,(char*)categoria)==0) 
					{	
						gasit=1;
						pos=s_pos;
					}
				}
			} while (gtk_tree_model_iter_next (model, &s_pos) && !gasit);
		}
		if (gasit)
		{
			GtkTreeIter pos_;
			gboolean a;
			int stil_font;
			if (activ) 
			{	
				a=TRUE;
				stil_font=PANGO_STYLE_NORMAL;
			}
			else
			{				
				a=FALSE;
				stil_font=PANGO_STYLE_ITALIC;
			}
			gtk_tree_store_append ((GtkTreeStore*)model,&pos_,&pos);
			gtk_tree_store_set ((GtkTreeStore*)model, &pos_, ps_c_albume_nume, albumul, 
								ps_c_albume_activ, a,

								ps_c_albume_fisier, fisierul,			
								ps_c_albume_vizibil, TRUE,
								ps_c_albume_grosime_font, PANGO_WEIGHT_NORMAL,
								ps_c_albume_stil_font, stil_font,
								ps_c_albume_poza, gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/album.png")),24,24,GDK_INTERP_BILINEAR),
								//ps_c_albume_culoare_font, "#000000",
								-1);
		}
	}
}

void sterge_album (const char *fisierul)
{
	remove (fisierul);
	ps_clear_albums_list ();
	_ps_config *config = ps_get_config ();
	ps_load_albums_from_path (config->system.shared_albums);
	ps_load_albums_from_path (config->system.albums);	
}

char * ps_get_random_album ()
{
	char * result = NULL;
	GList *lista_albume = NULL;
	GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
	GtkTreeIter s_pos,parent_pos;
	if (model!=NULL)
	{
	//gtk_tree_selection_unselect_all (sel);
		if (gtk_tree_model_get_iter_first (model, &parent_pos))
		do
		{
			gchar *s;
			int activ;
			if (gtk_tree_model_iter_has_child (model, &parent_pos))
			if (gtk_tree_model_iter_children (model, &s_pos, &parent_pos))	
			do
			{
				gtk_tree_model_get (model, &s_pos, ps_c_albume_fisier, &s, ps_c_albume_activ, &activ,-1);
				if (s!=NULL && activ)
				{
					lista_albume = g_list_append (lista_albume, strdup(s));
				}
			} while (gtk_tree_model_iter_next (model, &s_pos));
		} while (gtk_tree_model_iter_next (model, &parent_pos));
	}
	int nr = g_list_length (lista_albume);
	if (nr>0)
	{
		int i = ps_random (nr);
		char *albumul = (char*)g_list_nth_data (lista_albume, i);
		if (albumul!=NULL) 
		{	
			result = strdup (albumul);
			free (albumul);
		}
		if (lista_albume!=NULL) g_list_free (lista_albume);
	}
	return result;
}

void ps_write_albums_info (FILE *f)
{
	if (f==NULL) return;
	GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
	GtkTreeIter s_pos,parent_pos;
	if (model!=NULL)
	{
	//gtk_tree_selection_unselect_all (sel);
		if (gtk_tree_model_get_iter_first (model, &parent_pos))
		do
		{
			gchar *s;
			int activ;
			if (gtk_tree_model_iter_has_child (model, &parent_pos))
			if (gtk_tree_model_iter_children (model, &s_pos, &parent_pos))	
			do
			{
				gtk_tree_model_get (model, &s_pos, ps_c_albume_fisier, &s, ps_c_albume_activ, &activ,-1);
				if (s!=NULL)
				{
					fprintf (f, "%d %s\n", activ, s);
				}
			} while (gtk_tree_model_iter_next (model, &s_pos));
		} while (gtk_tree_model_iter_next (model, &parent_pos));
	}
}

int selecteaza_album (const char *fisierul)
{
	int gasit=0;
	if (fisierul!=NULL)
	{
		GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
		GtkTreeIter pos,s_pos,parent_pos;
		GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
		//gtk_tree_selection_unselect_all (sel);
		if (gtk_tree_model_get_iter_first (model, &parent_pos))
		do
		{
			gchar *s;
			if (gtk_tree_model_iter_has_child (model, &parent_pos))
			if (gtk_tree_model_iter_children (model, &s_pos, &parent_pos))	
			do
			{
				gtk_tree_model_get (model, &s_pos, ps_c_albume_fisier, &s, -1);
				if (s!=NULL)
				{
					//printf ("(gasit: %s)\n", s);
					if (strcmp (s,(char*)fisierul)==0) 
					{	
						//printf ("(%s)\n", s);
						gasit=1;
						pos=s_pos;
						gtk_tree_selection_select_iter (sel, &pos);
						//cb_ps_album_selectat (sel, NULL);
					}
				}
				g_free (s);
			} while (gtk_tree_model_iter_next (model, &s_pos) && !gasit);
		} while (!gasit && gtk_tree_model_iter_next (model, &parent_pos));
	}
	return gasit;
}

int exista_album (const char *fisierul)
{
	int gasit=0;
	if (fisierul!=NULL)
	{
		GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
		GtkTreeIter pos,s_pos,parent_pos;
		//gtk_tree_selection_unselect_all (sel);
		if (gtk_tree_model_get_iter_first (model, &parent_pos))
		do
		{
			gchar *s;
			if (gtk_tree_model_iter_has_child (model, &parent_pos))
			if (gtk_tree_model_iter_children (model, &s_pos, &parent_pos))	
			do
			{
				gtk_tree_model_get (model, &s_pos, ps_c_albume_fisier, &s, -1);
				if (s!=NULL)
				{
					//printf ("(gasit: %s)\n", s);
					if (strcmp (s,(char*)fisierul)==0) 
					{	
						//printf ("(%s)\n", s);
						gasit=1;
						pos=s_pos;
						//cb_ps_album_selectat (sel, NULL);
					}
				}
				g_free (s);
			} while (gtk_tree_model_iter_next (model, &s_pos) && !gasit);
		} while (!gasit && gtk_tree_model_iter_next (model, &parent_pos));
	}
	return gasit;
}

void ps_set_album (const char *fisierul, int activ)
{
	if (fisierul!=NULL)
	{
		GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
		GtkTreeIter pos,s_pos,parent_pos;
		//gtk_tree_selection_unselect_all (sel);
		int gasit=0;
		if (gtk_tree_model_get_iter_first (model, &parent_pos))
		do
		{
			gchar *s;
			if (gtk_tree_model_iter_has_child (model, &parent_pos))
			if (gtk_tree_model_iter_children (model, &s_pos, &parent_pos))	
			do
			{
				gtk_tree_model_get (model, &s_pos, ps_c_albume_fisier, &s, -1);
				if (s!=NULL)
				{
					//printf ("(gasit: %s)\n", s);
					if (strcmp (s,(char*)fisierul)==0) 
					{	
						//printf ("(%s)\n", s);
						gasit=1;
						pos=s_pos;
						int stil_font;
						if (activ) 
						{
							stil_font=PANGO_STYLE_NORMAL;
						}
						else 
						{
							stil_font=PANGO_STYLE_ITALIC;
						}
						gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_albume_activ, activ, 
											ps_c_albume_activ, activ, 
											ps_c_albume_stil_font, stil_font,
											-1);
					}
				}
				g_free (s);
			} while (gtk_tree_model_iter_next (model, &s_pos) && !gasit);
		} while (!gasit && gtk_tree_model_iter_next (model, &parent_pos));
	}
}

void selecteaza_primul_album ()
{
		GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
		GtkTreeIter pos,s_pos,parent_pos;
		GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
		//gtk_tree_selection_unselect_all (sel);
		int gasit=0;
		if (gtk_tree_model_get_iter_first (model, &parent_pos))
		do
		{
			gchar *s;
			if (gtk_tree_model_iter_has_child (model, &parent_pos))
			if (gtk_tree_model_iter_children (model, &s_pos, &parent_pos))	
			do
			{
				gtk_tree_model_get (model, &s_pos, ps_c_albume_fisier, &s, -1);
				if (s!=NULL)
				{
					gasit=1;
					pos=s_pos;
					gtk_tree_selection_select_iter (sel, &pos);
				}
			} while (gtk_tree_model_iter_next (model, &s_pos) && !gasit);
		} while (!gasit && gtk_tree_model_iter_next (model, &parent_pos));
}

void adauga_poza (GdkPixbuf *poza, char *nume, char *autor, char *descriere, char *optiuni, char *metadata, int nr, gboolean activa)
{
	GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_poze"));
	GtkTreeIter pos;
	int dx,dy;
	
	gtk_tree_store_append ((GtkTreeStore*)model,&pos,NULL);
	dx=gdk_pixbuf_get_width (poza);
	dy=gdk_pixbuf_get_height (poza);
	//poza = gdk_pixbuf_scale_simple (poza,(int)(50*(double)dx/dy),50,GDK_INTERP_BILINEAR);
	gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_poze_nume, nume, 
						ps_c_poze_poza, poza,
						ps_c_poze_autor, autor,
						ps_c_poze_descriere, descriere,
						ps_c_poze_optiuni, optiuni,
						ps_c_poze_date, metadata,
						ps_c_poze_dx, dx+5,
						ps_c_poze_dy, dy+5,
 						//ps_c_albume_culoare_font, "#000000",
						ps_c_poze_nr, nr,
						-1);
}

char *ps_fisier_album_curent_din_lista_albume ()
{
	GtkTreeModel * model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
	GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
	gchar * albumul = NULL;
	if (sel!=NULL)
	{
		GtkTreeIter pos;
		if (gtk_tree_selection_get_selected (sel, &model, &pos)) gtk_tree_model_get (model, &pos, ps_c_albume_fisier, &albumul, -1);
	}
	return (char*)albumul;
}

char * ps_get_current_album ()
{
	GtkTreeView * lista_albume = (GtkTreeView*)ps_get_widget ("ps_main_lista_albume");
	GtkTreeSelection * sel = gtk_tree_view_get_selection (lista_albume);
	char * albumul = NULL;
	if (sel!=NULL)
	{
		GtkTreeModel * model;
		GtkTreeIter pos;
		//lista_albume = gtk_tree_selection_get_tree_view (sel);
		if (gtk_tree_selection_get_selected (sel, &model, &pos))
		{
		    gtk_tree_model_get (model, &pos, ps_c_albume_fisier, &albumul, -1);
		}
	}
	return albumul;
}

GList * ps_get_albums_categories ()
{
	GList *lista = NULL;
	GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
	GtkTreeIter pos;
	if (gtk_tree_model_get_iter_first (model, &pos))
	{
		lista = g_list_alloc ();
		do
		{
			gchar *s;
			gtk_tree_model_get (model, &pos, ps_c_albume_nume, &s, -1);
			if (s!=NULL)
			{
				g_list_append (lista, s);
			}
		} while (gtk_tree_model_iter_next (model, &pos));
	}
	return lista;
}
/********************************/


GtkTreeModel * ps_model_lista_albume ()
{
	GtkTreeStore *store = gtk_tree_store_new (ps_c_albume_numar,G_TYPE_STRING,G_TYPE_BOOLEAN,
											  G_TYPE_BOOLEAN,G_TYPE_INT,G_TYPE_INT,G_TYPE_STRING,
											  GDK_TYPE_PIXBUF,G_TYPE_STRING);
	return (GtkTreeModel*) store;
}

GtkTreeModel * ps_model_lista_poze ()
{
	GtkTreeStore *store = gtk_tree_store_new (ps_c_poze_numar,GDK_TYPE_PIXBUF,G_TYPE_STRING,
											  G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,
											  G_TYPE_BOOLEAN,G_TYPE_INT,G_TYPE_INT,
											  G_TYPE_INT,G_TYPE_STRING,G_TYPE_STRING,
											  G_TYPE_STRING,G_TYPE_INT);
	return (GtkTreeModel*) store;
}

GtkWidget * ps_create_lista_albume ()
{
	GtkWidget * lista_albume = gtk_tree_view_new_with_model (ps_model_lista_albume());
	GtkCellRenderer *render;
	GtkTreeSelection *sel;
	GtkTreeViewColumn *col;
	
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, "Nume Album");
	render = gtk_cell_renderer_pixbuf_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 0.5, NULL);
	gtk_tree_view_column_set_attributes (col, render, "pixbuf", ps_c_albume_poza,
													NULL);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, TRUE);
	g_object_set ((GObject*)render, "xalign", 0.0, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_albume_nume, 
													"foreground", ps_c_albume_culoare_font,
													"weight", ps_c_albume_grosime_font,
													"style", ps_c_albume_stil_font,
													NULL);
	gtk_tree_view_column_set_resizable (col, FALSE);
	gtk_tree_view_column_set_min_width (col, 160);
	gtk_tree_view_column_set_max_width (col, 300);
	gtk_tree_view_append_column ((GtkTreeView*)lista_albume, col);
	
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col,"Activ");
	render = gtk_cell_renderer_toggle_new ();
	gtk_tree_view_column_pack_end (col, render, FALSE);
	g_signal_connect (render, "toggled", (GCallback)cb_ps_main_album_check, gtk_tree_view_get_model ((GtkTreeView*)lista_albume));
	g_object_set ((GObject*)render, "xalign", 0.0, NULL);
	gtk_tree_view_column_set_attributes (col,render, "active", ps_c_albume_activ, 
												    "visible", ps_c_albume_vizibil,
													NULL);
	gtk_tree_view_column_set_resizable (col, FALSE);
	gtk_tree_view_column_set_max_width (col, 40);
	gtk_tree_view_append_column ((GtkTreeView*)lista_albume, col);
	
	gtk_widget_set_size_request (lista_albume, 230, 100);
	gtk_tree_view_expand_all ((GtkTreeView*)lista_albume);
	gtk_tree_view_set_headers_visible ((GtkTreeView*)lista_albume, FALSE);
	gtk_tree_view_set_rules_hint ((GtkTreeView*)lista_albume, TRUE);
	
	sel = gtk_tree_view_get_selection ((GtkTreeView*)lista_albume);
	//g_signal_connect (lista_poze, "row-activated", (GCallback)cb_ps_dublu_click_picture, NULL);
	gtk_tree_selection_set_mode (sel, GTK_SELECTION_SINGLE);
	g_signal_connect (sel, "changed", (GCallback)cb_ps_selected_album, NULL);
	g_signal_connect (lista_albume, "button-release-event", (GCallback)cb_ps_meniu_lista_albume, NULL);
	return lista_albume;
}

GtkWidget * ps_create_lista_poze ()
{
	GtkWidget * lista_poze = gtk_tree_view_new_with_model (ps_model_lista_poze());
	GtkCellRenderer * render;
	GtkTreeSelection * sel;
	GtkTreeViewColumn * col;
	
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Photo"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_poze_nume);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_pixbuf_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.5, "yalign", 0.5, NULL);
	gtk_tree_view_column_set_attributes (col, render, "pixbuf", ps_c_poze_poza, 
										 "width", ps_c_poze_dx,
										 "height", ps_c_poze_dy,
										 NULL);
	render = ps_photo_cell_renderer_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 0.0, NULL);
	gtk_tree_view_column_set_attributes (col, render, "nume", ps_c_poze_nume, "autor", ps_c_poze_autor, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_poze, col);
	
	/*col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Properties"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_poze_optiuni);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.5, "yalign", 1.0, "style", PANGO_STYLE_NORMAL, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_poze_optiuni, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_poze, col);*/
	
	gtk_widget_set_size_request (lista_poze, 100, 100);
	gtk_tree_view_set_headers_visible ((GtkTreeView*)lista_poze, FALSE);
	gtk_tree_view_set_rules_hint ((GtkTreeView*)lista_poze, TRUE);
	
	sel = gtk_tree_view_get_selection ((GtkTreeView*)lista_poze);
	g_signal_connect (lista_poze, "row-activated", (GCallback)cb_ps_dublu_click_picture, NULL);
	gtk_tree_selection_set_mode (sel, GTK_SELECTION_SINGLE);
	g_signal_connect (sel, "changed", (GCallback)cb_ps_selected_picture, NULL);
	
	g_signal_connect (lista_poze, "button-release-event", (GCallback)cb_ps_meniu_lista_poze, NULL);
	
	return lista_poze;
}

int ps_sterge_lista_poze ()
{
	int stearsa=0;
	GtkTreeModel * lista_poze = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_poze"));
	if (lista_poze!=NULL)
	{
		gtk_tree_store_clear ((GtkTreeStore*)lista_poze);
		gtk_image_set_from_file ((GtkImage*)ps_get_widget ("ps_main_imagine"), NULL);
		stearsa=1;
	}
	return stearsa;
}

int ps_sterge_lista_albume ()
{
	int stearsa=0;
	GtkTreeModel * lista_albume = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
	if (lista_albume!=NULL)
	{
		gtk_tree_store_clear ((GtkTreeStore*)lista_albume);
		//gtk_image_set_from_file ((GtkImage*)ps_get_widget ("ps_main_imagine"), NULL);
		stearsa=1;
	}
	return stearsa;
}

void ps_extinde_lista_albume ()
{
	gtk_tree_view_expand_all ((GtkTreeView*)ps_get_widget ("ps_main_lista_albume"));
}

void ps_lista_poze_selecteaza_prima_poza ()
{
	GtkTreeModel * lista_poze = gtk_tree_view_get_model ((GtkTreeView*)ps_get_widget ("ps_main_lista_poze"));
	GtkTreeIter pos;
	GtkTreeSelection *sel = gtk_tree_view_get_selection ((GtkTreeView*)ps_get_widget ("ps_main_lista_poze"));
	if (gtk_tree_model_get_iter_first (lista_poze, &pos)) gtk_tree_selection_select_iter (sel, &pos);
}

void ps_menu_activeaza (char *data)
{
	if (data!=NULL)
	{
		GtkMenuItem *item = (GtkMenuItem*)ps_get_widget(data);
		if (item!=NULL) gtk_widget_set_sensitive ((GtkWidget*)item, TRUE);
	}
}

void ps_menu_dezactiveaza (char *data)
{
	if (data!=NULL)
	{
		GtkMenuItem *item = (GtkMenuItem*)ps_get_widget(data);
		if (item!=NULL) gtk_widget_set_sensitive ((GtkWidget*)item, FALSE);
	}
}

GtkWidget * ps_create_meniu_albume()
{
	GtkWidget *meniu_albume;
	meniu_albume = gtk_menu_new ();
	GtkWidget *item;
	GtkWidget *item_img;
	/*// pune tapetul ecranului
	item_img = gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/photos.png")), 24, 24, GDK_INTERP_BILINEAR));
	item = gtk_image_menu_item_new_with_label (__("Photos"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_tooltips_set_tip ((GtkTooltips*)ps_get_widget ("ps_main_tips"), item, 
						  __("Options regarding the photos in this album"), "");
	gtk_menu_append ((GtkMenu*)meniu_albume, (GtkWidget*)ps_get_widget ("ps_main_meniu_lista_poze"));
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_wallpaper");
	// informatii despre autor
	item_img = gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/wallpaper.png")), 24, 24, GDK_INTERP_BILINEAR));
	item = gtk_image_menu_item_new_with_label (__("The Author..."));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_tooltips_set_tip ((GtkTooltips*)ps_get_widget ("ps_main_tips"), item, 
						  __("Find more information about the photo's author."), "");
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_autor");
	// export
	item_img = gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/wallpaper.png")), 24, 24, GDK_INTERP_BILINEAR));
	item = gtk_image_menu_item_new_with_label (__("Export"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_tooltips_set_tip ((GtkTooltips*)ps_get_widget ("ps_main_tips"), item, 
						  __("Save the selected photo to your hard drive."), "");
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_export");*/
	// adaugare
	item_img = gtk_image_new_from_stock ("gtk-new", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (__("Create Album"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu_albume, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_nou");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_albume_nou"));
	// stergere
	item_img = gtk_image_new_from_stock ("gtk-remove", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (__("Erase"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_sterge");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_albume_sterge"));
	gtk_menu_append ((GtkMenu*)meniu_albume, item);
	// linia
	item = gtk_image_menu_item_new ();
	gtk_widget_set_sensitive (item, FALSE);
	gtk_menu_append ((GtkMenu*)meniu_albume, item);
	// import
	item_img = gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (__("Import Album"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu_albume, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_import");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_albume_import"));
	// linia
	item = gtk_image_menu_item_new ();
	gtk_widget_set_sensitive (item, FALSE);
	gtk_menu_append ((GtkMenu*)meniu_albume, item);
	// adaugare
	item_img = gtk_image_new_from_stock ("gtk-add", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (__("Add Photos"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu_albume, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_adauga");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_poze_adauga"));
	// linia
	item = gtk_image_menu_item_new ();
	gtk_widget_set_sensitive (item, FALSE);
	gtk_menu_append ((GtkMenu*)meniu_albume, item);
	// proipietati
	item_img = gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/properties.png")), 24, 24, GDK_INTERP_BILINEAR));
	item = gtk_image_menu_item_new_with_label (__("Properties"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_tooltips_set_tip ((GtkTooltips*)ps_get_widget ("ps_main_tips"), item, 
						  __("Edit the current album's properties"), "");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_albume_propietati"));
	gtk_menu_append ((GtkMenu*)meniu_albume, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_propietati");
	gtk_widget_show_all (meniu_albume);
	return meniu_albume;
}

GtkWidget * ps_create_meniu_poze()
{
	GtkWidget *meniu_poze;
	meniu_poze = gtk_menu_new ();
	GtkWidget *item;
	GtkWidget *item_img;
	// pune tapetul ecranului
	item_img = gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/wallpaper.png")), 24, 24, GDK_INTERP_BILINEAR));
	item = gtk_image_menu_item_new_with_label (__("Set as Wallpaper"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_tooltips_set_tip ((GtkTooltips*)ps_get_widget ("ps_main_tips"), item, 
						  __("Put the selected photo on the desktop."), "");
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_wallpaper");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_poze_wallpaper"));
	// informatii despre autor
	item_img = gtk_image_new_from_stock ("gtk-dialog-info", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (__("The Author..."));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_tooltips_set_tip ((GtkTooltips*)ps_get_widget ("ps_main_tips"), item, 
						  __("Find more information about the photo's author."), "");
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_autor");
	// export
	item_img = gtk_image_new_from_stock ("gtk-save", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (__("Export"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_tooltips_set_tip ((GtkTooltips*)ps_get_widget ("ps_main_tips"), item, 
						  __("Save the selected photo to your hard drive."), "");
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_export");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_poze_export"));
	// linia
	item = gtk_image_menu_item_new ();
	gtk_widget_set_sensitive (item, FALSE);
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	// adaugare
	item_img = gtk_image_new_from_stock ("gtk-add", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (__("Add"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_adauga");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_poze_adauga"));
	// stergere
	item_img = gtk_image_new_from_stock ("gtk-remove", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (__("Remove"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_sterge");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_poze_sterge_poza_nr"));
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	// linia
	item = gtk_image_menu_item_new ();
	gtk_widget_set_sensitive (item, FALSE);
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	// proipietati
	item_img = gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/properties.png")), 24, 24, GDK_INTERP_BILINEAR));
	item = gtk_image_menu_item_new_with_label (__("Properties"));
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_tooltips_set_tip ((GtkTooltips*)ps_get_widget ("ps_main_tips"), item, 
						  __("Edit the current photo's properties"), "");
	g_signal_connect (item, "activate", (GCallback)cb_ps_main_menu, strdup ("ps_lista_poze_propietati"));
	gtk_menu_append ((GtkMenu*)meniu_poze, item);
	ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_poze_propietati");
	gtk_widget_show_all (meniu_poze);
	return meniu_poze;
}

GtkWidget * ps_create_design ()
{
	GdkPixbuf *pict;
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	ps_main_window = window;
	gtk_window_set_title ((GtkWindow*)window,ps_name);
	gtk_window_set_position ((GtkWindow*)window,GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request (window, -1, 480);
	#ifndef WIN32
	pict=ps_create_pixbuf_from_file (pict_path ("picts/photostorm.png"));
	if (pict!=NULL) 
	{	
		gtk_window_set_icon ((GtkWindow*)window, pict);
		gdk_pixbuf_unref (pict);
	}
	#endif
	gtk_container_set_border_width ((GtkContainer*)window, 7);
	
	GtkTooltips *tips = gtk_tooltips_new ();
	ps_hookup_object (window, tips, "ps_main_tips");
	
	GtkWidget *vbox = gtk_vbox_new (FALSE, 7);
	gtk_container_add ((GtkContainer*)window, vbox);
	
	GtkWidget *upper_hbox = gtk_hbox_new (FALSE, 7);
	gtk_box_pack_start ((GtkBox*)vbox, upper_hbox, FALSE, TRUE, 0);
	
	GtkWidget *logo_image = ps_create_image_from_file (pict_path ("logos/photostorm.jpg"));
	//gtk_box_pack_start ((GtkBox*)upper_hbox, logo_image, FALSE, FALSE, 0);
	
	GtkWidget *bara_butoane = gtk_toolbar_new ();
	gtk_box_pack_start ((GtkBox*)upper_hbox, bara_butoane, TRUE, TRUE, 0);
	gtk_toolbar_set_style ((GtkToolbar*)bara_butoane, GTK_TOOLBAR_BOTH_HORIZ);
	GtkSizeGroup *grup_bara_butoane = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
	
	GtkWidget *buton_wallpaper = gtk_toolbar_append_element ((GtkToolbar*)bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Wallpaper"), 
														__("Select an image from your computer and turn it into a wallpaper."), 
														NULL,
														gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/wallpaper.png")), 28, 28, GDK_INTERP_BILINEAR)),
														NULL, NULL);
	ps_hookup_object (window, buton_wallpaper, "ps_main_buton_wallpaper");
	gtk_size_group_add_widget (grup_bara_butoane, buton_wallpaper);
	g_signal_connect (buton_wallpaper, "clicked", (GCallback)cb_ps_main_buton_wallpaper, NULL);
	
	GtkWidget *buton_albume = gtk_toolbar_append_element ((GtkToolbar*)bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Albums"), 
														__("Add, edit or delete albums"), 
														NULL,
														gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/album.png")), 28, 28, GDK_INTERP_BILINEAR)),
														NULL, NULL);
	ps_hookup_object (window, buton_albume, "ps_main_buton_albume");
	gtk_size_group_add_widget (grup_bara_butoane, buton_albume);
	g_signal_connect (buton_albume, "clicked", (GCallback)cb_ps_main_buton_albume, NULL);
	GtkWidget *buton_poze = gtk_toolbar_append_element ((GtkToolbar*)bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Photos"), 
														__("Add, edit, delete, set as wallpaper and other actions related to the current photo."), 
														NULL, 
														gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/photos.png")), 28, 28, GDK_INTERP_BILINEAR)),
														NULL, NULL);
	ps_hookup_object (window, buton_poze, "ps_main_buton_poze");
	gtk_size_group_add_widget (grup_bara_butoane, buton_poze);
	g_signal_connect (buton_poze, "clicked", (GCallback)cb_ps_main_buton_poze, NULL);
	GtkWidget *buton_config = gtk_toolbar_append_element ((GtkToolbar*)bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Settings"), 
														__("Configure the way PhotoStorm works (wallpaper, calendar, etc.)"), 
														NULL, 
														gtk_image_new_from_stock ("gtk-preferences",gtk_toolbar_get_icon_size((GtkToolbar*)bara_butoane)), 
														NULL, NULL);
	ps_hookup_object (window, buton_config, "ps_main_buton_config");
	gtk_size_group_add_widget (grup_bara_butoane, buton_config);
	g_signal_connect (buton_config, "clicked", (GCallback)cb_ps_configuration, NULL);
	GtkWidget *buton_help = gtk_toolbar_append_element ((GtkToolbar*)bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Help"), 
														__("Get help for using PhotoStorm."), 
														NULL, 
														gtk_image_new_from_stock ("gtk-help",gtk_toolbar_get_icon_size((GtkToolbar*)bara_butoane)), 
														NULL, NULL);
	ps_hookup_object (window, buton_help, "ps_main_buton_help");
	gtk_size_group_add_widget (grup_bara_butoane, buton_help);
	g_signal_connect (buton_help, "clicked", (GCallback)cb_ps_open_browser, help_path ("index.html"));
	GtkWidget *buton_website = gtk_toolbar_append_element ((GtkToolbar*)bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("PhotoStorm!"), 
														__("Visit the PhotoStorm website. Download photos and albums or share yours. http://www.photostorm.net"), 
														NULL, 
														gtk_image_new_from_pixbuf (gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/website.png")), 28, 28, GDK_INTERP_BILINEAR)),
														NULL, NULL);
	ps_hookup_object (window, buton_website, "ps_main_buton_website");
	gtk_size_group_add_widget (grup_bara_butoane, buton_website);
	g_signal_connect (buton_website, "clicked", (GCallback)cb_ps_open_browser, strdup ("http://www.photostorm.net"));
	GtkWidget *hpanou = gtk_hpaned_new ();
	gtk_box_pack_start ((GtkBox*)vbox, hpanou, TRUE, TRUE, 0);
	gtk_paned_set_position ((GtkPaned*)hpanou, 0);
	ps_hookup_object (window, hpanou, "ps_main_hpanou");
	
	GtkWidget *lista_albume_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)lista_albume_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)lista_albume_scroll, GTK_SHADOW_IN);
	gtk_paned_pack1 ((GtkPaned*)hpanou, lista_albume_scroll, FALSE, FALSE);
	ps_hookup_object (window, lista_albume_scroll, "ps_main_lista_albume_scroll");
	
	GtkWidget *lista_albume = ps_create_lista_albume ();
	gtk_container_add ((GtkContainer*)lista_albume_scroll, lista_albume);
	ps_hookup_object (window, lista_albume, "ps_main_lista_albume");
	
	GtkWidget *vpanou = gtk_vpaned_new ();
	gtk_paned_pack2 ((GtkPaned*)hpanou, vpanou, TRUE, TRUE);
	gtk_paned_set_position ((GtkPaned*)hpanou, 0);
	ps_hookup_object (window, vpanou, "ps_main_vpanou");
	
	GtkWidget *panou;
	panou = gtk_viewport_new (NULL, NULL);
	gtk_paned_pack1 ((GtkPaned*)vpanou, panou, TRUE, TRUE);
	
	GtkWidget *imagine_ev = gtk_event_box_new ();
	ps_hookup_object (window, imagine_ev, "ps_main_imagine_ev");
	GtkWidget *imagine = ps_create_image_from_file ("data/imagine.png");
	gtk_container_add ((GtkContainer*)imagine_ev, imagine);
	gtk_widget_set_size_request (imagine, 150, 100);
	gtk_container_add ((GtkContainer*)panou, imagine_ev);
	ps_hookup_object (window, imagine, "ps_main_imagine");
	g_signal_connect (imagine_ev, "button-release-event", (GCallback)cb_ps_meniu_lista_poze, NULL);
	//gtk_paned_pack1 ((GtkPaned*)ps_main_vpanou, ps_main_imagine, TRUE, TRUE);
	
	GtkWidget *lista_poze_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)lista_poze_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type ((GtkScrolledWindow*)lista_poze_scroll, GTK_SHADOW_IN);
	gtk_paned_pack2 ((GtkPaned*)vpanou, lista_poze_scroll, TRUE, FALSE);
	ps_hookup_object (window, lista_poze_scroll, "ps_main_lista_poze_scroll");
	
	GtkWidget *lista_poze = ps_create_lista_poze ();
	gtk_container_add ((GtkContainer*)lista_poze_scroll, lista_poze);
	ps_hookup_object (window, lista_poze, "ps_main_lista_poze");
	
	GtkWidget *menu_lista_poze = ps_create_meniu_poze ();
	ps_hookup_object (window, menu_lista_poze, "ps_main_menu_lista_poze");
	
	// meniu de la albume trebuie neaparat creat dupa meniu de la poze
	// deoarece acesta il contine pe primul (submeniul)
	GtkWidget *menu_lista_albume = ps_create_meniu_albume ();
	ps_hookup_object (window, menu_lista_albume, "ps_main_menu_lista_albume");
	
	g_signal_connect (window, "destroy", (GCallback)cb_ps_main_exit, NULL);
	
	return window;
}
