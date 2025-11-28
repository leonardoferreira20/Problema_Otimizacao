//
// Created by jonhr on 18/11/2025.
//

#include "utils.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// Leitura do ficheiro de input
// Recebe: nome do ficheiro, numero de vertices (ptr), numero de iteracoes (ptr)
// Devolve a matriz de adjacencias
float* init_dados(char *nome_fich, int *vert, int *m)
{
    FILE *f;
    int C, i, j;
    int ei_num, ej_num;
    char ei[10], ej[10];
    float distancia;
    float *mat;

    f = fopen(nome_fich, "r");
    if(!f)
    {
        printf("Erro no acesso ao ficheiro dos dados\n");
        exit(1);
    }

    // Lê primeira linha: C (candidaturas) e m (locais a construir)
    fscanf(f, "%d %d", &C, m);
    *vert = C;

    // Aloca matriz de distâncias (C x C)
    mat = malloc(sizeof(float) * C * C);
    if(!mat)
    {
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }

    // Inicializa matriz com zeros
    for(i = 0; i < C * C; i++)
        mat[i] = 0.0;

    // Lê as distâncias entre pares de pontos
    while(fscanf(f, "%s %s %f", ei, ej, &distancia) == 3)
    {
        // Extrai número do ponto (e1 -> 0, e2 -> 1, etc.)
        ei_num = atoi(ei + 1) - 1;  // +1 pula o 'e', -1 para índice base 0
        ej_num = atoi(ej + 1) - 1;

        // Preenche matriz simetricamente (dist(i,j) = dist(j,i))
        *(mat + ei_num * C + ej_num) = distancia;
        *(mat + ej_num * C + ei_num) = distancia;
    }

    fclose(f);
    return mat;
}

// Gera a solucao inicial
// Parametros: solucao, numero de vertices
void gera_sol_inicial(int sol[], int vert, int m)
{
    int i, count, pos;

    // Inicializa tudo com 0 (nenhum ponto selecionado)
    for(i = 0; i < vert; i++)
        sol[i] = 0;

    // Seleciona aleatoriamente m pontos diferentes
    count = 0;
    while(count < m)
    {
        pos = random_l_h(0, vert - 1);
        if(sol[pos] == 0)  // Se ainda não está selecionado
        {
            sol[pos] = 1;
            count++;
        }
    }
}

// Escreve solucao
// Parametros: solucao e numero de vertices
void escreve_sol(int sol[], int vert)
{
    int i;
    printf("[ ");
    for(i = 0; i < vert; i++)
    {
        if(sol[i] == 1)
            printf("e%d ", i+1);  // Mostra e1, e2, e3...
    }
    printf("]\n");
}

// copia vector b para a (tamanho n)
void substitui(int a[], int b[], int n)
{
    int i;
    for(i=0; i<n; i++)
        a[i]=b[i];
}

// Inicializa o gerador de numeros aleatorios
void init_rand()
{
    srand((unsigned)time(NULL));
}

// Devolve valor inteiro aleatorio entre min e max
int random_l_h(int min, int max)
{
    return min + rand() % (max-min+1);
}

// Devolve um valor real aleatorio do intervalo [0, 1]
float rand_01()
{
    return ((float)rand())/RAND_MAX;
}
