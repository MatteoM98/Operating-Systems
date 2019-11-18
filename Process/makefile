# definizione variabili
CFLAGS=-Wall -O -g -std=c99
CC=gcc

# elenco eseguibili
EXECS=leggisc leggisc_smart fork conta_primi2

all: $(EXECS)
	
leggisc: leggisc.o xerrori.o
	$(CC) $(CFLAGS) -o $@ $^
	
leggisc_smart: leggisc_smart.o xerrori.o
	$(CC) $(CFLAGS) -o $@ $^
	
fork: fork.o xerrori.o
	$(CC) $(CFLAGS) -o $@ $^
	
conta_primi2: conta_primi2.o xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

# regola generale per i file oggetto
%.o: %.c xerrori.h
	$(CC) $(CFLAGS) -o $@ -c $<

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
