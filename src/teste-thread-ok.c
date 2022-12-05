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
#define PATH_LDR "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define PATH_ULTRASONIC "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define GPIO_60 "60"
#define OUT "out"
#include <pthread.h>

pthread_t task_lixo;
pthread_t task_aproximacao;

void leituraLixo(){
    FILE *dir_leitura_lixo = NULL; // ponteiro para acessar o canal do HC-SR04
    int valueAnalog = 0;
    dir_leitura_lixo = fopen("teste.txt", "r");
    if (!dir_leitura_lixo){
        printf("nao e possivel abrir o path ADC leitura Lixo!\n");
        exit(-1);
    }
    fscanf(dir_leitura_lixo, "%d", &valueAnalog);
    printf("Inicio da task leituraLixo\n");
    while(1){
        printf("leitura do lixo: %d\n", valueAnalog);
        /*suspende a execucao do thread de chamada por 10 segundo*/
        usleep(10000000);
        valueAnalog += 500;
    }
}

void leituraAproximacao(){
    int valueAnalog = 0;
    FILE *diretorio_adc = NULL;
    diretorio_adc = fopen("teste.txt", "r");
    if (!diretorio_adc){
        printf("nao e possivel abrir o path ADC!\n");
        exit(-1);
    }
    printf("Inicio da task leituraAproximacao\n");
    while(1){
        //printf("BBBBB\n");
        fscanf(diretorio_adc, "%d", &valueAnalog);
        // if(valueAnalog > 0){
        //     //printf("valor atual\n");
        //     //printf("%d\n", valueAnalog);
        // }else{
        //     //printf("valor do arquivo é menor\n");
        //     //printf("%d\n", valueAnalog);
        // }
    }
}

int main(){
    
    printf("inicializacao principal\n\n");
    /*parametros: address da thread, atributo, nome da funcao, parametro da thread a criacao*/
    pthread_create(&task_lixo, NULL, (void*) leituraLixo, NULL);

    /*parametros: address da thread, atributo, nome da funcao, parametro da thread a criacao*/
    pthread_create(&task_aproximacao, NULL, (void*) leituraAproximacao, NULL);

    /*bloquea a main ate t1 terminar. parametros: thread, motivo do termino*/
    pthread_join(task_lixo, NULL);

    /*bloquea a main ate t2 terminar. parametros: thread, motivo do termino*/
    pthread_join(task_aproximacao, NULL);

    printf("Fim\n");
    return(0);
}
