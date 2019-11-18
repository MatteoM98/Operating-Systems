#include "xerrori.h"

int main(int argc,char *argv[])
{
	int n=3;
	for(int i=0;i<n;i++)
	{
		pid_t p = xfork(__FILE__,__LINE__);
		if(p==0)
		{
			printf("Io sono %d , il %d-esimo figlio di %d\n",getpid(),i,getppid());
			exit(i);
		}
	}
	
	/* i figli ora sono stati creati
	 * e uccisi ora da qua in poi
	 * il codice è solamente del padre*/
	 
	 printf("Io sono %d padre dei precedenti processi e figlio di %d\n",getpid(),getppid());
	 for(int i=0;i<n;i++)
	 {
		 int status;
		 pid_t p = xwait(&status,__FILE__,__LINE__);
		 printf("E' terminata la missione di del figlio %d\n",p);
		 if(WIFEXITED(status))
			printf("Questo figlio ha ucciso: %d draghi\n",WEXITSTATUS(status));
		else
			printf("Figlio non terminato correttamente");
	 }
	 
	 printf("Finito\n");
	 return 0;
}
