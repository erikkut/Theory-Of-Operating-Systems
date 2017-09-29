#include <stdlib.h>
#include "mytoc.h"

int countWords(char *str, char delim){
  int tokenCount = 0;
  int counted = 0;

  while(*str){
    if(*str == delim)
      counted = 0;
    else if(counted == 0){
      counted = 1;
      tokenCount++;
    }
    ++str;
  }
  return tokenCount;
}

char ** mytoc(char *str, char delim){

  //COUNT TOKENS IN STRING
  int tokenCount = countWords(str, delim);
  
  //ALLOCATING SPACE FOR TOKENS
  char **tokenVec = (char **)calloc(tokenCount+1, sizeof(char *));
  tokenVec[tokenCount] = (char *)0;//setting last token to NULL

  //ALLOCATING EACH TOKEN
  int currentWord = 0;
  for(int i = -1; i < tokenCount-1;){
    if(*str != delim){
      i++;   
      while(*str != delim && *str != '\0'){
	currentWord++;//count characters in current word for pointer use
	str++;
      }
      tokenVec[i] = (char *)malloc(currentWord+1);
      for(int p = 0; p < currentWord; p++){//copy word onto token array
	tokenVec[i][p] = str[p-currentWord];
      }
      tokenVec[i][currentWord] = 0;//add end of string to every word
    }
    currentWord = 0;//reset current word size
    ++str;
  }

  //RETURN POINTER
  return tokenVec;
}


