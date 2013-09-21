
#include <windows.h>
#include <winuser.h>
#include <gtk/gtk.h>

#include "resource.h"
#include "configs.h"
#include "ps_win32.h"
#include "album.h"
#include "misc.h"
#include "photo.h"

#define SYSTRAY_ICON_BASE 1024

static _ps_config *config;
static time_t ps_timestamp = 0;
static time_t ps_last_wallpaper_change = 0;
static time_t ps_last_email_update = 0;
GtkWidget *textul;
static HWND hwnd;               /* This is the handle for our window */
static MSG messages;            /* Here messages to the application are saved */
static WNDCLASSEX wincl;        /* Data structure for the windowclass */
static GtkWidget *meniu;

static int photostorm_running = false;

void * start_cmd (void *data)
{
	photostorm_running = true;
	
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if(CreateProcess(NULL,   // No module name (use command line). 
        TEXT ((char*)data), // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    ) 
    {
        // Wait until child process exits.
        WaitForSingleObject( pi.hProcess, INFINITE );

        // Close process and thread handles. 
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
     }

	
	photostorm_running = false;
	g_thread_exit (0);
	return (0);
}

void system__ (const char *cmd)
{
	if (!photostorm_running)
	{
		GThread *firul;
		firul = g_thread_create (start_cmd, (void*)cmd, TRUE, NULL);
	}
}

void ps_applet_load_config ()
{
	config = ps_load_config (ps_get_config_filename());
	char temp[10000];
	sprintf (temp, "%s\\.photostorm\\photostorm.cwa", ps_get_user_dir ());
	_ps_album *date_album = ps_load_album_heather (temp);
	if (date_album!=NULL)
	{
		ps_last_wallpaper_change = date_album->data;
		ps_album_free (date_album);
	} else ps_last_wallpaper_change = 0;
}

static void display_start_photostorm (GtkWidget *meniu, const char *data) 
{
    char t[10000];
	sprintf (t, "\"%s\\photostorm.exe\"", ps_get_install_dir ());
	system__ (t);
	ps_applet_load_config ();
}

static void display_new_wallpaper (GtkWidget *meniu, const char *data) 
{
	char t[10000];
	sprintf (t, "\"%s\\photostorm.exe\" --new_wallpaper", ps_get_install_dir ());
	system__ (t);
	ps_applet_load_config ();
}

static void display_update_wallpaper (GtkWidget *meniu, const char *data) 
{
    char t[10000];
	sprintf (t, "\"%s\\photostorm.exe\" --update_wallpaper", ps_get_install_dir ());
	system__ (t);
	ps_applet_load_config ();
}
   
static void display_configuration_dialog (GtkWidget *meniu, const char *data) 
{
	char t[10000];
	sprintf (t, "\"%s\\photostorm.exe\" --configure", ps_get_install_dir ());
	system__ (t);
	ps_applet_load_config ();
}

static void display_messages_editor_dialog (GtkWidget *meniu, const char *data) 
{
	char t[10000];
	sprintf (t, "\"%s\\photostorm.exe\" --messages_editor", ps_get_install_dir ());
	system__ (t);
	ps_applet_load_config ();
}

void cb_ps_about_close (GtkWidget *window, GdkEventButton *ev_b, gpointer data)
{
	if (ev_b->button==1) if ((GtkWidget*)data!=NULL) gtk_widget_destroy ((GtkWidget*)data);
}

static gboolean cb_close_about_dialog (gpointer data)
{
    if (GTK_IS_WIDGET((GtkWidget*)data)) gtk_widget_destroy ((GtkWidget*)data);
    return false;
}

static void display_about_dialog (GtkWidget *meniu, gpointer data) 
{
	GtkWidget *about_window = gtk_window_new (GTK_WINDOW_POPUP);
	gtk_window_set_decorated ((GtkWindow*)about_window, FALSE);
	gtk_window_set_position ((GtkWindow*)about_window, GTK_WIN_POS_CENTER);
	GtkWidget *pslogo_ev = gtk_event_box_new ();
	char temp[10000];
	sprintf (temp, "%s\\pics\\logos\\splash_screen.jpg", ps_get_install_dir ());
	GtkWidget *photostorm_logo = gtk_image_new_from_file (temp);
	gtk_container_add ((GtkContainer*)pslogo_ev, photostorm_logo);
	gtk_container_add ((GtkContainer*)about_window, pslogo_ev);
	g_signal_connect (pslogo_ev, "button-release-event", (GCallback)cb_ps_about_close, about_window);
	gtk_widget_show_all (about_window);
	if ((int*)data!=NULL)
	{
	   int n = (int)data;
	   gtk_timeout_add (n, (GtkFunction)cb_close_about_dialog, about_window);
    }
}

static void quit_trayicon (GtkWidget *meniu, const char *data) 
{
    gtk_main_quit ();
    PostQuitMessage (0);
}

GtkWidget * ps_create_meniu()
{
	GtkWidget *meniu;
	meniu = gtk_menu_new ();
	GtkWidget *item;
	GtkWidget *item_img;
	// Start PhotoStorm
	item_img = gtk_image_new_from_stock ("gtk_go-forward", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label ("Start PhotoStorm");
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu, item);
	//ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_nou");
	g_signal_connect (item, "activate", (GCallback)display_start_photostorm, NULL);
	// New Wallpaper
	item_img = gtk_image_new_from_stock ("gtk-new", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label ("Make New Wallpaper");
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu, item);
	//ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_nou");
	g_signal_connect (item, "activate", (GCallback)display_new_wallpaper, NULL);
	// Update Wallpaper
	item_img = gtk_image_new_from_stock ("gtk-refresh", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label ("Update Wallpaper");
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu, item);
	//ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_nou");
	g_signal_connect (item, "activate", (GCallback)display_update_wallpaper, NULL);
	// Configuration
	item_img = gtk_image_new_from_stock ("gtk-preferences", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label ("Configuration");
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu, item);
	//ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_nou");
	g_signal_connect (item, "activate", (GCallback)display_configuration_dialog, NULL);
	// Messages Editor
	item_img = gtk_image_new_from_stock ("gtk-index", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label ("Messages Editor");
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu, item);
	//ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_nou");
	g_signal_connect (item, "activate", (GCallback)display_messages_editor_dialog, NULL);
	// About
	item_img = gtk_image_new_from_stock ("gtk-about", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label ("About");
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu, item);
	//ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_nou");
	g_signal_connect (item, "activate", (GCallback)display_about_dialog, NULL);
	// linia
	item = gtk_image_menu_item_new ();
	gtk_widget_set_sensitive (item, FALSE);
	gtk_menu_append ((GtkMenu*)meniu, item);
	// Quit
	item_img = gtk_image_new_from_stock ("gtk-close", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label ("Quit");
	gtk_image_menu_item_set_image ((GtkImageMenuItem*)item, item_img); 
	gtk_menu_append ((GtkMenu*)meniu, item);
	//ps_hookup_object (ps_get_main_window(), item, "ps_main_menu_lista_albume_nou");
	g_signal_connect (item, "activate", (GCallback)quit_trayicon, NULL);
	gtk_widget_show_all (meniu);
	return meniu;
}

gboolean cb_photostorm_server (gpointer data)
{   
	if (ps_timestamp == 0) time(&ps_timestamp);
	time_t ts;
	time (&ts);
	
	tm *t1_;
	tm t1;
	t1_ = localtime (&ps_last_wallpaper_change);
	memcpy (&t1, t1_, sizeof (tm));
	tm *t2_;
	tm t2;
	t2_ = localtime (&ts);
	memcpy (&t2, t2_, sizeof (tm));
	
	
	
	int w_new=0;
	int w_update=0;
	
	if (ps_last_wallpaper_change>0)
	{
		switch (config->wallpaper.cint)
		{
			case 0:
				break;
			case 1:
				if (t1.tm_hour != t2.tm_hour) w_new=1;
				break;
			case 2:
				if (t1.tm_wday != t2.tm_wday) w_new=1;
				break;
			case 3:
				if (t1.tm_wday > t2.tm_wday) w_new=1;
				break;
			case 4:
				if (t1.tm_mon != t2.tm_mon) w_new=1;
				break;
			case 5:
				if (t1.tm_year != t2.tm_year) w_new=1;
				break;
		}
		if (!w_new)
		{
			if (config->email.disp && abs (ts-ps_last_wallpaper_change)>(config->email.update?config->email.update:10)*60) w_update=1;
			if (config->calendar.disp && (t1.tm_mon!=t2.tm_mon || t1.tm_mday!=t2.tm_mday || t1.tm_year!=t2.tm_year)) w_update=1;
		}
	}
	if (w_new)
	{
	    char t[10000];
	    sprintf (t, "\"%s\\photostorm.exe\" --new_wallpaper", ps_get_install_dir ());
	    system__ (t);
		ps_applet_load_config ();
	}
	if (w_update)
	{
	    char t[10000];
        sprintf (t, "\"%s\\photostorm.exe\" --update_wallpaper", ps_get_install_dir ());
		system__ (t);
		ps_applet_load_config ();
	}
	/*if (abs (ts-ps_timestamp) > 10)
	{
		ps_timestamp = ts;
		char t[1000];
		sprintf (t, "%s/photostorm --update_wallpaper", config->system.libs);
		system (t);
	}*/
	return TRUE;
}

BOOL systray_add(HWND hwnd, UINT uID, HICON hIcon, LPSTR lpszTip)
{
  NOTIFYICONDATA tnid;
  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd = hwnd;
  tnid.uID = uID+SYSTRAY_ICON_BASE;
  tnid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
  tnid.uCallbackMessage = WM_USER + 2048;
  tnid.hIcon = hIcon;
  lstrcpyn(tnid.szTip,lpszTip,sizeof(tnid.szTip)-1);
  return (Shell_NotifyIcon(NIM_ADD, &tnid));
}

BOOL systray_del(HWND hwnd, UINT uID) {
  NOTIFYICONDATA tnid;
  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd = hwnd;
  tnid.uID = uID+SYSTRAY_ICON_BASE;
  return(Shell_NotifyIcon(NIM_DELETE, &tnid));
}

void * ps_win32_messages_server (void *data)
{
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
	g_thread_exit (0);
	return (0);
}

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "PhotoStorm TrayIcon Server";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND trayIconWindow = FindWindow ("PhotoStorm TrayIcon Server", NULL);
    int ps_wm_user = 0;
    if (nFunsterStil > 0)
    {
        if (strcmp (lpszArgument, "--about")==0) 
        {
            if (trayIconWindow > 0)
            {
               SendMessage (trayIconWindow, WM_USER, 0, 1);
            }
            ps_wm_user = 1;
        }
        
        if (strcmp (lpszArgument, "--quit")==0) 
        {
            if (trayIconWindow > 0)
            {
               SendMessage (trayIconWindow, WM_USER, 0, -1);
               long n = 0;
               while (FindWindow ("PhotoStorm TrayIcon Server", NULL)>0 && n<100000)
               {
                  n++;
               }
            }
            exit (0);
        }
    }
    if (trayIconWindow > 0) exit (0);
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    
    systray_add (hwnd, 0, LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON1)), "PhotoStorm Desktop");       
    
    gtk_init (0, NULL);
    g_thread_init (NULL);
    meniu = ps_create_meniu ();
    ps_applet_load_config ();
    GThread *fir_win32_msg;
    fir_win32_msg = g_thread_create (ps_win32_messages_server, NULL, TRUE, NULL);
    gtk_timeout_add (1000, (GtkFunction)cb_photostorm_server, NULL);
    if (ps_wm_user == 1) display_about_dialog (NULL, (int*)7000);
    gtk_main ();
    systray_del (hwnd, 0);
//    gtk_main ();
    /* Run the message loop. It will run until GetMessage() returns 0 */
    
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message==WM_USER+2048)
	{
		int which = LOWORD (wParam) - 1024;
		switch (LOWORD(lParam))
		{
            case WM_RBUTTONUP:
				gtk_menu_popup ((GtkMenu*)meniu, NULL, NULL, NULL, NULL, 0, gtk_get_current_event_time ());
				break;
		
			case WM_LBUTTONUP:
				gtk_menu_popup ((GtkMenu*)meniu, NULL, NULL, NULL, NULL, 0, gtk_get_current_event_time ());
				break;
		}
	}
	else
	if (message==WM_USER)
	{
       if (lParam==1) display_about_dialog (NULL, (int*)7000);
	   if (lParam==-1) quit_trayicon (NULL, NULL);
	}
    else return DefWindowProc (hwnd, message, wParam, lParam);
    return 0;
}
