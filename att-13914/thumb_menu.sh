#!/bin/bash

# converts a whole directory of images into thumbnails
# the thumbnails are stored in a centralized location

# forces recreation of thumbs
recreate=0

# which extensions a convert is attempted on
# this is actually a grep regex
extensions="jpg|jpeg|png|gif|tif|tga"

# how many images to convert in parallel
# ymmv, but my machine is more efficient when converting images in parallel, 
#   especially large numbers of small images
# however, if we get several huge images, thrashing can occur
# 8 seems like a good balance
concurrency=8

# sole arg is the directory to convert
dir="$1"
if [[ ! -d "$dir" ]] ; then
    exit 1
fi

# where to store the thumbs
if [[ -d "$FVWM_USERDIR" ]] ; then
    thumb_dir="$FVWM_USERDIR/thumbs"
else
    thumb_dir="$HOME/.thumbs"
fi

if [[ ! -d "$thumb_dir/$dir" ]] ; then
    /bin/mkdir -p "$thumb_dir/$dir"
fi

num_jobs=0
cd $dir
for i in * ; do
    orig="$dir/$i"
    thumb="$thumb_dir/$dir/$i"
    if [[ ! -d "$orig" && ! -e "$thumb" && "$orig" -nt "$thumb" || $recreate != 0 ]] ; then
        # only try to convert files with the specified extensions
        if [[ -n `echo "$orig" | grep -iE "\.($extensions)$"` ]] ; then
            # kick off a convert in the background
            convert -scale 64x64 -quality 0 "$orig" png:"$thumb" >&/dev/null &
            num_jobs=$(( $num_jobs + 1 ))
        fi
        
        # to prevent too many converts taking place simutaneously
        if [[ $num_jobs -ge $concurrency ]] ; then
            wait
            num_jobs=0
        fi
    fi
done

# generate the menu
fvwm-menu-directory --dir "$dir" --links --check-subdir --func-name WallpaperBrowser \
    --command-title "StartImageViewer $dir" --command-file "SetWallpaper \"%f\"" --command-app "SetWallpaper \"%f\"" \
    --icon-dir mini.folder.xpm --icon-title mini.term.xpm --icon-file __PIXMAP__ --icon-app __PIXMAP__ \
    | sed "s%__PIXMAP__\\(.*\\)\\\"\\(.*/\\)\\(.*\\)\\\"%$thumb_dir/$dir/\\3\\1\\2\\3%g"

# clean out old thumbnails if the directory is too big
$FVWM_USERDIR/clean_thumbs.sh $thumb_dir &

exit 0

