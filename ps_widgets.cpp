
#include "ps_widgets.h"
#include "ps_dialogs.h"
#include "design.h"
#include "lang/translate.h"
#include "misc.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
	prop_0,
	ps_photo_nume,
	ps_photo_autor
};

static GtkWindowClass *parent_class=NULL;

static void ps_photo_cell_renderer_get_property (GObject *object, guint param_id, GValue *value, GParamSpec *pspec)
{
	PSPhotoCellRenderer *render = (PSPhotoCellRenderer*)object;
	switch (param_id)
	{
		case ps_photo_nume:
			g_object_get_property ((GObject*)render->nume, "text", value);
			break;
		case ps_photo_autor:
			g_object_get_property ((GObject*)render->autor, "text", value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
			break;
	}
}

static void ps_photo_cell_renderer_set_property (GObject *object, guint param_id, const GValue *value, GParamSpec *pspec)
{
	PSPhotoCellRenderer *render = (PSPhotoCellRenderer*)object;
	switch (param_id)
	{
		case ps_photo_nume:
			g_object_set_property ((GObject*)render->nume, "text", value);
			break;
		case ps_photo_autor:
		{
			char *temp=NULL;
			if (g_value_get_string(value)!=NULL)
			{
				temp=(char*)malloc (strlen(g_value_get_string(value))+6);
				sprintf (&temp[0], "(C) %s",g_value_get_string(value));
			} else temp=strdup ("");
			g_object_set ((GObject*)render->autor, "text", temp, NULL);
			if (temp!=NULL) free (temp);
			break;
		}
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
			break;
	}
}

GtkCellRenderer * ps_photo_cell_renderer_new ()
{
	return (GtkCellRenderer*) g_object_new (ps_photo_cell_renderer_get_type(), NULL);
}

static void ps_photo_cell_renderer_get_size (GtkCellRenderer *gtk_render,
					       GtkWidget *widget,
					       GdkRectangle *cell_area,
					       int *x_offset,
					       int *y_offset,
					       int *width,
					       int *height)
{
	int text_x_offset;
	int text_y_offset;
	int text_width;
	int text_height;
	
	PSPhotoCellRenderer *render = (PSPhotoCellRenderer*)gtk_render;
	gtk_cell_renderer_get_size ((GtkCellRenderer*)render->nume, widget, cell_area, NULL, NULL, width, height);
	gtk_cell_renderer_get_size ((GtkCellRenderer*)render->autor, widget, cell_area,
 					&text_x_offset, 
				    &text_y_offset,
				    &text_width,
				    &text_height);
	if (height!=0) {
		*height = *height + text_height + 2;
	}
	if (width!=0) {
		*width = MAX (*width, text_width);
		*width += 3 * 2;
	}
}

static void ps_photo_cell_renderer_render (GtkCellRenderer *gtk_render,
					     GdkWindow *window,
					     GtkWidget *widget,
					     GdkRectangle *background_area,
					     GdkRectangle *cell_area,
					     GdkRectangle *expose_area,
					     GtkCellRendererState flags)

{
	PSPhotoCellRenderer *render = (PSPhotoCellRenderer*)gtk_render;
	GdkRectangle nume_area;
	GdkRectangle autor_area;
	int nw, nh, aw, ah;
	
	gtk_cell_renderer_get_size ((GtkCellRenderer*)render->nume, widget, cell_area, NULL, NULL, &nw, &nh);
	gtk_cell_renderer_get_size ((GtkCellRenderer*)render->autor, widget, cell_area, NULL, NULL, &aw, &ah);	

	nume_area.x = cell_area->x;
	nume_area.y = cell_area->y+((cell_area->height-(nh+ah+2))/2);
	nume_area.height = nh;
	nume_area.width = nw;
	autor_area.x = cell_area->x;
	autor_area.y = cell_area->y+((cell_area->height-(nh+ah+2))/2)+nh+1;;
	autor_area.height = ah;
	autor_area.width = aw;
	//printf ("%d\n", autor_area.width);
	gtk_cell_renderer_render ((GtkCellRenderer*)render->nume, window, widget, 
				  background_area, &nume_area,
				  expose_area, (GtkCellRendererState)flags);
	gtk_cell_renderer_render ((GtkCellRenderer*)render->autor, window, widget,
				  background_area, &autor_area,
				  expose_area, (GtkCellRendererState)flags);
}

static void ps_photo_cell_renderer_dispose (GObject *object)
{
	PSPhotoCellRenderer *render = (PSPhotoCellRenderer*)object;
	gtk_object_sink ((GtkObject*)render->nume);
	gtk_object_sink ((GtkObject*)render->autor);
	/*g_object_unref (render->nume);
	g_object_unref (render->autor);*/
}

static void ps_photo_cell_instance_init (PSPhotoCellRenderer *render)
{
	render->nume = (GtkCellRendererText*)gtk_cell_renderer_text_new ();
	g_object_set ((GObject*)render->nume, "family", "Monospace","scale", 1.0, "weight", PANGO_WEIGHT_BOLD, NULL);
	render->autor = (GtkCellRendererText*)gtk_cell_renderer_text_new ();
	g_object_set ((GObject*)render->autor, "family", "tahoma","scale", 0.9, "style", PANGO_STYLE_ITALIC, NULL);
}

static void ps_photo_cell_renderer_class_init (PSPhotoCellRendererClass *clasa)
{
	GObjectClass *object_class = (GObjectClass*) clasa;
	GtkCellRendererClass *cell_class = (GtkCellRendererClass*)clasa;
	parent_class = (GtkWindowClass*)g_type_class_peek_parent (clasa);
	object_class->set_property = ps_photo_cell_renderer_set_property;
	object_class->get_property = ps_photo_cell_renderer_get_property;
	cell_class->get_size = ps_photo_cell_renderer_get_size;
	cell_class->render = ps_photo_cell_renderer_render;
	object_class->dispose = ps_photo_cell_renderer_dispose;
	g_object_class_install_property (object_class,
					 ps_photo_nume,
					 g_param_spec_string ("nume",
							      __("Name"),
							      __("The photo's name"),
							      NULL,
							      (GParamFlags)G_PARAM_READWRITE));
	g_object_class_install_property (object_class,
					 ps_photo_autor,
					 g_param_spec_string ("autor",
							      __("Author"),
							      __("The photo's author"),
								  NULL,
							      (GParamFlags)G_PARAM_READWRITE));
}

GType ps_photo_cell_renderer_get_type ()
{
	static GType ps_photo_type = 0;
	if (!ps_photo_type)
	{
		static const GtkTypeInfo ps_photo_info = {
			"PSPhotoCellRenderer",
			sizeof (PSPhotoCellRenderer),
			sizeof (PSPhotoCellRendererClass),
			(GtkClassInitFunc) ps_photo_cell_renderer_class_init,
			(GtkObjectInitFunc) ps_photo_cell_instance_init,
			NULL,
			NULL,
		};
		ps_photo_type = gtk_type_unique (GTK_TYPE_CELL_RENDERER, &ps_photo_info);
	}
	return ps_photo_type;
}

GdkPixbuf * ps_create_pixbuf_from_file (const char *filename)
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

GdkPixbuf * ps_create_pixbuf_from_memory (const char *data, long n)
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

GtkWidget * ps_create_image_from_file (const char *filename)
{
	GtkWidget *image;
	image=gtk_image_new_from_file (filename);
	return image;
}

GtkWidget * ps_create_button_with_image (const char *img, char *str, int tip)
{
	GtkWidget *button=NULL;
	if (img!=NULL && str!=NULL)
	{
		button = gtk_button_new ();
		gtk_container_set_border_width ((GtkContainer*)button, 3);
		GtkWidget *align = gtk_alignment_new (0.5, 0.5, 0, 0);
		GtkWidget *box;
		if (tip==1)	box = gtk_hbox_new (FALSE, 2);
			else box = gtk_vbox_new (FALSE, 2);
		gtk_misc_set_alignment ((GtkMisc*)box, 0.5, 0.5);
		GtkWidget *image = gtk_image_new_from_file (img);
		ps_hookup_object (button, image, "img");
		GtkWidget *label = gtk_label_new (str);
		gtk_label_set_use_markup ((GtkLabel*)label, TRUE);
		ps_hookup_object (button, label, "str");
		gtk_box_pack_start ((GtkBox*)box, image, FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox*)box, label, FALSE, FALSE, 0);
		gtk_container_add ((GtkContainer*)align, box);
		gtk_container_add ((GtkContainer*)button, align);
	}
	return button;
}

