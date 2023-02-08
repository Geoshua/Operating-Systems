#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include  <sys/types.h>
#include "quiz.h"
#define SIZE 100

extern char* fetch(char *url){
	char * str = malloc(sizeof(char) * SIZE);
	int p[2],pid,status;
	
	if (pipe(p) <0) //pipe error handling
		perror("****ERROR:pipe failed\n");
		return NULL;

	if((pid = fork()) < 0) { //fork error handling
		perror("****ERROR:forking child process failed\n");
		return NULL;
	}
	else if (pid == 0){
		dup2(p[1],1);
		dup2(p[1],2);
		if(execvp("curl",(char* []){"curl","-s","/dev/null/",url,NULL})){perror("****ERROR:fetch failed\n");return NULL;}; //fetch error handling
		read(p[0],str,SIZE);
		str = (char*)realloc(str,strlen(str)*sizeof(char)); //dynamically allocate question variable str
	}
	else {while (wait(&status)!= pid);} //wait for any parent processes to finish
	close(p[1]);
	return (str);
}
/*debug
int main(){
	char * str = malloc(sizeof(char) * SIZE);
	str = fetch("http://numbersapi.com/random/math?min=1&max=100&fragment&json");
	printf("%s",str);
	free(str);
}*/