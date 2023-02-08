#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int tflag = 0;
int nflag = 0;

int main(int argc, char *argv[])
{
	int opt, arglim = 0;
	char *buffer[128], *buff[64];
	while((opt = getopt(argc,argv,"n:tj:")) != -1){
		switch(opt)
		{
			case 'n':
				arglim = strtol(optarg,NULL,10);
				nflag = 1;
				break;
			case 't':
				tflag = 1;
				break;
			default:
				break;
				return EXIT_FAILURE;
		}
	}
	buff[0] = "/bin/echo";
	if (tflag) {
		int i = 1;
		int len = 0;
		while(getline(&buff[i],&len,stdin) != -1){
			buff[i][strlen(*buff)-1] = '\0';
			fprintf(stderr,"%s",buff[i]);
	}
	}
	if(nflag){
		int c = 0, i = 0;
		while(getline(&buffer[i],0,stdin) != -1)
		{
			int j = getline(&buffer[i],0,stdin);
			c++;
			if(c == arglim){
				pid_t new = fork();
				execvp(buffer[0],buffer);
				if(new == 0){
					waitpid(new,0,0);
				}
				else if(new < 0){
					fprintf(stderr,"Fork error");
				}
				c = 0, i = 0;
			}
			i++;
		}
	} 
	else{
		int i = 1;
		buffer[0] = "/bin/echo ";
		int len = 0;
		buffer[1] = NULL;
		while(getline(&buffer[i],&len,stdin) != -1)
		{
			printf("%s",buffer[0]);
			i++;
		}
		buffer[i] = NULL;
		execvp(buffer[1],buffer);
	}
}