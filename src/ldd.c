/*
 * Run ldd and return output of ldd as structure libraries
 */
#include <stdio.h>
#include "libraries.h"

#ifndef MAX_LIBRARIES
  #define MAX_LIBRARIES 50000
#endif

#ifndef BUFFER_SIZE
  #define BUFFER_SIZE 50000
#endif

/* Return:
 * 		1 - error
 * 		0 - ok
 * Parameters:
 * 		program - relative or full path
 * 		root_path - full path
 */
t_libraries *ldd_get(char *program)
{
	char buffer[BUFFER_SIZE];
	int i;
	int name_i_start;
	int name_i_end;
	int path_i_start;
	int path_i_end;
	t_libraries *libraries;
	FILE *fprogram;
	
	if(program == NULL)
		goto error;
	
	snprintf(buffer, 999, "ldd '%s'", program);
	
	fprogram=popen(buffer, "r");
	if(fprogram == NULL)
		goto error;
	
	libraries=libraries_init(MAX_LIBRARIES);
	if(libraries == NULL)
		goto error;
	
	int have=0;
	while(1)
	{
		int ret=fread(buffer+have, 1, 1, fprogram);
		if(ret == 0)
			break;
		have++;
		if(have == BUFFER_SIZE)
			goto error;
	}
	
	pclose(fprogram);
	i=0;
	
	while(1)
	{
		if(buffer[i] == '\0')
			break;
		if(buffer[i] != '\t')
			goto error;
		i++;
		
		/* check library in format: "\t/lib/ld-linux.so.2\n" */
		if(buffer[i] == '/')
		{
			// first parameter is none
			name_i_start=-1;
			goto path_library;
		}
		name_i_start=i;
		for(; i<BUFFER_SIZE && buffer[i]!=' ' && buffer[i] != '\0';i++);
		name_i_end=i;
		
		i++;
		if(buffer[i]!='=')
		{
			/* is static library */
			if(libraries_add(libraries, buffer+name_i_start, NULL,
									name_i_end-name_i_start, 0))
				goto error;
		}
		else
		{
			/* is shared library */
			i+=3;
path_library:
			path_i_start=i;
			for(; i<BUFFER_SIZE && buffer[i]!=' ' && buffer[i] != '\0';i++);
			path_i_end=i;
			//printf("line %d\n", __LINE__);
			if(name_i_start==-1)
			{
				if(libraries_add(libraries,
							NULL, buffer+path_i_start,
							0, path_i_end-path_i_start))
					goto error;
			}
			else
				if(libraries_add(libraries,
							buffer+name_i_start,
							buffer+path_i_start,
							name_i_end-name_i_start,
							path_i_end-path_i_start))
					goto error;
		}
		/* read chars to end line */
		for(; i<BUFFER_SIZE && buffer[i]!='\n' && buffer[i]!='\0';i++);
		if(buffer[i] == '\0')
			break;
		i++;
	}
	
	return libraries;
error:
	fputs("Errror!! I don't get shared library.\n", stderr);
	return NULL;
}
