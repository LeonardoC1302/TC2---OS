#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLE_NAME 50

void process_input_file(char* filename){
    FILE* file = fopen(filename, "r");

    if (file == NULL){
        printf("No se pudo abrir el archivo %s\n", filename);
        return;
    }

    char line[256];
    char command[10];
    char variable_name[MAX_VARIABLE_NAME];
    size_t size;

    while (fgets(line, sizeof(line), file)){
        if (line[0] == '#' || line[0] == '\n') continue; // Ignorar comentarios y lineas en blanco

        if(sscanf(line, "%s", command) == 1){
            if (strcmp(command, "ALLOC") == 0) {
                if (sscanf(line, "%*s %s %zu", variable_name, &size) == 2) {
                    printf("alloc %s %zu\n", variable_name, size);
                }
            } else if (strcmp(command, "REALLOC") == 0){
                if (sscanf(line, "%*s %s %zu", variable_name, &size) == 2) {
                    printf("REALLOC %s %zu\n", variable_name, size);
                }
            } else if (strcmp(command, "FREE") == 0) {
                if (sscanf(line, "%*s %s", variable_name) == 1) {
                    printf("FREE %s\n", variable_name);
                }
            } else if (strcmp(command, "PRINT") == 0) {
                printf("PRINT\n");
            }
        }
    }
    fclose(file);
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        printf("Uso: %s <archivo_de_entrada>\n", argv[0]);
        return 1;
    }
    process_input_file(argv[1]);
    return 0;
}