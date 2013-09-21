
#ifndef _PS_DIALOGS__H
#define _PS_DIALOGS__H

#include "ps_wizards.h"
#include "ps_widgets.h"
#include "messages.h"
#include <gtk/gtk.h>

#define PS_BUTTON_YES 10
#define PS_BUTTON_NO 11
#define PS_PHOTO_DELETE_DIALOG 1
#define PS_ALBUM_DELETE_DIALOG 11

// data types

enum
{
	ps_c_quotes_list_id,
	ps_c_quotes_list_autor,
	ps_c_quotes_list_keywords,
	ps_c_quotes_list_quotetext,
	ps_c_quotes_list_numar
};

enum
{
	ps_c_messages_list_id,
	ps_c_messages_list_time,
	ps_c_messages_list_message_text,
	ps_c_messages_list_numar
};

struct _ps_wizard
{
	int dx, dy;
	char *name;
	char *b_begin;
	char *b_cont;
	char *b_done;
	char *b_back;
	char *b_cancel;
	char *path;
	int (*actionf) (_ps_wizard *wizard, int page, int button); 
	// int button: {0 - begin, 1 - forward, 2 - back, 3 - done}
	GtkWidget *ps_main_window;
	GtkWidget *window;
};

struct _ps_progress
{
	GtkWidget *parent;
	GtkWidget *window;
	int position;
	char *text;
	void * data;
	gboolean close;
	int quit;
};

typedef void _cb_ps_open_dialog_result (GtkWidget *parent, char *filename);
typedef void _cb_ps_question_dialog_result (int dialog_id, int button);

struct _ps_question_dialog
{
	GtkWidget *window;
	int id;
	_cb_ps_question_dialog_result *functie;
};

struct _ps_album_properties
{
	GtkWidget *parent;
	int tip_album;
	_ps_album *date_album;
	GtkWidget *window;
	void (*functie)(_ps_album_properties *props);
};

struct _ps_photo_properties
{
	GtkWidget *parent;
	int photo_nr;
	int tip_album;
	_ps_photo *date_poza;
	GtkWidget *window;
	void (*functie)(_ps_photo_properties *props);
};

struct _ps_message_edit
{
	GtkWidget *window;
	_ps_message *message;
};

// functions
char * ps_get_trimmed_text_from_entry (GtkEditable *edit);
char * ps_get_text_from_entry (GtkEditable *edit);
char * ps_get_text_from_text_view (GtkTextView *textview);
void ps_question_dialog (char *poza, int stock, char *text, GtkWidget *parent, int id, _cb_ps_question_dialog_result *functie);
void ps_information_dialog (char *poza, int stock, char *text, GtkWidget *parent);
GtkWidget * ps_create_open_dialog (char *calea, GtkWidget *parent, _cb_ps_open_dialog_result functie);
GtkWidget * ps_create_wizard_dialog (_ps_wizard *date);
_ps_wizard * ps_create_wizard (char *title, char *bbegin, char *bcont, char *bback, char *bdone, char *bcancel, int (*functie) (_ps_wizard *wizard, int page, int button), int dx, int dy);
void ps_add_wizard_page (_ps_wizard *wizard, GtkWidget *page);
void ps_wizard_free (_ps_wizard *wizard);
_ps_progress * ps_progress_dialog (GtkWidget *parent, void *data);
void ps_progress_set_text (_ps_progress *progress, const char *str);
void ps_progress_set_progress (_ps_progress *progress, int p);
void ps_progress_dialog_close (_ps_progress *progress);
GtkWidget * ps_album_properties_dialog (_ps_album_properties *props);
GtkWidget * ps_photo_properties_dialog (_ps_photo_properties *props);
void ps_select_color_dialog (GtkWidget *parent, _ps_color *color);
void ps_wizard_free (_ps_wizard *wizard);
void ps_progress_free (_ps_progress *progress);
void ps_album_properties_free (_ps_album_properties *props);
void ps_photo_properties_free (_ps_photo_properties *props);

// Dialogs

char *ps_open_dialog_single_file (GtkWidget *parent, const char *title, const char *filename, int show_preview);
char *ps_save_dialog_single_file (GtkWidget *parent, const char *title, const char *filename, int show_preview);
char *ps_select_folder_dialog (GtkWidget *parent, const char *title, const char *path);

// Quotes Library
GtkWidget *ps_create_quotes_lib_dialog (GtkWindow *parent);

// Message Editor
GtkWidget * ps_message_edit_dialog (GtkWindow *parent, _ps_message *message);
GtkWidget * ps_messages_edit_dialog (GtkWindow *parent);

#endif
