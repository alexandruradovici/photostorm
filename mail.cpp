
#include "mail.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ps_system.h"
#include "lang/translate.h"

_ps_mail * ps_process_email (char *email)
{
	_ps_mail *mail = (_ps_mail*)malloc (sizeof(_ps_mail));
	char from[1000];
	strcpy (from, __("<Unknown sender>"));
	char subject[1000];
	strcpy (subject, __("<no subject>"));
	if (email!=NULL)
	{			
		char *pfrom = strstr (email, "From: ");
		if (pfrom!=NULL)
		if (pfrom+5!=NULL)
		{
			sscanf (pfrom+5, "%[^\n]", from);
			//printf ("From: %s\n", from);
		}
		char *psubject = strstr (email, "Subject: ");
		if (psubject!=NULL)
		if (psubject+9!=NULL)
		{
			sscanf (psubject+9, "%[^\n]", subject);
			//printf ("Subject: %s\n", subject);
		}
	}
	mail->from = strdup (from);
	mail->subject = strdup (subject);
	mail->date = strdup ("");
	return mail;
}

_ps_emails * ps_get_mail_list (char *server, int port, char* user, char *password, int max_nr)
{
	_ps_emails *result = NULL;
	int sock = socket_connect (server, port);
	if (sock != -1)
	{
		char buf[MAX_BUF];
		int l = ps_recv (sock, buf, sizeof (buf), 0);
		buf[l]='\0';
		//printf ("(%s)", buf);
		if (buf[0]=='+')
		{
			//printf ("Connected to mailserver\n");
			sprintf (buf, "USER %s\n", user);
			ps_send (sock, buf, strlen (buf), 0);
			l = ps_recv (sock, buf, sizeof(buf), 0);
			buf[l]='\0';
			//printf ("(%s)", buf);
			if (buf[0]=='+')
			{
				//printf ("User ok\n");
				sprintf (buf, "PASS %s\n", password);
				ps_send (sock, buf, strlen (buf), 0);
				l = ps_recv (sock, buf, sizeof(buf), 0);
				buf[l]='\0';
				//printf ("(%s)", buf);
				if (buf[0]=='+')
				{
					//printf ("Password ok\n");
					strcpy (buf, "LIST\n");
					ps_send (sock, buf, strlen (buf), 0);
					l=0;
					char data[MAX_BUF];
					l = ps_recv (sock, buf, sizeof(buf), 0);
					buf[l]='\0';
					strcpy (data, buf);
					while (buf[l-3]!='.')
					{
						//printf ("[%s]\n", buf);
						l = ps_recv (sock, buf, sizeof(buf), 0);
						buf[l]='\0';
						if (strlen(data)+l < MAX_BUF) strcat (data, buf);
					}
					//printf ("(%s)", data);
					if (data[0]=='+')
					{
						//printf ("Got the list\n");
						int mails=0;
						char *mail_id = strtok (data, "\n");
						while (mail_id!=NULL)
						{
							mail_id = strtok (NULL, "\n");
							mails++;
						}
						mails-=2;
						//printf ("Found %d mails.\n", mails);
						int n = mails;
						if (n > max_nr) n=max_nr;
						if (mails>0)
						{
							result = (_ps_emails*)malloc (sizeof(_ps_emails));
							result->count = 0;
							char email[MAX_BUF];
							for (int i=0;i<n;i++)
							{
								strcpy (buf,"");
								sprintf (buf, "TOP %d 0\n", mails-i);
								ps_send (sock, buf, strlen (buf), 0);
								l=0;
								l = ps_recv (sock, buf, sizeof(buf), 0);
								buf[l]='\0';
								strcpy (email, buf);
								while (buf[l-3]!='.')
								{
									l = ps_recv (sock, buf, sizeof(buf), 0);
									buf[l]='\0';
									//printf ("l: %d\n", l);
									if (strlen (email)+l < MAX_BUF) strcat (email, buf);
								}
								//printf ("(%s)\n", mailstr);
								//printf ("e-mail %d\n", i);
								result->email[result->count] = ps_process_email (email);
								if (result->email[result->count]!=NULL) result->count++;
							}
						}
					}
				}
			}
		}
		socket_close (sock);
	}
	return result;
}

void ps_mail_free (_ps_mail *mail)
{
	if (mail!=NULL)
	{
		if (mail->from!=NULL) free (mail->from);
		if (mail->subject!=NULL) free (mail->subject);
		if (mail->date!=NULL) free (mail->date);
		free (mail);
	}
}

void ps_emails_free (_ps_emails *emails)
{
	if (emails!=NULL)
	{
		for (int i=0;i<emails->count;i++) if (emails->email[i]!=NULL) ps_mail_free (emails->email[i]);
		free (emails);
	}
}
