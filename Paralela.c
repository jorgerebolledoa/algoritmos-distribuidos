#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX 100  // Define el tamaño máximo de la matriz de adyacencia

// Estructura para pasar múltiples argumentos a la función de hilo
typedef struct {
    int graph[MAX][MAX];
    int n;
    int v;
    bool *visited;
} ThreadData;

// Función para realizar una búsqueda en profundidad (DFS) en el grafo
void* DFS(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;
    int v = data->v;
    bool* visited = data->visited;
    
    visited[v] = true;  // Marca el vértice `v` como visitado

    // Recorre todos los vértices
    for (int i = 0; i < n; i++) {
        // Si hay una conexión y el vértice `i` no ha sido visitado, llama a DFS de forma recursiva
        if (data->graph[v][i] && !visited[i]) {
            ThreadData newData = { .graph = data->graph, .n = n, .v = i, .visited = visited };
            DFS(&newData);  // Llama a DFS recursivamente
        }
    }
    return NULL;
}

// Función para verificar si un grafo es fuertemente conectado
bool isStronglyConnected(int graph[MAX][MAX], int n) {
    bool visited[MAX];  // Array para rastrear los vértices visitados en la DFS

    // Paso 1: Realizar DFS desde el primer vértice para ver si se pueden alcanzar todos los vértices
    for (int i = 0; i < n; i++) visited[i] = false;  // Inicializa todos los vértices como no visitados

    ThreadData data = { .graph = {0}, .n = n, .v = 0, .visited = visited };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            data.graph[i][j] = graph[i][j];  // Copiar el grafo a la estructura de datos del hilo
        }
    }

    pthread_t thread;
    pthread_create(&thread, NULL, DFS, &data);  // Crea un hilo para ejecutar DFS
    pthread_join(thread, NULL);  // Espera a que el hilo termine

    // Comprobar si todos los vértices fueron alcanzados
    for (int i = 0; i < n; i++) {
        if (!visited[i]) return false;  // Si algún vértice no fue visitado, el grafo no es fuertemente conectado
    }

    // Paso 2: Crear el grafo transpuesto (invirtiendo las direcciones de las aristas)
    int transposed[MAX][MAX];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            transposed[i][j] = graph[j][i];  // El grafo transpuesto tiene la conexión inversa
        }
    }

    // Paso 3: Realizar DFS en el grafo transpuesto desde el primer vértice
    for (int i = 0; i < n; i++) visited[i] = false;  // Vuelve a marcar todos los vértices como no visitados

    ThreadData transposedData = { .graph = {0}, .n = n, .v = 0, .visited = visited };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            transposedData.graph[i][j] = transposed[i][j];  // Copiar el grafo transpuesto
        }
    }

    pthread_create(&thread, NULL, DFS, &transposedData);  // Crea un hilo para ejecutar DFS en el grafo transpuesto
    pthread_join(thread, NULL);  // Espera a que el hilo termine

    // Comprobar si todos los vértices fueron alcanzados en el grafo transpuesto
    for (int i = 0; i < n; i++) {
        if (!visited[i]) return false;  // Si algún vértice no fue visitado, el grafo no es fuertemente conectado
    }

    return true;  // Si todos los vértices fueron alcanzados en ambas DFS, el grafo es fuertemente conectado
}

int main() {
    int n;  // Variable para almacenar el tamaño de la matriz de adyacencia
    int graph[MAX][MAX];  // Matriz de adyacencia para representar el grafo
    FILE *file = fopen("data.txt", "r");  // Abre el archivo "data.txt" en modo lectura

    // Comprobar si el archivo se abrió correctamente
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fscanf(file, "%d", &n);  // Leer el tamaño de la matriz de adyacencia desde el archivo

    // Leer la matriz de adyacencia desde el archivo
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%d", &graph[i][j]);  // Lee cada elemento de la matriz
        }
    }

    fclose(file);  // Cierra el archivo después de leer los datos

    // Verifica si el grafo es fuertemente conectado usando la función `isStronglyConnected`
    if (isStronglyConnected(graph, n)) {
        printf("El grafo es fuertemente conectado.\n");
    } else {
        printf("El grafo NO es fuertemente conectado.\n");
    }

    return 0;
}
