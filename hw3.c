#include <stdio.h>
#include <stdlib.h>

int **array;
int length;

void processMatrix() {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            array[i][j] = (array[i][j] <= 127) ? 0 : 255;
        }
    }
}

void writeMatrixToFile(int **modArray) {
    const char *fileName = "HW-3-output.txt";
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

int main() {
    char name[] = "data.dat";

    if (readMatrixFromFile(name) != 0) {
        return 1;
    }

    processMatrix();

    writeMatrixToFile(array);

    return 0;
}
