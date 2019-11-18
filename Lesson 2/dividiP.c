#include "xerrori.h"

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("Uso: %s nomefile",argv[0]);
		exit(-1);
	}
	
	FILE *f = xfopen(argv[1],"rt",__FILE__,__LINE__); //apro il file in lettura
	
	int p0[2]; //creo pipe per il processo p0 
	int p1[2]; //creo pipe per il processo p1
	xpipe(p0,__FILE__,__LINE__);
	xpipe(p1,__FILE__,__LINE__);
	
	
	for(int i=0;i<2;i++)
	{
		pid_t pid = xfork(__FILE__,__LINE__);
		if(pid==0) //codice dei figli
		{
			
			if(i==0) //codice del primo figlio p0
			{
				close(p0[1]); //p0 deve solo leggere i valori
				close(p1[1]); //chiudo tutte le altre pipe
				FILE *f0 = xfopen(strcat(argv[1],"pari"),"wt",__FILE__,__LINE__);
				int tot=0;
				while(true)
				{
					int x;
					ssize_t e = read(p0[0],&x,sizeof(int));
					if(e==0) break;
					tot+=x;
					fprintf(f0,"%d\n",x);
				}
				fclose(f0); //chiudo il file creato
				printf("La somma del processo p0 e': %d\n",tot); 
				close(p0[0]); //ho finito anche di leggere ora posso chiudere
				exit(0);
				
			}else //codice del secondo figlio p1
			{
				
				close(p1[1]); //p1 deve solo leggere i valori
				close(p0[1]); //chiudo tutte le altre pipe
				FILE *f1 = xfopen(strcat(argv[1],"dispari"),"wt",__FILE__,__LINE__);
				int tot=0;
				while(true)
				{
					int x;
					ssize_t e = read(p1[0],&x,sizeof(int));
					if(e==0) break;
					tot+=x;
					fprintf(f1,"%d\n",tot);
				}
				
				fclose(f1); //chiudo il file creato
				printf("La somma del processo p1 e': %d\n",tot); 
				close(p1[0]); //ho finito anche di leggere ora posso chiudere
				exit(0);
				
			}
		}
		
	}
	
	//codice del padre
	close(p0[0]);  //il padre deve solo scrivere
	close(p1[0]);  //il padre deve solo scrivere
	
	while(true)
	{
		int x;
		int e = fscanf(f,"%d",&x);
		if(e==0 || e==EOF) break;
		if(x%2==0) //se il numero letto è pari
		{
			ssize_t ex = write(p0[1],&x,sizeof(int));
			if(ex!=sizeof(int)) die("Errore scrittura pipe p0 (padre)\n");
			
		}else
		{
			ssize_t ex = write(p1[1],&x,sizeof(int));
			if(ex!=sizeof(int)) die("Errore scrittura pipe p0 (padre)\n");
		}
	}
	
	close(p0[1]);
	close(p1[1]);
	fclose(f);
	printf("Fine scrittura del padre\n"); 
	return 0;
	
	
}
