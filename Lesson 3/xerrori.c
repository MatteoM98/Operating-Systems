#include "xerrori.h"

// xerrori.c
// collezione di chiamate a funzioni di sistema con controllo output
// i prototipi sono in xerrori.h




void die(const char *s) {
  perror(s);
  exit(1);
}

FILE *xfopen(const char *pathname, const char *mode, const char *file, const int line) {
  FILE *f = fopen(pathname,mode);
  if(f==NULL) {
    perror("Errore apertura file");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),line,file);
    exit(1);
  }
  return f;
}


// processi 

pid_t xfork(const char *file, const int line) {
  pid_t p = fork();
  if(p<0) {
    perror("Errore chiamata fork");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),line,file);
    exit(1);
  }
  return p;
}

pid_t xwait(int *status, const char *file, const int line)
{
  pid_t p = wait(status);
  if(p<0) {
    perror("Errore chiamata wait");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),line,file);
    exit(1);
  }
  return p;
}

int xpipe(int pipefd[2], const char *file, const int line) {
  int e = pipe(pipefd);
  if(e!=0) {
    perror("Errore creazione pipe"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),line,file);
    exit(1);
  }
  return e;
}

// ---------------- memoria condivisa POSIX
int xshm_open(const char *name, int oflag, mode_t mode, int linea, char *file)
{
  int e = shm_open(name, oflag, mode);
  if(e== -1) {
    perror("Errore shm_open"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;  
}

int xshm_unlink(const char *name, int linea, char *file)
{
  int e = shm_unlink(name);
  if(e== -1) {
    perror("Errore shm_unlink"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;  
}

int xftruncate(int fd, off_t length, int linea, char *file)
{
  int e = ftruncate(fd,length);
  if(e== -1) {
    perror("Errore ftruncate"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;  
}


void *simple_mmap(size_t length, int fd, int linea, char *file)
{
  void *a =  mmap(NULL, length ,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(a == (void *) -1) {
    perror("Errore mmap"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return a;
}

//----------SEMAFORI----------

int xsem_init(sem_t *sem, int pshared, unsigned int value, int linea, char *file) {
  int e = sem_init(sem,pshared,value);
  if (e!=0) {
    perror(e, "Errore sem_init");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    sleep(Attesa);  // si mette in attesa per non terminare subito gli altri thread 
    exit(1);
  }
  return e;
}

int xsem_post(sem_t *sem, int linea, char *file) {
  int e = sem_post(sem);
  if (e!=0) {
    perror(e, "Errore sem_post");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    sleep(Attesa);  // si mette in attesa per non terminare subito gli altri thread 
    exit(1);
  }
  return e;
}

int xsem_wait(sem_t *sem, int linea, char *file) {
  int e = sem_wait(sem);
  if (e!=0) {
    perror(e, "Errore sem_wait");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    sleep(Attesa);  // si mette in attesa per non terminare subito gli altri thread 
    exit(1);
  }
  return e;
}
