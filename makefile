# makefile for diskschedule.c

# -lm stands for load math to be able to use sqrt( ... ) for standard deviation calculation
all: diskschedule.c 
	gcc -g -Wall -o diskschedule diskschedule.c -lm

clean: 
	$(RM) diskschedule
