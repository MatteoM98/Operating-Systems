#include "xerrori.h"
#define MAX 255
//prototipi
void die(const char *s);
bool primo(int n);

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("Uso: %s npositivo\n",argv[0]);
		exit(1);
	}
	
	int n = atoi(argv[1]);
	
	if(n<=0)
	{
		printf("Il numero deve essere maggiore o uguale a 1\n");
		exit(1);
	}
	
	pid_t p = xfork(__FILE__,__LINE__);
	
	if(p==0) //codice del figlio
	{
	  int count=0;
		for(int i=(n/2)+1;i<n;i++)
		{
			if(primo(i)) count+=1;
		}
	   if(count>MAX)
	   {
			fprintf(stderr,"Il processo figlio %d ha contato più di 255 numeri primi\n",getpid());
			exit(-1);
	   }
	   exit(count); //termina il figlio restituendo il count 
	}
	
	int status;
	pid_t x = xwait(&status,__FILE__,__LINE__);
	printf("Terminato il processo figlio %d\n",x);
	int count=0;
	if(WIFEXITED(status))
	{
		if(WEXITSTATUS(status)==MAX) die("Figlio terminato non correttamente");
		for(int i=1;i<(n/2);i++)
		{
			if(primo(i)) count+=1;
		}
		
	}
	
	printf("Numeri primi calcolati dal figlio: %d\n",WEXITSTATUS(status));
	printf("Numeri primi calcolati dal padre: %d\n",count);
	printf("Numeri primi compresi fa 1 e %d:  %d\n",n,count+WEXITSTATUS(status));
	return 0;

}

bool primo(int n)
{
	int i;
	
	if(n<2) return false;
	
	if(n%2==0)
	{
		if(n==2) return true;
		else return false;
	}
	
	for(i=3;i*i<=n;i+=2)
	{
		if(n%i==0) return false;
	}
	
	return true;
}

void die(const char *s)
{
	perror(s);
	exit(-1);
}
