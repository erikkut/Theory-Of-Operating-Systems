#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mytoc.h"


int main(int argc, char **argv){

  char *line = NULL; //pointer to allocated line
  char **tokens;
  int state = 1;

  //print local prompt string var if there is one
  if(getenv("PS1") != NULL)
    printf("%s", getenv("PS1"));
  else
    printf("");
  
  //loop
  while(state){
    //read line from stdout
    ssize_t sizeOfLine = 0;
    getline(&line, &sizeOfLine, stdin);

    //for debugging purposes
    //line = "uname \n uname";
    
    //empty string, don't do anything and return 
    if(line[0] == '\0')
      return 1;
    
    //send line to tokenizer-(implemented by me) and count tokens.
    int numWords = countWords(line, ' ');
    tokens = mytoc(line, ' ');
    //tokens[numWords-1] = NULL; //make last element NULL
      
    //execute line
    state = exe(tokens, numWords);
    

    //state = 0;
  }

  //free vars
  //free(line);
  free(tokens);
  
  return 0; //0 means successful return
}

int exe(char **tokens, int numWords){
  pid_t wP;
  int status;
  int dummy;
  char *fixed[numWords];


  //turn all tokens into usable commands if possible
  int k = 0;
  while(k < numWords){
    //get rid of trailing newlines if they exist
    int commandSize = countChars(tokens[k]);
    if(tokens[k][commandSize-1] == '\n' && commandSize>1)
      tokens[k][commandSize-1] = '\0';
    if(tokens[k][0] != '/')
      fixed[k] = findPath(tokens[k]);
    else
      fixed[k] = tokens[k];
    //printf("%s\n",fixed[k]);
    k++;
  }

  int i = 0;
  while(i < numWords){

    if(i+1 < numWords){
      if(tokens[i+1][0] == '/'){
	char *run[3];
	run[0] = fixed[i];
	i++; //param found, skip a token
	run[1] = tokens[i];
	run[2] = NULL;
	executeExact(run);
      }
    }
    if(tokens[i][0] != '\n'){
      char *run[2];
      run[0] = fixed[i];
      run[1] = NULL; 
      executeExact(run);
    }

    /*
    //not nice command found, have to do work.
    else if(tokens[i][0] != '/'){
      char *run[2];
      run[0] = findPath(tokens[i]);
      run[1] = NULL;
      executeExact(run);
    }
    */
    
    i++;
  }
    
  return 0;
}

int countChars(char *x){
  int count = 0;
  while(x[count])
    count++;
  return count;
}

/* Executes a command as is (assumes path and params are set) */
int executeExact(char **command){
  pid_t wP;
  int status;
  int commandSize = countChars(command[0]);
  pid_t p = fork();  //makes fork to run process

  if(p == 0){  //forking succeded
    execve(command[0], command, NULL);
  }
  else if(p < 0){  //forking failed
    fprintf(stderr, "child forking failed\n");
    return 0;
  }
  else{      
    wait(&status);
  }
  return 0;
}

//finds the first correct path of a command
char *findPath(char *command){
  char *path = getenv("PATH");
  char **possiblePaths = mytoc(path, ':');
  int pathSize = 0;
  int commandSize = countChars(command);
  
  int i = 0;
  while(possiblePaths[i]){
    pathSize = countChars(possiblePaths[i]);
    path = malloc(commandSize+pathSize+2);
    
    for(int j = 0; j < pathSize; j++)
      path[j] = possiblePaths[i][j];
    path[pathSize] = '/';
    for(int j = 0; j < commandSize; j++){
      path[j+1+pathSize] = command[j];
    }
    path[commandSize+pathSize+2] = '\0';
    /*printf("%s", path); //FOR DEBUGGING PURPOSES
      printf("%d\n", countChars(path));*/
    
    //found it! return it now
    if(access(path, X_OK) == 0)
      return path;
    
    i++;
  }
  
  free(possiblePaths);
  return NULL; //found nothing
}
