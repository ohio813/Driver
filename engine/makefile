COMPILER	= g++ -Wall -shared
HEADERS		= request.h role.h subject.h object.h database.h MD5lib.h
SOURCES		= request.cpp role.cpp subject.cpp object.cpp database.cpp MD5lib.c
TARGET		= bin/engine.dll

$(TARGET) : makefile $(SOURCES) $(HEADERS)
	$(COMPILER) $(SOURCES) -o $(TARGET)