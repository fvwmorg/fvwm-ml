#!/bin/sh

if [ ! $# = 8 ]; then
	exit 1
fi

if [ $[ $1 < $5 ] = 1 ]; then
	SX=$[ $1 - $5 ]
elif [ $[ $1 + $3 > $5 + $7 ] = 1 ]; then
	SX=$[ $1 + $3 - $5 - $7 ]
else
	SX=0
fi

if [ $[ $2 < $6 ] = 1 ]; then
	SY=$[ $2 - $6 ]
elif [ $[ $2 + $4 > $6 + $8 ] = 1 ]; then
	SY=$[ $2 + $4 - $6 - $8 ]
else
	SY=0
fi

if [ ! "$SX$SY" = 00 ]; then
	echo "scroll ${SX}p ${SY}p"
fi

