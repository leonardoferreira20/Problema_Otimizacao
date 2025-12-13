#include "algoritmo_pesquisa_local.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../funcao_problema/funcao.h"
#include "../utils.h"

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
//swap one vertice
void gera_vizinho(int a[], int b[], int n)
{
    int i, p1, p2;

    // Copia solução atual
    for(i = 0; i < n; i++)
        b[i] = a[i];

    // Encontra uma posição NÃO selecionada (0)
    do
        p1 = random_l_h(0, n-1);
    while(b[p1] != 0);

    // Encontra uma posição selecionada (1)
    do
        p2 = random_l_h(0, n-1);
    while(b[p2] != 1);

    // Troca: adiciona p1, remove p2
    b[p1] = 1;
    b[p2] = 0;
}

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
//swap two vertices
void gera_vizinho2(int a[], int b[], int n)
{
    int i, p1, p2, p3, p4;

    for(i=0; i<n; i++)
        b[i]=a[i];
    // Encontra posicao com valor 0
    do
        p1=random_l_h(0, n-1);
    while(b[p1] != 0);
    // Encontra posicao com valor 0
    do
        p2=random_l_h(0, n-1);
    while(b[p2] != 1);
    // Encontra posicao com valor 0
    do
        p3=random_l_h(0, n-1);
    while(b[p3] != 0 || p1 == p3);
    // Encontra posicao com valor 0
    do
        p4=random_l_h(0, n-1);
    while(b[p4] != 1 || p2 == p4);
    // Troca
    b[p1] = 1;
    b[p2] = 0;
    b[p3] = 1;
    b[p4] = 0;
}

// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
float trepa_colinas_recristalizacao_simulada(int sol[], float *mat, int vert, int m, int num_iter, int tipo_vizinhanca)
{
    int *nova_sol, *best_sol, i;
    float custo, custo_viz, best_custo;
    double tmax, tmin, t, decrementoT;

    // Parâmetros de temperatura
    tmax = 10.0;
    tmin = 0.5;
    t = tmax;
    decrementoT = (tmax - tmin) / num_iter;

    // Alocação de memória
    nova_sol = malloc(sizeof(int) * vert);
    best_sol = malloc(sizeof(int) * vert);
    if (nova_sol == NULL || best_sol == NULL)
    {
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }

    // Avalia solução inicial
    custo = calcula_fit(sol, mat, vert, m);
    best_custo = custo;
    substitui(best_sol, sol, vert);

    for (i = 0; i < num_iter; i++){
        switch(tipo_vizinhanca){
            case 1:  // Vizinhanca 1
                gera_vizinho(sol, nova_sol, vert);
            break;
            case 2:  // Vizinhanca 2
                gera_vizinho2(sol, nova_sol, vert);
            break;
            case 3:  // Ambas
                if(rand_01() < 0.5)
                    gera_vizinho(sol, nova_sol, vert);
                else
                    gera_vizinho2(sol, nova_sol, vert);
            break;
            default:
                if(rand_01() < 0.5)
                    gera_vizinho(sol, nova_sol, vert);
                else
                    gera_vizinho2(sol, nova_sol, vert);
        }

        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, vert, m);

        // Guarda a melhor solução encontrada
        if (custo_viz > best_custo)  // MAXIMIZAÇÃO
        {
            best_custo = custo_viz;
            substitui(best_sol, nova_sol, vert);
        }

        // Critério de aceitação
        if (custo_viz >= custo)  // Aceita se melhorar ou igual
        {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }
        else  // Aceita solução pior com probabilidade
        {
            // Para maximização: exp((novo - atual) / T)
            // Como novo < atual, o expoente é negativo
            if (rand_01() <= exp((custo_viz - custo) / t))
            {
                substitui(sol, nova_sol, vert);
                custo = custo_viz;
            }
        }

        // Arrefecimento linear
        t = t - decrementoT;
        if (t < tmin) t = tmin;  // Garante que não fica negativa
    }

    // Retorna a melhor solução encontrada
    substitui(sol, best_sol, vert);
    free(nova_sol);
    free(best_sol);

    return best_custo;
}
