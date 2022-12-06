#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <stdio.h>

/*-----------------------------------------------------------------------------
 *  Function prototype
 *-----------------------------------------------------------------------------*/
 int writeFile(const char* str, int size_str, const char* path);
 int readFileInt(int* num, const char* path);

#endif /*FILEHANDLER_H_*/