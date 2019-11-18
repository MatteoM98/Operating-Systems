#include "xerrors.h"


typedef struct {
  int bytesLeft;                   // bytes attualmente disponibili
  pthread_cond_t cond;      // condition variable
  pthread_mutex_t mutex;  // mutex associato alla condition variable
} heap;

heap *h;

typedef struct{
  heap *h;
  int mem;
  int sec;
	
}datit;



void get_heapmemory(){}   
void free_heapmemory(){}

void heap_init(heap *h, int maxSize){
	
	h->bytesLeft = maxSize;
	pthread_cond_init(&h->cond,NULL);
	pthread_mutex_init(&h->mutex,NULL);
	
}

void allocate(heap *h, int size){
	
	xpthread_mutex_lock(&h->mutex,__LINE__,__FILE__);
	while(h->bytesLeft < size){
		xpthread_cond_wait(&h->cond, &h->mutex, __LINE__,__FILE__);
	}
	get_heapmemory();
	h->bytesLeft -= size;
	xpthread_mutex_unlock(&h->mutex, __LINE__,__FILE__);
}


void free_mem(heap *h, int size){
	
	xpthread_mutex_lock(&h->mutex, __LINE__,__FILE__);
	free_heapmemory();
	h->bytesLeft += size;
	xpthread_cond_broadcast(&h->cond,__LINE__,__FILE__);
	xpthread_mutex_unlock(&h->mutex, __LINE__,__FILE__);
}

void *tbody(void *arg){
	
	datit *data = (datit *)arg;
	allocate(data->h, data->mem);
	sleep(data->sec);
	free_mem(data->h, data->mem);
	pthread_exit(NULL);
	
}


// funzione che viene invocata quando viene ricevuto un segnale USR1 USR2 o INT (Control-C)
void handler(int s){
  printf("Segnale %d ricevuto dal processo %d\n", s, getpid());
  if(s==SIGUSR1) {
	int mem, sec;
    printf("Inserire due interi, memoria e secondi:\n");
    scanf("%d %d", &mem, &sec);
    pthread_t t;
    datit data;
    data.h = h;
    data.mem = mem;
    data.sec = sec;
    xpthread_create(&t, NULL, tbody, (void*)&data,__LINE__,__FILE__);
    xpthread_join(t,NULL,__LINE__,__FILE__);
  }
  if(s==SIGUSR2){
	  xpthread_mutex_lock(&h->mutex, __LINE__, __FILE__);
	  printf("Quantita' di memoria rimanente: %d bytes\n", h->bytesLeft);
	  xpthread_mutex_unlock(&h->mutex, __LINE__,__FILE__);
  }
}

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Uso: %s [maxSize]", argv[0]);
		exit(1);
	}
	
	h = malloc(sizeof(heap));
	heap_init(h, atoi(argv[1]));
	
	// definisce signal handler 
	struct sigaction sa;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);     // setta a "insieme vuoto" sa.sa_mask maschera di segnali da bloccare
	sa.sa_flags = SA_RESTART;     // restart system calls  if interrupted
	sigaction(SIGUSR1,&sa,NULL);  // handler per USR1
	sigaction(SIGUSR2,&sa,NULL);  // handler per USR2
	sigaction(SIGINT,&sa,NULL);   // handler per Control-C
	printf("Numero del processo corrente: %d\n", getpid());
	
	while(true) pause();
	
	return 0;
}
