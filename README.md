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
- int main(int argc, char *argv[])
- void process_input_file(char *filename, int option)
- void *reallocMemory(size_t size, const char *variable_name, int algorithm)
- void print_memory_status()
- bool freeMemory(const char *variable_name, bool isRealloc)
- char *worst_fit_alloc(size_t size, const char *variable_name, bool isRealloc)
- char *best_fit_alloc(size_t size, const char *variable_name, bool isRealloc)
- char *first_fit_alloc(size_t size, const char *variable_name, bool isRealloc)
- void init_memory()