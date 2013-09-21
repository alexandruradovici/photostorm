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
#include "data.h"
#include "lang/translate.h"
#include "album.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget * ps_main_window;
GtkWidget * ps_main_vbox;
GtkWidget * ps_main_upper_hbox;
GtkWidget * ps_main_logo_image;
GtkWidget * ps_main_bara_butoane;
GtkWidget * ps_main_buton_albume;
GtkWidget * ps_main_buton_poze;
GtkWidget * ps_main_buton_add_poze;
GtkWidget * ps_main_buton_edit_poze;
GtkWidget * ps_main_buton_del_poze;
GtkWidget * ps_main_buton_config;
GtkWidget * ps_main_buton_help;
GtkWidget * ps_main_buton_website;
GtkWidget * ps_main_hpanou;
GtkWidget * ps_main_lista_albume_scroll;
GtkWidget * ps_main_lista_albume;
GtkWidget * ps_main_lista_poze_scroll;
GtkWidget * ps_main_lista_poze;
GtkWidget * ps_main_vpanou;
GtkWidget * ps_main_imagine;
GtkTooltips *ps_main_tips;

enum
{
	ps_c_albume_nume,
	ps_c_albume_activ,
	
	ps_c_albume_vizibil,
	ps_c_albume_grosime_font,
	ps_c_albume_stil_font,
	ps_c_albume_culoare_font,
	ps_c_albume_poza,
	ps_c_albume_date,
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

char * pict_path (char *pict)
{
	char *result;
	if (pict==NULL) result=strdup ("");
	else
	{
		if (ps_pics_path!=NULL) result=(char*)malloc (strlen(ps_pics_path)+strlen(pict)+10);
			else result=(char*)malloc (strlen(pict)+10);
		if (result!=NULL)
		{
			if (ps_pics_path!=NULL) sprintf ((char*)result,"%s/%s",ps_pics_path,(char*)pict);
				else sprintf ((char*)result,"./%s",(char*)pict);
		}
	}
	return result;
}

void ps_album_selectat (GtkTreeSelection *sel, gpointer data)
{
	GtkTreeIter pos;
	GtkTreeModel *model;
	if (gtk_tree_selection_get_selected (sel, &model, &pos))
	{
		
	}
}

void cb_ps_selected_picture (GtkTreeSelection *sel)
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
		gtk_window_get_size ((GtkWindow*)ps_main_window, &dx, &dy);
		dx=dx-10-gtk_paned_get_position ((GtkPaned*)ps_main_hpanou);
		dy=gtk_paned_get_position ((GtkPaned*)ps_main_vpanou);
		//long n;
		//char * poza_mem;
		//poza_mem = ps_incarca_poza (date, n);
		_ps_photo * date_poza = ps_incarca_poza_nr_din_album_fisier (album_path ("test"), nr);
		if (date_poza!=NULL) 
		{	
			int n;
			char *date = ps_creaza_poza_pentru_afisare (date_poza, dx, dy, n);
			if (date!=NULL) 
			{
				poza = ps_create_pixbuf_from_memory (date, n);
				free (date);
			}
			else poza = ps_create_pixbuf_from_memory (date_poza->date, date_poza->n_poza);
			ps_photo_free (date_poza);
		}
		else poza=NULL;
		//poza = ps_create_pixbuf_from_memory (poza_mem, n);
		//free (poza_mem);
		//poza = ps_create_pixbuf_from_file (date);
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
		gtk_image_set_from_pixbuf ((GtkImage*)ps_main_imagine, poza_preview);
		g_object_unref (poza_preview);
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

/*** asta vine mutate de aici ***/
void adauga_categorie (char *categoria)
{
	if (categoria!=NULL)
	{
		GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_main_lista_albume);
		GtkTreeIter pos;
		gtk_tree_store_append ((GtkTreeStore*)model,&pos,NULL);
		gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_albume_nume, categoria, 
							ps_c_albume_activ, TRUE, 
							ps_c_albume_vizibil, FALSE,
							ps_c_albume_grosime_font, PANGO_WEIGHT_BOLD,
							ps_c_albume_stil_font, PANGO_STYLE_NORMAL,
							ps_c_albume_poza, gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/collection.png")),24,24,GDK_INTERP_BILINEAR),
							//ps_c_albume_culoare_font, "#000000",
							-1);
	}
}

void adauga_album (char *categoria, char *albumul, int activ)
{
	if (categoria!=NULL && albumul!=NULL)
	{
		GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_main_lista_albume);
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
								ps_c_albume_vizibil, TRUE,
								ps_c_albume_grosime_font, PANGO_WEIGHT_NORMAL,
								ps_c_albume_stil_font, stil_font,
								ps_c_albume_poza, gdk_pixbuf_scale_simple (ps_create_pixbuf_from_file (pict_path ("picts/album.png")),24,24,GDK_INTERP_BILINEAR),
								//ps_c_albume_culoare_font, "#000000",
								-1);
		}
	}
}

