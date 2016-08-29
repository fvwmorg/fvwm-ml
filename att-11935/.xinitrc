#!/bin/sh
# $XConsortium: xinitrc.cpp,v 1.4 91/08/22 11:41:34 rws Exp $

userresources=$HOME/.Xresources
usermodmap=$HOME/.Xmodmap
sysresources=/usr/X11R6/lib/X11/xinit/.Xresources
sysmodmap=/usr/X11R6/lib/X11/xinit/.Xmodmap

# merge in defaults and keymaps

if [ -f $sysresources ]; then
    xrdb -merge $sysresources
fi

if [ -f $sysmodmap ]; then
    xmodmap $sysmodmap
fi

if [ -f $userresources ]; then
    xrdb -merge $userresources
fi

if [ -f $usermodmap ]; then
    xmodmap $usermodmap
fi

rm -f ${HOME}/.netscape/lock
# start some nice programs
#xmodmap ${HOME}/.xmodmap-`uname -n`
# Mouse accel/start
xset m 3.6 2
# Keyboard delay/repeat
xset r rate 300 25
# Screen saver on
xset s blank
# Screen saver blank after 15min
xset s 1800
# Enable DPMS
xset +dpms
# DPMS standby/suspend/off
xset dpms 1800 3600 7200
# xset b off
xrdb -merge ${HOME}/.Xdefaults
rm -f ${HOME}/.licq/licq.pid
if [ `hostname -s` == "jimblewix" ];
then
	fvwm2 -f ${HOME}/.fvwm2rc-jim
else
	fvwm2
fi

