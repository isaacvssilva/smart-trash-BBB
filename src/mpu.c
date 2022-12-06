#include "../inc/mpu.h"

int writeMPU(uint8_t addr_start, uint8_t dados, int file){
    /*mestre coloca endereço do registrador e os dados do registrador no barramento.*/
    char buffer_write[2];
    buffer_write[0] = addr_start;
    buffer_write[1] = dados;
    
    if(write(file, buffer_write, 2) <= 0){
        printf("Falha ao adquirir acesso ao barramento e/ou falar com o escravo.\n");
        return -1;
    }
    return 0;
}

int readMPU(uint8_t addr_start, char *buffer, uint32_t tamByte, int file){
    char buffer_write[2];
    buffer_write[0] = addr_start;
    if(write(file, buffer_write, 1) <= 0){
        printf("falha na escrita do endereco\n");
        exit(1);
    }

    if (read(file, buffer, tamByte) <= 0) {
        perror("falha na leitura\n");
    }
    return 0;
}

void mod_MPU6050(int file){
    //sleep mode: Quando definido como 1, este bit coloca o MPU-60X0 no modo de suspensão.
    writeMPU(PWR_MGMT_1, 0x00, file); // Tirando o MPU6050 do modo sleep
    writeMPU(GYRO_CONFIG, 0x00, file); // habilitando giroscopio
    writeMPU(ACCEL_CONFIG, 0x00, file); // habilitando acelerometro
}

void read_acc(short int *value, int file){
    
    /*inicia a leitura do endereço base dos valores do acelerômetro
    cada valor de eixo é armazenado em 2 registros
    */
    char acc_buffer[6];
    readMPU(REG_ACCEL_XOUT_H, acc_buffer, 6, file);
    
    /*value[0] = valor do eixo acc x, value[1] = valor do eixo acc y, value[2] = valor do eixo acc z*/
    value[0] = (int) ((acc_buffer[0] << 8) | acc_buffer[1]); // lendo os registradores X
    value[1] = (int) ((acc_buffer[2] << 8) | acc_buffer[3]); // lendo os registradores Y
    value[2] = (int) ((acc_buffer[4] << 8) | acc_buffer[5]); // lendo os registradores Z
}

void read_gyro(short *value, int file){
    
    /*inicia a leitura do endereço base dos valores do giroscopio
    cada valor de eixo é armazenado em 2 registros
    */
    char gyro_buffer[6];
    readMPU(REG_GYRO_XOUT_H, gyro_buffer, 6, file);
    
    /*value[0] = valor do eixo gyro x, value[1] = valor do eixo gyro y, value[2] = valor do eixo gyro z*/
    value[0] = ((gyro_buffer[0] << 8) + gyro_buffer[1]); // lendo os registradores X
    value[1] = ((gyro_buffer[2] << 8) + gyro_buffer[3]); // lendo os registradores Y
    value[2] = ((gyro_buffer[4] << 8) + gyro_buffer[5]); // lendo os registradores Z
}