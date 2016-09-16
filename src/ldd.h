/*
 * Run ldd and return output of ldd as structure libraries
 */
#ifndef _LDD_H
 #define _LDD_H
 #include "libraries.h"

/* 1 - error, 0 - ok */
extern t_libraries *ldd_get(char *program);
#endif
