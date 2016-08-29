/* This program doesn't do much... It also only takes one argument which
 * is the number of times to create and destory a window.  Doesn't sound
 * like much right?  Wrong, some window managers don't seem to respond
 * very well when this program is run against them.  An argument of
 * 500000 causes fvwm to allocate some 160MB of ram, and it takes a few
 * minutes to straighten its self out and be use able again.
 * Please set DISPLAY to the appropiate X session.  I suggest you only
 * run it agains Xnest, If your lost so far, stop, delete this program,
 * be happy.  In that order.
 * -- David Fries <dfries@mail.win.org>
 */

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int GotError( Display *dpy, XErrorEvent *event)
{
	char buf[10240];
	XGetErrorText( event->display, event->error_code, buf, sizeof(buf));
	fprintf(stderr, "Error received  %s\n", buf);
	exit(1);
	return 0;
}

int main( int argc, char ** argv)
{
	int fromScreen;
	Display * fromDpy;
	Window root;
	Window fromWin;
	int i=0;
	int stop;
	struct timespec req, reqs={0, 50000000};
	XSetWindowAttributes xswa = { 0 };
	if(argc!=2)
	{
		fprintf(stderr, "Wrong # arguments\n");
		return -1;
	}
	stop=atoi(argv[1]);

	XSetErrorHandler( GotError);
	fromDpy = XOpenDisplay( "" );
	if( !fromDpy )
	{
		exit(-1);
	}


	fromScreen = XDefaultScreen(fromDpy);
	/* create a window to use for grabs */
	root = XDefaultRootWindow( fromDpy );
	if( !root )
		return -1;
	xswa.override_redirect = 1;
	for( i=0; i<stop; i++)
	{
	fromWin = XCreateWindow( /*Display*/ fromDpy, /*Parent*/ root,
		/*X*/ 0,/*Y*/ 0,
		/*Width*/ 1,/*Height*/ 1, /*BorderWidth*/ 0,
		/*depth*/ CopyFromParent,
		/*Class*/ InputOutput, /*Visual*/ DefaultVisual(fromDpy,
			fromScreen),
		/*Valuemask*/ 0,//CWBackPixel | CWOverrideRedirect,
		/*XSetWindowAttributes*/ &xswa);
	//SetWindowName(fromDpy, toDpy);
	XMapWindow( fromDpy,fromWin);
	XFlush(fromDpy);
	XDestroyWindow( fromDpy,fromWin);
	XFlush(fromDpy);
	//printf("loop %d\n", i);
	req=reqs;
	nanosleep(&req, 0);
	}
	XCloseDisplay (fromDpy);

	return 0;
}