GtkWidget * ps_create_button_with_image_from_stock (const char *img, char *str, int tip)
{
	GtkWidget *button=NULL;
	if (img!=NULL && str!=NULL)
	{
		button = gtk_button_new ();
		gtk_container_set_border_width ((GtkContainer*)button, 3);
		GtkWidget *align = gtk_alignment_new (0.5, 0.5, 0, 0);
		GtkWidget *box;
		if (tip==1) box = gtk_hbox_new (FALSE, 2);
			else box = gtk_vbox_new (FALSE, 2);
		GtkWidget *image = gtk_image_new_from_stock (img, GTK_ICON_SIZE_BUTTON);
		ps_hookup_object (button, image, "img");
		GtkWidget *label = gtk_label_new (str);
		gtk_label_set_use_markup ((GtkLabel*)label, TRUE);
		ps_hookup_object (button, label, "str");
		gtk_box_pack_start ((GtkBox*)box, image, FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox*)box, label, FALSE, FALSE, 0);
		gtk_container_add ((GtkContainer*)align, box);
		gtk_container_add ((GtkContainer*)button, align);
	}
	return button;
}

GtkWidget * ps_create_radio_button_with_image (const char *img, const char *str, int tip)
{
	GtkWidget *button=NULL;
	if (img!=NULL && str!=NULL)
	{
		button = gtk_radio_button_new (NULL);
		GtkWidget *box;
		if (tip==1)	box = gtk_hbox_new (FALSE, 2);
			else box = gtk_vbox_new (FALSE, 2);
		GtkWidget *image = gtk_image_new_from_file (img);
		ps_hookup_object (button, image, "img");
		GtkWidget *label = gtk_label_new (str);
		gtk_label_set_use_markup ((GtkLabel*)label, TRUE);
		gtk_label_set_line_wrap ((GtkLabel*)label, TRUE);
		gtk_label_set_justify ((GtkLabel*)label, GTK_JUSTIFY_FILL);
		ps_hookup_object (button, label, "str");
		gtk_box_pack_start ((GtkBox*)box, image, FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox*)box, label, FALSE, FALSE, 0);
		gtk_container_add ((GtkContainer*)button, box);
	}
	return button;
}

GtkWidget * ps_create_radio_button_with_image_from_stock (const char *img, char *str, int tip)
{
	GtkWidget *button=NULL;
	if (img!=NULL && str!=NULL)
	{
		button = gtk_radio_button_new (NULL);
		GtkWidget *box;
		if (tip==1)	box = gtk_hbox_new (FALSE, 2);
			else box = gtk_vbox_new (FALSE, 2);
		GtkWidget *image = gtk_image_new_from_stock (img, GTK_ICON_SIZE_BUTTON);
		ps_hookup_object (button, image, "img");
		GtkWidget *label = gtk_label_new (str);
		gtk_label_set_use_markup ((GtkLabel*)label, TRUE);
		gtk_label_set_line_wrap ((GtkLabel*)label, TRUE);
		gtk_label_set_justify ((GtkLabel*)label, GTK_JUSTIFY_FILL);
		ps_hookup_object (button, label, "str");
		gtk_box_pack_start ((GtkBox*)box, image, FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox*)box, label, FALSE, FALSE, 0);
		gtk_container_add ((GtkContainer*)button, box);
	}
	return button;
}

