/*
 * Laboratorio 4 - Ejercicio 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Reserva memoria para una matriz cuadrada
 */
void allocateMatrix(int ***matrix, int size) {
    *matrix = (int **) malloc(size * sizeof(int *));

    if (*matrix == NULL) {
        printf("Error, memoria para la matriz.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        *(*matrix + i) = (int *) malloc(size * sizeof(int));

        if (*(*matrix + i) == NULL) {
            printf("Error, memoria para una fila.\n");

            for (int j = 0; j < i; j++) {
                free(*(*matrix + j));
            }

            free(*matrix);
            *matrix = NULL;
            return;
        }
    }
}

/*
 * Llena la matriz con valores aleatorios de 0 y 1
 */
void fillMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            *(*(matrix + i) + j) = rand() % 2;
        }
    }
}

/*
 * Imprime la matriz
 */
void printMatrix(int **matrix, int size) {
    printf("Matriz (%dx%d):\n", size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", *(*(matrix + i) + j));
        }
        printf("\n");
    }
}

/*
 * Busca la linea mas larga de 1s consecutivos
 */
void LargestLine(int **matrix, int size, int *result) {
    int largest = 0;
    int count = 0;


    for (int i = 0; i < size; i++) {
        int *row = *(matrix + i);

        for (int j = 0; j < size; j++) {
            if (*(row + j) == 1) {
                count++;
            } else {
                if (count > largest) {
                    largest = count;
                }

                count = 0;
            }
        }
    }
    if (count > largest) {
        largest = count;
    }

    *result = largest;
}

/*
 * Borra la memoria
 */
void freeMatrix(int **matrix, int size) {
    if (matrix != NULL) {
        for (int i = 0; i < size; i++) {
            free(*(matrix + i));
        }

        free(matrix);
    }
}
\
int main(void) {
    int size;
    int largestLine = 0;
    int **matrix = NULL;

    srand((unsigned int) time(NULL));

    printf("Ingrese el tamanio de la matriz: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("El tamanio debe ser un entero mayor que 0.\n");
        return 1;
    }

    allocateMatrix(&matrix, size);

    if (matrix == NULL) {
        return 1;
    }

    fillMatrix(matrix, size);
    printMatrix(matrix, size);
    LargestLine(matrix, size, &largestLine);

    printf("El tamano de la secuencia de 1s mas grande es: %d\n", largestLine);

    freeMatrix(matrix, size);

    return 0;
}
