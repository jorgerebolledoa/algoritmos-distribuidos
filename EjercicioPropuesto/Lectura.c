#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct {
    const char *filename;
    int thread_id;
} ThreadData;

void *read_file(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    FILE *file = fopen(data->filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file in thread %d\n", data->thread_id);
        pthread_exit(NULL);
    }

    int ch; // Variable para leer cada carácter.
    printf("Thread %d starts reading the file\n", data->thread_id);
    while ((ch = fgetc(file)) != EOF) {
        // Imprime el carácter leído con el identificador del hilo.
        printf("Thread %d read: %c\n", data->thread_id, ch);
    }

    fclose(file);
    printf("Thread %d finished reading the file\n", data->thread_id);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].filename = argv[1];
        thread_data[i].thread_id = i;
        if (pthread_create(&threads[i], NULL, read_file, &thread_data[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}
