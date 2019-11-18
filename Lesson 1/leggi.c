#include "xerrori.h"

//prototipi
void die(const char *s);
int *leggi_interi(char *nome,int *dim);
int somma_array(int *array,int n);

int main(int argc,char *argv[])
{
	if(argc!=2) //verifico input 
	{
		fprintf(stderr,"Uso: %s nomefile",argv[0]);
		exit(1);
	}
	int dim;
	int *array = leggi_interi(argv[1],&dim); 
	int somma = somma_array(array,dim);
	fprintf(stdout,"La somma dei numeri contenuti nel file e': %d\n",somma);
	return 0;
	

}

void die(const char *s)
{
	perror(s);
	exit(1);
}

int *leggi_interi(char *nome,int *dim)
{
	FILE *f = xfopen(nome,"rb",__FILE__,__LINE__); //apro file da cui leggere il contenuto
	assert(f!=NULL);
	fseek(f,0,SEEK_END);  	//sposto il puntatore alla fine del file
	long numero_bytes = ftell(f); 	//ritorno la dimensione in byte del file
	fseek(f,0,SEEK_SET); 	//risposto all'inizio il puntatore del file
	size_t n_numeri = (numero_bytes/sizeof(int)); 	//ottengo il numero dei numeri (int) contenuti nel file
	int *array = malloc(n_numeri*sizeof(int));	 //alloco un array di dimensione n_numeri*la dimensione di un singolo int
	size_t e = xfread(array,sizeof(int),n_numeri,f,__FILE__,__LINE__);  //leggo da file e salvo nell' array allocato prima gli elementi (int) contenuti nel file
	assert(e==n_numeri);
	if(fclose(f)!=0) die("Errore chiusura file");	//chiudo il file
	*dim = e;	//ritorno la dimensione del mio array
	return array; //ritorno l'array
}

int somma_array(int *array,int n) //sommo gli elementi dell'array
{
	int somma=0;
	for(int i=0;i<n;i++)
	{
		somma += array[i];
	}
	
	return somma;
}
