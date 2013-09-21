Summary: A GNOME Wallpaper Desktop Manager
Name: photostorm
Version: 0.1.1
Release: fc2
URL: http://www.photostorm.net
License: GPL
ExclusiveOS: Linux
Group: Applications/Sound and Video
BuildRoot: /home/alex/programe/PhotoStorm/install/rpms
BuildArch: i386
Source: (none)
Requires: gtk+ >= 2.4

%description
PhotoStorm is a free web wallpapers comunity. PhotoStorm software
allows GNOME users to change their wallpapers, manage them, organize
them into albums.
It also provides some special features like calendar display, messages,
etc.

%prep
cd /home/alex/programe/PhotoStorm
make all
cd applet
make
cd ..
cp ps_wallpaper /home/alex/programe/PhotoStorm/install/rpms/usr/local/bin/ps_wallpaper
cp applet/ps_applet /home/alex/programe/PhotoStorm/install/rpms/usr/local/bin/ps_applet
cp photostorm /home/alex/programe/PhotoStorm/install/rpms/usr/local/bin/photostorm
cp ps_whats-new.txt /home/alex/programe/PhotoStorm/install/rpms/usr/local/lib/photostorm/whats-new.txt

%install

%preun

%files
%defattr(-,root,root)
#/usr/local/bin/xmp
#/usr/lib/linuxeyes/music/linuxeyes.mp3
#/usr/lib/linuxeyes/eq/*
#/usr/lib/linuxeyes/linuxeyes
#/usr/lib/linuxeyes/readme
%attr(0555,root,root) /usr/local/bin/ps_wallpaper
%attr(0555,root,root) /usr/local/bin/ps_applet
%attr(0555,root,root) /usr/local/bin/photostorm
%attr(0444,root,root) /usr/local/lib/photostorm/whats-new.txt
#Directoare
%attr(0555,root,root) /usr/local/lib/photostorm
%attr(0555,root,root) /usr/local/lib/photostorm/Albums
%attr(0555,root,root) /usr/local/lib/photostorm/Quotes
%attr(0555,root,root) /usr/local/lib/photostorm/fonts
%attr(0555,root,root) /usr/local/lib/photostorm/languages
%attr(0555,root,root) /usr/local/lib/photostorm/pics/picts
%attr(0555,root,root) /usr/local/lib/photostorm/pics/logos
%attr(0555,root,root) /usr/local/lib/photostorm/pics/char
%attr(0555,root,root) /usr/local/lib/photostorm/Docs
%attr(0555,root,root) /usr/local/lib/photostorm/Docs/images
%attr(0555,root,root) /usr/local/lib/photostorm/Docs/css
#Fisiere
%attr(0444,root,root) /usr/local/lib/photostorm/Albums/*
%attr(0444,root,root) /usr/local/lib/photostorm/Quotes/*
%attr(0444,root,root) /usr/local/lib/photostorm/fonts/*
%attr(0444,root,root) /usr/local/lib/photostorm/languages/*
%attr(0444,root,root) /usr/local/lib/photostorm/pics/picts/*
%attr(0444,root,root) /usr/local/lib/photostorm/pics/logos/*
%attr(0444,root,root) /usr/local/lib/photostorm/pics/char/*
%attr(0444,root,root) /usr/lib/bonobo/servers/PhotoStorm_Factory.server
%attr(0444,root,root) /usr/local/lib/photostorm/Docs/*.html
%attr(0444,root,root) /usr/local/lib/photostorm/Docs/images/*.jpg
%attr(0444,root,root) /usr/local/lib/photostorm/Docs/css/*.css



%changelog
* Sun Aug 29 2004 Alexandru Radovici <alex@photostorm.net> 0.1.1
Lost of bugfixes, but still not tested enough
* Wed Jul 07 2004 Alexandru Radovici <alex@photostorm.net> 0.1-aphla1
First RPM release
