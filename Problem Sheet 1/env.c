#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char *argv[]){
	int opt;
	while((opt = getopt(argc,argv,"vu:") != -1));
	switch(opt)
	{
		case'v'://case e
			for(int i = 0;i<argc;i++){
				fprintf(stderr,"%s\n",argv[i]);
			}
			break;
		case'u'://case f
			putenv(argv[optind]);
		default:
			if(argc < 3){//case a	
				for(int i = 0; environ[i] != NULL;i++){
					printf("%s\n",environ[i]);
				}
			}
			if (strchr(optind[argv],'=') != NULL){//case b
				for(int i = 0;i<argc;i++){
					putenv(argv[i]);
				}
				for(int i = 0; environ[i] != NULL;i++){
					printf("%s\n",environ[i]);
				}
			}
			if (strchr(optind[argv],'=') == NULL){//case c
				execvp(argv[optind],argv);
			}

			break;

	}

	return EXIT_SUCCESS;
}