# Administración de Memoria

## ¿Cómo se compila?
Asegurarse de tener el compilador de C instalado localmente. Utilizar el siguiente comando para compilar y obtener un archivo ejecutable:
```bash
gcc file.c -o file
```

## ¿Cómo se ejecuta?
Tener un archivo llamado `input.txt` en la misma carpeta que el ejecutable. Para correr el programa se utiliza el siguiente comando:
```bash
.\file input.txt <option>
```
Donde option puede tener 3 valores diferentes:
- 1: First-Fit 
- 2: Best-Fit
- 3: Worst-Fit

En caso de utilizar alguna otra opción, se ejecutará con Worst-Fit por defecto.

## ¿Cómo funciona el programa?
(Cada funcion tiene su respectivo manejo de errores).
El programa simula la gestión dinámica de memoria en un sistema, utilizando diferentes algoritmos de asignación para simular cómo se realizarían las asignaciones de memoria en un sistema operativo real. A continuación se explica cada una de las funciones:

- int main(int argc, char *argv[])
Funcion principal que inicializa la memoria, procesa el archivo de entrada y realiza la gestion de memoria
Valida que el numero de argumentos sea correcto y al final, libera la memoria asignada con free().

- void process_input_file(char *filename, int option)
Procesa un archivo de entrada que contiene comandos para asignar, liberar, realocar o imprimir el estado
de la memoria. Abre el archivo y lo lee linea por linea interpretando los comandos (ALLOC, REALLOC, FREE, PRINT) 
y ejecutando el correspondiente. Tambien recibe como para metro 'option' que se usa para seleccionar el
algoritmo a ejecutar.

- void *reallocMemory(size_t size, const char *variable_name, int algorithm)
Realiza la realocacion de memoria. Libera la memoria de la variable indicada utilizando 'freeMemory' y
luego realiza la nueva asignacion de memoria usando el algoritmo escogido. 

- void print_memory_status()
Imprime el estado actual de la memoria, recorra la memoria y muestra los bloques libres y ocupados.
Para los bloques libres, imprime el tamano y la direccion, y para los bloques ocupados muestra la 
variable asociada y su tamano.

- bool freeMemory(const char *variable_name, bool isRealloc)
Libera un bloque de memoria previamente asignado a una variable. Busca en memoria un bloque que
coincide con la variable y libera la memoria. 

- char *worst_fit_alloc(size_t size, const char *variable_name, bool isRealloc)
Usa el algoritmo de worst fit para asignar memoria. Usa el bloque libre mas grande. 

- char *best_fit_alloc(size_t size, const char *variable_name, bool isRealloc)
Usa el algoritmo de best fit. Busca el bloque libre mas pequeno que pueda contener el tamano solicitado.

- char *first_fit_alloc(size_t size, const char *variable_name, bool isRealloc)
Usa el algoritmo de first fit. Busca el primer bloque de memoria libre que sea los suficientemente grande para
acomodar el 'size' bytes.

- void init_memory()
Inicializa la memoria del sistema. Asigna un bloque de memoria de tamano 'MEMORY_SIZE' usando malloc.
Inicializa toda la memoria con ceros utilizando memset. Y establece el tamano de la memoria a 'MEMORY_SIZE'.