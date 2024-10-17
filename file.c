#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLE_NAME 50
#define MEMORY_SIZE 1024

char *memory_base;       // Puntero a la memoria base
size_t memory_available; // Tamaño disponible de la memoria

// Inicializa la memoria
void init_memory()
{
    memory_base = (char *)malloc(MEMORY_SIZE);
    if (memory_base == NULL)
    {
        printf("Error: No se pudo asignar memoria.\n");
        exit(1);
    }
    // Inicializamos la memoria con ceros
    memset(memory_base, 0, MEMORY_SIZE);
    memory_available = MEMORY_SIZE;
    printf("Memoria inicializada: %zu bytes disponibles.\n", memory_available);
}

char *first_fit_alloc(size_t size, const char *variable_name)
{
    if (size > memory_available)
    {
        printf("Error: No hay suficiente memoria disponible.\n");
        return NULL;
    }

    // Recorrer la memoria buscando el primer espacio libre suficientemente grande
    for (int i = 0; i < MEMORY_SIZE;)
    {
        if (memory_base[i] == 0)
        {
            int free_block_start = i;
            int free_block_size = 0;

            while (i < MEMORY_SIZE && memory_base[i] == 0 && free_block_size < size)
            {
                free_block_size++;
                i++;
            }

            // Si encontramos un bloque libre lo suficientemente grande
            if (free_block_size >= size)
            {
                // Rellenar el bloque de memoria con el primer carácter de variable_name
                memset(&memory_base[free_block_start], variable_name[0], size);

                memory_available -= size;
                printf("ALLOC (First-Fit): Asignados %zu bytes a la variable '%s' en la direccion %p\n", size, variable_name, &memory_base[free_block_start]);

                return &memory_base[free_block_start];
            }
        }
        i++;
    }

    printf("Error: No se encontró un bloque libre suficientemente grande.\n");
    return NULL;
}

char *best_fit_alloc(size_t size, const char *variable_name)
{
    if (size > memory_available)
    {
        printf("Error: No hay suficiente memoria disponible.\n");
        return NULL;
    }

    int best_block_start = -1;
    int best_block_size = MEMORY_SIZE + 1; // Inicializamos con un valor mayor que cualquier bloque posible

    // Recorrer la memoria para encontrar el bloque más pequeño que sea suficientemente grande
    for (int i = 0; i < MEMORY_SIZE;)
    {
        if (memory_base[i] == 0)
        {
            int free_block_start = i;
            int free_block_size = 0;

            while (i < MEMORY_SIZE && memory_base[i] == 0)
            {
                free_block_size++;
                i++;
            }

            // Si el bloque libre es suficientemente grande y más pequeño que el mejor encontrado
            if (free_block_size >= size && free_block_size < best_block_size)
            {
                best_block_start = free_block_start;
                best_block_size = free_block_size;
            }
        }
        else
        {
            i++; // Avanza si no es un bloque libre
        }
    }

    // Si encontramos un bloque adecuado
    if (best_block_start != -1)
    {
        // Rellenar el bloque de memoria con el primer carácter de variable_name
        memset(&memory_base[best_block_start], variable_name[0], size);

        memory_available -= size;
        printf("ALLOC (Best-Fit): Asignados %zu bytes a la variable '%s' en la direccion %p\n", size, variable_name, &memory_base[best_block_start]);

        return &memory_base[best_block_start];
    }

    printf("Error: No se encontró un bloque libre suficientemente grande.\n");
    return NULL;
}

char *worst_fit_alloc(size_t size, const char *variable_name)
{
    int worst_block_start = -1;
    int worst_block_size = 0;
    int current_block_start = -1;
    int current_block_size = 0;

    // Recorrer la memoria buscando el bloque libre más grande
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (memory_base[i] == 0)
        {
            // Iniciar o continuar un bloque libre
            if (current_block_start == -1)
            {
                current_block_start = i;
            }
            current_block_size++;
        }
        else
        {
            // Si encontramos un bloque ocupado, revisamos si el bloque libre anterior es el más grande hasta ahora
            if (current_block_size > worst_block_size)
            {
                worst_block_start = current_block_start;
                worst_block_size = current_block_size;
            }
            current_block_start = -1; // Reiniciamos el bloque libre
            current_block_size = 0;
        }
    }

    // Comparar el último bloque libre (si es el mayor)
    if (current_block_size > worst_block_size)
    {
        worst_block_start = current_block_start;
        worst_block_size = current_block_size;
    }

    // Si no se encuentra un bloque suficientemente grande
    if (worst_block_size < size)
    {
        printf("Error: No hay un bloque libre suficientemente grande.\n");
        return NULL;
    }

    // Asignar memoria en el bloque más grande
    memset(&memory_base[worst_block_start], variable_name[0], size);
    memory_available -= size;
    printf("ALLOC (Worst-Fit): Asignados %zu bytes a la variable '%s' en la direccion %p\n", size, variable_name, &memory_base[worst_block_start]);

    return &memory_base[worst_block_start];
}

