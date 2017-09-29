all: producer-consumer

clean:
	rm -f *.o queues producer-consumer producer-consumer 

queues.o: queues.c queues.h
	gcc -c queues.c

producer-consumer.o: producer-consumer.c queues.h
	gcc -c producer-consumer.c

producer-consumer: producer-consumer.o queues.o
	gcc -lpthread -o producer-consumer producer-consumer.o queues.o
