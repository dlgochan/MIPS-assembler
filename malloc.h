#include <stdlib.h>

char* MALLOC_char1(int n){
    char* arr = (char*)malloc(sizeof(char)*n);
}

char* MALLOC_int1(int n){
    int* arr = (int*)malloc(sizeof(int)*n);
}

void FREE_1(int* arr){
    free(arr);
}

char** MALLOC_char2(int n, int m){
    char** arr = (char**)malloc(sizeof(char*) * n);
    for(int i=0; i<n; i++){
        arr[i] = (char*)malloc(sizeof(char) * m);
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            arr[i][j] = 0;
        }
    }

    return arr;
}

void FREE_2(char** arr, int N){
    for(int i=0; i<N; i++){
        free(arr[i]);
    }
    free(arr);
    return;
}

int** MALLOC_int2(int n, int m){
    int** arr = (int**)malloc(sizeof(int*) * n);
    for(int i=0; i<n; i++){
        arr[i] = (int*)malloc(sizeof(int) * m);
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            arr[i][j] = 0;
        }
    }

    return arr;
}


