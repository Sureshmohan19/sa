// runtime.c
#include <stdio.h>

// This is the implementation of the 'print' function that your compiler can call.
void print(const char* message) {
    printf("%s\n", message);
}