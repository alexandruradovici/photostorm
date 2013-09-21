
#ifndef _PS_WIDGETS__H
#define _PS_WIDGETS__H

#include <gtk/gtk.h>
#include "album.h"

#define ps_check_button_get_value(widget)\
		gtk_toggle_button_get_active ((GtkToggleButton*)widget);
#define ps_check_button_set_value(widget,value)\
		gtk_toggle_button_set_active ((GtkToggleButton*)widget, value);
		
#define ps_radio_button_get_value(widget)\
		gtk_toggle_button_get_active ((GtkToggleButton*)widget);
#define ps_radio_button_set_value(widget,value)\
		gtk_toggle_button_set_active ((GtkToggleButton*)widget, value);

typedef struct _ps_photo_cell_renderer PSPhotoCellRenderer;
typedef struct _ps_photo_cell_renderer_class PSPhotoCellRendererClass;

struct _ps_photo_cell_renderer
{
	GtkCellRenderer parent;
	GtkCellRendererText *nume;
	GtkCellRendererText *autor;
};

struct _ps_photo_cell_renderer_class
{
	GtkCellRendererClass parent_class;
};
GType ps_photo_cell_renderer_get_type ();
GtkCellRenderer * ps_photo_cell_renderer_new ();

// widget functions
GtkWidget * ps_create_image_from_file (const char *filename);
GdkPixbuf * ps_create_pixbuf_from_file (const char *filename);
GdkPixbuf * ps_create_pixbuf_from_memory (const char *data, long n);
GtkWidget * ps_create_button_with_image (const char *img, char *str, int tip);
GtkWidget * ps_create_button_with_image_from_stock (const char *img, char *str, int tip);
GtkWidget * ps_create_radio_button_with_image (const char *img, const char *str, int tip);
GtkWidget * ps_create_radio_button_with_image_from_stock (const char *img, char *str, int tip);
GtkWidget * ps_create_radio_button_with_image_with_widget (const char *img, GtkWidget *display, int tip);
GtkWidget * ps_create_radio_button_with_image_from_stock_with_widget (const char *img, GtkWidget *display, int tip);
GtkWidget * ps_create_check_button_with_image (const char *img, char *str, int tip);
GtkWidget * ps_create_check_button_with_image_from_stock (const char *img, char *str, int tip);
GtkWidget * ps_create_mascot_header (const char *img, char *str);
GtkWidget * ps_create_mascot_header_with_image_from_stock (const char *img, char *str);
GtkWidget * ps_create_mascot_header_with_image_from_memory (const char *img, long size, char *str);
GtkWidget * ps_create_album_props_label (_ps_album *date_album);
GtkWidget * ps_create_photo_props_label (_ps_photo *date_poza);
GtkWidget * ps_create_position_widget (const char *str, int x, int y);
void ps_position_set_values (GtkWidget *position, int a, int x, int y);
int ps_position_get_a (GtkWidget *position);
int ps_position_get_x (GtkWidget *position);
int ps_position_get_y (GtkWidget *position);
GtkWidget *ps_create_edit (const char *str, int maxlength, int size, int tip, char cp);
char *ps_edit_get_text (GtkWidget *edit);
void ps_edit_set_text (GtkWidget *edit, const char *str);
GtkWidget * ps_server_data (char *server_name, const char *_server, int _port, const char *_user, const char *_password);
char * ps_server_data_get_server (GtkWidget *server_data);
void ps_server_data_set_server (GtkWidget *server_data, const char *str);
int ps_server_data_get_port (GtkWidget *server_data);
void ps_server_data_set_port (GtkWidget *server_data, int t);
char * ps_server_data_get_username (GtkWidget *server_data);
void ps_server_data_set_username (GtkWidget *server_data, const char *str);
char * ps_server_data_get_password (GtkWidget *server_data);
void ps_server_data_set_password (GtkWidget *server_data, const char *str);
GtkWidget * ps_create_number_edit (const char *str, double min, double max, double step, int tip);
double ps_number_edit_get_number (GtkWidget *number_edit);
void ps_number_edit_set_number (GtkWidget *number_edit, double nr);
GtkWidget * ps_create_time_edit (const char *str, int h, int m);
int ps_time_edit_get_hour (GtkWidget *time_edit);
int ps_time_edit_get_minute (GtkWidget *time_edit);
void ps_time_edit_set_time (GtkWidget *time_edit, int h, int m);
GtkWidget * ps_create_color_select (const char *str, _ps_color *color);
_ps_color * ps_color_select_get_color (GtkWidget *color_select);
_ps_color * ps_color_select_set_color (GtkWidget *color_select, _ps_color *color);
GtkWidget *ps_create_option_menu (const char *str, const char *items[], int n, int tip);
int ps_option_menu_get_index (GtkWidget *options);
void ps_option_menu_set_index (GtkWidget *options, int index);
GtkWidget *ps_create_location_edit (const char *str, int maxlen, int size, int tip);
char * ps_location_edit_get_location (GtkWidget *location);
void ps_location_edit_set_location (GtkWidget *location, const char *loc);
GtkWidget *ps_preview_create ();
void ps_preview_set_image (GtkWidget* preview, char *fpoza);
void ps_preview_set_image_from_pixbuf (GtkWidget* preview, GdkPixbuf* pixbuf);

#endif
