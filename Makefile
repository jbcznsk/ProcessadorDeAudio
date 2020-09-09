CC = gcc
CFLAGS = -Wall -g -pedantic

progs = wavinfo wavvol wavnorm wavrev wavecho wavwide wavcat wavmix wavabs
objs = acesso.o tratamento.o wavinfo.o wavvol.o wavnorm.o wavrev.o wavecho.o wavwide.o wavcat.o wavmix.o wavabs.o

all: $(progs)

wavinfo: acesso.o tratamento.o wavinfo.o 
wavvol : acesso.o tratamento.o wavvol.o  
wavnorm: acesso.o tratamento.o wavnorm.o 
wavrev : acesso.o tratamento.o wavrev.o 
wavecho: acesso.o tratamento.o wavecho.o 
wavwide: acesso.o tratamento.o wavwide.o 
wavcat : acesso.o tratamento.o wavcat.o 
wavmix : acesso.o tratamento.o wavmix.o 

wavabs : acesso.o tratamento.o wavabs.o 

acesso.o: estruturas.h acesso.c 
tratamento.o: estruturas.h tratamento.c

wavinfo.o: wavinfo.c 
wavvol.o : wavvol.c 
wavnorm.o: wavnorm.c
wavrev.o : wavrev.c
wavecho.o: wavecho.c
wavwide.o: wavwide.c
wavcat.o : wavcat.c
wavmix.o : wavmix.c
wavabs.o : wavabs.c

clean:
	rm $(objs)

purge: clean
	rm $(progs)