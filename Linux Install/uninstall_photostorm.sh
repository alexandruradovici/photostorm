#!/bin/sh
echo "PhotoStorm will be uninstalled in /usr/local"
echo "uninstalling binaries"
rm /usr/local/bin/photostorm
rm /usr/local/bin/ps_applet
rm /usr/local/bin/ps_wallpaper
echo "Installing data"
rm /usr/lib/bonobo/servers/PhotoStorm_Factory.server
rm -r /usr/local/lib/photostorm
echo "PhotoStorm is now uninstalled"
echo ""
