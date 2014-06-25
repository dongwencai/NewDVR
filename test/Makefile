 PARAM_FILE ?=../Makefile.param
 include $(PARAM_FILE)


CC :=$(CROSS_COMPILE)gcc
LIB_LIST :=$(BOARD_LIB)
#LIB_LIST +=$(WINFORM_OBJ)
#LIB_LIST +=$(GRAPHICS_OBJ)
#LIB_LIST +=$(COMM_OBJ)

SRC  := $(wildcard *.c) 
OBJ  := $(SRC:%.c=%.o)

TARGET := $(OBJ:%.o=%)
.PHONY : clean all

all: $(TARGET)

#$(TARGET):%:%.o 
test: *.o
	$(CC) $(CFLAGS) -o $@ $^ -Wl,--whole-archive $(LIB_LIST) -Wl,--no-whole-archive  


clean:
	@rm -f $(TARGET)
	@rm -f $(OBJ)
	@rm -f $(COMM_OBJ)