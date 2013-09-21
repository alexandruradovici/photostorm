#!/bin/sh
echo "Welcome to PhotoStorm v0.1"
echo  ""
echo "PhotoStorm will be installed in /usr/local"
echo "Installing binaries"
tar xvfz photostorm-bin-0.1.tgz -C /usr/local/bin > /dev/null
echo "Installing data"
tar xvfz photostorm-data-0.1.tgz -C /usr/local/lib > /dev/null
echo "Configuring"
cp /usr/local/lib/photostorm/bonobo_data/PhotoStorm_Factory.server /usr/lib/bonobo/servers > /dev/null
echo "PhotoStorm is now installed"
echo "---------------"
echo "Restart the panel"
echo "Load the applet from Add to panel -> Multimedia -> PhotoStorm"
echo "Thank you for using PhotoStorm"