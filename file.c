#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLE_NAME 50
#define MEMORY_SIZE 1024

char *memory_base;  // Puntero a la memoria base
size_t memory_available;  // Tamaño disponible de la memoria

// Inicializa la memoria
void init_memory() {
    memory_base = (char *)malloc(MEMORY_SIZE);
    if (memory_base == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        exit(1);
    }
    // Inicializamos la memoria con ceros
    memset(memory_base, 0, MEMORY_SIZE);
    memory_available = MEMORY_SIZE;
    printf("Memoria inicializada: %zu bytes disponibles.\n", memory_available);
}

char* first_fit_alloc(size_t size, const char *variable_name) {
    if (size > memory_available) {
        printf("Error: No hay suficiente memoria disponible.\n");
        return NULL;
    }

    // Recorrer la memoria buscando el primer espacio libre suficientemente grande
    for (int i = 0; i < MEMORY_SIZE; ) {
        if (memory_base[i] == 0) {
            int free_block_start = i;
            int free_block_size = 0;

            while (i < MEMORY_SIZE && memory_base[i] == 0 && free_block_size < size) {
                free_block_size++;
                i++;
            }

            // Si encontramos un bloque libre lo suficientemente grande
            if (free_block_size >= size) {
                // Rellenar el bloque de memoria con el primer carácter de variable_name
                memset(&memory_base[free_block_start], variable_name[0], size);

                memory_available -= size;
                printf("ALLOC: Asignados %zu bytes a la variable '%s' en la dirección %p\n", size, variable_name, &memory_base[free_block_start]);

                return &memory_base[free_block_start];
            }
        }
        i++;
    }

    printf("Error: No se encontró un bloque libre suficientemente grande.\n");
    return NULL;
}

// Función para imprimir el estado de la memoria
void print_memory_status() {
    printf("Estado actual de la memoria:\n");
    int i = 0;

    while (i < MEMORY_SIZE) {
        if (memory_base[i] == 0) {
            // Bloque libre
            int free_block_start = i;
            int free_block_size = 0;

            // Contamos el tamaño del bloque libre
            while (i < MEMORY_SIZE && memory_base[i] == 0) {
                free_block_size++;
                i++;
            }
            printf("Libre: %d bytes desde la dirección %p\n", free_block_size, &memory_base[free_block_start]);
        } else {
            // Bloque ocupado
            char variable_name = memory_base[i];  // Tomamos el nombre de la variable
            int occupied_block_start = i;
            int occupied_block_size = 0;

            // Contamos el tamaño del bloque ocupado
            while (i < MEMORY_SIZE && memory_base[i] == variable_name) {
                occupied_block_size++;
                i++;
            }
            printf("Variable '%c': %d bytes desde la dirección %p\n", variable_name, occupied_block_size, &memory_base[occupied_block_start]);
        }
    }
}

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
                    first_fit_alloc(size, variable_name);
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
                print_memory_status();
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
    init_memory();
    process_input_file(argv[1]);

    // Liberar la memoria
    free(memory_base);
    return 0;
}