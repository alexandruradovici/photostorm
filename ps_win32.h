
#include <winsock.h>
#include <io.h>

const unsigned int MAX_BUF = 3*1024; //maximum size of the read/recv buffer

#define ps_recv(sock, data, size, tip) recv (sock, data, size, tip)
#define ps_send(sock, data, size, tip) send (sock, data, size, tip)

int ps_random (int n);
int ps_set_wallpaper (const char *file);
char * ps_get_username ();
char *ps_get_install_dir ();
char *ps_get_user_dir ();
char *ps_get_user_docs_dir ();
void sockaddr_init (sockaddr_in *addr, const char*hostname, int port);
int socket_connect (const char *host, int port);
void socket_close (int sock);

