CC := g++
GEMSTONE := /home/amber/projects/gemstone
LIB := $(GEMSTONE)/lib
INCLUDE := $(GEMSTONE)/include
flags := -Wall -I$(INCLUDE) -I. -L$(LIB)
libs := -lgemstone -lm 
obj := boilerplate.o
proj := gemstone

mode := rel
ifeq ($(mode), debug)
flags := $(flags) -g -DDEBUG
endif


all: $(proj)


$(proj): $(proj).cpp $(obj) config.h
	$(CC) $< $(obj) $(flags) $(libs) -o $@

%.o : %.cpp %.h config.h
	$(CC) -c $< -o $@ $(flags) $(libs)

clean:
	rm $(proj) $(obj) *.ppm 

cleanPPM:
	rm *.ppm

encode:
	imageToVideo .
	