void adauga_poza (GdkPixbuf *poza, char *nume, char *autor, char *descriere, char *optiuni, char *metadata, int nr, gboolean activa)
{
	GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)ps_main_lista_poze);
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

GdkPixbuf * ps_create_pixbuf_from_file (char *filename)
{
	GdkPixbuf *pict;
	GError *error=NULL;
	pict=gdk_pixbuf_new_from_file (filename,&error);
	if (pict==NULL) 
	{	
		printf (__("Error loading picture from file \"%s\" (GTK2: %s)"),filename,error->message);
		printf ("\n");
	}
	return pict;
}

GdkPixbuf * ps_create_pixbuf_from_memory (char *data, long n)
{
	GdkPixbuf * result=NULL;
	if (data!=NULL && n>0)
	{
		GdkPixbufLoader * incarca_imagine;
		GError *error = NULL;
		incarca_imagine = gdk_pixbuf_loader_new ();
	    if (gdk_pixbuf_loader_write (incarca_imagine, (guchar*)data, n, &error))
			result = gdk_pixbuf_loader_get_pixbuf (incarca_imagine);
		gdk_pixbuf_loader_close (incarca_imagine, NULL);
		g_free (incarca_imagine);
	}
	return result;
}

GtkWidget * ps_create_image_from_file (char *filename)
{
	GtkWidget *image;
	image=gtk_image_new_from_file (filename);
	return image;
}

GtkWidget * ps_create_lista_albume ()
{
	GtkWidget * lista_albume = gtk_tree_view_new_with_model (ps_model_lista_albume());
	GtkCellRenderer *render;
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
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_signal_connect (render, "toggled", (GCallback)cb_ps_main_album_check, gtk_tree_view_get_model ((GtkTreeView*)lista_albume));
	g_object_set ((GObject*)render, "xalign", 1.0, NULL);
	gtk_tree_view_column_set_attributes (col,render, "active", ps_c_albume_activ, 
												    "visible", ps_c_albume_vizibil,
													NULL);
	gtk_tree_view_column_set_resizable (col, FALSE);
	gtk_tree_view_column_set_max_width (col, 30);
	gtk_tree_view_append_column ((GtkTreeView*)lista_albume, col);
	
	gtk_widget_set_usize (lista_albume, 200, 100);
	gtk_container_add ((GtkContainer*)ps_main_lista_albume_scroll, lista_albume);
	gtk_tree_view_expand_all ((GtkTreeView*)lista_albume);
	gtk_tree_view_set_headers_visible ((GtkTreeView*)lista_albume, FALSE);
	gtk_tree_view_set_rules_hint ((GtkTreeView*)lista_albume, TRUE);
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
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 0.5, "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_poze_nume, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_poze, col);
	
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Author"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_poze_autor);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 1.0, "style", PANGO_STYLE_ITALIC, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_poze_autor, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_poze, col);
	
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Description"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_poze_descriere);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.0, "yalign", 1.0, "style", PANGO_STYLE_NORMAL, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_poze_descriere, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_poze, col);
	
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, __("Properties"));
	gtk_tree_view_column_set_sort_column_id (col, ps_c_poze_optiuni);
	gtk_tree_view_column_set_resizable (col, TRUE);
	render = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, render, FALSE);
	g_object_set ((GObject*)render, "xalign", 0.5, "yalign", 1.0, "style", PANGO_STYLE_NORMAL, NULL);
	gtk_tree_view_column_set_attributes (col, render, "text", ps_c_poze_optiuni, NULL);
	gtk_tree_view_append_column ((GtkTreeView*)lista_poze, col);
	
	gtk_widget_set_usize (lista_poze, 100, 100);
	gtk_tree_view_set_headers_visible ((GtkTreeView*)lista_poze, TRUE);
	gtk_tree_view_set_rules_hint ((GtkTreeView*)lista_poze, FALSE);
	
	sel = gtk_tree_view_get_selection ((GtkTreeView*)lista_poze);
	gtk_tree_selection_set_mode (sel, GTK_SELECTION_SINGLE);
	g_signal_connect (sel, "changed", (GCallback)cb_ps_selected_picture, NULL);
	
	
	GtkTreeModel *model = gtk_tree_view_get_model ((GtkTreeView*)lista_poze);
	GtkTreeIter pos;
	GdkPixbuf * poza;
	int dx,dy;
	
	gtk_tree_store_append ((GtkTreeStore*)model,&pos,NULL);
	poza = ps_create_pixbuf_from_file (pict_path ("photos/poza1.jpg"));
	dx=gdk_pixbuf_get_width (poza);
	dy=gdk_pixbuf_get_height (poza);
	poza = gdk_pixbuf_scale_simple (poza,(int)(50*(double)dx/dy),50,GDK_INTERP_BILINEAR);
	gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_poze_nume, "Satul Masriel,\njudetul Cluj, 2003", 
						ps_c_poze_poza, poza,
						ps_c_poze_autor, "Ovidiu Stoica",
						ps_c_poze_descriere, "O poza de la\nmarisel.",
						ps_c_poze_optiuni, "640x320\nShirnk Only",
						ps_c_poze_date, pict_path ("photos/poza1.jpg"),
						ps_c_poze_dx, (int)(50*(double)dx/dy)+10,
						ps_c_poze_dy, 60,
 						//ps_c_albume_culoare_font, "#000000",
						ps_c_poze_nr, 1,
						-1);
						
	gtk_tree_store_append ((GtkTreeStore*)model,&pos,NULL);
	long n;
	int np;
	char * poza_mem = ps_incarca_poza (pict_path ("photos/ioana.jpg"),n);
	char * poza_mem_p = NULL;
	if (poza_mem!=NULL) 
	{	
		poza_mem_p = ps_creaza_preview_poza (poza_mem, n, 0, 50, np);
		free (poza_mem);
	}
	if (poza_mem_p!=NULL)
	{
		poza = ps_create_pixbuf_from_memory (poza_mem_p, np);
		free (poza_mem_p);
	}
	//poza = ps_create_pixbuf_from_file (pict_path ("photos/ioana.jpg"));
	dx=gdk_pixbuf_get_width (poza);
	dy=gdk_pixbuf_get_height (poza);
	//poza = gdk_pixbuf_scale_simple (poza,(int)(50*(double)dx/dy),50,GDK_INTERP_BILINEAR);
	gtk_tree_store_set ((GtkTreeStore*)model, &pos, ps_c_poze_nume, "Cineva doarme...\nciudat", 
						ps_c_poze_poza, poza,
						ps_c_poze_autor, "Alexandru Radovici",
						ps_c_poze_descriere, "Ioana dormind pe autocar\nin drum spre\nGermania, 2004.",
						ps_c_poze_optiuni, "1280x960\nAlways Scale",
						ps_c_poze_date, pict_path ("photos/ioana.jpg"),
						ps_c_poze_dx, (int)(50*(double)dx/dy)+10,
						ps_c_poze_dy, 60,
 						//ps_c_albume_culoare_font, "#000000",
						ps_c_poze_nr, 2,
						-1);
	
	
	return lista_poze;
}

