#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#define PATH_GPIO "/sys/class/gpio/"
#define PATH_LDR  "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define GPIO_45 "45"
#define GPIO_60 "60"
#define GPIO_44 "44"
#define GPIO_48 "48"
#define OUT "out"
#define IN "in"
#define SOUND_SPEED 0.034 // calculo da distância para o obstáculo, valor em cm/uS.
#include <math.h>

int main(){
    FILE *diretorio_gpio = NULL; // ponteiro para acessar diretorio do gpio
    FILE *diretorio_adc = NULL;
    FILE *set_IO = NULL; // ponteiro para setar o tipo de saida da funcao do gpio
    int valueAnalog = 0;

    /*exportando diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor*/
    diretorio_gpio = fopen(PATH_GPIO"export", "w");
    if (!diretorio_gpio){
        printf("nao e possivel exportar gpio!\n");
        exit(-1);
    }
    fwrite(GPIO_48, 1, sizeof(GPIO_48), diretorio_gpio);
    fclose(diretorio_gpio);

    /*GPIO P9_12*/
    set_IO = fopen(PATH_GPIO"gpio48/direction", "w");
    if (!set_IO){
        printf("nao e possivel configurar gpio!\n");
        exit(-1);
    }
    fwrite(IN, 1, sizeof(IN), set_IO);
    fclose(set_IO);

    /*exportando diretorio do gpio60(P9_12), e setando como saida para conectar o LED*/
    diretorio_gpio = fopen(PATH_GPIO"export", "w");
    if (!diretorio_gpio){
        printf("nao e possivel exportar gpio!\n");
        exit(-1);
    }
    fwrite(GPIO_60, 1, sizeof(GPIO_60), diretorio_gpio);
    fclose(diretorio_gpio);

    /*GPIO P9_12*/
    set_IO = fopen(PATH_GPIO"gpio60/direction", "w");
    if (!set_IO){
        printf("nao e possivel configurar gpio!\n");
        exit(-1);
    }
    fwrite(OUT, 1, sizeof(OUT), set_IO);
    fclose(set_IO);

    while (1) {

        diretorio_gpio = fopen(PATH_GPIO"gpio48/value", "r");
        fscanf(diretorio_gpio, "%d", &valueAnalog);
        fclose(diretorio_gpio);
        if(valueAnalog == 0){
            set_IO = fopen(PATH_GPIO"gpio60/value", "w");
            fwrite("1", 1, 1, set_IO);
            fclose(set_IO);
            sleep(1);
        }
        else{
            set_IO = fopen(PATH_GPIO"gpio60/value", "w");
            fwrite("0", 1, 1, set_IO);
            fclose(set_IO);
            
        }
         
    }
    return(0);
}