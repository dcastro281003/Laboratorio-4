/*
 * Laboratorio 4 - Ejercicio 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Lee una imagen PGM 
 */
int *read_pgm(const char *filename, int *width, int *height, int *max_val) {
    FILE *file = NULL;
    char format[3];
    int total;
    int *pixels = NULL;

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", filename);
        return NULL;
    }
    if (fscanf(file, "%2s", format) != 1) {
        printf("No se pudo leer el formato del archivo.\n");
        fclose(file);
        return NULL;
    }
    if (strcmp(format, "P2") != 0) {
        printf("El archivo no esta en formato PGM P2.\n");
        fclose(file);
        return NULL;
    }
    if (fscanf(file, "%d %d", width, height) != 2) {
        printf("No se pudo leer el ancho y el alto.\n");
        fclose(file);
        return NULL;
    }
    if (fscanf(file, "%d", max_val) != 1) {
        printf("No se pudo leer el valor maximo.\n");
        fclose(file);
        return NULL;
    }
    
    total = (*width) * (*height);
    pixels = (int *) malloc(total * sizeof(int));

    if (pixels == NULL) {
        printf("No se pudo reservar memoria para los pixeles.\n");
        fclose(file);
        return NULL;
    }
    for (int i = 0; i < total; i++) {
        if (fscanf(file, "%d", pixels + i) != 1) {
            printf("No se pudo leer todos los pixeles.\n");
            free(pixels);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);

    return pixels;
}

/*
 * Crea una copia dinamica de un arreglo de pixeles.
 */
int *copy_pixels(int *pixels, int total) {
    int *copy = NULL;

    copy = (int *) malloc(total * sizeof(int));

    if (copy == NULL) {
        printf("No se pudo reservar memoria para la copia.\n");
        return NULL;
    }

    for (int i = 0; i < total; i++) {
        *(copy + i) = *(pixels + i);
    }

    return copy;
}

/*
 * Aplica el umbral T 
 */
void apply_threshold(int *pixels, int total, int threshold) {
    for (int i = 0; i < total; i++) {
        if (*(pixels + i) >= threshold) {
            *(pixels + i) = 255;
        } else {
            *(pixels + i) = 0;
        }
    }
}

/*
 * Genera el negativo 
 */
int *make_negative(int *pixels, int total) {
    int *negative = NULL;

    negative = (int *) malloc(total * sizeof(int));

    if (negative == NULL) {
        printf("No se pudo reservar memoria para el negativo.\n");
        return NULL;
    }

    for (int i = 0; i < total; i++) {
        *(negative + i) = 255 - *(pixels + i);
    }

    return negative;
}


void write_pgm(const char *filename, int *pixels,
               int width, int height, int max_val) {
    FILE *file = NULL;
    int total = width * height;

    file = fopen(filename, "w");

    if (file == NULL) {
        printf("No se pudo crear el archivo %s\n", filename);
        return;
    }

    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "%d\n", max_val);

    for (int i = 0; i < total; i++) {
        fprintf(file, "%d ", *(pixels + i));

        if ((i + 1) % width == 0) {
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

/*
 * Imprime la cantidad de pixeles blancos y negros con el promedio
 */
void print_stats(int *original, int *thresholded, int total) {
    int whitePixels = 0;
    int blackPixels = 0;
    int sum = 0;
    float average;

    for (int i = 0; i < total; i++) {
        sum += *(original + i);

        if (*(thresholded + i) == 255) {
            whitePixels++;
        } else if (*(thresholded + i) == 0) {
            blackPixels++;
        }
    }

    average = (float) sum / total;
    printf("Pixeles blancos en la imagen: %d\n", whitePixels);
    printf("Pixeles negros en la imagen: %d\n", blackPixels);
    printf("Promedio de pixeles de la imagen original: %.2f\n", average);
}

int main(void) {
    int width;
    int height;
    int max_val;
    int threshold;
    int total;
    int *pixels = NULL;
    int *thresholded = NULL;
    int *negative = NULL;

    pixels = read_pgm("input.pgm", &width, &height, &max_val);

    if (pixels == NULL) {
        return 1;
    }

    total = width * height;

    printf("Ingrese el valor de umbral: ");
    scanf("%d", &threshold);

    thresholded = copy_pixels(pixels, total);

    if (thresholded == NULL) {
        free(pixels);
        return 1;
    }

    apply_threshold(thresholded, total, threshold);

    negative = make_negative(thresholded, total);

    if (negative == NULL) {
        free(pixels);
        free(thresholded);
        return 1;
    }

    write_pgm("output_threshold.pgm", thresholded, width, height, 255);
    write_pgm("output_negative.pgm", negative, width, height, 255);
    print_stats(pixels, thresholded, total);

    free(pixels);
    free(thresholded);
    free(negative);

    return 0;
}
