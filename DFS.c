#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100  // Tamaño máximo del grafo

// Definición de la estructura de pila
typedef struct Stack {
    unsigned int maxSize;
    int top;
    unsigned int *data;
} Stack;

// Función para crear una pila con memoria dinámica
Stack *createStack(int maxSize) {
    Stack *stack = (Stack *)calloc(1, sizeof(Stack));
    if (stack == NULL) {
        printf("Error al asignar memoria para la pila\n");
        exit(EXIT_FAILURE);
    }

    stack->maxSize = maxSize;
    stack->top = -1;
    stack->data = (int *)calloc(maxSize, sizeof(int));
    if (stack->data == NULL) {
        printf("Error al asignar memoria para los datos de la pila\n");
        free(stack);
        exit(EXIT_FAILURE);
    }

    return stack;
}

// Función para empujar un elemento en la pila
void push(Stack *stack, int value) {
    if (stack->top == stack->maxSize - 1) {
        printf("Error: pila llena\n");
        return;
    }
    stack->data[++stack->top] = value;
}

// Función para sacar un elemento de la pila
int pop(Stack *stack) {
    if (stack->top == -1) {
        printf("Error: pila vacía\n");
        return -1;
    }
    return stack->data[stack->top--];
}

// Función para liberar la memoria de la pila
void freeStack(Stack *stack) {
    if (stack) {
        free(stack->data);
        free(stack);
    }
}

// Función DFS con pila dinámica
void DFS(int graph[MAX][MAX], int n, int start) {
    bool visited[MAX] = { false };
    Stack *stack = createStack(n); // Crear la pila con tamaño n (o el valor necesario)

    push(stack, start); // Empujar el vértice de inicio en la pila

    while (stack->top != -1) { // Mientras la pila no esté vacía
        int v = pop(stack);    // Sacar un vértice de la pila
        if (!visited[v]) {     // Si no ha sido visitado
            visited[v] = true; // Marcar como visitado
            printf("%d ", v);  // Procesar el vértice (por ejemplo, imprimirlo)

            // Recorrer todos los vértices
            for (int i = n - 1; i >= 0; i--) {  // Recorrer de atrás hacia adelante para mantener el orden
                if (graph[v][i] && !visited[i]) { // Si hay una conexión y no ha sido visitado
                    push(stack, i); // Empujar el vértice en la pila
                }
            }
        }
    }

    freeStack(stack); // Liberar la memoria de la pila al final
}

int main() {
    int n = 5; // Número de vértices
    int graph[MAX][MAX] = { // Grafo de ejemplo
        {0, 1, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 1},
        {0, 0, 1, 1, 0}
    };

    int startVertex = 0;
    printf("Recorrido DFS comenzando desde el vértice %d:\n", startVertex);
    DFS(graph, n, startVertex);

    return 0;
}
