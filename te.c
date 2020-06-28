#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#include<string.h>
#include<dirent.h>

#include<sys/types.h>

#define buffer_size 512

static int execute_command(const char* str, ...);
static int get_files(const char* path, const char* type, char ***files, int *size);

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

        execute_command("mkdir -p %s/build/debug", argv[2]);

        execute_command("mkdir -p %s/build/release", argv[2]);

        execute_command("mkdir -p %s/project/resources", argv[2]);

        execute_command("mkdir -p %s/project/include", argv[2]);

        execute_command("mkdir -p %s/project/source", argv[2]);

        execute_command("cp -r /usr/bin/TE %s/",argv[2]);

        execute_command("cp /usr/bin/main.c %s/project/source", argv[2]);
    }
    else if(strcmp("build", argv[1]) == 0){
        float percent = 0;

        int size;
        char **files;
        get_files("./project/source", ".c", &files, &size);        

        int i;
        for(i = 0; i < size; i++){
            if(argc == 3 && ( strcmp("--release", argv[2]) == 0 
                            ||  strcmp("-r", argv[2]) == 0 )){

            }
            else{
                execute_command("gcc -c -Iproject/include -ITE/include project/source/%s", files[i]);
                size_t shift = strcspn(files[i], ".c");
                memset(files[i] + shift + 1, 0, 1);
                *(files[i] + shift + 1) = 'o';
            }
        }
        
        char buffer[buffer_size] = {0};
        for(i = 0; i < size; i++){
            strcat(buffer, " ");
            strcat(buffer, files[i]);
        }
        execute_command("gcc -o build/debug/game -Iproject/include -ITE/include -LTE/lib %s TE/lib/libTE.a -lSDL2main -lSDL2 -lGLEW -lGLU -lGL -lSOIL -lm", buffer);

        for(i = 0; i < size; i++){
            execute_command("rm %s", files[i]);
        }

        execute_command("cp -r project/resources/ build/debug/resources/");

        for(i = 0; i < size; i++)
            free(files[i]);
        free(files);
    }
    else if(strcmp("run", argv[1]) == 0){
        if(argc == 3 && ( strcmp("--release", argv[2]) == 0 ||  strcmp("-r", argv[2]) == 0 )){
            //execute_command("./build/release/game", 0);
        }
        else{
            execute_command("./build/debug/game");
        }
    }

    return 0;
}

static int execute_command(const char* str, ...){

    va_list valist;
    char _command[4*buffer_size] = {0};
    int i = 0, j = 0;

    va_start(valist, str);
    while(str && str[i]){
        if(str[i] == '%'){
            i++;
            if(str[i] == 's'){
                char* string = (char*)va_arg(valist, char*);
                size_t string_size = strlen(string);
                int k;
                for(k = 0; k < string_size; k++){
                    _command[j] = string[k];
                    j++;
                }
            }
        }
        else{
            _command[j] = str[i];
            j++;
        }
        i++;
    }
    va_end(valist);

    system(_command);

    return 0;
}

static int get_files(const char* path, const char* type, char ***files, int *size){
    DIR *dir;
    struct dirent* sd;

    dir = opendir(path);
    if(dir == NULL){
        printf("Unable to open directory.\n");
        return -1;
    }

    int _size = 0;
    char **_files;

    while((sd = readdir(dir)) != NULL){
        if(strstr(sd->d_name, type) != NULL){
            _size += 1;
            _files = (char**)realloc(_files, _size * sizeof(char*));
            _files[_size - 1] = (char*)malloc(128 * sizeof(char));

            strcat(_files[_size - 1], sd->d_name);
        }
    } 

    *files = _files;
    *size = _size;

    return 0;
}
