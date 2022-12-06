CROSS_COMPILE=arm-linux-gnueabihf-gcc
SRC=src/
OBJ=obj/
BIN=bin/

OBJS = main.o mpu_i2c.o threads.o file_read.o
all: folder app

app: $(OBJS)
	${CROSS_COMPILE} $(OBJ)main.o $(OBJ)mpu_i2c.o $(OBJ)threads.o $(OBJ)file_read.o -static -o $(BIN)app

main.o: $(SRC)main.c
	${CROSS_COMPILE} -c $(SRC)main.c -o $(OBJ)main.o -Wall

mpu_i2c.o: $(SRC)mpu_i2c.c
	${CROSS_COMPILE} -c $< -o $(OBJ)mpu_i2c.o -Wall

threads.o: $(SRC)threads.c
	${CROSS_COMPILE} -c $< -o $(OBJ)threads.o -Wall

file_read.o: $(SRC)file_read.c
	${CROSS_COMPILE} -c $< -o $(OBJ)file_read.o -Wall

folder: 
	mkdir -p bin obj

clean:
	rm obj/ bin/ -rf obj/
