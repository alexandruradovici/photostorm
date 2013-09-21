#include <windows.h>

int main(int argc, char *argv[])
{
    HWND w = FindWindow ("PhotoStorm TrayIcon Server", NULL);
    if (w > 0) SendMessage (w, WM_USER, 0, -1);
    long  n = 0;
    while (FindWindow ("PhotoStorm TrayIcon Server", NULL)>0 && n < 100000)
    {
        n++;
    }
    Sleep (2000);
}

