/*
 * =====================================================================================
 *
 *       Filename:  file_read.c
 *
 *    Description:  -
 *
 *        Version:  1.0
 *        Created:  06/12/2022 17:52:25
 *       Revision:  none
 *       Compiler:  arm-linux-gnueabihf-gcc
 *
 *         Author:  Isaac Vinicius, Hugo Bessa, Gislan Souza
 *                  isaacvinicius2121@alu.ufc.br, hugobessa@alu.ufc.br, gislansouza@alu.ufc.br
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "../inc/file_read.h"

int writeFile(const char* str, int size_str, const char* path){
    FILE *file = NULL; 
    file = fopen(path, "w");
    if(file){
        fwrite(str, 1, size_str, file);
        fclose(file);
        return 0;
    }else{
        printf("\nERRO ao abrir arquivo!\n");
        exit(-1);
    }
}

int readFileInt(int* num, const char* path){
    FILE *file = NULL;
    file = fopen(path, "r");
    if(file){
        fscanf(file, "%d", num);
        fclose(file);   
        return 0;    
    }else{
        printf("\nERRO ao abrir arquivo!\n");
        exit(-1);
    }    
}