int ps_sterge_lista_poze ()
{
	int stearsa=0;
	GtkTreeModel * lista_poze = gtk_tree_view_get_model ((GtkTreeView*)ps_main_lista_poze);
	if (lista_poze!=NULL)
	{
		gtk_tree_store_clear ((GtkTreeStore*)lista_poze);
		stearsa=1;
	}
	return stearsa;
}

GtkWidget * ps_create_design ()
{
	GdkPixbuf *pict;
	/************** Main Window **************/
	/* aspect */
	ps_main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title ((GtkWindow*)ps_main_window,ps_name);
	gtk_window_set_position ((GtkWindow*)ps_main_window,GTK_WIN_POS_CENTER);
	gtk_widget_set_usize (ps_main_window, 700, 480);
	pict=ps_create_pixbuf_from_file (pict_path ("picts/photostorm.png"));
	if (pict!=NULL) 
	{	
		gtk_window_set_icon ((GtkWindow*)ps_main_window, pict);
		gdk_pixbuf_unref (pict);
	}
	gtk_container_set_border_width ((GtkContainer*)ps_main_window, 7);
	
	ps_main_vbox = gtk_vbox_new (FALSE, 7);
	gtk_container_add ((GtkContainer*)ps_main_window, ps_main_vbox);
	
	ps_main_upper_hbox = gtk_hbox_new (FALSE, 7);
	gtk_box_pack_start ((GtkBox*)ps_main_vbox, ps_main_upper_hbox, FALSE, TRUE, 0);
	
	ps_main_logo_image = ps_create_image_from_file (pict_path ("logos/photostorm.jpg"));
	gtk_box_pack_start ((GtkBox*)ps_main_upper_hbox, ps_main_logo_image, FALSE, FALSE, 0);
	
	ps_main_bara_butoane = gtk_toolbar_new ();
	gtk_box_pack_start ((GtkBox*)ps_main_upper_hbox, ps_main_bara_butoane, TRUE, TRUE, 0);
	gtk_toolbar_set_style ((GtkToolbar*)ps_main_bara_butoane, GTK_TOOLBAR_BOTH_HORIZ);
	
	ps_main_buton_albume = gtk_toolbar_append_element ((GtkToolbar*)ps_main_bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Albums"), 
														__("Add, edit or delete albums"), 
														NULL,
														NULL,
														NULL, NULL);
	ps_main_buton_poze = gtk_toolbar_append_element ((GtkToolbar*)ps_main_bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Photo"), 
														__("Add, edit, delete, set as wallpaper and other actions related to the current photo."), 
														NULL, 
														gtk_image_new_from_stock ("gtk-add",gtk_toolbar_get_icon_size((GtkToolbar*)ps_main_bara_butoane)), 
														NULL, NULL);
	/*ps_main_buton_edit_poze = gtk_toolbar_append_element ((GtkToolbar*)ps_main_bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Edit"), 
														__("Edit the current photo's properties"), 
														NULL, 
														gtk_image_new_from_stock ("gtk-dialog-info",gtk_toolbar_get_icon_size((GtkToolbar*)ps_main_bara_butoane)), 
														NULL, NULL);
	ps_main_buton_del_poze = gtk_toolbar_append_element ((GtkToolbar*)ps_main_bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Delete"), 
														__("Remove the current photo from the album"), 
														NULL, 
														gtk_image_new_from_stock ("gtk-remove",gtk_toolbar_get_icon_size((GtkToolbar*)ps_main_bara_butoane)), 
														NULL, NULL);*/
	ps_main_buton_config = gtk_toolbar_append_element ((GtkToolbar*)ps_main_bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Settings"), 
														__("Configure the way PhotoStorm works (wallpaper, screensaver, etc.)"), 
														NULL, 
														gtk_image_new_from_stock ("gtk-preferences",gtk_toolbar_get_icon_size((GtkToolbar*)ps_main_bara_butoane)), 
														NULL, NULL);
	ps_main_buton_help = gtk_toolbar_append_element ((GtkToolbar*)ps_main_bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("Help"), 
														__("Get help for using PhotoStorm."), 
														NULL, 
														gtk_image_new_from_stock ("gtk-help",gtk_toolbar_get_icon_size((GtkToolbar*)ps_main_bara_butoane)), 
														NULL, NULL);
	ps_main_buton_website = gtk_toolbar_append_element ((GtkToolbar*)ps_main_bara_butoane, 
														GTK_TOOLBAR_CHILD_BUTTON, NULL, 
														__("PhotoStorm!"), 
														__("Visit the PhotoStorm website. Download photos and albums or share yours."), 
														NULL, 
														gtk_image_new_from_stock ("gtk-home",gtk_toolbar_get_icon_size((GtkToolbar*)ps_main_bara_butoane)), 
														NULL, NULL);
	ps_main_hpanou = gtk_hpaned_new ();
	gtk_box_pack_start ((GtkBox*)ps_main_vbox, ps_main_hpanou, TRUE, TRUE, 0);
	gtk_paned_set_position ((GtkPaned*)ps_main_hpanou, 0);
	
	ps_main_lista_albume_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)ps_main_lista_albume_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack1 ((GtkPaned*)ps_main_hpanou, ps_main_lista_albume_scroll, FALSE, FALSE);
	
	ps_main_lista_albume = ps_create_lista_albume ();
	ps_main_tips = gtk_tooltips_new ();
	
	ps_main_vpanou = gtk_vpaned_new ();
	gtk_paned_pack2 ((GtkPaned*)ps_main_hpanou, ps_main_vpanou, TRUE, TRUE);
	gtk_paned_set_position ((GtkPaned*)ps_main_hpanou, 0);
	
	ps_main_imagine = ps_create_image_from_file ("data/imagine.png");
	gtk_widget_set_usize (ps_main_imagine, 150, 100);
	gtk_paned_pack1 ((GtkPaned*)ps_main_vpanou, ps_main_imagine, TRUE, TRUE);
	
	ps_main_lista_poze_scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy ((GtkScrolledWindow*)ps_main_lista_poze_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_paned_pack2 ((GtkPaned*)ps_main_vpanou, ps_main_lista_poze_scroll, TRUE, FALSE);
	
	ps_main_lista_poze = ps_create_lista_poze ();
	gtk_container_add ((GtkContainer*)ps_main_lista_poze_scroll, ps_main_lista_poze);
	/* signals */
	/*****************************************/
	return ps_main_window;
}
