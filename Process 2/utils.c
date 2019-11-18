#include "utils.h"

int *random_array(int n)
{
  assert(n>0);
  int *a = malloc(n* sizeof(int));
  assert(a!=NULL);
  for(int i=0;i < n;i++)
    a[i] = (int) random() % 2000;
  return a;
}

int intcmpadre(const void *a, const void *b)
{
  if(*((int *)a) <1000 ||  *((int *)b)<1000)
	return *((int *) a) - *((int *) b);
  else return 0;
}

int intcmpfiglio(const void *a, const void *b)
{
  if(*((int *)a) >=1000 ||  *((int *)b)>=1000)
	return *((int *) a) - *((int *) b);
  else return 0;
}

void stampa_array(int *a,int n)
{
	assert(n>0);
	for(int i=0;i<n;i++) printf("%d\n ",a[i]);
	
}

// verifica che array a[0..n] sia ordinato 
bool check_sort(int *a, int n)
{
  for(int i=0; i < n-1; i++)
     if(a[i] > a[i+1]) return false;
  return true;
}
