# Verificación de Conectividad Fuerte en un Grafo Dirigido

Este programa en C verifica si un grafo dirigido es **fuertemente conectado**, es decir, si existe un camino para ir y regresar entre todos los vértices del grafo.

## Descripción del Código

El código implementa una función que utiliza la técnica de **búsqueda en profundidad** (DFS) para determinar si un grafo es fuertemente conectado. La verificación se realiza tanto en el grafo original como en su transpuesto, un proceso basado en el algoritmo de **Kosaraju**.

## Estructura del Código

### Archivos
- **data.txt**: Contiene la matriz de adyacencia del grafo, donde:
  - La primera línea es el número de vértices (tamaño de la matriz).
  - Las siguientes líneas contienen los elementos de la matriz de adyacencia en formato columna.
  
- **Secuencial.c**: Archivo fuente en C que lee el archivo `data.txt`, crea la matriz de adyacencia, y verifica si el grafo es fuertemente conectado.

### Funciones Principales

- **DFS**: Realiza una búsqueda en profundidad para marcar todos los vértices alcanzables desde un vértice inicial.
- **isStronglyConnected**: Comprueba si el grafo es fuertemente conectado:
  1. Realiza DFS en el grafo original.
  2. Verifica si se han visitado todos los vértices.
  3. Crea el **grafo transpuesto** (invirtiendo las aristas).
  4. Realiza DFS en el grafo transpuesto.
  5. Verifica nuevamente si todos los vértices se alcanzaron en el transpuesto.
  
  Si ambas búsquedas (original y transpuesto) alcanzan todos los vértices, el grafo es fuertemente conectado.

## Ejemplo de Archivo de Entrada `data.txt`

```plaintext
4
0
1
0
0
0
0
1
0
0
0
0
1
0
0
0
0
