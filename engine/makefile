COMPILER	= g++ -Wall -shared
HEADERS		= request.h role.h subject.h object.h
SOURCES		= request.cpp role.cpp subject.cpp object.cpp
TARGET		= bin/engine.dll

$(TARGET) : makefile $(SOURCES) $(HEADERS)
	$(COMPILER) $(SOURCES) -o $(TARGET)