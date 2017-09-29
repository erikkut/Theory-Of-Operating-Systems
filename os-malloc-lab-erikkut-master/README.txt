This directory contains:

myAllocator.c: a first-fit allocator
myAllocator.h: its header file

myAllocatorTest1.c: a test program for my allocator 

malloc.c: a replacement for malloc that uses my allocator
test1.c: a test program that uses this replacement malloc

Makefile: a fairly portable "makefile", targets "all" and "clean"

To compile: 
 $ make 
To clean:
 $ make clean

The cygwin runtime uses malloc() and brk() extensively.  It is
interesting to compare the output of test1 & myAllocatorTest1.  All
those extra allocated regions are being used by cygwin's libraries!

******************************************************************************

Lab 3, Eric Torres, ID# 80526035

Code modified to fulfill lab 3 requirements. Files modified: myAllocator.c,
test1.c, and myAllocatorTest1.c (took the liberty to adapt the testing code to
prove functionality of the new and modified myAllocator.c methods. Also,
malloc.c was modified to use nextFitAllocRegion() instead of
firstFitAllocRegion() and myAllocator.h was modified to state the new method
added, nextFitAllocRegion().
[Important!] A lot of notes and comments added in code, for further
	     explanations please visit .c files, thank you.

Improving resizeRegion() to actually resize a block:

	  For this part of the lab I created a new method called
	  computeHoleSpace(), that takes in 2 Block_prefixes and returns the
	  empty space in between them. This is particularly useful when trying
	  to figure out if there is enough space next to a block to just
	  increase the boundaries of it rather than finding a bigger block and
	  copying it over to that one. Here is a snippet of the method:

	  size_t computeHoleSpace(BlockPrefix_t *x, BlockPrefix_t *y){  
  	  	 void *suffixOfFirst = x->suffix;
  		 return (void *)y - suffixOfFirst;
	  }

	  Very trivial method. The way it was used for the big picture is that
	  by finding the prefix next to the one you want to increase (using
	  the method getNextPrefix()) you can simply calculate the free space
	  in between them and increase the size of the prefix you want if
	  there is enough space available. Here is a snippet of the added code
	  to the resizeRegion method:

    	  if(oldSize < newSize){
      	       BlockPrefix_t *current = regionToPrefix(r);  
      	       BlockPrefix_t *nxt = getNextPrefix(current);   
      	       while(!nxt->allocated && nxt)  //makes sure its empty
	            nxt = getNextPrefix(nxt);
      
	       if(nxt != (void *)0){//ensure next prefix exists
	            size_t forwardSpace = computeHoleSpace(current, nxt);
	       if(oldSize + forwardSpace >= newSize){
	            freeRegion(r);                 //free region to extend
	  	    makeFreeBlock(r, newSize);     //make region (empty)
	  	    current->allocated = 1;        //mark it as allocated
	  
	       return prefixToRegion(current);
	  }

	  The idea here is that if the block needs to be resized it will then
	  look for the prefix next to it, use computeHoleSpace() to determine
	  the free space in between itself and its next prefix. And if there
	  is enough space, then it will free up the region of the prefix you
	  are trying to increase, it will make a block the size you wanted
	  (since we know there is available space) and it will mark it as
	  allocated.
	  
	  [Important!] Due to lack of instruction I was unsure if resize was
	  	       required in both directions (forward and backward
		       prefixes), so I am adding the un-implemented code of
		       resizeRegion() using both prefixes rather than just
		       the next one in this readme. I will include it at the
		       end of this file.

Implementing find-next algorithm to allocate memory instead of find-first:

	     For this last part of the lab I simply added and modified a
	     handful of lines of code from Dr. Freudenthal's find-first
	     allocation code.
	     Firstly, I changed the method names to findNextFit() and
	     nextFitAllocRegion() respectively, and added a static global
	     to be used to save the last allocated block and begin looking
	     for a new one from there rather than from the beginning of the
	     arena on every function call.
	     Snippet of code:

	     static BlockPrefix_t *z; 
	     BlockPrefix_t *findNextFit(size_t s) {    
  	          if(z == NULL)
    		       z = arenaBegin;
	     //...rest of code//

	     Variable p was replaced to be z throughout this method to make
	     it less confusing and to prevent name collision on other methods.
	     The first line in this snippet makes a NULL static variable in
	     order to keep it throughout method calls. The third line
	     initializes it if it has not been initialized already. If this
	     is the first method call it will be initialized to the beginning
	     of the arena, and then every method call afterwards the previous
	     set value of z will be used instead of initializing it everytime
	     it gets called.
	     So now that we have a static spot holder to start searching for
	     a block from the same block we last found, then we just modify
	     the method we use in nextFitAllocRegion().
	     We call:
	     	p = findNextFit(s)
	     rather than:
	     	p = findFirstFit(s)

RESULTS:

    [student@localhost os-malloc-lab]$ ./myAllocatorTest1 
    Allocate 3 blocks of memory
      checking from 0x88b1000, size=     304, allocated=1...
      checking from 0x88b1140, size=     304, allocated=1...
      checking from 0x88b1280, size=     304, allocated=1...
      checking from 0x88b13c0, size= 1047600, allocated=0...
     mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k

    Free second block and resize first to overlap to where second used to be
      checking from 0x88b1000, size=     624, allocated=1...
      checking from 0x88b1280, size=     304, allocated=1...
      checking from 0x88b13c0, size= 1047600, allocated=0...
     mcheck: numBlocks=3, amtAllocated=0k, amtFree=0k, arenaSize=1023k

    Free first and allocate a new block that should be placed in a spot after third block rather than in the beginning where first block used to be
      checking from 0x88b1000, size=     624, allocated=0...
      checking from 0x88b1280, size=     304, allocated=1...
      checking from 0x88b13c0, size=     304, allocated=1...
      checking from 0x88b1500, size= 1047280, allocated=0...
     mcheck: numBlocks=4, amtAllocated=0k, amtFree=0k, arenaSize=1023k
    10000 firstFitAllocRegion(4) required 0.502000 seconds

      Notes on myAllocatorTest1 results,
      	    Notice how after allocating regions of same size and then
	    deleting the second region and resizing the first, it will
	    resize it to where the second used to be rather than looking
	    for a new region after the third and making it there. This
	    shows how a region actually resizes rather than just copying
	    over to a new found block of correct size.

	    Also notice how allocating a fourth region will place it in an
	    address space located after the third allocated region. This is
	    because it started searching for a new region after the last
	    allocated one, rather than searching from the beginning, thus
	    proving that the find-next algorithm works.
       Here are the results for test1, please notice how they are identical
       (and disregard how malloc creates an extra 1024 block at beginning):

    [student@localhost os-malloc-lab-erikkut]$ ./test1 
    Allocate 3 blocks of memory
      checking from 0x8431000, size=    1024, allocated=1...
      checking from 0x8431410, size=     304, allocated=1...
      checking from 0x8431550, size=     304, allocated=1...
      checking from 0x8431690, size=     304, allocated=1...
      checking from 0x84317d0, size= 1046560, allocated=0...
     mcheck: numBlocks=5, amtAllocated=1k, amtFree=0k, arenaSize=1022k

    Free second block and resize first to overlap to where second used to be
      checking from 0x8431000, size=    1024, allocated=1...
      checking from 0x8431410, size=     624, allocated=1...
      checking from 0x8431690, size=     304, allocated=1...
      checking from 0x84317d0, size= 1046560, allocated=0...
     mcheck: numBlocks=4, amtAllocated=1k, amtFree=0k, arenaSize=1022k

    Free first and allocate a new block that should be placed in a spot after third block rather than in the beginning where first block used to be
      checking from 0x8431000, size=    1024, allocated=1...
      checking from 0x8431410, size=     624, allocated=0...
      checking from 0x8431690, size=     304, allocated=1...
      checking from 0x84317d0, size=     304, allocated=1...
      checking from 0x8431910, size= 1046240, allocated=0...
     mcheck: numBlocks=5, amtAllocated=1k, amtFree=0k, arenaSize=1022k
    10000 malloc(4) required 0.001000 seconds


Unimplemented code from first part due to lack of instruction:
      (This code uses getPrevPrefix() in addition to getNextPrefix() in order
      to determine available size in both directions of prefix to resize)
      
      /*BlockPrefix_t *nxt = getNextPrefix(current);
      printf("nxt: %x curr: %x \n.", nxt, current);
      int forwardSpace = computeHoleSpace(current, nxt);

      BlockPrefix_t *prv = getPrevPrefix(current);
      printf("prv: %x \n.", prv);
      int backSpace = computeHoleSpace(prv, current);
     
      printf("backspace: %x forwardspace: %x \n. oldSize: %x newSize: %x \n. ", backSpace, forwardSpace, oldSize, newSize);
      
      //if backward space is enough, increase prefix size to required
      if((oldSize+backSpace) >= newSize){
        printf("curr prefix: %x new addr: %x SPACE TO ADD: %x \n.", r,r-(newSize-oldSize), newSize-oldSize);
	
	//freeRegion(r);
	//free(r);
	printf("WORKED! \n WORKED! \n.");
	//current = makeFreeBlock(r, oldSize);
	//current += newSize;
	//coalesce(nxt);
	
	return r;
      }

      if((oldSize+forwardSpace) >= newSize){
	freeRegion(current);
	makeFreeBlock(current, newSize);

	return current;
      }
    
      //other-other wise check if both forward and back are enough, else just look for a block in whole arena
      if((oldSize+forwardSpace+backSpace) >= newSize){
	printf("address of current, %8zx . address of next, %8zx . address of previous, %8zx\n.", (void *)current, (void *)nxt, (void *)prv);

	freeRegion(current);
	current = makeFreeBlock(current-(newSize-oldSize), newSize);

	return current;
	}*/

	  
