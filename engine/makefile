COMPILER	= g++ -Wall -shared
HEADERS		= request.h role.h subject.h object.h database.h
SOURCES		= request.cpp role.cpp subject.cpp object.cpp database.cpp
TARGET		= bin/engine.dll

$(TARGET) : makefile $(SOURCES) $(HEADERS)
	$(COMPILER) $(SOURCES) -o $(TARGET)