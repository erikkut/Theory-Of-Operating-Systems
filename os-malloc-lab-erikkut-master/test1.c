#include "stdio.h"
#include "stdlib.h"
#include "myAllocator.h"
#include "sys/time.h"
#include <sys/resource.h>
#include <unistd.h>

double diffTimeval(struct timeval *t1, struct timeval *t2) {
  double d = (t1->tv_sec - t2->tv_sec) + (1.0e-6 * (t1->tv_usec - t2->tv_usec));
  return d;
}

void getutime(struct timeval *t)
{
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  *t = usage.ru_utime;
}



int main() 
{
  void *p1, *p2, *p3, *p4;
  
  printf("Allocate 3 blocks of memory\n");
  p1 = malloc(300);
  p2 = malloc(300);
  p3 = malloc(300);
  arenaCheck();

  printf("\nFree second block and resize first to overlap to where second used to be\n"); 
  free(p2);//Leave a gap in the middle to test resizeRegion()
  p1 = realloc(p1, 500);
  arenaCheck();

  printf("\nFree first and allocate a new block that should be placed in a spot after third block rather than in the beginning where first block used to be\n");
  free(p1);//Leave a gap in beginning to show how the next allocated region
           //will be placed after p3 rather than where p1 used to be.
  p4 = malloc(300); //p4 should appear after p3, not before
  arenaCheck();
  free(p4);
  free(p3);
 
  {				/* measure time for 10000 mallocs */
    struct timeval t1, t2;
    int i;
    getutime(&t1);
    for(i = 0; i < 10000; i++)
      if (malloc(4) == 0) 
	break;
    getutime(&t2);
    printf("%d malloc(4) required %f seconds\n", i, diffTimeval(&t2, &t1));
  }
  return 0;
}



