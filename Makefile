syncam: syncam.c
	gcc -Wall -o syncam syncam.c `pkg-config --cflags --libs gstreamer-1.0`