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
  p1 = nextFitAllocRegion(300);
  p2 = nextFitAllocRegion(300);
  p3 = nextFitAllocRegion(300);
  arenaCheck();

  printf("\nFree second block and resize first to overlap to where second used to be\n"); 
  freeRegion(p2);//Leave a gap in the middle to test resizeRegion()
  p1 = resizeRegion(p1, 500);
  arenaCheck();

  printf("\nFree first and allocate a new block that should be placed in a spot after third block rather than in the beginning where first block used to be\n");
  freeRegion(p1);//Leave gap in beginning to test nextFitAllocRegion()
                 //(It should place the block after p3, not where p1 was!)
  p4 = nextFitAllocRegion(300); //p4 should appear after p3, not before
  arenaCheck();
  freeRegion(p3);
  freeRegion(p4);
 
  {				/* measure time for 10000 mallocs */
    struct timeval t1, t2;
    int i;
    getutime(&t1);
    for(i = 0; i < 10000; i++)
      if (firstFitAllocRegion(4) == 0) 
	break;
    getutime(&t2);
    printf("%d firstFitAllocRegion(4) required %f seconds\n", i, diffTimeval(&t2, &t1));
  }
  return 0;
}



