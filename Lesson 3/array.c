#include "xerrors.h"
#define Nome "/contaprimi_int"
#define Nome1 "/contaprimi_sem"

//prototipi
void stampa_array(int *a,int n);
bool check_sort(int *a, int n);
int intcmp(const void *a,const void *b);

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("Uso: %s npositivo\n",argv[0]);
		exit(-1);
	}
	
	int n = atoi(argv[1]);
	if(n<=0) die("Inserire numero maggiore di zero\n");
	
	
	// ---- creazione array memoria condivisa
  int shm_size = (3*n)*sizeof(int); // numero di byte necessari
  int fd = xshm_open(Nome,O_RDWR | O_CREAT, 0600,__LINE__,__FILE__);
  xftruncate(fd, shm_size, __LINE__,__FILE__);
  int *array = simple_mmap(shm_size,fd, __LINE__,__FILE__);
  close(fd); // dopo mmap e' possibile chiudere il file descriptor
  
  // ---- memoria condivisa per i semafori
  int shm_size2 = sizeof(sem_t); // numero di byte necessari
  int fd2 = xshm_open(Nome1,O_RDWR | O_CREAT, 0600,__LINE__,__FILE__);
  xftruncate(fd2, shm_size2, __LINE__,__FILE__);
  sem_t *sem = simple_mmap(shm_size2,fd2, __LINE__,__FILE__);
  close(fd2); // dopo mmap e' possibile chiudere il file descriptor
  sem_t *sem_buffer = &sem[0];  // oppure  = sem 
  xsem_init(sem_buffer,1,1,__LINE__, __FILE__);
  
  srand(time(NULL));
  
  for(int i=0;i < n;i++)  //riempo array con valori random
    array[i] = (int) rand() % 2000;
  
  int k = n;
  
 for(int i=0;i<n;i++) //riempo zona array padre
 {
	 if(array[i]<1000)
	 {
		 array[k] = array[i];
		 k++;
	 }
 }
 
 int j=k;
 
 for(int i=0;i<n;i++) //riempo zona array figlio
 {
	  if(array[i]>=1000)
	 {
		 array[j] = array[i];
		 j++;
	 }
 }
  
  
  
  pid_t pid = xfork(__LINE__,__FILE__);
  if(pid==0) //codice del figlio
  {
	  xsem_wait(sem_buffer,__LINE__,__FILE__);
	  qsort(array+k,j-k,sizeof(int),intcmp);  //ordino zona array figlio
	  xsem_post(sem_buffer,__LINE__,__FILE__);
      xsem_wait(sem_buffer,__LINE__,__FILE__);
	  memcpy(array+(k-n),array+k,(j-k)*sizeof(int)); //copio memoria del figlio sull'array principale
	  xsem_post(sem_buffer,__LINE__,__FILE__);
	  exit(0);
	  
	  
  }
  
  
 xsem_wait(sem_buffer,__LINE__,__FILE__);
 qsort(array+n,k-n,sizeof(int),intcmp); //ordino array padre
 memcpy(array,array+n,(k-n)*sizeof(int));  //copio memoria del padre sull'array principale  //memcpy(array,array+n,n*sizeof(int)); e  non far copiare al figlio
 xsem_post(sem_buffer,__LINE__,__FILE__);
 int status;
 pid_t w = xwait(&status,__LINE__,__FILE__);
 assert(status==0 && w!=-1);
 check_sort(array,n);
 stampa_array(array,n);
 
 
 // unmap e rimozione memoria condivisa
  munmap(array,shm_size);
  munmap(sem,shm_size2);
  xshm_unlink(Nome,__LINE__, __FILE__);
  xshm_unlink(Nome1,__LINE__, __FILE__);
 return 0;	
	
}

void stampa_array(int *a,int n)
{
	assert(n>0);
	for(int i=0;i<n;i++) printf("%d\n",a[i]);
	
}

int intcmp(const void *a,const void *b)
{
	return *((int *)a) - *((int*)b);
}

bool check_sort(int *a, int n)
{
  for(int i=0; i < n-1; i++)
     if(a[i] > a[i+1]) return false;
  return true;
}

