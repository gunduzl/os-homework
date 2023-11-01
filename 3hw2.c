#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **array;
int length;

void *thread_function(void *arg) {
    int thread_num = *((int *)arg);
    printf("\nStarted Thread: %d", thread_num);
        for (int i = 0; i < length; i++) {
        array[thread_num][i] = (array[thread_num][i] <= 127) ? 0 : 255;
    }
    return NULL;
}

void writeMatrixToFile(int **modArray) {
    const char *fileName = "HW-2-output.txt";
    // Çıktı dosyasını açın
    FILE *outputFile = fopen(fileName, "w");

    if (outputFile == NULL) {
        perror("error while opening file");
        return;
    }

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            fprintf(outputFile, "%d ", modArray[i][j]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);
}

int readMatrixFromFile(const char *fileName) {
    FILE *file;

    if ((file = fopen(fileName, "r")) == NULL) {
        perror("Failed to open input file");
        return 1;
    }

    fscanf(file, "%d", &length);

    array = (int **)malloc(length * sizeof(int *));
    for (int i = 0; i < length; i++) {
        array[i] = (int *)malloc(length * sizeof(int));
    }

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (fscanf(file, "%d", &array[i][j]) != 1) {
                perror("Failed to read input data");
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

int processMatrix() {
    pthread_t thread_id[length];
    int thread_args[length];

    for (int i = 0; i < length; i++) {
        thread_args[i] = i;
        pthread_create(&thread_id[i], NULL, thread_function, &thread_args[i]);
    }

    for (int i = 0; i < length; i++) {
        pthread_join(thread_id[i], NULL);
    }

    printf("\nFinished Thread Operations \n");
    return 0;
}

int main() {
    char name[] = "data.dat";

    if (readMatrixFromFile(name) != 0) {
        return 1;
    }

    if (processMatrix() != 0) {
        return 1;
    }

    writeMatrixToFile(array);

    return 0;
}
