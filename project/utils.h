//
// Created by jonhr on 18/11/2025.
//

#ifndef UTILS_H
#define UTILS_H

float* init_dados(char *nome_fich, int *vert, int *m);
void gera_sol_inicial(int sol[], int vert, int m);
void escreve_sol(int sol[], int vert);
void substitui(int a[], int b[], int n);
void init_rand();
int random_l_h(int min, int max);
float rand_01();


#endif //UTILS_H
