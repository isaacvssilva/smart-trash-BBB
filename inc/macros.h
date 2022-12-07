/*
 * =====================================================================================
 *
 *       Filename:  macros.h
 *
 *    Description:  -
 *
 *        Version:  1.0
 *        Created:  06/12/2022 17:55:16
 *       Revision:  none
 *       Compiler:  arm-linux-gnueabihf-gcc
 *
 *         Author:  Isaac Vinicius, Hugo Bessa, Gislan Souza
 *                  isaacvinicius2121@alu.ufc.br, hugobessa@alu.ufc.br, gislansouza@alu.ufc.br
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#ifndef MACROS_H
#define MACROS_H

/*inclusao de bibliotecas*/
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

/*modulos funcionais*/
#include "../inc/file_read.h"
#include "../inc/mpu_i2c.h"
#include "../inc/threads.h"

#define PATH_GPIO "/sys/class/gpio/"
#define PATH_PWM_EXPORT "/sys/class/pwm/pwmchip0/export"
#define PATH_PWM_UNEXPORT "/sys/class/pwm/pwmchip0/unexport"
#define PATH_PWM_CH_0 "/sys/class/pwm/pwmchip0/pwm0/"
#define PERIOD_DEFAULT "20000000"
#define DC_DEFAULT "1000000"
#define DC_OPEN "2000000"
#define GPIO_48 "48"
#define GPIO_60 "60"
#define GPIO_30 "30"
#define GPIO_31 "31"
#define OUT "out"
#define IN "in"

#endif
/*****************************END OF FILE**************************************/