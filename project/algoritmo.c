#include "algoritmo.h"
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "funcao.h"
#include "utils.h"

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
//swap two vertices

void gera_vizinho(int a[], int b[], int n)
{
    int i, p1, p2;

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

    // Troca
    b[p1] = 1;
    b[p2] = 0;

}

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
int trepa_colinas(int sol[], int *mat, int vert, int num_iter)
{
    int *nova_sol, custo, custo_viz, i;


    nova_sol = malloc(sizeof(int)*vert);
    if(nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, vert);
    for(i=0; i<num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, vert);
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, vert);
        // Aceita vizinho se o custo diminuir (problema de minimizacao) TRABALHO VAI SER MAXIMIZACAO
        if(custo_viz <= custo)
        {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }else { //Com este else aceitamos uma soluçao pior com uma probabilidade pequena
            if (rand_01()<=0.0005) {
                substitui(sol, nova_sol, vert);
                custo = custo_viz;
            };
        }
    }
    free(nova_sol);
    return custo;
}

int trepa_colinas_recristalizacao(int sol[], int *mat, int vert, int num_iter)
{
    int *nova_sol, custo, custo_viz, i;
    double tmax, tmin, t;
    tmax = 100;
    tmin = 5;
    t = tmax;
    nova_sol = malloc(sizeof(int) * vert);
    if (nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, vert);
    for (i = 0; i < num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, vert);
        // gera_vizinho2(sol, nova_sol, vert);
        //  Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, vert);
        // Aceita vizinho se o custo diminuir (problema de minimizacao)
        if (custo_viz >= custo)
        {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }
        else
        {
            int r = rand() % 101;
            if (rand_01() <= exp((custo - custo_viz) / t))
            {
                substitui(sol, nova_sol, vert);
                custo = custo_viz;
            }
        }
        t = t * 0.99;
    }
    free(nova_sol);
    return custo;
}


int trepa_colinas_recristalizacao_simulada(int sol[], int *mat, int vert, int num_iter)
{
    int *nova_sol, custo, custo_viz, best_custo, i, *best_sol;
    double tmax, tmin, t, decrementoT;

    tmax = 10;
    tmin = 0.5;
    t = tmax;
    decrementoT = (tmax - tmin) / num_iter;

    nova_sol = malloc(sizeof(int) * vert);
    best_sol = malloc(sizeof(int) * vert);
    if (nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, vert);
    best_custo = custo;
    substitui(best_sol, sol, vert);
    for (i = 0; i < num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, vert);
        // gera_vizinho2(sol, nova_sol, vert);
        //  Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, vert);
        // Aceita vizinho se o custo diminuir (problema de minimizacao)
        if (custo_viz < best_custo)
        {
            best_custo = custo_viz;
            substitui(best_sol, nova_sol, vert);
        }
        if (custo_viz >= custo)
        {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }
        else
        {
            int r = rand() % 101;
            if (r <= exp((custo - custo_viz) / t))
            {
                substitui(sol, nova_sol, vert);
                custo = custo_viz;
            }
        }
        t = t - decrementoT;
    }
    free(nova_sol);
    return custo;
}



/// if(custo_viz < custo)
/// if(custo_viz <= custo) planaltos
///
/*
if(custo_viz <= custo)
{
    substitui(sol, nova_sol, vert);
    custo = custo_viz;
}else
 { //Com este else aceitamos uma soluçao pior com uma probabilidade pequena
    if (rand_01()<=0.01) {
        substitui(sol, nova_sol, vert);
        custo = custo_viz;
    };
}
*/


/* Vizinhança 1

void gera_vizinho(int a[], int b[], int n)
{
    int i, p1, p2;

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

    // Troca
    b[p1] = 1;
    b[p2] = 0;

}


 */




/* Vizinança 2

void gera_vizinho(int a[], int b[], int n)
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

 */


/* Recristalização

int trepa_colinas(int sol[], int *mat, int vert, int num_iter)
{
    int *nova_sol, custo, custo_viz, i;


    nova_sol = malloc(sizeof(int)*vert);
    if(nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, vert);
    for(i=0; i<num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, vert);
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, vert);
        // Aceita vizinho se o custo diminuir (problema de minimizacao) TRABALHO VAI SER MAXIMIZACAO
        if(custo_viz <= custo)
        {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }else { //Com este else aceitamos uma soluçao pior com uma probabilidade pequena
            if (rand_01()<=0.0005) {
                substitui(sol, nova_sol, vert);
                custo = custo_viz;
            };
        }
    }
    free(nova_sol);
    return custo;
}






*/