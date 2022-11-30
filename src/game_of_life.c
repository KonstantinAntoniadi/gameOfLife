#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>

#define m 25
#define n 80

void readFile(int** mas, int height, int weight, char* fileName, int* flag);
void print(int** mas, int height, int weight);
void clone(int** mas, int **buff, int height, int weight);
void initRandom(int **mas, int height, int weight);
int** initDynamic(int height, int weight, int *flag);
void freeDynamic(int** mas);
void ceurses();
int sum(int** mas, int height, int weight, int i, int j);
void changeSpeed(char key, int start, int* change, int step);

int main() {
    int flag = 0;
    int** mas;
    int** new;
    mas = initDynamic(m, n, &flag);
    new = initDynamic(m, n, &flag);
    if (flag) {
        printf("Starting Game!\n");
        printf("Choose a board:\n");
        printf(" 1. Static figures\n");
        printf(" 2. Gliders\n");
        printf(" 3. Gun\n");
        printf(" 4. Period figures\n");
        printf(" 5. Eater\n");
        printf(" 6. Random board\n");

        char *fileName;
        char c = ' ';
        while (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6') {
            c = getchar();
        }
        switch (c) {
            case '1':
                fileName = "./figures/static.txt";
                readFile(mas, m, n, fileName, &flag);
                flag = 1;
                break;
            case '2':
                fileName = "./figures/gliders.txt";
                readFile(mas, m, n, fileName, &flag);
                break;
            case '3':
                fileName = "./figures/gun.txt";
                readFile(mas, m, n, fileName, &flag);
                break;
            case '4':
                fileName = "./figures/period_figure.txt";
                readFile(mas, m, n, fileName, &flag);
                break;
            case '5':
                fileName = "./figures/eater.txt";
                readFile(mas, m, n, fileName, &flag);
                break;
            case '6':
                initRandom(mas, m, n);
                break;
        }
        if (flag) {
            ceurses();
            int change = 0;
            int startPeriod = 30000;
            int step = 2500;
            while (1) {
                print(mas, m, n);
                int count;
                for (int i = 0; i < m; i++) {
                    for (int j = 0 ; j < n; j++) {
                        count = sum(mas, m, n, i, j);
                        if (mas[i][j] == 0 && count == 3)
                            new[i][j] = 1;
                        else if (mas[i][j] == 1 && (count == 2 || count == 3))
                            new[i][j] = 1;
                        else
                            new[i][j] = 0;
                        count = 0;
                    }
                }
                clone(mas, new, m, n);
                char key;
                key = getch();
                if (key == 'q')
                    break;
                else
                    changeSpeed(key, startPeriod, &change, step);
            }
            endwin();
        }
    }

    freeDynamic(mas);
    freeDynamic(new);

    return 0;
}

void changeSpeed(char key, int start, int* change, int step) {
    usleep(start + *change);
    switch (key) {
        case 'w':
            *change += step;
            if (*change == start) *change-=step;
            break;
        case 's':
            *change -= step;
            if (*change == start * (-1)) *change+=step;
            break;
    }
}

void ceurses() {
    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
}


int** initDynamic(int height, int weight, int *flag) {
    int** array = malloc(height * sizeof(int *));
    *flag = 0;
    if (array != NULL) {
        for (int i = 0; i < height; i++) {
            array[i] = malloc(weight * sizeof(int));
            if (array[i] == NULL) {
                *flag = 0;
                break;
            } else {
                *flag = 1;
            }
        }
        if (flag) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < weight; j++) {
                    array[i][j] = 0;
                }
            }
        }
    }

    return array;
}

void readFile(int** mas, int height, int weight, char* fileName, int* flag) {
    *flag = 0;
    FILE *file;
    file = fopen(fileName, "r");
    if (file != NULL) {
        *flag = 1;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < weight; j++) {
                fscanf(file, "%d", &mas[i][j]);
            }
        }
        fclose(file);
    }
}


void clone(int** mas, int **buff, int height, int weight) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            mas[i][j] = buff[i][j];
        }
    }
}


void print(int** mas, int height, int weight) {
    clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            if (mas[i][j] == 0) {
                printw(" ");
            } else {
                printw("*");
            }
        }
        printw("\n");
    }
    refresh();
}



void initRandom(int **mas, int height, int weight) {
    srand(time(NULL));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            mas[i][j] = rand() % 2;
        }
    }
}

int sum(int** mas, int height, int weight, int i, int j) {
    int suma = 0;
    for (int zi = -1; zi < 2; zi++) {
        for (int zj = -1; zj < 2; zj++) {
            suma +=mas[(zi + i + height) % height][(zj + j + weight) % weight];
        }
    }
    suma -= mas[i][j];
    return suma;
}

void freeDynamic(int** mas) {
    for (int i = 0; i < m; i++)
        free(mas[i]);
    free(mas);
}
