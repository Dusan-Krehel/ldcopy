/*
 * Data structure for save data - name and path to shared library  
 */
#ifndef _LIBRARIES_H
 #define _LIBRARIES_H
 #include <stdlib.h>
 
typedef struct{
	char **name;
	char **path;
	int count_alloc; // count allocated
	int have;
} t_libraries;

/* pointer to structure, or on error return false */
extern t_libraries *libraries_init(int max_count);
/* ok return 0, error return 1 */
extern int libraries_add(t_libraries *libraries, char *name, char *path,
							size_t longth_name, size_t longth_path);
extern void libraries_free(t_libraries *libraries);
#endif
