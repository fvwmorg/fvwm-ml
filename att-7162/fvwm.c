/****************************************************************************
 * This module is based on Twm, but has been siginificantly modified 
 * by Rob Nation (nation@rocket.sanders.lockheed.com)
 ****************************************************************************/
/*****************************************************************************/
/**       Copyright 1988 by Evans & Sutherland Computer Corporation,        **/
/**                          Salt Lake City, Utah                           **/
/**  Portions Copyright 1989 by the Massachusetts Institute of Technology   **/
/**                        Cambridge, Massachusetts                         **/
/**                                                                         **/
/**                           All Rights Reserved                           **/
/**                                                                         **/
/**    Permission to use, copy, modify, and distribute this software and    **/
/**    its documentation  for  any  purpose  and  without  fee is hereby    **/
/**    granted, provided that the above copyright notice appear  in  all    **/
/**    copies and that both  that  copyright  notice  and  this  permis-    **/
/**    sion  notice appear in supporting  documentation,  and  that  the    **/
/**    names of Evans & Sutherland and M.I.T. not be used in advertising    **/
/**    in publicity pertaining to distribution of the  software  without    **/
/**    specific, written prior permission.                                  **/
/**                                                                         **/
/**    EVANS & SUTHERLAND AND M.I.T. DISCLAIM ALL WARRANTIES WITH REGARD    **/
/**    TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES  OF  MERCHANT-    **/
/**    ABILITY  AND  FITNESS,  IN  NO  EVENT SHALL EVANS & SUTHERLAND OR    **/
/**    M.I.T. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL  DAM-    **/
/**    AGES OR  ANY DAMAGES WHATSOEVER  RESULTING FROM LOSS OF USE, DATA    **/
/**    OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER    **/
/**    TORTIOUS ACTION, ARISING OUT OF OR IN  CONNECTION  WITH  THE  USE    **/
/**    OR PERFORMANCE OF THIS SOFTWARE.                                     **/
/*****************************************************************************/


/***********************************************************************
 * fvwm - "Feeble Virtual Window Manager"
 ***********************************************************************/

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include "fvwm.h"
#include "misc.h"
#include "menus.h"
#include "screen.h"
#include <X11/Xproto.h>
#include <X11/Xatom.h>

ScreenInfo Scr;		        /* structures for the screen */
Display *dpy;			/* which display are we talking to */
Window ResizeWindow;		/* the window we are resizing */

static int CatchRedirectError();	/* for settting RedirectError */
static int FvwmErrorHandler();
void newhandler(int sig);
void CreateCursors();

XContext FvwmContext;		/* context for fvwm windows */
XContext MenuContext;		/* context for all menu windows */

XClassHint NoClass;		/* for applications with no class */

XGCValues Gcv;

int JunkX = 0, JunkY = 0;
Window JunkRoot, JunkChild;		/* junk window */
unsigned int JunkWidth, JunkHeight, JunkBW, JunkDepth, JunkMask;

#define gray_width 2
#define gray_height 2
static char gray_bits[] = {0x02, 0x01};

#define light_gray_width 3
#define light_gray_height 3
static char light_gray_bits[] = {0x04, 0x02, 0x01};
Bool debugging = False;

#define USAGE "Fvwm: Feeble Virtual Window Manager Version 0.5\n\nusage:  fvwm [-d dpy] [-debug]\n"

/***********************************************************************
 *
 *  Procedure:
 *	main - start of fvwm
 *
 ***********************************************************************
 */

