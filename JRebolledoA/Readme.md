# Verificación de Conexidad de un Grafo

Este programa verifica si un grafo dirigido es fuertemente conectado mediante una búsqueda en profundidad (DFS) paralelizada. Utiliza múltiples hilos y permite la opción de visualizar cada paso de la verificación.

## Requisitos

- GCC para compilar
- Biblioteca Pthread (generalmente incluida con GCC)

## Compilación

Para compilar el programa, utiliza el siguiente comando en la terminal:

```bash
gcc JorgeRebolledoT1.c -o t1.exe
```
## Ejecución
Para ejecutar el programa, utiliza el siguiente comando en la terminal:

```bash
./t1.exe <num_hilos> -O<modo> < data.txt
```
