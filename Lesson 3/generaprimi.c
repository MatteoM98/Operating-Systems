#include "xerrors.h"
#define K 1000

//prototipi
int scrivi_primi_su_file(char *nome,int n);
bool primo(int n);


int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("Uso: %s nomefile npositivo\n",argv[0]);
		exit(-1);
	}
	
	srand(time(NULL));
	int n = atoi(argv[2]);
	assert(n>0);
	int nprimi = scrivi_primi_su_file(argv[1],n);
	//assert(nprimi==n);
	printf("Finito,sono stati scritti %d numeri primi\n",nprimi);
	return 0;
	
}

int scrivi_primi_su_file(char *nome,int n)
{
	assert(n>0);
	FILE *f = xfopen(nome,"wt",__LINE__,__FILE__);
	int count=0;
	int tot=0;
	while(tot<n)
	{
		int  i = rand()%K; //genero numero random
		
		if(primo(i)) count++; //se i è primo aumento il contatore
		
		fprintf(f,"%d\n",i); //stampo i su file
		
		tot++;	
		
	}
	
	assert(tot==n);
	fclose(f);
	return count;
}

bool primo(int n)
{
  int i;
  if(n<2) return false;
  if(n%2==0) {
    if(n==2)
      return true;
    else
      return false;
  }
  for (i=3; i*i<=n; i += 2) {
      if(n%i==0) {
          return false;
      }
  }
  return true;
}