void main(int argc, char **argv)
{
    Window root, parent, *children;
    unsigned int nchildren;
    int i, j;
    char *display_name = NULL;
    unsigned long valuemask;	/* mask for create windows */
    XSetWindowAttributes attributes;	/* attributes for create windows */
    extern XEvent Event;
    void InternUsefulAtoms ();
    void InitVariables();

    for (i = 1; i < argc; i++) 
      {
	if (strncasecmp(argv[i],"-debug",6)==0)
	  debugging = True;
	else if (strncasecmp(argv[i],"-d",2)==0)
	  {
	    if (++i >= argc)
	      fprintf (stderr,USAGE);	      
	    display_name = argv[i];
	  }
	else 
	  fprintf (stderr,USAGE);
      }

    newhandler (SIGINT);
    newhandler (SIGHUP);
    newhandler (SIGQUIT);
    newhandler (SIGTERM);

    if (!(dpy = XOpenDisplay(display_name))) 
      {
	fprintf (stderr, "fvwm:  unable to open display \"%s\"\n",
		 XDisplayName(display_name));
	exit (1);
      }
    
    if (fcntl(ConnectionNumber(dpy), F_SETFD, 1) == -1) 
      {
	fprintf(stderr,"fvwm: can't mark display connection close-on-exec\n");
	exit (1);
      }
    
    Scr.screen = DefaultScreen(dpy);

    InternUsefulAtoms ();

    /* Make sure property priority colors is empty */
    XChangeProperty (dpy, RootWindow(dpy,Scr.screen), _XA_MIT_PRIORITY_COLORS,
		     XA_CARDINAL, 32, PropModeReplace, NULL, 0);

    XSetErrorHandler(CatchRedirectError);
    XSelectInput(dpy, RootWindow (dpy, Scr.screen),
		 ColormapChangeMask | EnterWindowMask | PropertyChangeMask | 
		 SubstructureRedirectMask | KeyPressMask |
		 ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XSync(dpy, 0);

    XSetErrorHandler(FvwmErrorHandler);
    XSetInputFocus (dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
    CreateCursors();
    InitVariables();
    /* read config file, set up menus, colors, fonts */
    MakeMenus();
    Scr.TitleHeight = Scr.StdFont.height+2;
    Scr.gray = 
      XCreateBitmapFromData(dpy, Scr.Root, gray_bits, gray_width, gray_height);
    Scr.border_gray = 
      XCreatePixmapFromBitmapData(dpy, Scr.Root, gray_bits, gray_width, 
				  gray_height,Scr.StdColors.fore, 
				  Scr.StdColors.back,Scr.d_depth);
    Scr.light_gray = 
      XCreateBitmapFromData(dpy, Scr.Root, light_gray_bits, light_gray_width, 
			    light_gray_height);
    XGrabServer(dpy);
    XSync(dpy, 0);
    if(debugging)
      XSynchronize(dpy,1);
    XQueryTree(dpy, Scr.Root, &root, &parent, &children, &nchildren);
    /*
     * weed out icon windows
     */
    for (i = 0; i < nchildren; i++) 
      {
	if (children[i]) 
	  {
	    XWMHints *wmhintsp = XGetWMHints (dpy, children[i]);
	    if (wmhintsp) 
	      {
		if (wmhintsp->flags & IconWindowHint) 
		  {
		    for (j = 0; j < nchildren; j++) 
		      {
			if (children[j] == wmhintsp->icon_window) 
			  {
			    children[j] = None;
			    break;
			  }
		      }
		  }
		XFree ((char *) wmhintsp);
	      }
	  }
      }
    /*
     * map all of the non-override windows
     */
    for (i = 0; i < nchildren; i++)
      {
	if (children[i] && MappedNotOverride(children[i]))
	  {
	    XUnmapWindow(dpy, children[i]);
	    Event.xmaprequest.window = children[i];
	    HandleMapRequest ();
	  }
      }
    Scr.SizeStringWidth = XTextWidth (Scr.StdFont.font,
				      " 8888 x 8888 ", 13);
    attributes.border_pixel = Scr.StdColors.fore;
    attributes.background_pixel = Scr.StdColors.back;
    attributes.bit_gravity = NorthWestGravity;
    valuemask = (CWBorderPixel | CWBackPixel | CWBitGravity);
    Scr.SizeWindow = XCreateWindow (dpy, Scr.Root, 0, 0, 
				     (unsigned int) Scr.SizeStringWidth,
				     (unsigned int) (Scr.StdFont.height +
						     SIZE_VINDENT*2),
				     (unsigned int) BW, 0,
				     (unsigned int) CopyFromParent,
				     (Visual *) CopyFromParent,
				     valuemask, &attributes);
    XUngrabServer(dpy);
    InitEvents();
    HandleEvents();
    return;
}

/***********************************************************************
 *
 *  Procedure:
 *	InternUsefulAtoms:
 *            Dont really know what it does
 *
 ***********************************************************************
 */
Atom _XA_MIT_PRIORITY_COLORS;
Atom _XA_WM_CHANGE_STATE;
Atom _XA_WM_STATE;
Atom _XA_WM_COLORMAP_WINDOWS;
Atom _XA_WM_PROTOCOLS;
Atom _XA_WM_TAKE_FOCUS;
Atom _XA_WM_SAVE_YOURSELF;
Atom _XA_WM_DELETE_WINDOW;

void InternUsefulAtoms ()
{
  /* 
   * Create priority colors if necessary.
   */
  _XA_MIT_PRIORITY_COLORS = XInternAtom(dpy, "_MIT_PRIORITY_COLORS", False);   
  _XA_WM_CHANGE_STATE = XInternAtom (dpy, "WM_CHANGE_STATE", False);
  _XA_WM_STATE = XInternAtom (dpy, "WM_STATE", False);
  _XA_WM_COLORMAP_WINDOWS = XInternAtom (dpy, "WM_COLORMAP_WINDOWS", False);
  _XA_WM_PROTOCOLS = XInternAtom (dpy, "WM_PROTOCOLS", False);
  _XA_WM_TAKE_FOCUS = XInternAtom (dpy, "WM_TAKE_FOCUS", False);
  _XA_WM_SAVE_YOURSELF = XInternAtom (dpy, "WM_SAVE_YOURSELF", False);
  _XA_WM_DELETE_WINDOW = XInternAtom (dpy, "WM_DELETE_WINDOW", False);
  return;
}

/***********************************************************************
 *
 *  Procedure:
 *	newhandler: Installs new signal handler
 *
 ***********************************************************************
 */
void newhandler(int sig)
{
  if (signal (sig, SIG_IGN) != SIG_IGN)
    signal (sig, Done);
}

/***********************************************************************
 *
 *  Procedure:
 *	CreateCursors - Loads fvwm cursors
 *
 ***********************************************************************
 */
void CreateCursors()
{
  /* define cursors */
  Scr.PositionCursor = XCreateFontCursor(dpy,XC_top_left_corner);
  Scr.RightButt  = XCreateFontCursor(dpy,XC_rightbutton);;
  Scr.LeftButt = XCreateFontCursor(dpy, XC_leftbutton);
  Scr.MiddleButt = XCreateFontCursor(dpy, XC_middlebutton);
  Scr.FrameCursor = XCreateFontCursor(dpy, XC_top_left_arrow);
  Scr.TitleCursor = XCreateFontCursor(dpy, XC_top_left_arrow);
  Scr.IconCursor = XCreateFontCursor(dpy, XC_top_left_arrow);
  Scr.MoveCursor = XCreateFontCursor(dpy, XC_fleur);
  Scr.ResizeCursor = XCreateFontCursor(dpy, XC_fleur);
  Scr.MenuCursor = XCreateFontCursor(dpy, XC_sb_left_arrow);
  Scr.WaitCursor = XCreateFontCursor(dpy, XC_watch);
  Scr.SelectCursor = XCreateFontCursor(dpy, XC_dot);
  Scr.DestroyCursor = XCreateFontCursor(dpy, XC_pirate);
}

/***********************************************************************
 *
 *  Procedure:
 *	InitVariables - initialize fvwm variables
 *
 ***********************************************************************
 */

void InitVariables()
{
  FvwmContext = XUniqueContext();
  MenuContext = XUniqueContext();
  NoClass.res_name = NoName;
  NoClass.res_class = NoName;

  Scr.d_depth = DefaultDepth(dpy, 0);
  Scr.d_visual = DefaultVisual(dpy, 0);
  Scr.Root = RootWindow(dpy, 0);
  Scr.root_pushes = 0;
  Scr.pushed_window = &Scr.FvwmRoot;

  Scr.MyDisplayWidth = DisplayWidth(dpy, 0);
  Scr.MyDisplayHeight = DisplayHeight(dpy, 0);
  Scr.MaxWindowWidth = 32767 - Scr.MyDisplayWidth;
  Scr.MaxWindowHeight = 32767 - Scr.MyDisplayHeight;
    
  Scr.SizeStringOffset = 0;
  Scr.BorderWidth = BW;
  Scr.IconBorderWidth = NOFRAME_BW;
  Scr.NoBorderWidth = NOFRAME_BW;
  Scr.BoundaryWidth = BOUNDARY_WIDTH;
  Scr.CornerWidth = CORNER_WIDTH;
  Scr.Focus = NULL;
  
  Scr.StdFont.font = NULL;
  Scr.StdFont.name = "fixed";

  Scr.VxMax = 2*Scr.MyDisplayWidth;
  Scr.VyMax = 2*Scr.MyDisplayHeight;
  Scr.Vx = 0;
  Scr.Vy = 0;

  return;
}



/***********************************************************************
 *
 *  Procedure:
 *	Reborder - Removes fvwm border windows
 *
 ***********************************************************************
 */

void Reborder()
{
  FvwmWindow *tmp;			/* temp fvwm window structure */
  /* put a border back around all windows */
  
  XGrabServer (dpy);
  
  InstallWindowColormaps (0, &Scr.FvwmRoot);	/* force reinstall */
  for (tmp = Scr.FvwmRoot.next; tmp != NULL; tmp = tmp->next)
    {
      RestoreWithdrawnLocation (tmp); 
      XMapWindow (dpy, tmp->w);
    }
  XUngrabServer (dpy);
  XSetInputFocus (dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
}

/***********************************************************************
 *
 *  Procedure:
 *	Done - cleanup and exit fvwm
 *
 ***********************************************************************
 */
SIGNAL_T Done()
{
  Reborder ();
  XCloseDisplay(dpy);
  exit(0);
  SIGNAL_RETURN;
}

/***********************************************************************
 *
 *  Procedure:
 *	CatchRedirectError - Figures out if there's another WM running
 *
 ***********************************************************************
 */
static int CatchRedirectError(Display *dpy, XErrorEvent event)
{
  fprintf (stderr, "fvwm: another window manager is already running\n");
  exit(1);
  return 0;
}


/***********************************************************************
 *
 *  Procedure:
 *	FvwmErrorHandler - displays info on internal errors
 *
 ***********************************************************************
 */
static int FvwmErrorHandler(Display *dpy, XErrorEvent event)
{
  extern int move_on, resize_on,menu_on,unhighlight;
  extern Window last_event_window;
  extern FvwmWindow *last_event_Fvwmwindow;
  extern int last_event_type;
  extern int level;

  fprintf(stderr,"fvwm: internal error\n");
  fprintf(stderr,"     Handler was ");
  switch(last_event_type)
    {
    case Expose:
      fprintf(stderr, "HandleExpose");
      break;
    case DestroyNotify:
      fprintf(stderr, "HandleDestroyNotify");
      break;
    case MapRequest:
      fprintf(stderr, "HandleMapRequest");
      break;
    case MapNotify:
      fprintf(stderr,"HandleMapNotify");
      break;
    case UnmapNotify:
      fprintf(stderr,"HandleUnmapNotify");
      break;
    case MotionNotify:
      fprintf(stderr,"HandleMotionNotify");
      break;
    case ButtonRelease:
      fprintf(stderr,"HandleButtonRelease");
      break;
    case ButtonPress:
      fprintf(stderr,"HandleButtonPress");
      break;
    case EnterNotify:
      fprintf(stderr,"HandleEnterNotify, level = %d",level);
      break;
    case LeaveNotify:
      fprintf(stderr,"HandleLeaveNotify");
      break;
    case ConfigureRequest:
      fprintf(stderr,"HandleConfigureRequest");
      break;
    case ClientMessage:
      fprintf(stderr,"HandleClientMessage");
      break;
    case PropertyNotify:
      fprintf(stderr,"HandlePropertyNotify");
      break;
    case KeyPress:
      fprintf(stderr,"HandleKeyPress");
      break;
    default:
      fprintf(stderr,"Unknown event %d",last_event_type);
      break;
    }
  fprintf(stderr,"\n     In FvwmWindow 0x%x Xwindow 0x%x\n",
	  last_event_Fvwmwindow,last_event_window);
  if(menu_on)
    fprintf(stderr,"   A menu was active\n");
  if(resize_on)
    fprintf(stderr,"   A window was being resized %d\n",resize_on);
  if(move_on)
    fprintf(stderr,"   A window was being moved\n");
  if(unhighlight)
    fprintf(stderr,"   A window was being unhighlighted\n");
  return 0;
}


