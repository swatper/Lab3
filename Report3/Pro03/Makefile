#Execute File
All: 
	gcc -o execute/FileCheck source/FileCheck.c
	gcc -o execute/FileCopy source/FileCopy.c
	gcc -o execute/FileSize source/FileSize.c
	gcc -o execute/StdIO source/StdIO.c
	gcc -o execute/FileDes source/FileDes.c
	gcc -o execute/DirList source/DirList.c
	gcc -o execute/DirCd source/DirCd.c

#Execute File
Execute:
	execute/DirCd ..
	
#Delete File 
Clean:
	rm execute/FileCheck
	rm execute/FileCopy
	rm execute/FileSize
	rm execute/StdIO
	rm execute/FileDes
	rm execute/DirList
	rm execute/DirCd


#Test Codes
# execute/FileCheck source/FileCheck.c
# execute/FileCheck source/FileCheck2.c
#-------------------------------------------------------
# execute/FileCopy source/FileCopy.c source/CopyDest.c
# diff source/FileCopy.c source/CopyDest.c
#-------------------------------------------------------
# execute/FileSize source/FileSize.c
#-------------------------------------------------------
# execute/StdIO
#-------------------------------------------------------
# execute/FileDes source/Sample1.txt
# cat source/Sample1.txt
#-------------------------------------------------------
# execute/DirList /home/swatper/OpenSource_Report/
#-------------------------------------------------------

