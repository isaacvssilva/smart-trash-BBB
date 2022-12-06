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
#define PATH_PWM_EXPORT "/sys/class/pwm/pwmchip0/export"
#define PATH_PWM_UNEXPORT "/sys/class/pwm/pwmchip0/unexport"
#define PATH_PWM_CH_0 "/sys/class/pwm/pwmchip0/pwm0/"
#define PERIOD_DEFAULT "20000000"
#define DC_DEFAULT "1000000"
#define DC_OPEN "2000000"
#define GPIO_48 "48"
#define OUT "out"
#define IN "in"


int main(){
    int valueAnalog = 0;

    /*exportando diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor*/
    writeFile(GPIO_48, sizeof(GPIO_48), PATH_GPIO"export");
    writeFile(IN, sizeof(IN), PATH_GPIO"gpio48/direction");

    /*exportando diretorio do PWM canal 0 (P9_22), para conectar o servo*/
    writeFile("0", 1, PATH_PWM_EXPORT);

    /*Definindo periodo do PWM para o servo*/
    writeFile(PERIOD_DEFAULT,  sizeof(PERIOD_DEFAULT), PATH_PWM_CH_0"period");

    /*Definindo DutyCycle do PWM para o servo*/
    writeFile(DC_DEFAULT,  sizeof(DC_DEFAULT), PATH_PWM_CH_0"duty_cycle");

    /*Habilitando do PWM para o servo*/
    writeFile("1",  1, PATH_PWM_CH_0"enable");

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
    /*removendo diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor*/
    writeFile(GPIO_48, sizeof(GPIO_48), PATH_GPIO"unexport");

    /*removendo diretorio do PWM canal 0 (P9_22), para conectar o servo*/
    writeFile("0", 1, PATH_PWM_UNEXPORT);
    return (0);
}