#include "xerrori.h"

//prototipi funzioni
int *random_array(int n);
int intcmp(const void *a,const void *b);
void stampa_array(int *a,int n);

int main(int argc,char *argv[])
{
	if(argc!=2) //verifico input
	{
		printf("Uso: %s npositivo\n",argv[0]);
		exit(-1);
	}
	
	int n = atoi(argv[1]);
	if(n<=0) die("N deve essere maggiore di 0\n"); //verifico che n sia maggiore di 0
	assert(n>0);
	int *a = random_array(n); //creo array random di dimensione n
	assert(a!=NULL);
	stampa_array(a,n); //stampo array prima di ordinarlo
	printf("Ordinamento:\n");
	
	//creazione pipe
	int p0[2];
	int p1[2];
	xpipe(p0,__FILE__,__LINE__);
	xpipe(p1,__FILE__,__LINE__);
	
	for(int i=0;i<2;i++)
	{
		pid_t pid = xfork(__FILE__,__LINE__);
		if(pid==0) //codice dei figli
		{
			if(i==0) //codice primo figlio
			{
				close(p0[0]); //chiudo le pipe non usate
				close(p1[0]);
				close(p1[1]);
				qsort(a,n/2,sizeof(int),intcmp); //ordino la prima meta dell'array
				
				for(int i=0;i<(n/2);i++) {
				  ssize_t e = write(p0[1],&a[i],sizeof(int)); //scrivo la prima metà dell'array sulla pipe
				  if(e!=sizeof(int)) die("Errore scrittura figlio 0\n");
				}
				
				printf("Processo figlio0 terminato\n");
				close(p0[1]); //chiudo pipe per la scrittura
				exit(0); //termimo processo figlio
				
			}else{ //codice secondo figlio
				
				close(p0[0]); //chiudo pipe che non uso
				close(p1[0]);
				close(p0[1]);
				qsort(a+(n/2),n-(n/2),sizeof(int),intcmp); //ordino seconda metà dell'array
				
				for(int i=n/2;i<n;i++) {
				  ssize_t e = write(p1[1],&a[i],sizeof(int)); //scrivo nella pipe la seconda metà dell'array
				  if(e!=sizeof(int)) die("Errore scrittura figlio 1\n");
				}
				
				printf("Processo figlio1 terminato\n");
				close(p1[1]);
				exit(0); //termino secondo processo
				
			}
		}
	}
	
	//codice processo padre
	
	close(p0[1]); //chiudo pipe che non uso
	close(p1[1]);
	int *arr = malloc(n*sizeof(int)); //alloco array dove salvare i valori letti da pipe
	assert(arr!=NULL);
	int x0,x1;
	ssize_t num0 = read(p0[0],&x0,sizeof(int));  //leggo il primo valore da ognuna delle pipe
	ssize_t num1 = read(p1[0],&x1,sizeof(int));
	
	for(int i=0;i<n;i++)
	{
		if(x0<=x1) //se il valore letto dalla pipe p0 è minore o uguale di quello letto dalla pipe p1
		{
			if(num0!=0) //e se la pipe p0 non è vuota 
			{
				arr[i]=x0; 		//scrivo valore sull'array
			    num0 = read(p0[0],&x0,sizeof(int));		//leggo un altro valore
			}else //altrimenti se la pipe p0 è vuota (num0==0) leggo dall'altra pipe
			{
				arr[i]=x1;
				num1 = read(p1[0],&x1,sizeof(int));
				
			}
		}else //se il valore letto dalla pipe p1 è minore di quello letto dalla pipe p0
		{
			if(num1!=0) //e se la pipe p1 non è vuota 
			{
				arr[i]=x1;
				num1 = read(p1[0],&x1,sizeof(int));
			}else //altrimenti se la pipe p1 è vuota (num1==0) leggo dall'altra pipe
			{
				arr[i]=x0;
			    num0 = read(p0[0],&x0,sizeof(int));
			}
		}
	}
	
	
	close(p0[0]);
	close(p1[0]); 		//chiudo le pipe
	stampa_array(arr,n);	//stampo array ordinato
	free(arr);		 //libero la memoria
	free(a);		 //libero la memoria
	return 0;	
		
}

int *random_array(int n)
{
	assert(n>0);
	int *a = malloc(n*sizeof(int));
	assert(a!=NULL);
	for(int i=0;i<n;i++) a[i] = (int) random() % 2000;
	return a;
}

int intcmp(const void *a,const void *b)
{
	return *((int *)a) - *((int *)b);
}

void stampa_array(int *a,int n)
{
	for(int i=0;i<n;i++) printf("%d\n",a[i]);
}
