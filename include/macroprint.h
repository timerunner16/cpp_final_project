#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef MACROPRINT
#define DBPRINTFCHK 1
#else
#define DBPRINTFCHK 0
#endif

#ifndef SOURCE_PATH_SIZE
#define SOURCE_PATH_SIZE 0
#endif

#define __FILENAME__ ((__FILE__) + (SOURCE_PATH_SIZE))
#ifdef VERBOSE_DBPRINTF
#define DBPRINTF(fmt, ...) \
	do { \
		if (DBPRINTFCHK) fprintf(stdout, "%s:%d:%s(): " fmt, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)
#else
#define DBPRINTF(fmt, ...) \
	do { \
		if (DBPRINTFCHK) fprintf(stdout, fmt, ##__VA_ARGS__); \
	} while (0)
#endif