GtkWidget * ps_create_radio_button_with_image_with_widget (const char *img, GtkWidget *display, int tip)
{
	GtkWidget *button=NULL;
	if (img!=NULL && display!=NULL)
	{
		button = gtk_radio_button_new (NULL);
		GtkWidget *box;
		if (tip==1)	box = gtk_hbox_new (FALSE, 2);
			else box = gtk_vbox_new (FALSE, 2);
		GtkWidget *image = gtk_image_new_from_file (img);
		ps_hookup_object (button, image, "img");
		ps_hookup_object (button, display, "display");
		gtk_box_pack_start ((GtkBox*)box, image, FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox*)box, display, FALSE, FALSE, 0);
		gtk_container_add ((GtkContainer*)button, box);
	}
	return button;
}

GtkWidget * ps_create_radio_button_with_image_from_stock_with_widget (const char *img, GtkWidget *display, int tip)
{
	GtkWidget *button=NULL;
	if (img!=NULL && display!=NULL)
	{
		button = gtk_radio_button_new (NULL);
		GtkWidget *box;
		if (tip==1)	box = gtk_hbox_new (FALSE, 2);
			else box = gtk_vbox_new (FALSE, 2);
		GtkWidget *image = gtk_image_new_from_stock (img, GTK_ICON_SIZE_BUTTON);
		ps_hookup_object (button, image, "img");
		ps_hookup_object (button, display, "display");
		gtk_box_pack_start ((GtkBox*)box, image, FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox*)box, display, FALSE, FALSE, 0);
		gtk_container_add ((GtkContainer*)button, box);
	}
	return button;
}

GtkWidget * ps_create_check_button_with_image (const char *img, char *str, int tip)
{
	GtkWidget *button=NULL;
	if (img!=NULL && str!=NULL)
	{
		button = gtk_check_button_new ();
		GtkWidget *box;
		if (tip==1)	box = gtk_hbox_new (FALSE, 2);
			else box = gtk_vbox_new (FALSE, 2);
		GtkWidget *image = gtk_image_new_from_file (img);
		ps_hookup_object (button, image, "img");
		GtkWidget *label = gtk_label_new (str);
		gtk_label_set_use_markup ((GtkLabel*)label, TRUE);
		gtk_label_set_line_wrap ((GtkLabel*)label, TRUE);
		gtk_label_set_justify ((GtkLabel*)label, GTK_JUSTIFY_FILL);
		ps_hookup_object (button, label, "str");
		gtk_box_pack_start ((GtkBox*)box, image, FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox*)box, label, FALSE, FALSE, 0);
		gtk_container_add ((GtkContainer*)button, box);
	}
	return button;
}

GtkWidget * ps_create_check_button_with_image_from_stock (const char *img, char *str, int tip)
{
	GtkWidget *button=NULL;
	if (img!=NULL && str!=NULL)
	{
		button = gtk_check_button_new ();
		GtkWidget *box;
		if (tip==1)	box = gtk_hbox_new (FALSE, 2);
			else box = gtk_vbox_new (FALSE, 2);
		GtkWidget *image = gtk_image_new_from_stock (img, GTK_ICON_SIZE_BUTTON);
		ps_hookup_object (button, image, "img");
		GtkWidget *label = gtk_label_new (str);
		gtk_label_set_use_markup ((GtkLabel*)label, TRUE);
		gtk_label_set_line_wrap ((GtkLabel*)label, TRUE);
		gtk_label_set_justify ((GtkLabel*)label, GTK_JUSTIFY_FILL);
		ps_hookup_object (button, label, "str");
		gtk_box_pack_start ((GtkBox*)box, image, FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox*)box, label, FALSE, FALSE, 0);
		gtk_container_add ((GtkContainer*)button, box);
	}
	return button;
}

GtkWidget * ps_create_mascot_header (const char *img, char *str)
{
	GtkWidget *event_box = gtk_event_box_new ();
	GtkWidget *hbox=NULL;
	if (img!=NULL && str!=NULL)
	{
		hbox = gtk_hbox_new (FALSE, 2);
		GtkWidget *mascota = ps_create_image_from_file (img);
		gtk_box_pack_start ((GtkBox*)hbox, mascota, FALSE, FALSE, 2);
		GtkWidget *info = gtk_label_new (str);
		GdkColor color;
		gdk_color_parse ("#162f6a", &color);
		gtk_widget_modify_fg (info, GTK_STATE_NORMAL, &color);
		gtk_label_set_use_markup ((GtkLabel*)info, TRUE);
		gtk_label_set_line_wrap ((GtkLabel*)info, TRUE);
		gtk_label_set_justify ((GtkLabel*)info, GTK_JUSTIFY_FILL);
		gtk_box_pack_start ((GtkBox*)hbox, info, TRUE, TRUE, 2);
	}
	gtk_container_add ((GtkContainer*)event_box, hbox);
	GdkColor color;
	gdk_color_parse ("white", &color);
	gtk_widget_modify_bg (event_box, GTK_STATE_NORMAL, &color);
	return event_box;
}

GtkWidget * ps_create_mascot_header_with_image_from_stock (const char *img, char *str)
{
	GtkWidget *hbox=NULL;
	if (img!=NULL && str!=NULL)
	{
		hbox = gtk_hbox_new (FALSE, 2);
		GtkWidget *mascota = gtk_image_new_from_stock (img, GTK_ICON_SIZE_DIALOG);
		gtk_box_pack_start ((GtkBox*)hbox, mascota, FALSE, FALSE, 2);
		GtkWidget *info = gtk_label_new (str);
		gtk_label_set_use_markup ((GtkLabel*)info, TRUE); 
		gtk_label_set_line_wrap ((GtkLabel*)info, TRUE);
		gtk_label_set_justify ((GtkLabel*)info, GTK_JUSTIFY_FILL);
		gtk_box_pack_start ((GtkBox*)hbox, info, TRUE, TRUE, 2);
	}
	return hbox;
}

