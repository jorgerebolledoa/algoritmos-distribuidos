#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

unsigned int isConnectedGlobal;
int numThreads;
char mode;

// Estructura para la pila dinámica
typedef struct {
    int maxSize;
    int top;
    int *data;
} Stack;

// Estructura para pasar múltiples argumentos a la función de DFS
typedef struct {
    int **graph;
    int n;
    int start;
    int end;
    int *visited;
    int threadId;
} ThreadData;

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

// Función DFS paralelizada con control de parada temprana
void* DFS(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int **graph = data->graph;
    int n = data->n;
    int* visited = data->visited;

    if (mode == 'V') {
        printf("Hilo %d iniciando verificación desde nodo %d hasta %d\n", data->threadId, data->start, data->end - 1);
    }

    for (int v = data->start; v < data->end && isConnectedGlobal == 1; v++) {
        printf("v es %d \n",v);
        Stack *stack = createStack(n);
        push(stack, v);
        if (mode == 'V') {
            printf("Hilo %d: Estado actual de la pila: ", data->threadId);
            for (int i = 0; i <= stack->top; i++) {
                printf("%d ", stack->data[i]);
            }
            printf("\n");
        }
        while (stack->top != -1 && isConnectedGlobal == 1) {
            int vertex = pop(stack);
            printf("vertex es %d \n",vertex);
            if (visited[vertex] != 1) {
                visited[vertex] = 1;

                if (mode == 'V') {
                    printf("Hilo %d visitando nodo %d\n", data->threadId, vertex);
                }

                for (int i = 0; i < n; i++) {
                    if (graph[vertex][i] && visited[i] != 1) {
                        push(stack, i);
                        if (mode == 'V') {
                            printf("Hilo %d: Estado actual de la pila: ", data->threadId);
                            for (int j = 0; j <= stack->top; j++) {
                                printf("%d ", stack->data[j]);
                                
                            }
                            printf("\n");
                            printf("visited[%d] es %d \n",i,visited[i]);
                        }
                        if (mode == 'V') {
                            printf("Hilo %d: Nodo %d conectado a nodo %d, añadiendo a la pila\n", data->threadId, vertex, i);
                        }
                    }
                }
              
            }
            
        }
        for (int i = 0; i < n; i++) {
            printf("visited[%d] es %d \n",i,visited[i]);
        if (visited[i] != 1) {
            isConnectedGlobal = 0;
            printf("Hilo %d detectó que el nodo %d no es alcanzable. Deteniendo otros hilos.\n", data->threadId, i);
        }
           if (mode == 'V') {
        printf("Hilo %d completó la verificación de su rango de nodos.\n", data->threadId);
        }
        }
        printf("Stack libre \n");
        freeStack(stack);
        

    }

    if (mode == 'V') {
        printf("Hilo %d completó la verificación de su rango de nodos.\n", data->threadId);
    }

    return NULL;
    
}

// Función para verificar si un grafo es fuertemente conectado
int isStronglyConnected(int **graph, int n) {
    isConnectedGlobal = 1;
    pthread_t *threads = calloc(numThreads, sizeof(pthread_t));
    int *visited = calloc(n, sizeof(int));

    if (numThreads > n) numThreads = n;
    int verticesPerThread = n / numThreads;
    int remainder = n % numThreads;

    for (int i = 0; i < numThreads; i++) {
        int start = i * verticesPerThread;
        int end = (i == numThreads - 1) ? (start + verticesPerThread + remainder) : (start + verticesPerThread);

        memset(visited, 0, n * sizeof(int));

        ThreadData *data = malloc(sizeof(ThreadData));
        data->graph = graph;
        data->n = n;
        data->start = start;
        data->end = end;
        data->visited = visited;
        data->threadId = i + 1;

        if (mode == 'V') {
            printf("Creando hilo %d para verificar nodos desde %d hasta %d\n", data->threadId, start, end - 1);
        }

        pthread_create(&threads[i], NULL, DFS, data);
        pthread_join(threads[i], NULL);

        // Verificar si algún nodo no se alcanzó
        for (int j = 0; j < n; j++) {
            if (visited[j] != 1) {
               isConnectedGlobal = 0;                
                if (mode == 'V') {
                    printf("Hilo %d detectó que el nodo %d no es alcanzable. Deteniendo otros hilos.\n", data->threadId, j);
                }
                return 0;
            }
            
        }

       free(data);
    }

    free(threads);
    free(visited);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <k> -O <modo> < data.txt\n", argv[0]);
        return 1;
    }
    numThreads = atoi(argv[1]);
    mode = argv[2][1]; 
    
    // Asignación dinámica para el grafo
    unsigned int n;
    scanf("%d", &n);
    int **graph = (int **)calloc(n, sizeof(int *));
    for (int i = 0; i < n; i++) {
        graph[i] = (int *)calloc(n, sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
        }
    }
     if (mode == 'V') {
    printf("El grafo ingresado es:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
    }
    
    if (mode == 'V') {
        printf("Iniciando verificación de conexidad con %d hilos\n", numThreads);
    }

    if (isStronglyConnected(graph, n) && isConnectedGlobal==1) {
        printf("%d", isConnectedGlobal);
        printf("El grafo es fuertemente conectado.\n");
    } else {
                printf("%d", isConnectedGlobal);

        printf("El grafo NO es fuertemente conectado.\n");
    }

    if (mode == 'V') {
        printf("Verificación de conexidad completada.\n");
    }

    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}
