#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100  // Define el tamaño máximo de la matriz de adyacencia

// Estructura para la pila dinámica
typedef struct {
    unsigned int maxSize;
    int top;
   unsigned int *data;
} Stack;

// Función para crear una pila con memoria dinámica
Stack *createStack(int maxSize) {
    Stack *stack = (Stack *)calloc(1, sizeof(Stack));
    stack->maxSize = maxSize;
    stack->top = -1;
    stack->data = (int *)calloc(maxSize, sizeof(int));
    return stack;
}

// Función para empujar un elemento en la pila
void push(Stack *stack, int value) {
    if (stack->top == stack->maxSize - 1) return;
    stack->data[++stack->top] = value;
}

// Función para sacar un elemento de la pila
int pop(Stack *stack) {
    return (stack->top == -1) ? -1 : stack->data[stack->top--];
}

// Función para liberar la memoria de la pila
void freeStack(Stack *stack) {
    free(stack->data);
    free(stack);
}

// Función para realizar una búsqueda en profundidad (DFS) usando una pila dinámica
void DFS(int graph[MAX][MAX], int n, int v, bool *visited) {
    Stack *stack = createStack(n);
    push(stack, v);

    while (stack->top != -1) {
        int vertex = pop(stack);
        if (!visited[vertex]) {
            visited[vertex] = true;

            for (int i = 0; i < n; i++) {
                if (graph[vertex][i] && !visited[i]) {
                    printf ("Nodo %d conectado a nodo %d, añadiendo a la pila %d \n ", vertex, i, visited[i]);
                    push(stack, i);
                }
            }
        }
    }

    freeStack(stack);
}

// Función para verificar si un grafo es fuertemente conectado
bool isStronglyConnected(int graph[MAX][MAX], int n) {
    bool visited[MAX];

    // Paso 1: Realizar DFS desde el primer vértice
    for (int i = 0; i < n; i++) visited[i] = false;

    DFS(graph, n, 0, visited);

    // Comprobar si todos los vértices fueron alcanzados
    for (int i = 0; i < n; i++) {
        if (!visited[i]) return false;
    }

    // Paso 2: Crear el grafo transpuesto (invirtiendo las aristas)
    int transposed[MAX][MAX];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            transposed[i][j] = graph[j][i];
        }
    }

    // Paso 3: Realizar DFS en el grafo transpuesto desde el primer vértice
    for (int i = 0; i < n; i++) visited[i] = false;

    DFS(transposed, n, 0, visited);

    // Comprobar si todos los vértices fueron alcanzados en el grafo transpuesto
    for (int i = 0; i < n; i++) {
        if (!visited[i]) return false;
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

    if (isStronglyConnected(graph, n)) {
        printf("El grafo es fuertemente conectado.\n");
    } else {
        printf("El grafo NO es fuertemente conectado.\n");
    }

    return 0;
}
