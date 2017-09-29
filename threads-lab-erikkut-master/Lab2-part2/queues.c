#include <stdio.h>
#include <stdlib.h>
#include "queues.h"

struct Node{
  int data;
  struct Node* next;
};struct Node*head=NULL; struct Node*tail=NULL;

void enqueue(int x){//queues a number
  struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
  temp -> data = x;
  temp -> next = NULL;
  if(head == NULL){
    head = tail = temp;
    return;
  }
  tail -> next = temp;
  tail = temp;
}
int dequeue(){//returns dequeued data, returns -1 if queue empty
  struct Node* temp = head;
  int dequeued = -1;
  if(head != NULL)
    dequeued = head -> data;
  if(tail != NULL)
    head = head -> next;
  if(head == NULL)
    head = tail = NULL;
  
  free(temp);
  return dequeued;
}
void printQueue(){
  struct Node* temp = head;
  printf("queue has, ");
  while(temp != NULL){
    printf("%d ",temp->data);
    temp = temp -> next;
  }
  printf("\n\n");
}