GtkWidget * ps_create_mascot_header_with_image_from_memory (const char *img, long size, char *str)
{
	GtkWidget *hbox=NULL;
	if (img!=NULL && str!=NULL)
	{
		hbox = gtk_hbox_new (FALSE, 2);
		GtkWidget *mascota = gtk_image_new_from_pixbuf (ps_create_pixbuf_from_memory (img, size));
		gtk_box_pack_start ((GtkBox*)hbox, mascota, FALSE, FALSE, 2);
		GtkWidget *info = gtk_label_new (str);
		gtk_label_set_use_markup ((GtkLabel*)info, TRUE);
		gtk_label_set_line_wrap ((GtkLabel*)info, TRUE);
		gtk_label_set_justify ((GtkLabel*)info, GTK_JUSTIFY_FILL);
		gtk_box_pack_start ((GtkBox*)hbox, info, TRUE, TRUE, 2);
	}
	return hbox;
}

GtkWidget * ps_create_album_props_label (_ps_album *date_album)
{
	const char *days[7] = { 
		                    __("Monday"), __("Tuesday"), __("Wednesday"), 
						    __("Thursday"), __("Friday"), __("Saturday"), 
						    __("Sunday")
		                  };
	GtkWidget *props = NULL;
	char props_text[1000];
	if (date_album!=NULL)
	{
		char data[100];
		tm *timp = localtime (&date_album->data);
		//sprintf (data, "%s,\n%s, %d,\n%d", days[timp->tm_wday], months[timp->tm_mon], timp->tm_mday, 1900+timp->tm_year);
		sprintf (data, "%s,\n%.2d/%.2d/%d\n%.2d:%.2d:%.2d", days[timp->tm_wday], timp->tm_mon, timp->tm_mday, 1900+timp->tm_year, timp->tm_hour, timp->tm_min, timp->tm_sec);
		sprintf (props_text, "%s\n%ld KB\n\n%s\n%d\n\n%s\n%s", __("Size"), (date_album->n_antet+date_album->n_poze)/1024,
					 __("Photos"), date_album->nr_poze, __("Created on"), data);
		props = gtk_label_new (strdup (props_text));
	}
	else strcpy (props_text, "");
	return props;
}

GtkWidget * ps_create_photo_props_label (_ps_photo *date_poza)
{
	GtkWidget *props = NULL;
	char props_text[1000];
	if (date_poza!=NULL)
	{
		GdkPixbuf *poza = ps_create_pixbuf_from_memory (date_poza->date, date_poza->n_poza);
		if (poza!=NULL)
		{
			sprintf (props_text, "%s\n%ld KB\n\n%s\n%d x %d", __("Size"), date_poza->n_poza/1024,
					 __("Resolution"), gdk_pixbuf_get_width (poza), gdk_pixbuf_get_height (poza));
			g_object_unref (poza);
		}
		props = gtk_label_new (strdup (props_text));
	}
	else strcpy (props_text, "");
	return props;
}

GtkWidget * ps_create_position_widget (const char *str, int x, int y)
{
	GtkWidget *position = gtk_vbox_new (FALSE, 2);
	GtkWidget *default_position = ps_create_radio_button_with_image_from_stock ("", __("Use default position"), 1);
	ps_hookup_object (position, default_position, "default_position");
	if (str==NULL) str = strdup ("");
	GtkWidget *box = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (position, box, "box");
	GtkSizeGroup *marime_x = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	GtkSizeGroup *marime_y = gtk_size_group_new (GTK_SIZE_GROUP_VERTICAL);
	//GtkWidget *info = gtk_label_new (__(strdup(str)));
	//ps_hookup_object (box, info, "info");
	//gtk_box_pack_start ((GtkBox*)box, info, FALSE, FALSE, 2);
	//gtk_size_group_add_widget (marime_y, info);
	GtkWidget *edit_x = gtk_entry_new_with_max_length (4);
	ps_hookup_object (position, edit_x, "edit_x");
	char temp[5];
	sprintf (temp, "%d", x);
	gtk_entry_set_text ((GtkEntry*)edit_x, temp);
	gtk_widget_set_size_request (edit_x, 40, 22);
	gtk_size_group_add_widget (marime_x, edit_x);
	gtk_size_group_add_widget (marime_y, edit_x);
	gtk_box_pack_start ((GtkBox*)box, edit_x, FALSE, FALSE, 2);
	GtkWidget *info_left = gtk_label_new (__("pixels left"));
	ps_hookup_object (box, info_left, "info_left");
	gtk_box_pack_start ((GtkBox*)box, info_left, FALSE, FALSE, 2);
	gtk_size_group_add_widget (marime_y, info_left);
	GtkWidget *info2 = gtk_label_new (__("and"));
	ps_hookup_object (box, info2, "info2");
	gtk_box_pack_start ((GtkBox*)box, info2, FALSE, FALSE, 2);
	gtk_size_group_add_widget (marime_y, info2);
	GtkWidget *edit_y = gtk_entry_new_with_max_length (4);
	sprintf (temp, "%d", y);
	gtk_entry_set_text ((GtkEntry*)edit_y, temp);
	ps_hookup_object (position, edit_y, "edit_y");
	gtk_box_pack_start ((GtkBox*)box, edit_y, FALSE, FALSE, 2);
	gtk_widget_set_size_request (edit_y, 40,22);
	gtk_size_group_add_widget (marime_x, edit_y);
	gtk_size_group_add_widget (marime_y, edit_y);
	GtkWidget *info_top = gtk_label_new (__("pixels top"));
	ps_hookup_object (box, info_top, "info_top");
	gtk_box_pack_start ((GtkBox*)box, info_top, FALSE, FALSE, 2);
	gtk_size_group_add_widget (marime_y, info_top);
	GtkWidget *custom_position = ps_create_radio_button_with_image_from_stock ("", __(strdup(str)), 1);
	ps_hookup_object (position, custom_position, "custom_position");
	GSList *group_position;
	group_position = gtk_radio_button_get_group ((GtkRadioButton*)default_position);
	gtk_radio_button_set_group ((GtkRadioButton*)custom_position, group_position);
	gtk_box_pack_start ((GtkBox*)position, default_position, FALSE, FALSE, 2);
	GtkWidget *custom_position_box = gtk_hbox_new (FALSE, 2);
	ps_hookup_object (position, custom_position_box, "custom_position_box");
	gtk_box_pack_start ((GtkBox*)custom_position_box, custom_position, FALSE, FALSE, 2);
	gtk_box_pack_start ((GtkBox*)custom_position_box, box, FALSE, FALSE, 2);
	gtk_box_pack_start ((GtkBox*)position, custom_position_box, FALSE, FALSE, 2);
	return position;
}

