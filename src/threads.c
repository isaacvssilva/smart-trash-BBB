/*
 * =====================================================================================
 *
 *       Filename:  threads.c
 *
 *    Description:  -
 *
 *        Version:  1.0
 *        Created:  06/12/2022 18:02:43
 *       Revision:  none
 *       Compiler:  arm-linux-gnueabihf-gcc
 *
 *         Author:  Isaac Vinicius, Hugo Bessa, Gislan Souza
 *                  isaacvinicius2121@alu.ufc.br, hugobessa@alu.ufc.br, gislansouza@alu.ufc.br
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */
#include "../inc/macros.h"

void leituraAproximacao(){
    printf("Inicio da task leituraAproximacao\n");
    int valueAnalog = 0;
    while(1){
        /*Verificando valor de saida do sensor IR*/
        readFileInt(&valueAnalog, PATH_GPIO"gpio48/value");

        /*Se saida do sensor IR igual a 0, obstaculo detectado*/
        if (valueAnalog == 0){
            /*Definindo DutyCycle do PWM para o servo*/
            writeFile(DC_OPEN, sizeof(DC_OPEN), PATH_PWM_CH_0"duty_cycle");
            sleep(1);
        } /*Se saida do sensor IR igual a 1, sem obstaculos*/
        else{
            /*Definindo DutyCycle do PWM para o servo*/
            writeFile(DC_DEFAULT, sizeof(DC_DEFAULT), PATH_PWM_CH_0"duty_cycle");
        }
    }
}

void leituraMPU(){
    printf("Inicio da task leituraMPU\n");
    int file;
    //variaveis para pegar os valores do sensor
    // double acc_X, acc_Y, acc_Z;
    double gyro_X, gyro_Y, gyro_Z;

    //variaveis para armazenar os valores bruto do sensor
    short acc_value[3], gyro_value[3];

    /*Abrindo barramento para leitura e escrita I2C*/
    if((file = open(I2C_DEVICE_FILE, O_RDWR)) < 0 ){
        perror("Falha ao abrir o barramento i2c!\n");
        exit(-1);
    }
    /*define o endereço do escravo I2C usando o comando ioctl I2C_SLAVE*/
    if(ioctl(file, I2C_SLAVE, ADDRESS_AD0_LOW) < 0 ){
        perror("Falha ao definir o endereço do escravo I2C!\n");
        close(file);
        exit(-1);
    }
    mod_MPU6050(file);

    while(1){
        read_acc(acc_value, file);
        read_gyro(gyro_value, file);

        /*
        GIROSCOPIO:
            convertendo valor bruto de gyro em graus:
            faixa sel=0 1graus/seg -> 131
            valor bruto / (valor faixa) = graus/seg : 131/131 = 1graus/seg
        */
       /*Obtendo valores de saida (esacala) ± 250°/seg*/
        gyro_X = (double) gyro_value[0] / GYRO_FS_SEL_0;
        gyro_Y = (double) gyro_value[1] / GYRO_FS_SEL_0;
        gyro_Z = (double) gyro_value[2] / GYRO_FS_SEL_0;

        /*verfica se os eixos ultrapassam o range de -40 a 40+, caso verdadeiro, aciona o buzzer*/
        if(gyro_X < -40 || gyro_X > 40 || gyro_Y < -40 || gyro_Y > 40 || gyro_Z < -40 || gyro_Z > 40){
            writeFile("1", 1, PATH_GPIO"gpio60/value");
        }else{
            writeFile("0", 1, PATH_GPIO"gpio60/value");
        }
        usleep(200000);
    } 
}

void leituraCapacidade(){
    printf("Inicio da task leituraCapacidade\n");
    int valueAnalog = 0;
    while(1){
        /*Verificando valor de saida do sensor IR*/
        readFileInt(&valueAnalog, PATH_GPIO"gpio30/value");

        /*Se saida do sensor IR igual a 0, lixera esta cheia*/
        if (valueAnalog == 0){
            writeFile("1", 1, PATH_GPIO"gpio31/value");
            sleep(1);
        } /*Se saida do sensor IR igual a 1, lixera nao atingiu a capacidade*/
        else{
            writeFile("0", 1, PATH_GPIO"gpio31/value");
        }
    }
}