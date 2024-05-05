#pragma once

#include <glad/glad.h>

#include <csignal>
#include <iostream>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
//* This funciton is used to check for an error in any of our functions and then print the line, the file, and the 
//* It will breakpoint whenever there is an error and quit the code and tell me where and stuff.
#define GLCall(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); \
} while (false)

void GLClearError();
bool GLLogCall(const char* fn, const char* file, int line);