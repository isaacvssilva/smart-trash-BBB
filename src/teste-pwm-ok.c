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
    FILE *diretorio_gpio = NULL; // ponteiro para acessar diretorio do gpio
    FILE *diretorio_pwm = NULL;
    FILE *set_IO = NULL; // ponteiro para setar o tipo de saida da funcao do gpio
    int valueAnalog = 0;
    FILE *io_PWM = NULL;

    /*exportando diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor*/
    diretorio_gpio = fopen(PATH_GPIO "export", "w");
    if (!diretorio_gpio){
        printf("nao e possivel exportar gpio!\n");
        exit(-1);
    }
    fwrite(GPIO_48, 1, sizeof(GPIO_48), diretorio_gpio);
    fclose(diretorio_gpio);

    /*GPIO P9_12*/
    set_IO = fopen(PATH_GPIO "gpio48/direction", "w");
    if (!set_IO){
        printf("nao e possivel configurar gpio!\n");
        exit(-1);
    }
    fwrite(IN, 1, sizeof(IN), set_IO);
    fclose(set_IO);

    /*exportando diretorio do PWM canal 0 (P9_22), para conectar o servo*/
    diretorio_pwm = fopen(PATH_PWM_EXPORT, "w");
    if (!diretorio_pwm){
        printf("nao e possivel exportar canal PWM!\n");
        exit(-1);
    }
    fwrite("0", 1, 1, diretorio_pwm);
    fclose(diretorio_pwm);

    /*Definindo periodo do PWM para o servo*/
    io_PWM = fopen(PATH_PWM_CH_0 "period", "w");
    if (!io_PWM){
        printf("nao e possivel definir o periodo!\n");
        exit(-1);
    }
    fwrite(PERIOD_DEFAULT, 1, sizeof(PERIOD_DEFAULT), diretorio_pwm);
    fclose(diretorio_pwm);

    /*Definindo DutyCycle do PWM para o servo*/
    io_PWM = fopen(PATH_PWM_CH_0 "duty_cycle", "w");
    if (!io_PWM){
        printf("nao e possivel definir o DutyCycle!\n");
        exit(-1);
    }
    fwrite(DC_DEFAULT, 1, sizeof(DC_DEFAULT), diretorio_pwm);
    fclose(diretorio_pwm);

    /*Habilitando do PWM para o servo*/
    io_PWM = fopen(PATH_PWM_CH_0 "enable", "w");
    if (!io_PWM){
        printf("nao e possivel definir o habilitar PWM!\n");
        exit(-1);
    }
    fwrite("1", 1, 1, diretorio_pwm);
    fclose(diretorio_pwm);

    while (1){
        diretorio_gpio = fopen(PATH_GPIO "gpio48/value", "r");
        fscanf(diretorio_gpio, "%d", &valueAnalog);
        fclose(diretorio_gpio);
        if (valueAnalog == 0){
            /*Definindo DutyCycle do PWM para o servo*/
            io_PWM = fopen(PATH_PWM_CH_0 "duty_cycle", "w");
            if (!io_PWM){
                printf("nao e possivel definir o DutyCycle!\n");
                exit(-1);
            }
            fwrite(DC_OPEN, 1, sizeof(DC_OPEN), diretorio_pwm);
            fclose(diretorio_pwm);
            sleep(1);
        }else{
            /*Definindo DutyCycle do PWM para o servo*/
            io_PWM = fopen(PATH_PWM_CH_0 "duty_cycle", "w");
            if (!io_PWM){
                printf("nao e possivel definir o DutyCycle!\n");
                exit(-1);
            }
            fwrite(DC_DEFAULT, 1, sizeof(DC_DEFAULT), diretorio_pwm);
            fclose(diretorio_pwm);
        }
    }
    /*removendo diretorio do gpio48(P9_15), e setando como entrada para conectar o sensor*/
    diretorio_gpio = fopen(PATH_GPIO "unexport", "w");
    if (!diretorio_gpio){
        printf("nao e possivel exportar gpio!\n");
        exit(-1);
    }
    fwrite(GPIO_48, 1, sizeof(GPIO_48), diretorio_gpio);
    fclose(diretorio_gpio);

    /*removendo diretorio do PWM canal 0 (P9_22), para conectar o servo*/
    diretorio_pwm = fopen(PATH_PWM_UNEXPORT, "w");
    if (!diretorio_pwm){
        printf("nao e possivel exportar canal PWM!\n");
        exit(-1);
    }
    fwrite("0", 1, 1, diretorio_pwm);
    fclose(diretorio_pwm);
    return (0);
}