void ps_position_set_values (GtkWidget *position, int a, int x, int y)
{
	if (position!=NULL)
	{
		if (a!=0)
		{
			GtkWidget *default_position = (GtkWidget*)ps_get_widget_from (position, "default_position");
			ps_check_button_set_value (default_position, TRUE);
			/*GtkWidget *custom_position = (GtkWidget*)ps_get_widget_from (position, "custom_position");
			ps_check_button_set_value (custom_position, FALSE);*/
		}
		else
		{
			/*GtkWidget *default_position = (GtkWidget*)ps_get_widget_from (position, "default_position");
			ps_check_button_set_value (default_position, FALSE);*/
			GtkWidget *custom_position = (GtkWidget*)ps_get_widget_from (position, "custom_position");
			ps_check_button_set_value (custom_position, TRUE);
		}
		if (x>-1)
		{
			GtkWidget *edit_x = (GtkWidget*)ps_get_widget_from (position, "edit_x");
			gtk_entry_set_text ((GtkEntry*)edit_x, ps_inttostr (x));
		}
		if (y>-1)
		{
			GtkWidget *edit_y = (GtkWidget*)ps_get_widget_from (position, "edit_y");
			gtk_entry_set_text ((GtkEntry*)edit_y, ps_inttostr (y));
		}
	}
}

int ps_position_get_a (GtkWidget *position)
{
	int a=-1;
	if (position!=NULL)
	{
		GtkWidget *default_position = (GtkWidget*)ps_get_widget_from (position, "default_position");
		a = ps_check_button_get_value (default_position);
	}
	return a;
}

int ps_position_get_x (GtkWidget *position)
{
	int x=-1;
	if (position!=NULL)
	{
		GtkWidget *edit_x = (GtkWidget*)ps_get_widget_from (position, "edit_x");
		x = ps_strtoint (gtk_entry_get_text ((GtkEntry*)edit_x));
	}
	return x;
}

int ps_position_get_y (GtkWidget *position)
{
	int y=-1;
	if (position!=NULL)
	{
		GtkWidget *edit_y = (GtkWidget*)ps_get_widget_from (position, "edit_y");
		y = ps_strtoint (gtk_entry_get_text ((GtkEntry*)edit_y));
	}
	return y;
}

GtkWidget *ps_create_edit (const char *str, int maxlength, int size, int tip, char cp)
{
	GtkWidget *edit = NULL;
	if (tip==1) edit = gtk_hbox_new (FALSE, 2);
		else edit = gtk_vbox_new (FALSE, 2);
	if (str!=NULL)
	{
		GtkWidget *label = gtk_label_new (__(str));
		gtk_label_set_use_markup ((GtkLabel*)label, true);
		ps_hookup_object (edit, label, "label");
		gtk_box_pack_start ((GtkBox*)edit, label, FALSE, FALSE, 2);
	}
	GtkWidget *entry = gtk_entry_new_with_max_length (maxlength);
	ps_hookup_object (edit, entry, "entry");
	gtk_entry_set_width_chars ((GtkEntry*)entry, size);
	gtk_entry_set_max_length ((GtkEntry*)entry, maxlength);
	if (cp!='\0')
	{
		gtk_entry_set_invisible_char ((GtkEntry*)entry, cp);
		gtk_entry_set_visibility ((GtkEntry*)entry, FALSE);
	}
	gtk_box_pack_start ((GtkBox*)edit, entry, FALSE, FALSE, 2);
	return edit;
}

char *ps_edit_get_text (GtkWidget *edit)
{
	char *result = NULL;
	if (edit!=NULL)
	{
		GtkWidget *entry = (GtkWidget*)ps_get_widget_from (edit, "entry");
		if (entry!=NULL) result = strdup (gtk_entry_get_text ((GtkEntry*)entry));
	}
	if (result==NULL) result = strdup ("");
	return result;
}

void ps_edit_set_text (GtkWidget *edit, const char *str)
{
	if (edit!=NULL)
	{
		GtkWidget *entry = (GtkWidget*)ps_get_widget_from (edit, "entry");
		if (entry!=NULL) gtk_entry_set_text ((GtkEntry*)entry, str);
	}
}

GtkWidget * ps_server_data (char *server_name, const char *_server, int _port, const char *_username, const char *_password)
{
	GtkWidget *server_data = gtk_vbox_new (FALSE, 2);
	if (_server!=NULL)
	{
		GtkWidget *server_port = gtk_hbox_new (FALSE, 2);
		ps_hookup_object (server_data, server_port, "server_port");
		gtk_box_pack_start ((GtkBox*)server_data, server_port, FALSE, FALSE, 2);	
		GtkWidget *server = ps_create_edit (server_name, 255, 30, 1, '\0');
		ps_hookup_object (server_data, server, "server");
		ps_edit_set_text (server, _server);
		gtk_box_pack_start ((GtkBox*)server_port, server, FALSE, FALSE, 2);
		GtkWidget *port = ps_create_number_edit ("Port", 0, 100000, 1, 1);
		ps_hookup_object (server_data, port, "port");
		ps_number_edit_set_number (port, _port);
		gtk_box_pack_start ((GtkBox*)server_port, port, FALSE, FALSE, 2);
	}
	if (_username!=NULL && _password!=NULL)
	{
		GtkWidget *user_password = gtk_hbox_new (FALSE, 2);
		ps_hookup_object (server_data, user_password, "user_password");
		gtk_box_pack_start ((GtkBox*)server_data, user_password, FALSE, FALSE, 2);	
		GtkWidget *username = ps_create_edit ("Username", 255, 20, 1, '\0');
		ps_hookup_object (server_data, username, "username");
		ps_edit_set_text (username, _username);
		gtk_box_pack_start ((GtkBox*)user_password, username, FALSE, FALSE, 2);
		GtkWidget *password = ps_create_edit ("Password", 255, 20, 1, '*');
		ps_hookup_object (server_data, password, "password");
		ps_edit_set_text (password, _password);
		gtk_box_pack_start ((GtkBox*)user_password, password, FALSE, FALSE, 2);
	}
	return server_data;
}

