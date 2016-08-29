#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

Display *dpy;
Screen *scr;
Window root_win;
int scr_width, scr_height;
int depth;

XSizeHints hint;
XSetWindowAttributes attrib;

Window win;
int win_width, win_height;

int main(int argc, char **argv)
{
  XEvent ev;
  int i, j;
  int x;
  
  
  dpy = XOpenDisplay(":0.0");
  scr = DefaultScreenOfDisplay(dpy);
  root_win = RootWindowOfScreen(scr);
  scr_height = XHeightOfScreen(scr);
  scr_width = XWidthOfScreen(scr);
  depth = DefaultDepth(dpy, 0);

  win_width = 100;
  win_height = 100;

  win = XCreateWindow(dpy, root_win, 500, 500, win_width, win_height, 0, CopyFromParent, InputOutput,
                      CopyFromParent, 0, &attrib);
  XMapWindow(dpy, win);
  XFlush(dpy);
  sleep(1);
  
  XUnmapWindow(dpy, win);
  XFlush(dpy);
//  sleep(1); // uncomment to make it work
  XMapWindow(dpy, win);
  XFlush(dpy);

  sleep(5);

  XCloseDisplay(dpy);
  return 0;
}

