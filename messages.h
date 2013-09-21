
#ifndef _MESSAGES__H
#define _MESSAGES__H

#include "data.h"
#include "ps_types.h"
#include <gtk/gtk.h>

int messages_list_message_to_id_comparator (const void *x, const void *y);
int messages_list_time_comparator (const void *x, const void *y);
void load_messages (const char *messages_file);
void save_messages (const char *messages_file);
_ps_message * create_message (int day, int month, int year, int wday, int h, int m, const char *message_text);
GList * get_messages_list ();
void set_messages_list (GList *new_messages_list);

#endif
