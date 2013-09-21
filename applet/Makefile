
PS_OBJS	=	*.o

ps_applet:ps_applet.cpp
	g++  ps_applet.cpp $(PS_OBJS) -lz `pkg-config --cflags --libs libpanelapplet-2.0 gtk+-2.0` -o ps_applet

applet:data.h ps_applet.cpp album.h album.cpp photo.h misc.h configs.h configs.cpp ps_linux.h ps_linux.cpp ps_system.h data.h
	g++ -c album.cpp
	g++ -c misc.cpp
	g++ -c ps_linux.cpp
	g++ -c configs.cpp
	g++  ps_applet.cpp $(PS_OBJS) -lz `pkg-config --cflags --libs libpanelapplet-2.0 gtk+-2.0` -o ps_applet
