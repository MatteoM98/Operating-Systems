#include "xerrori.h"
#define Nome "/contaprimi_int"
#define Nome1 "/contaprimi_sem"

// programma per il conteggio del numero dei primi 
// tra gli interi contenuti in un file 

// restituisce true/false a seconda che n sia primo o composto
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



int main(int argc, char *argv[]) {
  if(argc!=3) {
    printf("Uso:\n\t%s nomefile p\n",argv[0]);
    exit(1);
  }
  int p = atoi(argv[2]);
  assert(p>0);
  
  // ---- creazione array memoria condivisa
  int shm_size = 12*sizeof(int); // numero di byte necessari
  int fd = xshm_open(Nome,O_RDWR | O_CREAT, 0600,__LINE__,__FILE__);
  xftruncate(fd, shm_size, __LINE__,__FILE__);
  int *buffer = simple_mmap(shm_size,fd, __LINE__,__FILE__);
  close(fd); // dopo mmap e' possibile chiudere il file descriptor
  buffer[10]=0;
  buffer[11]=0;
  
   // ---- memoria condivisa per i semafori
  int shm_size1 = sizeof(sem_t); // numero di byte necessari
  int fd1 = xshm_open(Nome1,O_RDWR | O_CREAT, 0600,__LINE__,__FILE__);
  xftruncate(fd1, shm_size1, __LINE__,__FILE__);
  sem_t *sem = simple_mmap(shm_size1,fd1, __LINE__,__FILE__);
  close(fd1); // dopo mmap e' possibile chiudere il file descriptor
  sem_t *sem_buffer = &sem[0];  // oppure  = sem 
  xsem_init(sem_buffer,1,1,__LINE__, __FILE__);
  
  
  for(int i=0;i<p;i++) {
	  
    pid_t pid = xfork(__FILE__,__LINE__);
    
    if(pid==0) {		// figlio
    
    while(true){ 
		
     xsem_wait(sem_buffer,__LINE__,__FILE__);
      
	if(buffer[10]==-1) 
	{  
		xsem_post(sem_buffer,__LINE__,__FILE__);
		exit(0);
	} 
	  
    while(buffer[10]==0)
     {
		 xsem_post(sem_buffer,__LINE__,__FILE__);
		 sleep(1);
	     xsem_wait(sem_buffer,__LINE__,__FILE__);
		  
	  }
	   
	  int x = buffer[(buffer[10])-1];
	  buffer[10]--;
	  xsem_post(sem_buffer,__LINE__,__FILE__);
	  if(primo(x)) 
	  {
		  xsem_wait(sem_buffer,__LINE__,__FILE__);
		  buffer[11]++;
		  xsem_post(sem_buffer,__LINE__,__FILE__);
	  }
	  
		} 
	}	
       
}
  
  
  
  
  
 // i padre inizia a leggere gli interi dal file
  FILE *f = xfopen(argv[1],"rt",__FILE__,__LINE__);
  while(true) {
	
    int x;
    int e = fscanf(f,"%d",&x);
    if(e==0|| e==EOF) 
    {
		while(buffer[10]!=0) sleep(1);
		xsem_wait(sem_buffer,__LINE__,__FILE__);
		buffer[10]=-1;
		xsem_post(sem_buffer,__LINE__,__FILE__);
		break;
	}
	
	xsem_wait(sem_buffer,__LINE__,__FILE__);
    while(buffer[10]==10)
    {
		xsem_post(sem_buffer,__LINE__,__FILE__);  
		sleep(1);
		xsem_wait(sem_buffer,__LINE__,__FILE__);
	}
	buffer[buffer[10]] = x;
	buffer[10]++;
	xsem_post(sem_buffer,__LINE__,__FILE__);
  }
  
  fclose(f);
  int totpadre = buffer[11];
  printf("Numero primi trovati :%d\n",totpadre);
  
  // unmap e rimozione memoria condivisa
  munmap(buffer,shm_size);
  munmap(sem,shm_size1);
  xshm_unlink(Nome,__LINE__, __FILE__);
  xshm_unlink(Nome1,__LINE__, __FILE__);
  
  return 0;
  
}
