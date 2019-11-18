#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include "xerrors.h"

#define Nome1 "/contaprimi_sem"

typedef struct{
	
	int start;
	int end;
	int *max;
	int *array;
	sem_t *sembuffer;
	
}dati;

// prototipi di funzione
int *random_array(int n);
int massimo(int *a, int n, int p);
void *tbody (void *arg);

// main
int main(int argc, char **argv)
{
  if(argc!= 3)
  {
    fprintf(stderr,"Uso:\n\t %s n p\n",argv[0]);
    exit(1);
  }
  
  srand(time(NULL));
  int n= atoi(argv[1]);
  assert(n > 0);
  int p = atoi(argv[2]);
  assert(p > 0);

  int *a = random_array( n );
  int max = a[0];
  for(int i=1; i < n ;i++)
    if(a[i]>max) max=a[i];
      
  printf("Massimo calcolato senza thread ausiliari: %d\n",max); 
  
  // questa e' la funzione da scrivere per esercizio
   int max2 = massimo(a,n,p);
  printf("Massimo calcolato con %d thread: %d\n",p, max2); 
  
  assert(max==max2);

  free(a);
  return 0;
}

// calcola il massimo dell'array a[] di n elementi
// utilizzando p thread
// ogni thread deve leggere al piu' (n/p)+1 elementi 
int massimo(int *a, int n, int p) {
    
    int max=a[0];
    dati d[p];
    pthread_t t[p];
    int start=0;
    int da_leggere = (n/p);
    int end = da_leggere;
    
      // ---- memoria condivisa per i semafori
  int shm_size1 = sizeof(sem_t); // numero di byte necessari
  int fd1 = xshm_open(Nome1,O_RDWR | O_CREAT, 0600,__LINE__,__FILE__);
  xftruncate(fd1, shm_size1, __LINE__,__FILE__);
  sem_t *sem = simple_mmap(shm_size1,fd1, __LINE__,__FILE__);
  close(fd1); // dopo mmap e' possibile chiudere il file descriptor
  sem_t *sembuffer = &sem[0];  // oppure  = sem 
  xsem_init(sembuffer,0,1,__LINE__, __FILE__);
  
  
    for(int i=0;i<p;i++)
    {

		d[i].start = start;
		start+=da_leggere;
		if(i==p-1)
		{
			end = n;
			d[i].end = end;
	    }else
	     {
			 d[i].end = end;
			 end+=da_leggere;
		 }
		d[i].max = &max;
		d[i].array = a;
		d[i].sembuffer = sembuffer;
		
		xpthread_create(&t[i],NULL,tbody,&d[i],__LINE__,__FILE__);
	}
	
	for(int i=0;i<p;i++) xpthread_join(t[i],NULL,__LINE__,__FILE__);
	
	//unmap e rimozione condivisa
	
	munmap(sem,shm_size1);
	xshm_unlink(Nome1,__LINE__,__FILE__);
	
	
	return max;
	
}

void *tbody (void *arg)
{
	dati *d = (dati *) arg;
	xsem_wait(d->sembuffer,__LINE__, __FILE__);
	for(int i = d->start;i<d->end;i++)
	{
		if(d->array[i] > *(d->max))
		{
			*(d->max) = *(d->array+i);
		}
		
	}
	xsem_post(d->sembuffer,__LINE__, __FILE__);
	
	pthread_exit(NULL);
}

// genera array di n elementi con interi random tra -100000 e 100000
int *random_array(int n)
{
  assert(n>0);
  int *a = malloc(n* sizeof(int));
  assert(a!=NULL);
  for(int i=0; i < n ;i++)
    a[i] = (random()%200001) - 100000;
  return a;
}
