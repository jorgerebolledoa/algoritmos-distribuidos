#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX 100

// Variable global de control para detectar desconexión y detener otros hilos
bool isConnectedGlobal = true;
pthread_mutex_t lock;

// Estructura para la pila dinámica
typedef struct {
    int maxSize;
    int top;
    int *data;
} Stack;

Stack *createStack(int maxSize) {
    Stack *stack = (Stack *)calloc(1, sizeof(Stack));
    stack->maxSize = maxSize;
    stack->top = -1;
    stack->data = (int *)calloc(maxSize, sizeof(int));
    return stack;
}

void push(Stack *stack, int value) {
    if (stack->top == stack->maxSize - 1) return;
    stack->data[++stack->top] = value;
}

int pop(Stack *stack) {
    return (stack->top == -1) ? -1 : stack->data[stack->top--];
}

void freeStack(Stack *stack) {
    free(stack->data);
    free(stack);
}

// Estructura para pasar múltiples argumentos a la función de DFS
typedef struct {
    int (*graph)[MAX];
    int n;
    int v;
    bool *visited;
} ThreadData;

// Función DFS paralelizada con control de parada temprana
void* DFS(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int (*graph)[MAX] = data->graph;
    int n = data->n;
    int v = data->v;
    bool* visited = data->visited;

    Stack *stack = createStack(n);
    push(stack, v);

    while (stack->top != -1 && isConnectedGlobal) {
        int vertex = pop(stack);
        if (!visited[vertex]) {
            visited[vertex] = true;
            for (int i = 0; i < n; i++) {
                if (graph[vertex][i] && !visited[i]) {
                    push(stack, i);
                }
            }
        }
    }

    freeStack(stack);
    return NULL;
}

// Función para verificar si un grafo es fuertemente conectado
bool isStronglyConnected(int graph[MAX][MAX], int n) {
    pthread_t threads[MAX];
    bool visited[MAX];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) visited[j] = false;

        ThreadData data = { .graph = graph, .n = n, .v = i, .visited = visited };
        pthread_create(&threads[i], NULL, DFS, &data);

        // Esperar a que el hilo termine y verificar si el nodo no se puede alcanzar
        pthread_join(threads[i], NULL);

        // Verificar si todos los nodos fueron alcanzados
        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                pthread_mutex_lock(&lock);
                isConnectedGlobal = false;
                pthread_mutex_unlock(&lock);
                return false;
            }
        }
    }

    return true;
}

int main() {
    int n;
    int graph[MAX][MAX];
    FILE *file = fopen("data.txt", "r");

    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fscanf(file, "%d", &n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%d", &graph[i][j]);
        }
    }

    fclose(file);

    pthread_mutex_init(&lock, NULL);

    if (isStronglyConnected(graph, n) && isConnectedGlobal) {
        printf("El grafo es fuertemente conectado.\n");
    } else {
        printf("El grafo NO es fuertemente conectado.\n");
    }

    pthread_mutex_destroy(&lock);

    return 0;
}
