
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "messages.h"
#include "misc.h"

static GList *ps_messages_list;

char * message_make_date (_ps_message *message)
{
	char temp[100];
	strcpy (temp, "");
	if (message!=NULL)
	{
		if (message->date==0) strcat (temp, "*");
			else sprintf (temp, "%s%.2d", temp, message->date);
		if (message->date==0 && message->wday!=0) sprintf (temp, "%s%d", temp, message->wday);
		strcat (temp,"/");
		if (message->month==0) strcat (temp, "*");
			else sprintf (temp, "%s%.2d", temp, message->month);
		strcat (temp,"/");
		if (message->year==0) strcat (temp, "*");
			else sprintf (temp, "%s%d", temp, message->year);
	}
	return strdup (temp);
}

int messages_list_message_to_id_comparator (const void *x, const void *y)
{
	_ps_message *msg_1 = (_ps_message*)x;
	if (msg_1!=NULL && (const char*)y!=NULL)
	{
		return strcmp (msg_1->id, (const char*)y);
	}
	return -1;
}

int messages_list_date_comparator (const void *x, const void *y)
{
	_ps_message *msg_1 = (_ps_message*)x;
	_ps_message *msg_2 = (_ps_message*)y;
	char *msg_date_1 = message_make_date (msg_1);
	char *msg_date_2 = message_make_date (msg_2);
	int result = strcmp (msg_date_1, msg_date_2);
	free (msg_date_1);
	free (msg_date_2);
	return result;
}

int messages_list_time_comparator (const void *x, const void *y)
{
	_ps_message *msg_1 = (_ps_message*)x;
	_ps_message *msg_2 = (_ps_message*)y;
	char msg_time_1[100];
	char msg_time_2[100];
	sprintf (msg_time_1, "%.2d:%.2d", msg_1->hour, msg_1->minute);
	sprintf (msg_time_2, "%.2d:%.2d", msg_2->hour, msg_2->minute);
	return strcmp (msg_time_1, msg_time_2);
}

_ps_message * create_message (int day, int month, int year, int wday, int h, int m, const char *message_text)
{
	_ps_message *message = NULL;
	if ((message_text!=NULL) && (day>-1 && day<=31) && (month>-1 && month<=12) && ((year>=1900 && year<=3000) || year==0) && ((wday==0) || ((day==0 || month==0 || year==0) && (wday>0 && wday<=7))) && (h>=0 && h<24) && (m>=0 && m<60))
	{
		message = (_ps_message*)malloc (sizeof (_ps_message));
		strcpy (message->id, generate_security_code (32));
		message->date = day;
		message->month = month;
		message->year = year;
		message->wday = wday;
		message->hour = h;
		message->minute = m;
		strncpy (message->message_text, message_text, 250);
		message->message_text[249]='\0';
	}
	return message;
}

void add_message (char *date, char *time, const char *message_text)
{
	printf ("message date: %s\n", date);
	int day, month, year, wday, h, m;
	char *sday, *smonth, *syear;
	sday = strtok ((char*)date, "/");
	smonth = strtok (NULL, "/");
	syear = strtok (NULL, "\0");
	if (sday!=NULL && smonth!=NULL && syear!=NULL && time!=NULL && message_text!=NULL)
	{
		char *sday_t = strtrim (sday);
		char *smonth_t = strtrim (smonth);
		char *syear_t = strtrim (syear);
		if (sday_t!=NULL && smonth_t!=NULL && syear_t!=NULL)
		{
			wday = 0;
			day = 0;
			month = 0;
			year = 0;
			if (sday_t[0]=='*' && strlen (sday_t)==2) 
			{
				sprintf (sday_t, "%s", &sday_t[1]);
				sscanf (sday_t, "%d", &wday);
			}
			else sscanf (sday_t, "%d", &day);
			if (smonth_t[0]=='*' && strlen (smonth_t)==1) month=0;
				else sscanf (smonth_t, "%d", &month);
			if (syear_t[0]=='*' && strlen (syear_t)==1) year=0;
			sscanf (syear_t, "%d", &year);
			h = -1;
			m = -1;
			sscanf (time, "%d:%d", &h, &m);
			printf ("message: %d %d %d %d @ %d %d \"%s\"\n", day, month, year, wday, h, m, message_text);
			_ps_message *message = create_message (day, month, year, wday, h, m, message_text);
			printf ("message id code: %s\n", message->id);
			if (message!=NULL)
			{
				ps_messages_list = g_list_append (ps_messages_list, message);
			}
			else printf ("Error In Message\n");
		}
		else printf ("Error In Message\n");
		if (sday_t!=NULL) free (sday_t);
		if (smonth_t!=NULL) free (smonth_t);
		if (syear_t!=NULL) free (syear_t);
	}
	else printf ("Error In Message\n");
	if (date!=NULL) free (date);
	if (time!=NULL) free (time);
}

void load_messages (const char *messages_file)
{
	//messages_list = g_list_alloc ();
	FILE *f = fopen (messages_file, "r");
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
					char *var = strtrim (strtok (data, " "));
					//strtok (NULL, "\"");
					char *val = strtrim (strtok (NULL, "\0"));
					if (var!=NULL && val!=NULL)
					{
						//char *head_var = (char*)malloc (strlen(head)+strlen(var)+3);
						//printf ("%s=%s\n", var, val);
						//sprintf (head_var, "%s_%s", head, var);
						add_message (strdup (head), strdup (var), val);
						//ps_set_value (&config, head_var, val);
					}
					if (val!=NULL) free (val);
					if (var!=NULL) free (var);
				}
				free (data);
			}
		}
		fclose (f);
	}
	ps_messages_list = g_list_sort (ps_messages_list, (GCompareFunc)messages_list_time_comparator);
}

void save_messages (const char *messages_file)
{
	FILE *f = fopen (messages_file, "w");
	if (f!=NULL)
	{
		fprintf (f, "# THIS FILE AUTOMATICLY GENERATED BY PHOTOSTORM.\n# EDIT IT ONLY IF YOU KNOW WHAT YOU ARE DOING !!!\n\n");
		char *last_date = NULL;
		GList *ps_messages_list = get_messages_list ();
		ps_messages_list = g_list_sort(ps_messages_list, (GCompareFunc)messages_list_date_comparator);
		int nr_msg = g_list_length (ps_messages_list);
		for (int j=0;j<nr_msg;j++)
		{
			_ps_message *message = (_ps_message*)g_list_nth_data (ps_messages_list, j);
			if (message!=NULL)
			{
				printf ("Mesaj %d\n", j);
				char *date = message_make_date (message);
				if (last_date==NULL || (last_date!=NULL && strcmp (last_date, date)!=0)) fprintf (f, "%s[%s]\n", (last_date!=NULL)?"\n":"", date);
				fprintf (f, "%.2d:%.2d %s\n", message->hour, message->minute, message->message_text);
				if (last_date!=NULL) free (last_date);
				last_date = date;
			}
		}
		fclose (f);
		if (last_date!=NULL) free (last_date);
	}
}

GList * get_messages_list ()
{
	return ps_messages_list;
}

void set_messages_list (GList *new_messages_list)
{
	ps_messages_list = new_messages_list;
}
