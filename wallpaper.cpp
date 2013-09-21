
#include <gconf/gconf-client.h>

int main(int argc, char *argv[])
{
	if (argc>1 && argv[1]!=NULL)
	{
		GConfClient *conf=NULL;
		conf = gconf_client_get_default ();
		if (conf!=NULL)
		{
			GConfValue *data;
			data = gconf_value_new (GCONF_VALUE_STRING);
			if (data!=NULL)
			{
				gconf_value_set_bool (data, TRUE);
				gconf_client_set (conf, "/desktop/gnome/background/draw_background", data, NULL);
				gconf_value_set_string (data, "");
				gconf_client_set (conf, "/desktop/gnome/background/picture_filename", data, NULL);
				gconf_value_set_string (data, "centered");
				gconf_client_set (conf, "/desktop/gnome/background/picture_options", data, NULL);
				gconf_value_set_string (data, argv[1]);
				gconf_client_set (conf, "/desktop/gnome/background/picture_filename", data, NULL);
				gconf_value_free (data);
			}
			g_object_unref (conf);
		}
	}
	return 0;
}
