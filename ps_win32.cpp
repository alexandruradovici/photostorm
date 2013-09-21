
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include "misc.h"
#include <time.h>

#include "ps_win32.h"

#ifdef WIN32

int ps_random (int n)
{
    srand (time(0));
    long rnd = rand ();
    double nr = (rnd*rnd%100)/100.0;
    return (int)(nr*n);
}

#endif

int ps_set_wallpaper (const char *file)
{
	int result=0;
	if (file!=NULL)
	{
	    char temp[10000];
	    int n=strlen (file)-4;
	    strncpy (temp, file, n);
	    temp[n]='\0';
	    sprintf (temp, "%s.bmp", temp);
	    remove (temp);
	    sprintf (temp, "%s",ps_get_install_dir());
	    chdir (temp);
	    //printf ("temp: %s\n", temp);
	    sprintf (temp, "%s -q \"%s\"", "png2bmp.exe", file);
	    //printf ("temp: %s\n", temp);
	    system (temp);
	    n=strlen (file)-4;
	    strncpy (temp, file, n);
	    temp[n]='\0';
	    sprintf (temp, "%s.bmp", temp);
        HKEY hKey;
        RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"),
        0, KEY_SET_VALUE, &hKey);
    
        if (hKey!=NULL)
        {
           RegSetValueEx(hKey, "Wallpaper", 0, REG_SZ, (unsigned char*)temp, strlen(temp));
           RegSetValueEx(hKey, "WallpaperStyle", 0, REG_SZ, (unsigned char*)"0", 1);
           RegCloseKey (hKey);
        }
        SystemParametersInfo (SPI_SETDESKWALLPAPER, 0, strdup (temp), SPIF_SENDCHANGE);
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
		/*char *command = (char*)malloc (strlen(ps_path)+strlen("/wallpaper")+strlen(file)+1);
		if (command!=NULL)
		{
			strcpy (command, ps_path);
			strcat (command, "/wallpaper ");
			strcat (command, file);
			system (command);
			free (command);
			result=1;
		}*/
	}
	return result;
}

char * ps_get_username ()
{
    return strdup ("");
}

char *ps_get_install_dir ()
{
    char *instdir = strdup ("");
    
    HKEY hKey;
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\PhotoStorm"),
    0, KEY_QUERY_VALUE, &hKey);
    
    if (hKey!=NULL)
    {
        unsigned char buffer[_MAX_PATH];
        unsigned long datatype;
        unsigned long bufferlength = sizeof(buffer);
        RegQueryValueEx(hKey, "Libs", NULL, &datatype, buffer, &bufferlength);
        instdir = strdup ((const char*)buffer);
        RegCloseKey (hKey);
    }
    return strdup (instdir);
}

char *ps_get_user_dir ()
{
    char *usrdir = strdup ("");
    
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Volatile Environment"),
    0, KEY_QUERY_VALUE, &hKey);
    
    if (hKey!=NULL)
    {
        unsigned char buffer[_MAX_PATH];
        unsigned long datatype;
        unsigned long bufferlength = sizeof(buffer);
        RegQueryValueEx(hKey, "APPDATA", NULL, &datatype, buffer, &bufferlength);
        usrdir = strdup ((const char*)buffer);
        RegCloseKey (hKey);
    }
    return strdup (usrdir);
}

char *ps_get_user_docs_dir ()
{
    char *usrdocdir = strdup ("");
    
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"),
    0, KEY_QUERY_VALUE, &hKey);
    
    if (hKey!=NULL)
    {
        unsigned char buffer[_MAX_PATH];
        unsigned long datatype;
        unsigned long bufferlength = sizeof(buffer);
        RegQueryValueEx(hKey, "Personal", NULL, &datatype, buffer, &bufferlength);
        usrdocdir = strdup ((const char*)buffer);
        RegCloseKey (hKey);
    }
    return strdup (usrdocdir);
}

void sockaddr_init (sockaddr_in *addr, const char*hostname, int port)
{
    struct hostent *hostinfo;
	//sethostent (1);
    hostinfo = gethostbyname (hostname);
	if (hostinfo!=NULL)
	addr->sin_family = AF_INET;
    if (hostinfo!=NULL) addr->sin_addr = *(struct in_addr*) hostinfo->h_addr;
    addr->sin_port = htons (port);
}

int socket_connect (const char *host, int port)
{
	int sock = -1;
	sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt (sock, SOL_SOCKET, SO_KEEPALIVE, "1", 1);
    if (sock!=-1)
    {
		sockaddr_in name;
		sockaddr_init (&name, host, port);
		if (connect (sock, (sockaddr*) &name, sizeof (name)) != 0) closesocket (sock);
	}
	return sock;
}

void socket_close (int sock)
{
	closesocket (sock);
}


