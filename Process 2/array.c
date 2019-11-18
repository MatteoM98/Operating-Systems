#include "utils.h"
#include "xerrori.h"

int main(int argc,char *argv[])
{
	if(argc!=2)  //verifico correttezza input 
	{
		printf("Uso: %s npositivo\n",argv[0]);
		exit(-1);
	}
	
	int n = atoi(argv[1]);
	if(n<=0) die("Inserire numero maggiore di zero\n"); //verifico intero passato in input
    
    int *arr = random_array(n); //genero array con valori random
    int up[2]; 					
    xpipe(up,__FILE__,__LINE__);  //creo pipe
    qsort(arr,n,sizeof(int),intcmpadre); //ordino gli elementi <1000
    
    pid_t pid = xfork(__FILE__,__LINE__); 	//genero processo figlio
	if(pid==0) 	//codice del processo figlio
	{
		close(up[0]);
		qsort(arr,n,sizeof(int),intcmpfiglio); //faccio ordinare gli elementi >=1000
		int s=0;
		while(arr[s]<1000) s++;  //finchè gli elementi sono <1000 scorro(perchè sono quelli ordinati già dal padre
		ssize_t e = write(up[1],arr+s,sizeof(int)*(n-s)); //passo tutto in una volta 
		assert(e==sizeof(int)*(n-s));
		
		/*for(int i=s;i<n;i++)
		{
			ssize_t e = write(up[1],&arr[i],sizeof(int)); 	//passo alla pipe gli elementi ordinati prima dal processo figlio
			if(e!=sizeof(int)) die("Errore scrittura\n");
		}*/
		close(up[1]);
		exit(0); 	//esco dal processo figlio
	}
	
	
	close(up[1]);
	int i=0;
	while(arr[i]<1000) i++;  //scorro gli elementi ordinati dal padre 
	ssize_t e = read(up[0],arr+i,sizeof(int)*(n-i)); //passo tutto in una volta 
	assert(e==sizeof(int)*(n-i));
	/*while(true)
	{
		int val;
		ssize_t e = read(up[0],&val,sizeof(int));  //scrivo da qua in poi nell'array gli elementi ordinati dal figlio >= 1000
		if(e==0) break;
		arr[i] = val;
		i++;
	}*/
	close(up[0]);
	if(!check_sort(arr,n)) die("Errore");
	stampa_array(arr,n); //stampo array
	return 0;
	
	
}

