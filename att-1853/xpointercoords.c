#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <string.h>

#define NW 0 /* northwest gravity */
#define NE 1 /* northeast gravity */
#define SW 2 /* southwest gravity */
#define SE 3 /* southeast gravity */

Display *disp;
int screen_num;

static char *progname;

main(int argc, char **argv)
{
  int x,y,a,b,c;
  int dw, dh;
  int rel = NW;
  Window ch;
  Window ro;

  progname = argv[0];

  if (argc > 2) {
    fprintf(stderr,"Usage: %s -nw|-ne|-sw|-se\n",progname);
    exit(1);
  }

  if (argc == 2) {
    if (strcmp(argv[1],"-nw") == 0)
      rel = NW;
    else if (strcmp(argv[1],"-ne") == 0)
      rel = NE;
    else if (strcmp(argv[1],"-sw") == 0)
      rel = SW;
    else if (strcmp(argv[1],"-se") == 0)
      rel = SE;
    else {
      fprintf(stderr,"Usage: %s -nw|-ne|-sw|-se\n",progname);
      exit(1);
    }
  }
    
  if ((disp = XOpenDisplay(NULL)) == NULL) {
    fprintf(stderr,"Problem opening display\n");
    exit(1);
  }
  screen_num = DefaultScreen(disp);
  dw=DisplayWidth(disp,screen_num);
  dh=DisplayHeight(disp,screen_num);
  (void) XQueryPointer(disp, RootWindow(disp,screen_num), &ro, &ch,
                       &x, &y, &a, &b, &c);
  switch (rel) {
   case NW:
    break;
   case NE:
    x -= dw;
    break;
   case SW:
    y -= dh;
    break;
   case SE:
    x -= dw;
    y -= dh;
  }
  printf("%+d%+d\n",x,y);
}

