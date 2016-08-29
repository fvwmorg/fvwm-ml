#!/bin/bash

# deletes directories from a directory tree 
#   until the disk usage of the tree is below a specified threshold

# directory to operate on
if [[ -d $0 ]] ; then 
    dir=$0
else
    dir=~/.fvwm/thumbs
fi

# if disk usage of $dir is more than this, delete the oldest stuff
threshold=$(( 20 * 2**20 ))

# check that the disk usage is indeed higher than the threshold
if [[ `/usr/bin/du -s -b $dir | cut -f 1` -le $threshold ]] ; then
    exit 0
fi

# the admitted crude method is to delete directories that have been untouched 
# for 2^($start) days, then 2^($start - 1) days, and so on until we get to 1
start=5
for i in `seq $start -1 0` ; do
    days_untouched=$(( 2 ** $i ))
    for doomed_dir in `find $dir -type d -depth -mtime +$days_untouched` ; do
        rm -rf $doomed_dir
        #echo $doomed_dir deleted
        if [[ `/usr/bin/du -s -b $dir | cut -f 1` -le $threshold ]] ; then
            exit 0
        fi
    done
done

