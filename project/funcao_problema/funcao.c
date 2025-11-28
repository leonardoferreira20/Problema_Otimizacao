#include "funcao.h"

// Conta quantos pontos estão selecionados
int conta_selecionados(int sol[], int vert)
{
    int count = 0;
    for(int i = 0; i < vert; i++)
    {
        if(sol[i] == 1)
            count++;
    }
    return count;
}

// Função de avaliação COM PENALIZAÇÃO
float calcula_fit_penalizado(int sol[], float *mat, int vert, int m)
{
    int num_selecionados = conta_selecionados(sol, vert);

    // Se não tem exatamente m pontos, penaliza MUITO
    if(num_selecionados != m)
    {
        return -999999.0;  // Valor muito baixo (maximização)
    }

    // Caso contrário, calcula normalmente
    return calcula_fit(sol, mat, vert, m);
}

// Calcula a qualidade de uma solução
float calcula_fit(int sol[], float *mat, int vert, int m){
    float soma_distancias = 0.0;
    int i, j;

    // Soma todas as distâncias entre pares de pontos selecionados
    for (i = 0; i < vert; i++){
        if (sol[i] == 1){
            for (j = i + 1; j < vert; j++){
                if (sol[j] == 1){
                    soma_distancias += *(mat + i * vert + j);
                }
            }
        }
    }

    return soma_distancias / m;
}