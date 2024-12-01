#Create Execute File
All: 
	gcc -o execute/Lock1 source/Recordlock.c
	gcc -o execute/Map source/Memmap.c
	gcc -o execute/Pipe source/Pipe.c
	gcc -o execute/Nonpipe1 source/NonBlockpipe1.c
	gcc -o execute/Nonpipe2 source/NonBlockpipe2.c
	gcc -o execute/StdPipe source/StdPipe.c
	gcc -o execute/Read source/ReadPipe.c
	gcc -o execute/Write source/WritePipe.c


#Execute File
Execute:
	execute/Read &
	ls -la
	execute/Write Hello World!

#Delete File 
Clean:
	rm execute/Lock1
	rm execute/Map
	rm execute/Pipe
	rm execute/Nonpipe1
	rm execute/Nonpipe2
	rm execute/StdPipe
	rm execute/Write
	rm execute/Read
	rm fifo

#Test Codes
#-------------------------------------------------------
#execute/Lock1
#-------------------------------------------------------
#execute/Map
#-------------------------------------------------------
#execute/Pipe
#-------------------------------------------------------
#execute/Nonpipe1
#-------------------------------------------------------
#execute/Nonpipe2
#-------------------------------------------------------
#execute/StdPipe
#-------------------------------------------------------