char * ps_server_data_get_server (GtkWidget *server_data)
{
	char * result = NULL;
	if (server_data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from (server_data, "server");
		result = ps_edit_get_text (edit);
	}
	if (result == NULL) result = strdup ("");
	return result;
}

void ps_server_data_set_server (GtkWidget *server_data, const char *str)
{
	if (server_data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from (server_data, "server");
		ps_edit_set_text (edit, str);
	}
}

int ps_server_data_get_port (GtkWidget *server_data)
{
	int result = -1;
	if (server_data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from (server_data, "port");
		result = (int)ps_number_edit_get_number (edit);
	}
	return result;
}

void ps_server_data_set_port (GtkWidget *server_data, int t)
{
	if (server_data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from (server_data, "port");
		ps_number_edit_set_number (edit, t);
	}
}

char * ps_server_data_get_username (GtkWidget *server_data)
{
	char * result = NULL;
	if (server_data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from (server_data, "username");
		result = ps_edit_get_text (edit);
	}
	if (result == NULL) result = strdup ("");
	return result;
}

void ps_server_data_set_username (GtkWidget *server_data, const char *str)
{
	if (server_data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from (server_data, "username");
		ps_edit_set_text (edit, str);
	}
}

char * ps_server_data_get_password (GtkWidget *server_data)
{
	char * result = NULL;
	if (server_data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from (server_data, "password");
		result = ps_edit_get_text (edit);
	}
	if (result == NULL) result = strdup ("");
	return result;
}

void ps_server_data_set_password (GtkWidget *server_data, const char *str)
{
	if (server_data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from (server_data, "password");
		ps_edit_set_text (edit, str);
	}
}

GtkWidget * ps_create_number_edit (const char *str, double min, double max, double step, int tip)
{
	GtkWidget *number_edit = NULL;
	if (tip==1) number_edit = gtk_hbox_new (FALSE, 2);
		else number_edit = gtk_vbox_new (FALSE, 2);
	if (str!=NULL)
	{
		GtkWidget *label = gtk_label_new (__(str));
		ps_hookup_object (number_edit, label, "label");
		gtk_box_pack_start ((GtkBox*)number_edit, label, FALSE, FALSE, 2);
	}
	GtkWidget *number = gtk_spin_button_new_with_range (min, max, step);
	ps_hookup_object (number_edit, number, "number");
	gtk_spin_button_set_numeric ((GtkSpinButton*)number, TRUE);
	gtk_box_pack_start ((GtkBox*)number_edit, number, FALSE, FALSE, 2);
	return number_edit;
}

double ps_number_edit_get_number (GtkWidget *number_edit)
{
	double result = 0;
	if (number_edit!=NULL)
	{
		GtkWidget *number = (GtkWidget*)ps_get_widget_from (number_edit, "number");
		result = gtk_spin_button_get_value ((GtkSpinButton*)number);
	}
	return result;
}

void ps_number_edit_set_number (GtkWidget *number_edit, double nr)
{
	if (number_edit!=NULL)
	{
		GtkWidget *number = (GtkWidget*)ps_get_widget_from (number_edit, "number");
		gtk_spin_button_set_value ((GtkSpinButton*)number, nr);
	}
}

GtkWidget * ps_create_time_edit (const char *str, int h, int m)
{
	GtkWidget * time_edit = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start ((GtkBox*)time_edit, gtk_label_new (str), FALSE, FALSE, 0);
	GtkWidget *hour = ps_create_number_edit ("", 0, 23, 1, 1);
	ps_hookup_object (time_edit, hour, "hour");
	ps_number_edit_set_number (hour, h);
	gtk_box_pack_start ((GtkBox*)time_edit, hour, FALSE, FALSE, 0);
	gtk_box_pack_start ((GtkBox*)time_edit, gtk_label_new (":"), FALSE, FALSE, 0);
	GtkWidget *minute = ps_create_number_edit ("", 0, 59, 1, 1);
	ps_hookup_object (time_edit, minute, "minute");
	ps_number_edit_set_number (minute, m);
	gtk_box_pack_start ((GtkBox*)time_edit, minute, FALSE, FALSE, 0);
	return time_edit;
}

int ps_time_edit_get_hour (GtkWidget *time_edit)
{
	int result = 0;
	if (time_edit!=NULL)
	{
		GtkWidget *hour = (GtkWidget*)ps_get_widget_from (time_edit, "hour");
		result = (int)ps_number_edit_get_number (hour);
	}
	return result;
}

int ps_time_edit_get_minute (GtkWidget *time_edit)
{
	int result = 0;
	if (time_edit!=NULL)
	{
		GtkWidget *minute = (GtkWidget*)ps_get_widget_from (time_edit, "minute");
		result = (int)ps_number_edit_get_number (minute);
	}
	return result;
}

void ps_time_edit_set_time (GtkWidget *time_edit, int h, int m)
{
	if (time_edit!=NULL)
	{
		if (h>-1)
		{
			GtkWidget *hour = (GtkWidget*)ps_get_widget_from (time_edit, "hour");
			ps_number_edit_set_number (hour, h);
		}
		if (m>-1)
		{
			GtkWidget *minute = (GtkWidget*)ps_get_widget_from (time_edit, "minute");
			ps_number_edit_set_number (minute, m);
		}
	}
}

