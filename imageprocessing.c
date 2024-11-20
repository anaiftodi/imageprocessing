#include "imageprocessing.h"

#include <stdio.h>
#include <stdlib.h>

int ***aloc(int N, int M) {
    int ***img = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        img[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            img[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    return img;
}

void desaloc(int ***image, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
}

// Task 1
int ***flip_horizontal(int ***image, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M / 2; j++) {
            int *aux = image[i][j];
            image[i][j] = image[i][M - j - 1];
            image[i][M - j - 1] = aux;
        }
    }
    return image;
}

// Task 2
int ***rotate_left(int ***image, int N, int M) {
    int ***image2 = aloc(M, N);
    for (int i = M - 1; i >= 0; i--)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < 3; k++)
                image2[M - i - 1][j][k] = image[j][i][k];
    desaloc(image, N, M);
    return image2;
}

// Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int ***image3 = aloc(h, w);

    int a = 0;
    for (int i = y; i < y + h; i++) {
        int b = 0;
        for (int j = x; j < x + w; j++) {
            for (int k = 0; k < 3; k++) {
                image3[a][b][k] = image[i][j][k];
            }
            b++;
        }
        a++;
    }
    desaloc(image, N, M);
    return image3;
}

// Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R,
              int new_G, int new_B) {
    int ***image4 = aloc(N + 2 * rows, M + 2 * cols);
    for (int i = 0; i < N + 2 * rows; i++) {
        for (int j = 0; j < M + 2 * cols; j++) {
            image4[i][j][0] = new_R;
            image4[i][j][1] = new_G;
            image4[i][j][2] = new_B;
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            for (int k = 0; k < 3; k++)
                image4[i + rows][j + cols][k] = image[i][j][k];
    desaloc(image, N, M);
    return image4;
}

// TODO(me): Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int ***image_src,
             int N_src, int M_src, int x, int y) {
    int a = 0;
    for (int i = y; i < N_dst; i++) {
        int b = 0;
        for (int j = x; j < M_dst; j++) {
            for (int k = 0; k < 3; k++) {
                image_dst[i][j][k] = image_src[a][b][k];
            }
            b++;
            if (b == M_src) break;
        }
        a++;
        if (a == N_src) break;
    }

    return image_dst;
}

// TODO(me): Task 6
int ***apply_filter(int ***image, int N, int M, float **filter,
                    int filter_size) {
    int ***image2 = aloc(N, M);
    float const zero = 0, doi = 255;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                float r = 0;
                for (int a = 0, x = i - (filter_size / 2); a < filter_size;
                     a++, x++) {
                    for (int b = 0, y = j - (filter_size / 2); b < filter_size;
                         b++, y++) {
                        if (x < 0 || y < 0 || x >= N || y >= M) {
                            r += 0;
                        } else {
                            r += (float)image[x][y][k] * filter[a][b];
                        }
                    }
                }
                if (r < zero) r = zero;
                if (r > doi) r = doi;
                image2[i][j][k] = (int)r;
            }
        }
    }
    desaloc(image, N, M);
    return image2;
}
