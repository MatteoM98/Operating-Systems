#include "xerrori.h"

FILE *xfopen(const char *pathname, const char *mode, const char *file, const int line) {
  FILE *f = fopen(pathname,mode);
  if(f==NULL) {
    perror("Errore apertura file");
    fprintf(stderr,"(%s:%d)\n",file,line);
    exit(1);
  }
  return f;
}

size_t xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea)
{
	size_t n = fwrite(ptr,size,nmemb,stream);
	if(n!=nmemb){
		perror("Errore scrittura file");
		fprintf(stderr,"(%s:%d)\n",file,linea);
		exit(1);
				}
	return n;
}

size_t xfread(void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int line){
	int e = fread(ptr,size,nmemb,stream);
	if(e!=nmemb){
		perror("Errore lettura file");
		fprintf(stderr,"(%s:%d)",file,line);
		exit(1);
	}
	return e;
}

pid_t xfork(const char *file,const int line)
{
	pid_t p = fork();
	if(p<0)
	{
		perror("Errore chiamata fork");
		fprintf(stderr,"(%s:%d)\n",file,line);
		exit(1);
	}
	return p;
}

pid_t xwait(int *status,const char *file,const int line)
{
	pid_t p = wait(status);
	if(p<0)
	{
		perror("Errore chiamata wait");
		fprintf(stderr,"(%s:%d)\n",file,line);
		exit(1);
	}
	return p;
	
}
