#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#include<string.h>

#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define buffer_size 1024
#define word_size 32

static int execute_command(const char* filename, int argc, ...);

int main(int argc, char *argv[]){

    if(argc == 1){
        printf("No instructions!\n");
        return 0;
    }

    if(strcmp("new", argv[1]) == 0){
        if(argc == 2){
            printf("New what?\n");
            return 0;
        }

        char buffer[buffer_size];

        sprintf(buffer, "%s/build/debug", argv[2]);
        execute_command("/bin/mkdir", 3, "mkdir", "-p", buffer);

        sprintf(buffer, "%s/build/release", argv[2]);
        execute_command("/bin/mkdir", 3, "mkdir", "-p", buffer);

        sprintf(buffer, "%s/project/resources", argv[2]);
        execute_command("/bin/mkdir", 3, "mkdir", "-p", buffer);

        sprintf(buffer, "%s/project/include", argv[2]);
        execute_command("/bin/mkdir", 3, "mkdir", "-p", buffer);

        sprintf(buffer, "%s/project/source", argv[2]);
        execute_command("/bin/mkdir", 3, "mkdir", "-p", buffer);

        sprintf(buffer, "TE");
        char second_buffer[word_size];
        sprintf(second_buffer, "%s/", argv[2]);
        execute_command("/bin/cp", 4, "cp", "-r", buffer, second_buffer);
    }
    else if(strcmp("build", argv[1]) == 0){
        if(argc == 3 && strcmp("--release", argv[2]) == 0){
            execute_command("/usr/bin/g++", 14, "g++", "-o3", "-I/project/include","-ITE/include", 
                            "-LTE/lib", "project/source/main.cpp", "TE/lib/libTE.a", 
                            "-lSDL2main", "-lSDL2", "-lGLEW", "-lGLU", "-lGL", "-lSOIL", 
                            "-o", "build/debug/game");
        }
        else{
            execute_command("/usr/bin/g++", 14, "g++","-I/project/include","-ITE/include", 
                            "-LTE/lib", "project/source/main.cpp", "TE/lib/libTE.a", 
                            "-lSDL2main", "-lSDL2", "-lGLEW", "-lGLU", "-lGL", "-lSOIL", 
                            "-o", "build/debug/game");
        }
    }
    else if(strcmp("run", argv[1]) == 0){
        if(argc == 3 && strcmp("--release", argv[2]) == 0){
            execute_command("./build/release/game", 0);
        }
        else{
            execute_command("./build/debug/game", 0);
        }
    }

    return 0;
}

static int execute_command(const char* filename, int argc, ...){

    va_list valist;
    char **argv;
    int i;

    va_start(valist, argc);

    argv = (char**)malloc((argc + 1) * sizeof(char*));
    for(i = 0; i < argc; i++){
        argv[i] = (char*)malloc(word_size * sizeof(char));
        char* buffer = va_arg(valist, char*);
        sprintf(argv[i], "%s", buffer);
    }
    argv[argc] = NULL;

    va_end(valist);

    pid_t pid = fork();
    if(pid == 0){
        execv(filename, argv);
        exit(127);
    }
    else{
        waitpid(pid, 0, 0);
    }

    free(argv);

    return 0;
}