#menuconvert.sh
#----Menu converting script:
#by: Ahmed Abdalla
#!/bin/sh
echo RENAMED.28
exit
#!/bin/bash
#
# Creates converter() function
converter()
{
#This changes the current directory to the fonts directory.
cd $1  ###This function now requires a starting folder as input.
#This removes old versions of the icons in case there have been changes.
rm -f menu.* sidemenu.*
#Sets the variable flist to the list of files in the icon folder
flist="*"
#Starts a loop for all files in the directory.
for file in $flist
do
# Tests whether the file is a directory
if [ -d $file ]; then 
# if test returns true, changes to directory
cd $file
# runs converter function on current directory
converter ./
# goes back to previous directory when convert, and 
# subsequent recursive converts finish
cd ..
# if it's not a directory...
else
#These two lines create two sets of icons, one set (20x20)
# for the root menu, and the other set for my side menus (40x40)
convert -scale 20x20 ${file} menu.${file}
convert -scale 40x40 ${file} sidemenu.${file}
fi
#Closes the for loop
done
#Lists all the files in the icon folders.  This line is optional
# and does not effect program function
ls $1
# Prints "done" to stdout.  This line is optional and does not 
# effect program function
echo done
}
# this calls the converter function on the input directory.
converter $1

###  EOF

