#/bin/bash


fileLocalUtil="tintaCommon/src/tintaUtilLocalEn.h"
fileLocalShape="tintaShape/src/tintaShapeLocalEn.h"
fileLocalPix="tintaPix/src/tintaFunctions.h"

copy() {
	#mkdir 
	
	destPath="doc"
	#echo $destPath
	if [ ! -d $destPath ] ; then
		mkdir $destPath		
	fi
	cp "$fileLocalUtil" $destPath
	cp "$fileLocalShape" $destPath
	cp "$fileLocalPix" $destPath
	echo 'done'
}

copy