
#ifndef _PS_LINUX__H
#define _PS_LINUX__H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define ps_dir_sep '/'
#define ps_dir_sep_str "/"

const unsigned int MAX_BUF = 3*1024; //maximum size of the read/recv buffer

#define ps_recv(sock, data, size, tip) recv (sock, data, size, tip)
#define ps_send(sock, data, size, tip) send (sock, data, size, tip)

int ps_random (int n);
int ps_set_wallpaper (const char *file);
char * ps_get_username ();
char * ps_get_user_dir ();
char *ps_get_current_path ();
int socket_connect (const char *host, int port);
void socket_close (int sock);

#endif
