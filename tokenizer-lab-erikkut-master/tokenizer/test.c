#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytoc.h"

int main();

void printTokens(char **tokenVec){
  while(*tokenVec)
    printf("From allocated memory address- %d: %s\n\n", tokenVec, *tokenVec++);
  return;
}

void printNicely(char **tokenVec){
  while(*tokenVec)
    printf("%s ", *tokenVec++);
  return;
}

int main(){
  printf("Enter 0 for preset string demo or a 1 for loop\n");

  char ans;
  scanf("%c", &ans);

  if(ans == '0'){
    printf("Preset demo.\n\n");
    char **tokenVec;
    tokenVec = mytoc("          ", ' ');
    printTokens(tokenVec);
    printf("Single line form: ");
    printNicely(tokenVec);
    printf("\n");
    free(tokenVec);
    return 0;
  }

  else if(ans == '1'){
    printf("Loop entered.\nEnter \'exit\' to exit loop, enter any other string, to test that string\n");
    char str[256];
    char **tokenVec;

    write(1, "$ ", 2);
    
    while(1){
      
      scanf("%s", &str);
      tokenVec = mytoc(str, ' ');
      printNicely(tokenVec);printf("\n");
      free(tokenVec);
      //printf("\n");
      if(!strcmp(str, "exit"))
	break;
      //else
	//printf("\n");
    }

    printf("Loop exited. Ending program.\n");
    return 0;
  }

  printf("Please enter only 0 or 1. Ending program.\n");
  return 0;
}  
