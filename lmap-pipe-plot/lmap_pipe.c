#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 32
#define RMN 3.5
#define RMX 4.0
#define INTERVALO (RMX-RMN)/N
#define P 0.0005

int lmap(double rmin, int fd)
{
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
			dprintf(fd,"%.4f %.6f",r,xi);
      if(fabs(ref-xi)<1E-6){break;}
			x=xi;
      usleep(500);
		}
	}
	return 0;
}

int main()
{
  pid_t child_pid, ppid = getpid();
  int status = 0, fd[2];

  pipe(fd);

  for (int id=0; id<N; id++) {
      if ((child_pid = fork()) == 0)
      {
        close(fd[0]);
        lmap(RMN+id*INTERVALO,fd[1]);
        close(fd[1]);
        exit(0);
      }
  }

  close(fd[1]);
  FILE *f = fopen("valores.dat","w+");
  char line[15];
  while(read(fd[0],line,sizeof(line)))
  {
    fprintf(f,"%s\n",line);
  }
  close(fd[0]);

  return 0;
}
