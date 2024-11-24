# Verificación de Conexidad de un Grafo

Este programa verifica si un grafo dirigido es fuertemente conectado mediante una búsqueda en profundidad (DFS) paralelizada. Utiliza múltiples hilos y permite la opción de visualizar cada paso de la verificación.

## Requisitos

- GCC para compilar
- Biblioteca Pthread (generalmente incluida con GCC)

## Compilación

Para compilar el programa, utiliza el siguiente comando en la terminal:

```bash
gcc Trabajo1.c 
```
## Ejecución
Para ejecutar el programa, utiliza el siguiente comando en la terminal:

```bash
./grafo_conexidad <num_hilos> -O<modo> < data.txt
```
