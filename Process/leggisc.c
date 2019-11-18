#include "xerrori.h"

//prototipi funzioni
void die(const char *s);
int leggi_interi(char *nome);

int main(int argc,char *argv[])
{
	if(argc!=2)  //verifico input
	{
		printf("Uso: %s nomefilebinario\n",argv[0]);
		exit(1);
	}
	
	int somma = leggi_interi(argv[1]); //calcolo somma 
	printf("La somma dei numeri e': %d\n",somma); //stampo somma
	return 0;
	
}

int leggi_interi(char *nome)
{
	int fd = open(nome,O_RDONLY,0777);  //apre file in lettura
	if(fd<0) die("Errore creazione file");
	off_t size = lseek(fd,0,SEEK_END); //sposto offset alla fine e recupero la dimensione del file descriptor
	int n = (size/sizeof(int)); //ottengo numero di numeri interi
	
	int addendo; //variabile dove salvo il numero da sommare volta per volta
	int sum=0;  //variabile che contiene la somma
	lseek(fd,0,SEEK_SET); //risposto l'offset all'inizio del file
	for(int i=0;i<n;i++) {
		ssize_t e = read(fd,&addendo,sizeof(int)); //,leggo dal file descr. (fd) 
		if(e!=sizeof(int)) die("Errore lettura da file");
		sum += addendo; //sommo
	}
	assert(sum == ((n*(n+1))/2)); //verifico la correttezza della somma
	int e = close(fd); //chiudo il file descriptor
	if(e!=0) die("Errore chiusura file");	
	return sum;
}

void die(const char *s)
{
	perror(s);
	exit(1);
}
