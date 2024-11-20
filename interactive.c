#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "imageprocessing.h"

#define MAX_PATH_LENGTH 100

typedef struct {
    int*** m;
    int N;
    int M;
} imagine;

typedef struct {
    float** fi;
    int size;
} filter;

imagine* create(int N, int M) {
    imagine* img = (imagine*)malloc(sizeof(imagine));
    img->m = aloc(N, M);
    img->N = N;
    img->M = M;
    return img;
}

float** faloc(int N) {
    float** fimg = (float**)malloc(N * sizeof(float*));
    for (int j = 0; j < N; j++) {
        fimg[j] = (float*)malloc(N * sizeof(float));
    }
    return fimg;
}

filter* cfiltru(int size) {
    filter* fil = (filter*)malloc(sizeof(filter));
    fil->fi = faloc(size);
    fil->size = size;
    return fil;
}

void fdealoc(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    char path[MAX_PATH_LENGTH];
    char f[3];
    scanf("%s", f);
    int j = 0, i = 0, index = 0, idf = 0, N = 0, M = 0;
    const int a = 10;
    imagine* v[a];
    filter* fmatrix[a];

    while (strcmp(f, "e")) {
        if (strcmp(f, "l") == 0) {
            scanf("%d %d %s", &N, &M, path);
            v[index] = create(N, M);
            read_from_bmp(v[index]->m, v[index]->N, v[index]->M, path);
            index++;
        }

        if (strcmp(f, "ah") == 0) {
            scanf("%d", &i);
            v[i]->m = flip_horizontal(v[i]->m, v[i]->N, v[i]->M);
        }

        if (strcmp(f, "ar") == 0) {
            scanf("%d", &i);
            v[i]->m = rotate_left(v[i]->m, v[i]->N, v[i]->M);
            int aux = v[i]->N;
            v[i]->N = v[i]->M;
            v[i]->M = aux;
        }

        if (strcmp(f, "ac") == 0) {
            int x = 0, y = 0, w = 0, h = 0;
            scanf("%d %d %d %d %d", &i, &x, &y, &w, &h);
            v[i]->m = crop(v[i]->m, v[i]->N, v[i]->M, x, y, h, w);
            v[i]->N = h;
            v[i]->M = w;
        }

        if (strcmp(f, "ae") == 0) {
            int rows = 0, cols = 0, new_R = 0, new_G = 0, new_B = 0;
            scanf("%d %d %d %d %d %d", &i, &rows, &cols, &new_R, &new_G,
                  &new_B);
            v[i]->m = extend(v[i]->m, v[i]->N, v[i]->M, rows, cols, new_R,
                             new_G, new_B);
            v[i]->N += 2 * rows;
            v[i]->M += 2 * cols;
        }

        if (strcmp(f, "ap") == 0) {
            int x = 0, y = 0;
            scanf("%d %d %d %d", &i, &j, &x, &y);
            v[i]->m = paste(v[i]->m, v[i]->N, v[i]->M, v[j]->m, v[j]->N,
                            v[j]->M, x, y);
        }

        if (strcmp(f, "cf") == 0) {
            int size = 0;
            float r = 0;
            scanf("%d", &size);
            fmatrix[idf] = cfiltru(size);

            for (i = 0; i < size; i++)
                for (j = 0; j < size; j++) {
                    scanf("%f", &r);
                    fmatrix[idf]->fi[i][j] = r;
                }
            idf++;
        }

        if (strcmp(f, "af") == 0) {
            scanf("%d %d", &i, &j);
            v[i]->m = apply_filter(v[i]->m, v[i]->N, v[i]->M, fmatrix[j]->fi,
                                   fmatrix[j]->size);
        }

        if (strcmp(f, "s") == 0) {
            scanf("%d%s", &i, path);
            write_to_bmp(v[i]->m, v[i]->N, v[i]->M, path);
        }

        if (strcmp(f, "di") == 0) {
            scanf("%d", &i);
            for (; i < index - 1; i++) {
            imagine* aux = v[i];
            v[i] = v[i+1];
            v[i+1] = aux;
            }
            desaloc(v[index - 1]->m, v[index - 1]->N, v[index - 1]->M);
            free(v[index - 1]);
            index--;
        }

        if (strcmp(f, "df") == 0) {
            scanf("%d", &i);

            for (; i < idf - 1; i++) {
                filter* aux = fmatrix[i];
                fmatrix[i] = fmatrix[i + 1];
                fmatrix[i + 1] = aux;
            }
            fdealoc(fmatrix[idf - 1]->fi, fmatrix[idf - 1]->size);
            free(fmatrix[idf - 1]);
            idf--;
        }

        scanf("%s", f);
    }

    for (i = 0; i < index; i++) {
        desaloc(v[i]->m, v[i]->N, v[i]->M);
        free(v[i]);
    }

    for (i = 0; i < idf; i++) {
        fdealoc(fmatrix[i]->fi, fmatrix[i]->size);
        free(fmatrix[i]);
    }

    return 0;
}
