
#include "data.h"

#include "ps_linux.h"
#include "misc.h"
#include "configs.h"
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int ps_set_wallpaper (const char *file)
{
	int result=0;
	if (file!=NULL)
	{
		/*GConfClient *conf=NULL;
		conf = gconf_client_get_default ();
		if (conf!=NULL)
		{
			GConfValue *data;
			data = gconf_value_new (GCONF_VALUE_STRING);
			if (data!=NULL)
			{
				gconf_value_set_string (data, "");
				gconf_client_set (conf, "/desktop/gnome/background/picture_filename", data, NULL);
				gconf_value_set_string (data, file);
				gconf_client_set (conf, "/desktop/gnome/background/picture_filename", data, NULL);
				gconf_value_free (data);
			}
			g_object_unref (conf);
			free (file);
		}*/
		char command[1000];
		sprintf (command, "ps_wallpaper %s",file); 
		if (command!=NULL)
		{
			system (command);
			result=1;
		}
	}
	return result;
}

char * ps_get_username ()
{
	char *result = getlogin ();
	if (result==NULL) result = strdup ("");
	else
	{
		passwd *usr;
		usr = getpwnam (result);
		if (usr!=NULL)
		{
			if (strtrim (usr->pw_gecos))
			{
				result = strtrim (usr->pw_gecos);
			}
		}
	}
	return result;
}

char * ps_get_user_dir ()
{
	char *result = getenv ("HOME");
	if (result==NULL) result = strdup ("");
	return result;
}

char * ps_get_current_path ()
{
	char * result=NULL;
	char temp[10000];
	if (getcwd (temp, sizeof(temp))!=NULL)
	{
		result = strdup (temp);
	}
	return result;
}

int sockaddr_init (sockaddr_in *addr, const char*hostname, int port)
{
	int result = 1;
    struct hostent *hostinfo;
	//sethostent (1);
    hostinfo = gethostbyname (hostname);
	if (hostinfo!=NULL)
	{
		addr->sin_family = AF_INET;
        addr->sin_addr = *(struct in_addr*) hostinfo->h_addr;
    	addr->sin_port = htons (port);
	}
	else result = 0;
	return result;
}

int socket_connect (const char *host, int port)
{
	int sock = -1;
	sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt (sock, SOL_SOCKET, SO_KEEPALIVE, (void*)1, 1);
    if (sock!=-1)
    {
		sockaddr_in name;
		if (sockaddr_init (&name, host, port))
		{
			if (connect (sock, (sockaddr*) &name, sizeof (name)) != 0) 
			{	
				close (sock);
				sock = -1;
			}
		}
		else 
		{	
			close (sock);
			sock = -1;
		}
	}
	return sock;
}

void socket_close (int sock)
{
	close (sock);
}
