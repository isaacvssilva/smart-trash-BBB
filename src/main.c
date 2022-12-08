/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  -
 *
 *        Version:  1.0
 *        Created:  06/12/2022 18:05:05
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
pthread_t task_aproximacao;
pthread_t task_mpu;
pthread_t task_capacidade;

int main(){

    /*exportando diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor para aproximacao*/
    writeFile(GPIO_48, sizeof(GPIO_48), PATH_GPIO"export");
    writeFile(IN, sizeof(IN), PATH_GPIO"gpio48/direction");

    /*exportando diretorio do gpio30(P9_11), e setando como entrada para conectar o sensor para capacidade*/
    writeFile(GPIO_30, sizeof(GPIO_30), PATH_GPIO"export");
    writeFile(IN, sizeof(IN), PATH_GPIO"gpio30/direction");

    /*exportando diretorio do gpio60(P9_13), e setando como saida para acender o LED*/
    writeFile(GPIO_31, sizeof(GPIO_31), PATH_GPIO"export");
    writeFile(OUT, sizeof(OUT), PATH_GPIO"gpio31/direction");

    /*exportando diretorio do gpio60(P9_12), e setando como saida para conectar o buzzer*/
    writeFile(GPIO_60, sizeof(GPIO_60), PATH_GPIO"export");
    writeFile(OUT, sizeof(OUT), PATH_GPIO"gpio60/direction");

    /*exportando diretorio do PWM canal 0 (P9_22), para conectar o servo*/
    writeFile("0", 1, PATH_PWM_EXPORT);

    /*Definindo periodo do PWM para o servo*/
    writeFile(PERIOD_DEFAULT, sizeof(PERIOD_DEFAULT), PATH_PWM_CH_0"period");

    /*Definindo DutyCycle do PWM para o servo*/
    writeFile(DC_DEFAULT, sizeof(DC_DEFAULT), PATH_PWM_CH_0"duty_cycle");

    /*Habilitando do PWM para o servo*/
    writeFile("1", 1, PATH_PWM_CH_0"enable");

    printf("inicializacao principal\n\n");
    /*parametros: address da thread, atributo, nome da funcao, parametro da thread de criacao*/
    pthread_create(&task_aproximacao, NULL, (void*) leituraAproximacao, NULL);
    pthread_create(&task_capacidade, NULL, (void*) leituraCapacidade, NULL);
    pthread_create(&task_mpu, NULL, (void*) leituraMPU, NULL);

    /*bloquea a main ate as tasks terminar. parametros: thread, motivo do termino*/
    pthread_join(task_aproximacao, NULL);
    pthread_join(task_capacidade, NULL);
    pthread_join(task_mpu, NULL);

    /*removendo diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor*/
    writeFile(GPIO_48, sizeof(GPIO_48), PATH_GPIO"unexport");
    writeFile(GPIO_60, sizeof(GPIO_60), PATH_GPIO"unexport");
    writeFile(GPIO_30, sizeof(GPIO_30), PATH_GPIO"unexport");
    writeFile(GPIO_31, sizeof(GPIO_31), PATH_GPIO"unexport");
    /*removendo diretorio do PWM canal 0 (P9_22), para conectar o servo*/
    writeFile("0", 1, PATH_PWM_UNEXPORT);

    printf("Fim\n");
    return (0);
}