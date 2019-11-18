# definizione variabili
CFLAGS=-Wall -O -g -std=c99
CC=gcc

# elenco eseguibili
EXECS=array2 array

all: $(EXECS)

array2: array2.o xerrori.o
	$(CC) $(CFLAGS) -o $@ $^	

array: array.o xerrori.o utils.o
	$(CC) $(CFLAGS) -o $@ $^

# regola generale per i file oggetto
%.o: %.c xerrori.h utils.h
	$(CC) $(CFLAGS) -o $@ -c $<

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h *.py

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
