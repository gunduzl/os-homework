#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int **array;
int length;

void openFile(int *modified, int child_num, int boyut) {
    char name[10];
    snprintf(name, sizeof(name), "output_%d.txt", child_num);

    FILE *file = fopen(name, "w");

    for (int j = 0; j < boyut; j++) {
        fprintf(file, "%d\n", modified[j]);
    }

    fclose(file);
}

int main() {
    char name[] = "data.dat";
    FILE *file;

    pid_t child_pid, wpid;
    int status = 0;

    if ((file = fopen(name, "r")) == NULL) {
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

    for (int child_num = 0; child_num < length; child_num++) {
        child_pid = fork();
        if (child_pid == -1) {
            perror("Failed to fork");
            return 1;
        } else if (child_pid == 0) {
            int *modified = (int *)malloc(length * sizeof(int));
            for (int i = 0; i < length; i++) {
                if (array[child_num][i] <= 127) {
                    modified[i] = 0;
                } else {
                    modified[i] = 255;
                }
            }

            openFile(modified, child_num, length);
            exit(0);
        }
    }

    while ((wpid = wait(&status)) > 0);

    return 0;
}
