/**************************************************************
*		File: design.h                                         *
*		ID: PS_design_h                                        *
*		Author: Alexandru Radovici                             *
*		Date: February 6, 2004                                 *
*		Modified: never                                        *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This file is the interface header of          *
*               PhotoStorm.                                   *
**************************************************************/

#ifndef _DESIGN__H
#define _DESIGN__H

#include <stdio.h>
#include <gtk/gtk.h>

#define ps_hookup_object(obiect, nume, text) \
		g_object_set_data_full ((GObject*)obiect, text, g_object_ref(nume), (GDestroyNotify) g_object_unref)

#define ps_get_widget(text) \
		g_object_get_data ((GObject*)ps_get_main_window(), text)

#define ps_get_widget_from(obiect, text) \
		g_object_get_data ((GObject*)obiect, text)
		
void cb_ps_open_browser (GtkWidget *buton, gpointer data);
GtkWidget * ps_create_design ();
GtkWidget * ps_get_main_window ();
char * ps_get_current_album ();
GList * ps_get_albums_categories ();

void ps_menu_activeaza (char *data);
void ps_menu_dezactiveaza (char *data);

void adauga_categorie (const char *categoria);
void adauga_album (const char *categoria, const char *albumul, const char *fisierul, int activ);
void sterge_album (const char *fisierul);
int selecteaza_album (const char *fisierul);
int exista_album (const char *fisierul);
void ps_set_album (const char *fisierul, int activ);
void selecteaza_primul_album ();
char * ps_get_random_album ();
void ps_write_albums_info (FILE *f);
void adauga_poza (GdkPixbuf *poza, char *nume, char *autor, char *descriere, char *optiuni, char *metadata, int nr, gboolean activa);
char *ps_fisier_album_curent_din_lista_albume ();
void ps_lista_poze_selecteaza_prima_poza ();
int ps_sterge_lista_poze ();
int ps_sterge_lista_albume ();
void ps_extinde_lista_albume ();

#endif
