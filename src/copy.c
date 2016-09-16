#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "ldd.h"

#ifndef BUFFER_COPY_SIZE
 #define BUFFER_COPY_SIZE 500
#endif

static int is_number(char *s, int max)
{
	if(s == NULL)
		return 0;
	char *p=s;
	
	while(*p >= '0' && *p <='9')
		p++;

	if(*p == '\0' ||
	   *p == '\n' ||
	   *p == '-' ||
	   *p == '.' ||
	   *p == ' ')
	   return 1;
	return 0;
}

/* return path on file, or NULL
 * 
 * Return: pointer to static char name_lib[500]
 */
static char *get_name_library(char *name)
{
	static char name_lib[500];
	int i;
	int lenght;
	
	if(name == NULL)
		return NULL;
	
	bzero(name_lib, 500);
	lenght=strnlen(name, 500);
	
	for(i=0; i<lenght; i++)
		if(name[i] == '.')
			break;
	
	for(i++; i<lenght; i++)
		if(is_number(name+i,lenght-i))
			break;
		else
			for(i++; i<lenght && name[i] != '.'; i++);
	strncpy(name_lib, name, i);
	
	return name_lib;
}

/*
 * return path on file, or NULL  ... 
 */
static char *can_sym_link(char *name, char *path)
{
	char name_lib[500]; /* size as return static get_name_library() */
	char full_path[1000];
	DIR *dir;
	struct dirent *item;
	struct stat item_stat;
	char *p;
	p=get_name_library(name);
	strcpy(name_lib, p);
	
	dir=opendir(path);
	if (dir == NULL)
		return NULL;
    while ((item = readdir (dir)))
    {
		if(item->d_name[0] == '.')
			continue;
		snprintf(full_path, 1000, "%s/%s", path, item->d_name);
		
		stat(full_path, &item_stat);
		if(S_ISDIR(item_stat.st_mode))
			continue;
		
		p=get_name_library(item->d_name);
		if(strcmp(name_lib, p) == 0)
		{
			closedir (dir);
			return strndup(item->d_name, 499);
		}
	}
	closedir (dir);
	
	return NULL;
}

/*
 * copy library
 */
void copy(t_libraries *libraries, char *dest)
{
	int i,offset;
	struct stat item_stat;
	char *path;
	char *name;
	char *p;
	
	char dest_dir_path[1000];
	if(dest == NULL)
		return;
	
	char buffer[BUFFER_COPY_SIZE];
	snprintf(buffer, BUFFER_COPY_SIZE, "mkdir -p '%s'", dest);
	system(buffer);
	for(i=0; i<libraries->have; i++)
		if(libraries->path[i] != NULL)
		{
			bzero(dest_dir_path, 1000);
			path=libraries->path[i];
			offset=strlen(path);
			for(offset--;offset>0 && path[offset]!= '/'; offset--);
			strncpy(dest_dir_path, path, offset);

			name=path+offset;
			if(*name == '/')
				name++;
			
			/*
			 * /lib/i386-linux-gnu/i686/cmov/libc.so.6
			 *                              ^
			 *              position offset |
			 */
			/** user print name **/
			if(libraries->name[i] != NULL)
			{
				printf("%s ... ", libraries->name[i]);
				fflush(stdout);
			}
			else
			{
				if(offset != 0)
					offset++;
				printf("%s ... ", path+offset);
				fflush(stdout);
				
				libraries->name[i]=strdup(path+offset);
			}
			
			/** if exist file/symlink, skip */
			snprintf(buffer, BUFFER_COPY_SIZE, "%s%s/%s",
							dest, dest_dir_path+1,libraries->name[i]);
			if(stat(buffer, &item_stat) == 0)
			{
				if(S_ISREG(item_stat.st_mode)
				 ||S_ISLNK(item_stat.st_mode))
				{
					puts("exist");
					continue;
				}
				else
				{
					puts("error, somethink is here");
					continue;
				}
			}
			
			snprintf(buffer, BUFFER_COPY_SIZE,
						"%s%s", dest, dest_dir_path+1);
			p=can_sym_link(libraries->name[i],buffer);
			if(p != NULL)
			{
				/** can create symbol link **/
				snprintf(buffer, BUFFER_COPY_SIZE, 
					"mkdir -p %s%s/", dest, dest_dir_path+1);
				system(buffer);
				snprintf(buffer, BUFFER_COPY_SIZE, 
					"(cd %s%s; ln -s '%s' '%s')", dest, dest_dir_path+1, p,
					libraries->name[i]);
				system(buffer);
				puts(buffer);
				puts("link");
			}
			else
			{
				/** copy library */
				snprintf(buffer, BUFFER_COPY_SIZE, 
					"mkdir -p %s%s/", dest, dest_dir_path+1);
				//puts(buffer);
				system(buffer);
				snprintf(buffer, BUFFER_COPY_SIZE, 
					"cp '%s' %s%s/%s", libraries->path[i], dest,
									dest_dir_path+1, name);
				system(buffer);
				puts("copy");
			}
		}
}
