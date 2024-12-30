#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define EMPTY     0
#define FULL      2
#define NOCHIP    0
#define CHIP1     1
#define CHIP2     2
#define NOOWNER   3
#define UNVISITED 1
#define VISITED   0

struct GOCell {
    unsigned char element, statuscell, prop;
};

void ReadGOBoard(struct GOCell **goboard, unsigned int n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (unsigned int i = 0; i < n; ++i) {
        for (unsigned int j = 0; j < n; ++j) {
            fscanf(file, "%1hhu", &goboard[i][j].element);
            goboard[i][j].statuscell = UNVISITED;
        }
    }
    fclose(file);
}

void PrintGOBoard(struct GOCell **goboard, unsigned int n, int rank, int size, char partition_mode) {
    MPI_Barrier(MPI_COMM_WORLD);
    for (int r = 0; r < size; ++r) {
        if (rank == r) {
            printf("Process %d:\n", rank);
            for (unsigned int i = 0; i < ((partition_mode == 'H') ? n / size : n); ++i) {
                for (unsigned int j = 0; j < ((partition_mode == 'V') ? n / size : n); ++j) {
                    printf("%d ", goboard[i][j].element);
                }
                printf("\n");
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

void ProcessRegion(struct GOCell **subboard, unsigned int rows, unsigned int cols, unsigned int rank) {
    // Simple processing simulation (e.g., counting elements)
    unsigned int chip1_count = 0, chip2_count = 0;

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            if (subboard[i][j].element == CHIP1) chip1_count++;
            if (subboard[i][j].element == CHIP2) chip2_count++;
        }
    }

    printf("Process %d: CHIP1=%d, CHIP2=%d\n", rank, chip1_count, chip2_count);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 4) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s -P[V|H] -M[S|V] <filename>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    char partition_mode = argv[1][2];
    char output_mode = argv[2][2];
    const char *filename = argv[3];

    unsigned int n;
    struct GOCell **goboard = NULL;
    if (rank == 0) {
        FILE *file = fopen(filename, "r");
        if (!file) {
            fprintf(stderr, "Error opening file %s\n", filename);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fscanf(file, "%u", &n);
        fclose(file);

        goboard = malloc(n * sizeof(struct GOCell *));
        for (unsigned int i = 0; i < n; ++i) {
            goboard[i] = malloc(n * sizeof(struct GOCell));
        }
        ReadGOBoard(goboard, n, filename);
    }

    MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    unsigned int local_rows = (partition_mode == 'H') ? n / size : n;
    unsigned int local_cols = (partition_mode == 'V') ? n / size : n;

    MPI_Datatype MPI_GOCell;
    MPI_Type_contiguous(3, MPI_UNSIGNED_CHAR, &MPI_GOCell);
    MPI_Type_commit(&MPI_GOCell);

    struct GOCell **subboard = malloc(local_rows * sizeof(struct GOCell *));
    for (unsigned int i = 0; i < local_rows; ++i) {
        subboard[i] = malloc(local_cols * sizeof(struct GOCell));
    }

    if (partition_mode == 'H') {
        for (unsigned int i = 0; i < local_rows; ++i) {
            MPI_Scatter(goboard ? goboard[i] : NULL, local_cols, MPI_GOCell,
                        subboard[i], local_cols, MPI_GOCell, 0, MPI_COMM_WORLD);
        }
    } else if (partition_mode == 'V') {
        for (unsigned int i = 0; i < n; ++i) {
            MPI_Scatter(goboard ? goboard[i] : NULL, local_cols, MPI_GOCell,
                        subboard[i], local_cols, MPI_GOCell, 0, MPI_COMM_WORLD);
        }
    }

    ProcessRegion(subboard, local_rows, local_cols, rank);

    PrintGOBoard(subboard, local_rows, rank, size, partition_mode);

    for (unsigned int i = 0; i < local_rows; ++i) {
        free(subboard[i]);
    }
    free(subboard);

    if (rank == 0) {
        for (unsigned int i = 0; i < n; ++i) {
            free(goboard[i]);
        }
        free(goboard);
    }

    MPI_Finalize();
    return 0;
}
