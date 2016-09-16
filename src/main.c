#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "copy.h"
#include "ldd.h"
#include "libraries.h"

#ifndef STRING_LANG
 #define STRING_LANG 300
#endif // STRING_LANG


void usage(const char *argv_0)
{
    fprintf(stderr,
		"Usage: %s PATH_TO_PROGRAM ROOT_PATH\n\n"
		"Description:"
		"\n\tLdcopy is a small program to copy (tree) all shared "
			"library "
		"\n\tinto destination directory ROOT_PATH. If in destination "
		"\n\tdirectory existing another version of library, then don't "
		"\n\tcopy, but create symbolic link. existing library."
		"\n"
		"\nArguments:"
		"\n\tPATH_TO_PROGRAM:"
		"\n\t   - Full path to binary program, where will copy library"
		"\n"
		"\n"
		"\tROOT_PATH:"
		"\n\t   - Root path, where will then store library"
		"\n\t   - If no exist, then will created."
		"\n"
		"\nVersion:"
		"\n\t" PACKAGE_VERSION
		"\n"
		"\nExample:\n"
		"\t%s /bin/bash /tmp/"
		"\n"
		"\n"
		"Contact:\n"
		"\t" PACKAGE_BUGREPORT "\n",

            argv_0,argv_0,argv_0);
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		usage(argv[0]);
		return;
	}
	
	char *path;
	int lenght;
	t_libraries *libraries;
	
	libraries=ldd_get(argv[1]);
	if(libraries == NULL)
		return 1;
		
	/* if argv[2] dont have on end '/', then appeding */
	lenght=strlen(argv[2]);
	if(argv[2][lenght-1] != '/')
	{
		path=(char *) malloc(lenght+1);
		if(path == NULL)
			return 1;
		strncpy(path, argv[2], lenght);
		path[lenght]='/';
		path[lenght+1]='\0';
		
		copy(libraries, path);
		free(path);
	}
	else
		copy(libraries, argv[2]);
	
	libraries_free(libraries);
    return 0;
}
