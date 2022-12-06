#include "../inc/fileHandler.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  writeFile
 *  Description:  
 * =====================================================================================
 */
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  readFileInt
 *  Description:  
 * =====================================================================================
 */
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