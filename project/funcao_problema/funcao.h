//
// Created by jonhr on 18/11/2025.
//

#ifndef FUNCAO_H
#define FUNCAO_H

// Declaração correta:
float *mat;  // ou double *mat

// E na função:
float calcula_fit(int sol[], float *mat, int vert, int m);
int conta_selecionados(int sol[], int vert);
float calcula_fit_penalizado(int sol[], float *mat, int vert, int m);

#endif //FUNCAO_H
