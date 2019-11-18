#include "xerrors.h"

//prototipi
int *random_array(int n);
int *copy_array(int *a, int n);
bool check_sort(int *a, int *b, int n);
int intcmp(const void *a, const void *b);
void quicksort(int a[], int, int );
int partition(int A[], int p, int r);


int main(int argc, char *argv[])
{
	if(argc!=4)
	{
		printf("Uso: %s numel numth limite\n",argv[0]);
		exit(-1);
	}
	
	srand(time(NULL)); 
    int dimArray = atoi(argv[1]);
    int nThread = atoi(argv[2]);
    int limite = atoi(argv[3]);
    
    int *array = random_array(dimArray);
    
  // copia array e ordina la copia usando la funzione di libreria
    int *array_copia = copy_array(array,dimArray);
    qsort(array_copia,dimArray,sizeof(int),intcmp);
  
  
  
  // eseguo il mio algoritmo di ordinamento
  
  
  
  
  
  // confronto a[] ordinato da me con copia[]
 
}



//-------FUNZIONI---------


int *random_array(int n)
{
  assert(n>0);
  int *a = malloc(n* sizeof(int));
  assert(a!=NULL);
  for(int i=0;i<n;i++)
    a[i] = (int) random() % 1000000;
  return a;
}

int *copy_array(int *a, int n)
{
  int *b = (int *) malloc(n* sizeof(int));
  assert(b!=NULL);
  for(int i=0;i<n;i++)
    b[i] = a[i];
  return b;
}

bool check_sort(int *a, int *b, int n)
{
  for(int i=0;i<n;i++)
     if(a[i]!=b[i]) return false;
  return true;
}

int intcmp(const void *a, const void *b)
{
  return *((int *) a) - *((int *) b);
}


void quicksort(int *A, int p, int r)       {
  int q;
  
  if(p<r) {  // se p==r  c'e'un solo elemento e quindi non si deve ordinare
    q = partition(A,p,r);
    quicksort(A,p,q);
    quicksort(A,q+1,r);
  }
}


/* ***********************************************************
   partition con scelta pivot mediano di 3 
   per evitare l'uso di random che e' lenta e non thread safe
   *********************************************************** */ 
int partition(int A[], int p, int r)
{
  int t,x;
  int i,j;

  // il pivot sara' il mediano tra A[p],A[i],A[r]
  i = (p+r)/2;
  if( A[p] > A[i] )
      {t=A[i];A[i]=A[p];A[p]=t;} // scambia A[i]<->A[p] 
  if( A[p] > A[r] )
      {t=A[r];A[r]=A[p];A[p]=t;} // scambia A[r]<->A[p], ora A[p] e' il minino
  if( A[i] > A[r] )
      {t=A[r];A[r]=A[i];A[i]=t;} // scambia A[r]<->A[i], ora A[p]<= A[i] <= A[r]
  
  x = A[i];                   // pivot
  t=A[i];A[i]=A[p];A[p]=t;    // scambia A[i]<->A[p]      

  i= p-1; j= r+1;
  while(1) {
    while(A[--j]>x) ; // scan da destra a sinistra: esce se A[j]<=x
    while(A[++i]<x) ; // scan da sinistra a destra: esce se A[i]>=x
    if(i<j) {         
      t=A[i]; A[i]=A[j]; A[j]=t;  // scambia A[i] <-> A[j] e riprendi scansioni
    }
    else break;       // se i>=j termina
  } 
  return j;
}

