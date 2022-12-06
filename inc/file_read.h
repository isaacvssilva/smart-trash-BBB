/*
 * =====================================================================================
 *
 *       Filename:  file_read.h
 *
 *    Description:  -
 *
 *        Version:  1.0
 *        Created:  06/12/2022 17:47:51
 *       Revision:  none
 *       Compiler:  arm-linux-gnueabihf-gcc
 *
 *         Author:  Isaac Vinicius, Hugo Bessa, Gislan Souza
 *                  isaacvinicius2121@alu.ufc.br, hugobessa@alu.ufc.br, gislansouza@alu.ufc.br
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#ifndef FILEREAD_H
#define FILEREAD_H
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------
 *  Function prototype
 *-----------------------------------------------------------------------------*/
 int writeFile(const char* str, int size_str, const char* path);
 int readFileInt(int* num, const char* path);

#endif
/*****************************END OF FILE**************************************/