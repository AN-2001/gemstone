CC := gcc
GEMSTONE := /home/amber/projects/gemstone
LIB := $(GEMSTONE)/lib
SRC := $(GEMSTONE)/src
INCLUDE := $(GEMSTONE)/include
mode := rel
flags := -Wall -Werror -pedantic-errors -I$(INCLUDE)

ifeq ($(mode), debug)
flags := $(flags) -g -DDEBUG
endif


libs := -lm
obj := $(SRC)/linkedList.o $(SRC)/raster.o $(SRC)/gem.o $(SRC)/colour.o

include $(SRC)/makefile

all: $(obj) 
	$(CC) -shared $(obj) -o $(LIB)/libgemstone.so
	#ar cr $(LIB)/libgemstone.a $(obj) 

clean:
	$(shell rm $(LIB)/* 2> /dev/null)
	$(shell rm $(SRC)/*.o 2> /dev/null)
