#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;
int vflag = 0;

void print(); // print environment variables
void execCommand(char *argv[], int vflag); // execute utility/command 
void unset(const char *name, int vflag); // unset environment variable 'name'
int set(char *nameVal, int vflag); // set environment variable 'nameVal'

int main(int argc, char *argv[]) {
    int opt;

    // print environment variables if no arguments are supplied
    if (argc == 1) {
        print();
    }

    // getting the options passed in by the user
    while ((opt = getopt(argc, argv, "u:v")) != -1){
        switch(opt) {
            case 'v':
                vflag = 1;
                break;
            case 'u':
                unset(optarg, vflag);
                break;
            default:
                fprintf(stderr, 
                        "usage: env [-v] [-u name] [name=value ...] [utility [argument ...]]\n"
                        );
                return EXIT_FAILURE;
        }
    }

    char *nArgs[] = {};
    int comInd = 0;

    // checks for name=value pairs in the remaining arguments
    // stores not name=value pairs argyment in a new array
    for (; optind < argc; optind++) {
        if(set(argv[optind], vflag) == EXIT_FAILURE){
            nArgs[comInd] = argv[optind];
            comInd++;
        }
    }

    // execute command with optional arguments
    if (comInd) {
        nArgs[comInd] = NULL;
        execCommand(nArgs, vflag);
    }

    print();

    return EXIT_SUCCESS;
}

void print() {
    for (char **env = environ; *env != 0; env++) {
        if (printf("%s\n", *env) == EOF) {
            fprintf(stderr, "Printing error.\n");
        }
    }
}

void execCommand(char *argv[], int vflag) { 
    if (vflag) {
        fprintf(stderr, "#env executing: %s\n", argv[0]);
        for (int i = 0; argv[i] != NULL; i++) {
            fprintf(stderr, "#env arg[%d] = '%s'\n", i, argv[i]);
        }
    }
    if (execvp(argv[0], argv) == -1) {
        fprintf(stderr, "No such command found\n");
        exit(EXIT_FAILURE);
    };
}

void unset(const char *name, int vflag) {
    if (vflag) {
        fprintf(stderr, "#env unset: %s\n", name);
    }
    unsetenv(name);
}

int set(char *nameVal, int vflag){
    if(strchr(nameVal, '=') != NULL){
        char *token = strtok(nameVal, "=");
        char *nextToken = strtok(NULL, "=");
        if (setenv(token, nextToken, 1) != -1) {
            if (vflag) {
                fprintf(stderr, "#env setenv: %s=%s\n", token, nextToken);
            }
        }
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
