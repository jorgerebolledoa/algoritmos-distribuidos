#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Definición de constantes
#define USAGE "Uso: %s <k> -O <modo> < data.txt\n"

// Definición de tipos de datos
typedef struct {
    int thread_id;
    int start;
    int end;
    unsigned int num_nodes;
    int **graph;
    unsigned int *stop;
} ThreadData;

// Función para realizar DFS en un nodo
void dfs(unsigned int node, unsigned int num_nodes, int **graph, unsigned int *visited) {
    visited[node] = 1;
    for (unsigned int i = 0; i < num_nodes; i++) {
        if (graph[node][i] && !visited[i]) {
            dfs(i, num_nodes, graph, visited);
        }
    }
}

// Función para verificar la conectividad
void *check_connectivity(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    unsigned int *local_visited = calloc(data->num_nodes, sizeof(unsigned int));
    if (!local_visited) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    for (int i = data->start; i < data->end; i++) {
        if (*(data->stop)) {
            free(local_visited);
            return NULL;
        }

        for (unsigned int j = 0; j < data->num_nodes; j++) {
            local_visited[j] = 0;
        }

        dfs(i, data->num_nodes, data->graph, local_visited);

        for (unsigned int j = 0; j < data->num_nodes; j++) {
            if (!local_visited[j]) {
                *(data->stop) = 1;
                free(local_visited);
                return NULL;
            }
        }
    }

    free(local_visited);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf(USAGE, argv[0]);
        return 1;
    }

    // Parsear argumentos
    int num_threads = atoi(argv[1]);
    char mode = argv[2][1];

    // Leer el número de nodos
    unsigned int num_nodes;
    scanf("%u", &num_nodes);

    // Asignar memoria para el grafo
    int **graph = malloc(num_nodes * sizeof(int *));
    if (!graph) {
        perror("Error al asignar memoria para el grafo");
        return 1;
    }

    for (unsigned int i = 0; i < num_nodes; i++) {
        graph[i] = malloc(num_nodes * sizeof(int));
        if (!graph[i]) {
            perror("Error al asignar memoria para las filas del grafo");
            return 1;
        }
        for (unsigned int j = 0; j < num_nodes; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    if (mode == 'V') {
        printf("El grafo ingresado es:\n");
        for (unsigned int i = 0; i < num_nodes; i++) {
            for (unsigned int j = 0; j < num_nodes; j++) {
                printf("%d ", graph[i][j]);
            }
            printf("\n");
        }
    }

    unsigned int stop = 0;

    // Crear hilos
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    int nodes_per_thread = num_nodes / num_threads;
    int extra_nodes = num_nodes % num_threads;
    int start = 0;

    for (int i = 0; i < num_threads; i++) {
        int end = start + nodes_per_thread + (i < extra_nodes ? 1 : 0);

        thread_data[i].thread_id = i;
        thread_data[i].start = start;
        thread_data[i].end = end;
        thread_data[i].num_nodes = num_nodes;
        thread_data[i].graph = graph;
        thread_data[i].stop = &stop;

        pthread_create(&threads[i], NULL, check_connectivity, &thread_data[i]);

        start = end;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    if (stop) {
        printf("El grafo NO es fuertemente conexo.\n");
    } else {
        printf("El grafo es fuertemente conexo.\n");
    }

    // Liberar memoria
    for (unsigned int i = 0; i < num_nodes; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}
