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
#include <pthread.h>

#include "../inc/fileHandler.h"
#include "../inc/mpu.h"

#define PATH_GPIO "/sys/class/gpio/"
#define PATH_PWM_EXPORT "/sys/class/pwm/pwmchip0/export"
#define PATH_PWM_UNEXPORT "/sys/class/pwm/pwmchip0/unexport"
#define PATH_PWM_CH_0 "/sys/class/pwm/pwmchip0/pwm0/"
#define PERIOD_DEFAULT "20000000"
#define DC_DEFAULT "1000000"
#define DC_OPEN "2000000"
#define GPIO_48 "48"
#define GPIO_60 "60"
#define OUT "out"
#define IN "in"

pthread_t task_aproximacao;
pthread_t task_mpu;

void leituraAproximacao(){
    printf("Inicio da task leituraAproximacao\n");
    int valueAnalog = 0;
    while (1){
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
    double acc_X, acc_Y, acc_Z;
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
    mod_MPU6050();

    while(1){
        read_acc(acc_value, file);
        read_gyro(gyro_value, file);

        /*ACELEROMETRO:
            convertendo valor bruto de ACC em valor G:
                - verificar faixa de valores em operacao, exemplo:
                    faixa sel=2 : 1g -> 4096
                    valor bruto / (valor faixa)
        */
       /*Obtendo valores de saida de -2g a +2g*/
       acc_X = (double) acc_value[0] / ACCEL_FS_SEL_0;
       acc_Y = (double) acc_value[1] / ACCEL_FS_SEL_0;
       acc_Z = (double) acc_value[2] / ACCEL_FS_SEL_0;

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

        printf("Printando valores convertidos:\nGiroscopio:\n");
        printf("angulo X: %0.2f\n", gyro_X);
        printf("angulo Y: %0.2f\n", gyro_Y);
        printf("angulo Z: %0.2f\n\n", gyro_Z);
        /*suspende a execucao do thread de chamada por 1 segundo*/
        // usleep(1000000);
        /*suspende a execucao do thread de chamada por 0,2 segundo*/
        usleep(200000);
    } 
}

int main(){
    int valueAnalog = 0;

    /*exportando diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor*/
    writeFile(GPIO_48, sizeof(GPIO_48), PATH_GPIO"export");
    writeFile(IN, sizeof(IN), PATH_GPIO"gpio48/direction");

     /*exportando diretorio do gpio48(P9_15), e setando como saida para conectar o buzzer*/
    writeFile(GPIO_60, sizeof(GPIO_60), PATH_GPIO"export");
    writeFile(OUT, sizeof(OUT), PATH_GPIO"gpio60/direction");

    /*exportando diretorio do PWM canal 0 (P9_22), para conectar o servo*/
    writeFile("0", 1, PATH_PWM_EXPORT);

    /*Definindo periodo do PWM para o servo*/
    writeFile(PERIOD_DEFAULT,  sizeof(PERIOD_DEFAULT), PATH_PWM_CH_0"period");

    /*Definindo DutyCycle do PWM para o servo*/
    writeFile(DC_DEFAULT,  sizeof(DC_DEFAULT), PATH_PWM_CH_0"duty_cycle");

    /*Habilitando do PWM para o servo*/
    writeFile("1",  1, PATH_PWM_CH_0"enable");

    printf("inicializacao principal\n\n");
    /*parametros: address da thread, atributo, nome da funcao, parametro da thread a criacao*/
    pthread_create(&task_aproximacao, NULL, (void*) leituraAproximacao, NULL);
    pthread_create(&task_mpu, NULL, (void*) leituraMPU, NULL);

    /*bloquea a main ate t2 terminar. parametros: thread, motivo do termino*/
    pthread_join(task_aproximacao, NULL);
    pthread_join(task_mpu, NULL);


    /*removendo diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor*/
    writeFile(GPIO_48, sizeof(GPIO_48), PATH_GPIO"unexport");
    writeFile(GPIO_60, sizeof(GPIO_60), PATH_GPIO"unexport");
    /*removendo diretorio do PWM canal 0 (P9_22), para conectar o servo*/
    writeFile("0", 1, PATH_PWM_UNEXPORT);

    printf("Fim\n");
    return (0);
}