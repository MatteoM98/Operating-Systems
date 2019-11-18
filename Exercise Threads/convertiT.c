#include "xerrors.h"
#define Buf_size 10
#define Max_line_len 100

// il thread principale legge una riga alla volta da infile la copia in una stringa  il puntatore a questa stringa viene passato a t2
typedef struct{
	char **buf;
	sem_t *sem_data_items;
	sem_t *sem_free_slots;
	FILE *f;
	
}dati;

void convertiStringa(char *s){
	for(int i=0;i<strlen(s);i++){
		s[i] = (islower(s[i])) ? toupper(s[i]) : tolower(s[i]);
	}
}

// thread t3 consumatore
void *tbody2(void *arg){
	dati *d = (dati *)arg;
	int index = 0;
	while(1){
		xsem_wait(d->sem_data_items,__LINE__,__FILE__);
		char *s = d->buf[index++ % Buf_size];
		xsem_post(d->sem_free_slots,__LINE__,__FILE__);
		if(s==NULL) break;
		fputs(s,d->f);
		free(s);
	}
	
	pthread_exit(NULL);	
}

// thread t2 consumatore e produttore
// crea il thread t3 e poi svolge i suoi compiti
void *tbody1(void *arg){
	dati *d = (dati *)arg;
	int aindex = 0, bindex = 0;
	pthread_t t;
	dati b;
	char *buff[Buf_size];
	// creo i semafori per t3 e li inizializzo
	sem_t sem_data_items_2;
	sem_t sem_free_slots_2;
	xsem_init(&sem_data_items_2,0,0,__LINE__,__FILE__);
	xsem_init(&sem_free_slots_2,0,Buf_size,__LINE__,__FILE__);
	b.buf = buff;
	b.sem_data_items= &sem_data_items_2;
	b.sem_free_slots= &sem_free_slots_2;
	b.f = d->f;
	xpthread_create(&t,NULL,tbody2,(void *)&b,__LINE__,__FILE__);
	// ora t2 svolge i suoi compiti
	while(1){
		xsem_wait(d->sem_data_items,__LINE__,__FILE__);
		char *s = d->buf[aindex++ % Buf_size];
		xsem_post(d->sem_free_slots,__LINE__,__FILE__);
		if(s!=NULL){
			convertiStringa(s);
		}
		xsem_wait(&sem_free_slots_2,__LINE__,__FILE__);
		buff[bindex++ % Buf_size] = s;
		xsem_post(&sem_data_items_2,__LINE__,__FILE__);
		if(s==NULL) break;
	}
	// attendo il thread t3
	xpthread_join(t,NULL,__LINE__,__FILE__);	
	pthread_exit(NULL);
}




int main(int argc, char **argv){
	if(argc!=3){
		fprintf(stderr,"\n\tUsage: %s [infile] [outfile]",argv[0]);
		exit(1);
	}
	FILE *f = fopen(argv[1],"rt");
	FILE *g = fopen(argv[2],"wt");
	
	sem_t sem_data_items_1;
	sem_t sem_free_slots_1;
	
	// inizializzo i semafori
	xsem_init(&sem_data_items_1,0,0,__LINE__,__FILE__);
	xsem_init(&sem_free_slots_1,0,Buf_size,__LINE__,__FILE__);
	char *buffer_1[Buf_size];
	int pindex = 0;
	pthread_t t;
	dati a;
	a.buf = buffer_1;
	a.sem_data_items = &sem_data_items_1;
	a.sem_free_slots = &sem_free_slots_1;	
	a.f = g;
	xpthread_create(&t,NULL,tbody1,(void *)&a,__LINE__,__FILE__);
	char *str;
	while(1) {
		str = NULL;
		size_t siz;
		int nchar = getline(&str,&siz,f);
		if(nchar<0 || strlen(str)>Max_line_len) {
			if(str) free(str);
			str=NULL;
		}
		
		xsem_wait(&sem_free_slots_1,__LINE__,__FILE__);
		buffer_1[pindex % Buf_size] = str;
		pindex += 1;
		xsem_post(&sem_data_items_1,__LINE__,__FILE__);
		if(nchar==EOF) break;
	}
	fclose(f);
    
    // attendo il thread t2
	xpthread_join(t, NULL,__LINE__,__FILE__);
	
	fclose(g);
	
	return 0;
}