gboolean cb_ps_color_select_draw_color (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	if (widget->window)
	{
		GtkStyle *style;
		style = gtk_widget_get_style (widget);
		gdk_draw_rectangle (widget->window,
                            style->bg_gc[GTK_STATE_NORMAL],
                            TRUE,
                            event->area.x, event->area.y,
                            event->area.width, event->area.height);
	}
	return TRUE;
}

void cb_ps_color_select_select_color (GtkWidget *button, gpointer data)
{
	GtkWidget *draw_color = (GtkWidget*)ps_get_widget_from ((GtkWidget*)data, "draw_color");
	GtkWidget *display_alpha = (GtkWidget*)ps_get_widget_from ((GtkWidget*)data, "display_alpha");
	if (draw_color!=NULL && display_alpha!=NULL)
	{
		GtkStyle *stil = gtk_widget_get_style (draw_color);
		GdkColor col = stil->bg[GTK_STATE_NORMAL];
		long alpha = 127-(long)ps_number_edit_get_number (display_alpha)*127/255;
		_ps_color *color = ps_pack_color ((int)(col.red*255/65535), 
										  (int)(col.green*255/65535),
										  (int)(col.blue*255/65535),
										  alpha);
		ps_select_color_dialog (ps_get_main_window (), color);
		col.red = (int)(color->red*65535/255);
		col.green = (int)(color->green*65535/255);
		col.blue = (int)(color->blue*65535/255);
		gtk_widget_modify_bg (draw_color, GTK_STATE_NORMAL, &col);
		alpha = 255-(int)color->alpha*255/127;
		ps_number_edit_set_number (display_alpha, alpha);
	}
}

GtkWidget * ps_create_color_select (const char *str, _ps_color *color)
{
	GtkWidget *color_select = gtk_hbox_new (FALSE, 2);
	if (str!=NULL)
	{
		GtkWidget *label = gtk_label_new (__(str));
		ps_hookup_object (color_select, label, "label");
		gtk_box_pack_start ((GtkBox*)color_select, label, FALSE, FALSE, 2);
	}
	GtkWidget *buton = gtk_button_new ();
	ps_hookup_object (color_select, buton, "buton");
	gtk_widget_set_size_request (buton, 25, 25);
	gtk_box_pack_start ((GtkBox*)color_select, buton, FALSE, FALSE, 2);
	gtk_container_set_border_width ((GtkContainer*)buton, 3);
	GtkWidget *draw_color = gtk_drawing_area_new ();
	ps_hookup_object (color_select, draw_color, "draw_color");
	gtk_container_add ((GtkContainer*)buton, draw_color);
	GtkWidget *display_alpha = ps_create_number_edit ("opacity (0-transparent, 255-full)", 0, 255, 1, 1);
	ps_hookup_object (color_select, display_alpha, "display_alpha");
	gtk_box_pack_start ((GtkBox*)color_select, display_alpha, FALSE, FALSE, 2);
	GdkColor col;
	col.red = (int)(color->red*65535/255);
	col.green = (int)(color->green*65535/255);
	col.blue = (int)(color->blue*65535/255);
	gtk_widget_modify_bg (draw_color, GTK_STATE_NORMAL, &col);
	long alpha = 255-(int)color->alpha*255/127;
	ps_number_edit_set_number (display_alpha, alpha);
	g_signal_connect (draw_color, "expose_event", (GCallback)cb_ps_color_select_draw_color, NULL);
	g_signal_connect (buton, "clicked", (GCallback)cb_ps_color_select_select_color, color_select);
	return color_select;
}

_ps_color * ps_color_select_get_color (GtkWidget *color_select)
{
	_ps_color *color = ps_pack_color (255, 255, 255, 0);
	if (color_select!=NULL)
	{
		GtkWidget *draw_color = (GtkWidget*)ps_get_widget_from ((GtkWidget*)color_select, "draw_color");
		GtkWidget *display_alpha = (GtkWidget*)ps_get_widget_from ((GtkWidget*)color_select, "display_alpha");
		if (draw_color!=NULL && display_alpha!=NULL)
		{
			GtkStyle *stil = gtk_widget_get_style (draw_color);
			GdkColor col = stil->bg[GTK_STATE_NORMAL];
			long alpha = 127-(long)ps_number_edit_get_number (display_alpha)*127/255;
			color = ps_pack_color ((int)(col.red*255/65535), 
		  						   (int)(col.green*255/65535),
								   (int)(col.blue*255/65535),
								   alpha);
		}
	}
	return color;
}

_ps_color * ps_color_select_set_color (GtkWidget *color_select, _ps_color *color)
{
	if (color_select!=NULL)
	{
		GtkWidget *draw_color = (GtkWidget*)ps_get_widget_from ((GtkWidget*)color_select, "draw_color");
		GtkWidget *display_alpha = (GtkWidget*)ps_get_widget_from ((GtkWidget*)color_select, "display_alpha");
		if (draw_color!=NULL && display_alpha!=NULL)
		{
			GdkColor col;
			col.red = (int)(color->red*65535/255);
			col.green = (int)(color->green*65535/255);
			col.blue = (int)(color->blue*65535/255);
			gtk_widget_modify_bg (draw_color, GTK_STATE_NORMAL, &col);
			long alpha = (int)color->alpha*255/127;
			ps_number_edit_set_number (display_alpha, alpha);
		}
	}
	return color;
}

