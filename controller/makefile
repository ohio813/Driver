default : main.cpp main.h makefile
	make -r bin/controller.exe
	
bin/controller.exe : main.cpp main.h makefile
	if NOT EXIST bin mkdir bin
	g++ -Wall -g main.cpp -o bin/controller.exe
	