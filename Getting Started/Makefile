CXX = g++
CFLAGS = -std=c++20 -g -Wall -fdiagnostics-color=always
TARGET = main
SOURCES = main.cpp
C-SOURCE = /Users/noahdujovny/Documents/OpenGL/dependencies/glad.c

INC = -I/Users/noahdujovny/Documents/OpenGL/dependencies/include
LIB = -L/Users/noahdujovny/Documents/OpenGL/dependencies/library
MR_INC = /Users/noahdujovny/Documents/OpenGL/dependencies/library/libglfw.3.3.dylib

EXECUTABLE = app
FRAMEWORK = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated

default: 
	$(CXX) $(CFLAGS) $(INC) $(LIB) $(MR_INC) $(SOURCES) $(C-SOURCE) -o $(EXECUTABLE) $(FRAMEWORK)

.PHONY: clean
clean:
	rm -f app