GtkWidget *ps_create_option_menu (const char *str, const char *items[], int n, int tip)
{
	GtkWidget *options = NULL;
	if (tip==1) options = gtk_hbox_new (FALSE, 2);
		else options = gtk_vbox_new (FALSE, 2);
	if (str!=NULL)
	{
		GtkWidget *label = gtk_label_new (__(str));
		ps_hookup_object (options, label, "label");
		gtk_box_pack_start ((GtkBox*)options, label, FALSE, FALSE, 2);
	}
	GtkWidget *opt_menu = gtk_option_menu_new ();
	ps_hookup_object (options, opt_menu, "opt_menu");
	gtk_box_pack_start ((GtkBox*)options, opt_menu, FALSE, FALSE, 2);
	GtkWidget *menu = gtk_menu_new ();
	ps_hookup_object (options, menu, "menu");
	gtk_option_menu_set_menu ((GtkOptionMenu*)opt_menu, menu);
	GtkWidget *item;
	for (int i=0;i<n;i++)
		if (items[i]!=NULL)
		{
			item = gtk_menu_item_new_with_label (__(items[i]));
			ps_hookup_object (menu, item, strdup (str));
			gtk_menu_shell_append ((GtkMenuShell*)menu, item);
		}
	if (n>0) gtk_option_menu_set_history ((GtkOptionMenu*)opt_menu, 0);
	return options;
}

int ps_option_menu_get_index (GtkWidget *options)
{
	int index = 0;
	if (options!=NULL)
	{
		GtkWidget *opt_menu = (GtkWidget*)ps_get_widget_from (options, "opt_menu");
		if (opt_menu!=NULL) index = gtk_option_menu_get_history ((GtkOptionMenu*)opt_menu);
	}
	return index;
}

void ps_option_menu_set_index (GtkWidget *options, int index)
{
	if (options!=NULL)
	{
		GtkWidget *opt_menu = (GtkWidget*)ps_get_widget_from (options, "opt_menu");
		if (opt_menu!=NULL) gtk_option_menu_set_history ((GtkOptionMenu*)opt_menu, index);
	}
}

void cb_ps_location_brow (GtkWidget *widget, gpointer data)
{
	if ((GtkWidget*)data!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from ((GtkWidget*)data, "edit");
		/*GtkWidget *brow_d = gtk_file_selection_new (__("Select the location"));
		if (edit!=NULL && brow_d!=NULL)
		{
			gtk_file_selection_hide_fileop_buttons ((GtkFileSelection*)brow_d);
			gtk_file_selection_set_filename ((GtkFileSelection*)brow_d, ps_edit_get_text (edit));
			if (gtk_dialog_run ((GtkDialog*)brow_d) == GTK_RESPONSE_OK)
			{
				gtk_file_selection_complete ((GtkFileSelection*)brow_d, "");
				ps_edit_set_text (edit, gtk_file_selection_get_filename ((GtkFileSelection*)brow_d));
			}
		}
		if (brow_d!=NULL) gtk_widget_destroy (brow_d);*/
		char *dir = ps_select_folder_dialog (NULL, __("Select the location"), ps_edit_get_text(edit));
		if (dir!=NULL)
		{
			ps_edit_set_text (edit, dir);
			free (dir);
		}
	}
}

GtkWidget *ps_create_location_edit (const char *str, int maxlen, int size, int tip)
{
	GtkWidget *location;
	if (tip==1) location = gtk_hbox_new (FALSE, 2);
	else location = gtk_vbox_new (FALSE, 2);
	GtkWidget *edit = ps_create_edit (str, maxlen, size, tip, '\0');
	ps_hookup_object (location, edit, "edit");
	gtk_box_pack_start ((GtkBox*)location, edit, FALSE, TRUE, 2);
	GtkWidget *brow = ps_create_button_with_image_from_stock ("gtk-open", __("Browse"), 1);
	ps_hookup_object (location, brow, "brow");
	gtk_box_pack_start ((GtkBox*)location, brow, FALSE, FALSE, 2);
	g_signal_connect (brow, "clicked", (GCallback)cb_ps_location_brow, location);
	return location;
}

char * ps_location_edit_get_location (GtkWidget *location)
{
	if ((GtkWidget*)location!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from ((GtkWidget*)location, "edit");
		if (edit!=NULL) return ps_edit_get_text (edit);
	}
	return strdup ("");
}

void ps_location_edit_set_location (GtkWidget *location, const char *loc)
{
	if ((GtkWidget*)location!=NULL)
	{
		GtkWidget *edit = (GtkWidget*)ps_get_widget_from ((GtkWidget*)location, "edit");
		if (edit!=NULL) ps_edit_set_text (edit, loc);
	}
}

GtkWidget *ps_preview_create ()
{
	GtkWidget* frame = gtk_frame_new (__("Preview"));
	GtkWidget* box = gtk_vbox_new (FALSE, 2);
	ps_hookup_object (frame, box, "box");
	gtk_container_add ((GtkContainer*)frame, box);
	GtkWidget* img = gtk_image_new ();
	ps_hookup_object (frame, img, "img");
	gtk_box_pack_start ((GtkBox*)box, img, FALSE, FALSE, 2);
	gtk_widget_show_all (frame);
	return frame;
}

void ps_preview_set_image (GtkWidget* preview, char *fpoza)
{
	if (preview!=NULL && fpoza!=NULL)
	{
		GtkWidget* img = (GtkWidget*)ps_get_widget_from (preview, "img");
		if ((GtkImage*)img!=NULL)
		{
			GdkPixbuf *poza;
			poza = gdk_pixbuf_new_from_file (fpoza, NULL);
			if (poza!=NULL)
			{
				GdkPixbuf * poza_preview = gdk_pixbuf_scale_simple (poza, 160, 120, GDK_INTERP_BILINEAR);
				g_object_unref (poza);
				gtk_image_set_from_pixbuf ((GtkImage*)img, poza_preview);
				g_object_unref (poza_preview);
			} else gtk_image_set_from_file ((GtkImage*)img, NULL);
		}
	}
}

void ps_preview_set_image_from_pixbuf (GtkWidget* preview, GdkPixbuf* pixbuf)
{
	if (preview!=NULL && pixbuf!=NULL)
	{
		GtkWidget* img = (GtkWidget*)ps_get_widget_from (preview, "img");
		if ((GtkImage*)img!=NULL)
		{
			gtk_image_set_from_pixbuf ((GtkImage*)img, pixbuf);
		}
	}
}
