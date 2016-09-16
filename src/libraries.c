/*
 * Data structure for save data - name and path to shared library  
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEF_COUNT_MAX_STRING
 #define DEF_COUNT_MAX_STRING 1000
#endif

typedef struct{
	char **name;
	char **path;
	int count_alloc; // count allocated
	int have;
} t_libraries;

/* pointer to structure, or on error return false */
t_libraries *libraries_init(int max_count)
{
	t_libraries *libraries;
	
	libraries=malloc(sizeof(t_libraries));
	if(libraries == NULL)
		return NULL;

	libraries->name=NULL;
	libraries->path=NULL;
	libraries->count_alloc=0;
	libraries->have=0;

	libraries->name=malloc(DEF_COUNT_MAX_STRING*sizeof(char **));
	if(libraries->name == NULL)
	{
		free(libraries);
		return NULL;
	}

	libraries->path=malloc(DEF_COUNT_MAX_STRING*sizeof(char **));
	if(libraries->path == NULL)
	{
		free(libraries->name);
		free(libraries);
		return NULL;
	}

	libraries->count_alloc=DEF_COUNT_MAX_STRING;
	return libraries;
}

/* ok return 0, error return 1 */
int libraries_add(t_libraries *libraries, char *name, char *path,
							size_t longth_name, size_t longth_path)
{
	if(libraries->count_alloc == libraries->have)
	{
		fprintf(stderr,
		"Error: Items is max. Run with parameter -max COUNT set more as 1000.\n");
		return 1;
	}

	if(name == NULL)
		libraries->name[libraries->have]=NULL;
	else
		libraries->name[libraries->have]=strndup(name, longth_name);

	if(path == NULL)
		libraries->path[libraries->have]=NULL;
	else
		libraries->path[libraries->have]=strndup(path, longth_path);
	libraries->have++;

	/* ok, return */
	return 0;
}

void libraries_free(t_libraries *libraries)
{
	int i;
	char **p=NULL;

	if(libraries == NULL)
		return;
	p=(char **) libraries->name;
	if(libraries->name != NULL)
		for(i=0; i<libraries->have;i++,p++)
			if(*p != NULL)
			{
				free(*p);
				*p=NULL;
			}
	p=libraries->path;
	if(libraries->path != NULL)
		for(i=0; i<libraries->have;i++,p++)
			if(*p != NULL)
			{
				free(*p);
				*p=NULL;
			}
	libraries->count_alloc=0;
	libraries->have=0;
}
