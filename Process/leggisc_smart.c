#include "xerrori.h"

//prototipi funzioni
void die(const char *s);
int *leggi_Arrayinteri(char *nome,int *dimensione);
int somma_array(int *array,int n);

int main(int argc,char *argv[])
{
	if(argc!=2)  //verifico input
	{
		printf("Uso: %s nomefilebinario\n",argv[0]);
		exit(1);
	}
	
	int n;
	int *array = leggi_Arrayinteri(argv[1],&n);
	int somma = somma_array(array,n);
	printf("La somma dei numeri e': %d\n",somma); //stampo somma
	return 0;
	
}

int *leggi_Arrayinteri(char *nome,int *dimensione)
{
	int fd = open(nome,O_RDONLY,0777);  //apre file in lettura
	if(fd<0) die("Errore creazione file");
	off_t size = lseek(fd,0,SEEK_END); //sposto offset alla fine e recupero la dimensione del file descriptor
	int n = (size/sizeof(int)); //ottengo numero di numeri interi
	int *array = malloc(n*sizeof(int));
	if(array==NULL) die("Malloc fallita");
	lseek(fd,0,SEEK_SET); //risposto l'offset all'inizio del file
	ssize_t e = read(fd,array,n*sizeof(int));
	if(e!=n*sizeof(int)) die("Errore lettura da file");	
	int f = close(fd); //chiudo il file descriptor
	if(f!=0) die("Errore chiusura file");	
	*dimensione = n;
	return array;
}

void die(const char *s)
{
	perror(s);
	exit(1);
}

int somma_array(int *array,int n)
{
	int somma=0;
	for(int i=0;i<n;i++) somma += array[i];
	assert(somma==((n*(n+1))/2));
	return somma;
}