void freeMemory(const char *variable_name)
{
    int block_found = 0;

    // Recorrer la memoria buscando el bloque asociado a variable_name
    for (int i = 0; i < MEMORY_SIZE;)
    {
        if (memory_base[i] == variable_name[0])
        {
            int block_start = i;

            // Encontrar el tamaño del bloque
            while (i < MEMORY_SIZE && memory_base[i] == variable_name[0])
            {
                i++;
            }

            int block_size = i - block_start;

            // Marcar el bloque como libre (poniéndolo a 0)
            memset(&memory_base[block_start], 0, block_size);
            memory_available += block_size;

            printf("FREE: Liberados %d bytes asociados a la variable '%s' en la direccion %p\n", block_size, variable_name, &memory_base[block_start]);
            block_found = 1;
            break;
        }
        else
        {
            i++;
        }
    }

    if (!block_found)
    {
        printf("Error: No se encontró un bloque asociado a la variable '%s'.\n", variable_name);
    }
}

// Función para imprimir el estado de la memoria
void print_memory_status()
{
    printf("Estado actual de la memoria:\n");
    int i = 0;

    while (i < MEMORY_SIZE)
    {
        if (memory_base[i] == 0)
        {
            // Bloque libre
            int free_block_start = i;
            int free_block_size = 0;

            // Contamos el tamaño del bloque libre
            while (i < MEMORY_SIZE && memory_base[i] == 0)
            {
                free_block_size++;
                i++;
            }
            printf("Libre: %d bytes desde la direccion %p\n", free_block_size, &memory_base[free_block_start]);
        }
        else
        {
            // Bloque ocupado
            char variable_name = memory_base[i]; // Tomamos el nombre de la variable
            int occupied_block_start = i;
            int occupied_block_size = 0;

            // Contamos el tamaño del bloque ocupado
            while (i < MEMORY_SIZE && memory_base[i] == variable_name)
            {
                occupied_block_size++;
                i++;
            }
            printf("Variable '%c': %d bytes desde la direccion %p\n", variable_name, occupied_block_size, &memory_base[occupied_block_start]);
        }
    }
}


void process_input_file(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("No se pudo abrir el archivo %s\n", filename);
        return;
    }

    char line[256];
    char command[10];
    char variable_name[MAX_VARIABLE_NAME];
    size_t size;

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '#' || line[0] == '\n')
            continue; // Ignorar comentarios y lineas en blanco

        if (sscanf(line, "%s", command) == 1)
        {
            if (strcmp(command, "ALLOC") == 0)
            {
                if (sscanf(line, "%*s %s %zu", variable_name, &size) == 2)
                {
                    // first_fit_alloc(size, variable_name);
                    best_fit_alloc(size, variable_name);
                    // worst_fit_alloc(size, variable_name);
                }
            }
            else if (strcmp(command, "REALLOC") == 0)
            {
                if (sscanf(line, "%*s %s %zu", variable_name, &size) == 2)
                {
                    printf("REALLOC %s %zu\n", variable_name, size);
                }
            }
            else if (strcmp(command, "FREE") == 0)
            {
                if (sscanf(line, "%*s %s", variable_name) == 1)
                {
                    freeMemory(variable_name);
                }
            }
            else if (strcmp(command, "PRINT") == 0)
            {
                print_memory_status();
            }
        }
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <archivo_de_entrada>\n", argv[0]);
        return 1;
    }
    init_memory();
    process_input_file(argv[1]);

    // Liberar la memoria
    free(memory_base);
    return 0;
}