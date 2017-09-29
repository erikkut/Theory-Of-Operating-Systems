Eric Torres (emtorres6)
Lab 2 (Threads) Part 2

How to run:
    Clone repository folder called "Lab2-Part2" and type 'make' in console in
    order to compile the program using the makefile in order to compile and
    create the necessary files.

    The executable is called "producer-consumer", to run type
    './producer-consumer' in console in order run the program.

    Program should output the result of having 4 productors that make elements
    (0...99 each one) and a consumer that prints out every 10 it consumes.
    [Important!] Own queue implementation was used, it uses malloc to store
    the persistent queue's head and tail.

Explanation:
	Comments added to producer-consumer.c to explain how it works and
	how the semaphores are implemented plus some of	the reasoning behind
	the code. Comments where not added to queue.c due to being self-
	explanatory and trivial code.

	Some of the code will be explained here:

	The following snippets of code show how the locking was implemented
	to allow for safe, multiple thread use.
	This is the psuedo for producers:
	
  for(i = 0; i < some_limit; i++){
    pthread_mutex_lock(&the_mutex;)//Locks mutex
    while(buffer != 0) pthread_cond_wait(&condc, &the_mutex);//places consumer on wait
    buffer = i; //Takes ownership of buffer to do work

    //Critical Section. (for producer enqueue).

    pthread_cond_signal(&condc);//Wake consumer
    pthread_mutex_unlock(&the_mutex);//Unlocks mutex
  }

	This other similar snippet shows the same, but for the consumer

  for(i = 0; i < (some_limit*number_of_producers); i++){
    pthread_mutex_lock(&the_mutex); 
    while(buffer == 0) pthread_cond_wait(&condp, &the_mutex);//places producers on wait
    buffer = 0; //Takes ownership of buffer to do work

    //Critical Section. (for consumers, dequeue, and it producer limit reached, print sum)

    pthread_cond_signal(&condp);     //Wake producerS
    pthread_mutex_unlock(&the_mutex);
  }

Results:

	[student@localhost Lab2-part2]$ ./producer-consumer 
	Starting program

	Sum: 8
	Sum: 32
	Sum: 58
	Sum: 82
	Sum: 108
	Sum: 132
	Sum: 158
	Sum: 182
	Sum: 208
	Sum: 232
	Sum: 258
	Sum: 282
	Sum: 308
	Sum: 332
	Sum: 358
	Sum: 382
	Sum: 408
	Sum: 432
	Sum: 458
	Sum: 482
	Sum: 508
	Sum: 532
	Sum: 558
	Sum: 582
	Sum: 608
	Sum: 632
	Sum: 658
	Sum: 682
	Sum: 708
	Sum: 732
	Sum: 758
	Sum: 782
	Sum: 808
	Sum: 832
	Sum: 858
	Sum: 882
	Sum: 908
	Sum: 932
	Sum: 958
	Sum: 981

	Program Done
