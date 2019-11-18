#include "xerrors.h"
#define heap_size 1000

//strutture
typedef struct {
  int bytesLeft;                   // bytes attualmente disponibili
  pthread_cond_t cond;      	   // condition variable
  pthread_mutex_t mutex;  		   // mutex associato alla condition variable
} heap;

typedef struct {
	int mem;
	int sec;
}input_Handler1;

//prototipi funzioni
void heap_init(heap *h, int maxSize);
void allocate(heap *h, int size);
void free_mem(heap *h, int size);
void get_heapmemory();
void free_heapmemory();
void Handler_USR1(int s);
void Handler_USR2(int s);
void *funzione_thread(void *arg);

//varibili globali
heap *h;

int main(int argc,char *argv[])
{
	//handler per USR1
	struct sigaction sa;
	sa.sa_handler = Handler_USR1;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1,&sa,NULL);
	
	//handler per USR2
	struct sigaction sa2;
	sa2.sa_handler = Handler_USR2;
	sigemptyset(&sa2.sa_mask);
	sa2.sa_flags = SA_RESTART;
	sigaction(SIGUSR2,&sa2,NULL);
	
	h = (heap *)malloc(sizeof(heap));
	heap_init(h,heap_size);
	
	printf("Se vuoi inviarmi un segnale il mio pid e': %d\n",getpid());
	
	//attesa infinita
	while(true) sleep(1);
	
}


//----------------FUNZIONI----------------
void heap_init(heap *h, int maxSize)
{
	h->bytesLeft = maxSize;
	xpthread_cond_init(&h->cond,NULL,__LINE__,__FILE__);
	xpthread_mutex_init(&h->mutex,NULL,__LINE__,__FILE__); 
}

void allocate(heap *h, int size)
{
	 xpthread_mutex_lock(&h->mutex,__LINE__,__FILE__);
     while (h->bytesLeft < size)  xpthread_cond_wait(&h->cond,&h->mutex,__LINE__,__FILE__);  // aspetto che qualcuno attraverso cond indichi che ci sono novita' su byteLeft 
																		  // il mutex viene rilascato dalla cond_wait
																		  // all'uscita della cond_wait il mutex e' ottenuto   
																		  
     get_heapmemory(size);  // ottiene il puntatore alla zona di memoria
     h->bytesLeft -= size;                 // aggiorna byteLeft
     xpthread_mutex_unlock(&h->mutex,__LINE__,__FILE__);
     
}

void get_heapmemory()
{
	
}

void free_heapmemory()
{
	
}

void free_mem(heap *h, int size)
{
	 xpthread_mutex_lock(&h->mutex,__LINE__,__FILE__);
	 free_heapmemory();          // restituisce la memoria usata
     h->bytesLeft += size;             // modifico byteLeft sotto mutex
									// tutti quelli che stanno aspettando su c sono avvertiti ...
     xpthread_cond_broadcast(&h->cond,__LINE__,__FILE__);    // ... ma non partono subito perche' c'e' il mutex
     xpthread_mutex_unlock(&h->mutex,__LINE__,__FILE__);      // rilascio mutex
}


void Handler_USR1(int s)
{
	printf("Segnale %d ricevuto dal processo %d\n", s, getpid());
	int mem;
	int sec;
	printf("Inserire un valore intero per mem e sec:\n");
	scanf("%d %d", &mem, &sec);
	
	//creo il thread
	pthread_t t;
	
	//inizializzo struct di input
	input_Handler1 iH;
	iH.mem = mem;
	iH.sec = sec;
	
	//lancio il thread
	xpthread_create(&t,NULL,funzione_thread,(void *)&iH,__LINE__,__FILE__);
	xpthread_join(t,NULL,__LINE__,__FILE__);
}

void Handler_USR2(int s)
{
	  xpthread_mutex_lock(&h->mutex, __LINE__, __FILE__);
	  printf("Quantita' di memoria rimanente: %d bytes\n", h->bytesLeft);
	  xpthread_mutex_unlock(&h->mutex, __LINE__,__FILE__);
}

void *funzione_thread(void *arg)
{
	input_Handler1 *iH = (input_Handler1 *) arg;
	printf("Thread cerca di allocare memoria\n");
	allocate(h,iH->mem);
	printf("Thread dorme per %d secondi\n",iH->sec);
	sleep(iH->sec);
	printf("Thread cerca di liberare memoria\n");
	free_mem(h,iH->mem);
	printf("Memoria liberata: termino\n");
	pthread_exit(NULL);
}


