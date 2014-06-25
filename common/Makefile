PARAM_FILE ?=../Makefile.param
include $(PARAM_FILE)

CC :=$(CROSS_COMPILE)gcc
SRC :=$(wildcard *.c)
OBJ :=$(SRC:%.c=%.o)
all : $(OBJ) move

$(OBJ):%.o:%.c
	$(CC) $(CFLAGS) -c  $^ -o $@ 
move:
	mv *.o ../test/