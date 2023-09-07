#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>


#define NUM_THREADS 32
#define RMN 3.5
#define RMX 4.0
#define INTERVALO (RMX-RMN)/NUM_THREADS
#define P 0.0005

int fd[2];

void *lmap(void *_args)
{
	double rmin = *(double*) _args;
	double x, xi, r, ref;
	for(r=rmin;r<rmin+INTERVALO;r+=P)
	{
		x = rand()/(double)RAND_MAX;
		for(int i=0;i<20000;++i)
		{
			xi = r*x*(1-x);
			x=xi;
		}
	   ref = xi;
		for(int i=0;i<250;++i)
		{
			xi = r*x*(1-x);
			dprintf(fd[1],"%.4f %.6f\n",r,xi);
	    if(fabs(ref-xi)<1E-6){break;}
			x=xi;
			usleep(500);
		}
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[NUM_THREADS];
	double temp[NUM_THREADS];
	pipe(fd);
  for (int id=0; id<NUM_THREADS; id++)
	{
		temp[id]= RMN+id*INTERVALO;
		pthread_create(&threads[id],NULL,lmap,&temp[id]);
  }
	close(fd[1]);

	FILE *f = fopen("valores.dat","w+");
	char line[15];
	while(read(fd[0],line,sizeof(line)))
	{
		fprintf(f,"%s\n",line);
	}
	close(fd[0]);

	for (int id=0; id<NUM_THREADS; id++)
	{
		pthread_join(threads[id],NULL);
	}

  return 0;
}
