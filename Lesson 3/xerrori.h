#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>    

// prototipi di xerrori.c

// stampa messaggio d'errore e termina processo
void die(const char *s); 

// operazioni su file  
FILE *xfopen(const char *pathname, const char *mode, const char *file, const int line);

// operazioni su processi
pid_t xfork(const char *file, const int linea);
pid_t xwait(int *status, const char *file, const int linea);
// pipe
int xpipe(int pipefd[2], const char *file, const int linea);

//funzioni memoria condivisa 
int xshm_open(const char *name, int oflag, mode_t mode, int linea, char *file);
int xshm_unlink(const char *name, int linea, char *file);
int xftruncate(int fd, off_t length, int linea, char *file);
void *simple_mmap(size_t length, int fd, int linea, char *file);

//funzioni semafori
int xsem_init(sem_t *sem, int pshared, unsigned int value, int linea, char *file);
int xsem_post(sem_t *sem, int linea, char *file);
int xsem_wait(sem_t *sem, int linea, char *file);
