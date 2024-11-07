#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

#define MAX 100

bool isConnectedGlobal = true;
pthread_mutex_t lock;
int numThreads;
char mode;

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
    int start;
    int end;
    bool *visited;
    int threadId;
} ThreadData;

// Función DFS paralelizada con control de parada temprana
void* DFS(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int (*graph)[MAX] = data->graph;
    int n = data->n;
    bool* visited = data->visited;

    for (int v = data->start; v < data->end && isConnectedGlobal; v++) {
        Stack *stack = createStack(n);
        push(stack, v);

        while (stack->top != -1 && isConnectedGlobal) {
            int vertex = pop(stack);
            if (!visited[vertex]) {
                visited[vertex] = true;

                if (mode == 'V') {
                    printf("Hilo %d visitando nodo %d\n", data->threadId, vertex);
                }

                for (int i = 0; i < n; i++) {
                    if (graph[vertex][i] && !visited[i]) {
                        push(stack, i);
                    }
                }
            }
        }
        freeStack(stack);
    }
    return NULL;
}

// Función para verificar si un grafo es fuertemente conectado
bool isStronglyConnected(int graph[MAX][MAX], int n) {
    pthread_t threads[MAX];
    bool visited[MAX];

    if (numThreads > n) numThreads = n;
    int verticesPerThread = n / numThreads;
    int remainder = n % numThreads;

    for (int i = 0; i < numThreads; i++) {
        int start = i * verticesPerThread;
        int end = (i == numThreads - 1) ? (start + verticesPerThread + remainder) : (start + verticesPerThread);

        for (int j = 0; j < n; j++) visited[j] = false;

        ThreadData *data = malloc(sizeof(ThreadData));
        data->graph = graph;
        data->n = n;
        data->start = start;
        data->end = end;
        data->visited = visited;
        data->threadId = i + 1;

        pthread_create(&threads[i], NULL, DFS, data);
        pthread_join(threads[i], NULL);

        // Verificar si algún nodo no se alcanzó
        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                pthread_mutex_lock(&lock);
                isConnectedGlobal = false;
                pthread_mutex_unlock(&lock);
                return false;
            }
        }
        free(data);
    }

    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <k> -O <modo> < data.txt\n", argv[0]);
        return 1;
    }

    numThreads = atoi(argv[1]);
    mode = argv[2][1];  // Toma el modo de la opción -O

    int n;
    int graph[MAX][MAX];
    if (scanf("%d", &n) != 1) {
        printf("Error al leer el número de vértices.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scanf("%d", &graph[i][j]) != 1) {
                printf("Error al leer el grafo.\n");
                return 1;
            }
        }
    }

    pthread_mutex_init(&lock, NULL);

    if (isStronglyConnected(graph, n) && isConnectedGlobal) {
        printf("El grafo es fuertemente conectado.\n");
    } else {
        printf("El grafo NO es fuertemente conectado.\n");
    }

    pthread_mutex_destroy(&lock);

    return 0;
}
