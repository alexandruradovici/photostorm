
#ifndef _MAIL__H
#define _MAIL__H

struct _ps_mail
{
	char *from;
	char *subject;
	char *date;
};

struct _ps_emails
{
	_ps_mail *email[1000];
	int count;
};

_ps_emails * ps_get_mail_list (char *server, int port, char* user, char *password, int max_nr);
void ps_mail_free (_ps_mail *mail);
void ps_emails_free (_ps_emails *emails);

#endif
