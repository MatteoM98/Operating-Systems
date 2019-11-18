# definizione variabili
CFLAGS=-Wall -O -g -std=c99
LDFLAGS=-pthread -lrt
CC=gcc

# elenco eseguibili
EXECS=quicksort

all: $(EXECS)

quicksort: quicksort.o xerrors.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# regola generale per i file oggetto
%.o: %.c xerrors.h
	$(CC) $(CFLAGS) -o $@ -c $<

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h *.py

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
