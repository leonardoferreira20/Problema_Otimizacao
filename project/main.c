#include <stdio.h>
#include <stdlib.h>
#include "pesquisa_local/algoritmo_pesquisa_local.h"
#include "evolutivo/algoritmo_evolutivo.h"
#include "utils.h"

#define DEFAULT_RUNS 10
#define DEFAULT_ITER 10000

void executa_pesquisa_local(float *mat, int vert, int m);
void executa_algoritmo_evolutivo(float *mat, int vert, int m);
void executa_algoritmo_hibrido(float *mat, int vert, int m);
void mostra_menu();

int main(int argc, char *argv[])
{
   char nome_fich[100];
   int vert, m, k, opcao;
   float *mat;

   printf("==============================================\n");
   printf("  TRABALHO PRATICO 2 - IIA 2025/2026\n");
   printf("  Problema de Otimizacao - Turismo Sustentavel\n");
   printf("==============================================\n\n");

   // Pede nome do ficheiro
   printf("Nome do Ficheiro: ");
   scanf("%s", nome_fich);

   init_rand();

   // Lê dados do ficheiro (C e m vêm do ficheiro)
   mat = init_dados(nome_fich, &vert, &m);

   printf("\n========== DADOS DA CONFIGURACAO ==========\n");
   printf("Ficheiro: %s\n", nome_fich);
   printf("Candidaturas (C): %d\n", vert);
   printf("Locais a construir (m): %d\n", m);
   printf("=============================================\n\n");

   // Loop principal do menu
   do
   {
      mostra_menu();
      printf("\nEscolha uma opcao: ");
      scanf("%d", &opcao);

      switch(opcao)
      {
         case 1:
            printf("\n>>> PESQUISA LOCAL <<<\n");
            executa_pesquisa_local(mat, vert, m);
         break;
         case 2:
            printf("\n>>> ALGORITMO EVOLUTIVO <<<\n");
            executa_algoritmo_evolutivo(mat, vert, m);
         break;
         case 3:
            printf("\n>>> ALGORITMO HIBRIDO <<<\n");
            executa_algoritmo_hibrido(mat, vert, m);
         break;
         case 0:
            printf("\nA sair...\n");
         break;
         default:
            printf("\nOpcao invalida! Tente novamente.\n");
      }

      if(opcao != 0)
      {
         printf("\nPressione ENTER para continuar...");
         getchar(); // Limpa buffer
         getchar(); // Espera ENTER
      }

   } while(opcao != 0);


   // Liberta memória
   free(mat);

   printf("\nPrograma terminado.\n");
   return 0;
}

void mostra_menu()
{
   printf("\n");
   printf("==============================================\n");
   printf("                  MENU PRINCIPAL              \n");
   printf("==============================================\n");
   printf("  1 - Pesquisa Local (Recristalizacao Simulada)\n");
   printf("  2 - Algoritmo Evolutivo\n");
   printf("  3 - Algoritmo Hibrido\n");
   printf("  0 - Sair\n");
   printf("==============================================\n");
}

// PESQUISA LOCAL
void executa_pesquisa_local(float *mat, int vert, int m){
    int *sol, *best;
    float custo, best_custo = 0.0;
    float mbf = 0.0;
    int k, runs, num_iter;

    // Pergunta parâmetros
    printf("\nNumero de execucoes (runs) [default=%d]: ", DEFAULT_RUNS);
    scanf("%d", &runs);
    if(runs <= 0) {
       runs = DEFAULT_RUNS;
    }

    printf("Numero de iteracoes [default=%d]: ", DEFAULT_ITER);
    scanf("%d", &num_iter);
    if(num_iter <= 0) {
       num_iter = DEFAULT_ITER;
    }

    printf("\n========== CONFIGURACAO ==========\n");
    printf("Algoritmo: Recristalizacao Simulada\n");
    printf("Runs: %d\n", runs);
    printf("Iteracoes: %d\n", num_iter);
    printf("==================================\n\n");

    // Aloca soluções
    sol = malloc(sizeof(int) * vert);
    best = malloc(sizeof(int) * vert);
    if(sol == NULL || best == NULL)
    {
        printf("Erro na alocacao de memoria\n");
        return;
    }

    // Executa múltiplas vezes
    for(k = 0; k < runs; k++)
    {
        // Gera solução inicial
        gera_sol_inicial(sol, vert, m);

        printf("Run %d/%d\n", k+1, runs);
        printf("  Solucao inicial: ");
        escreve_sol(sol, vert);

        // Aplica algoritmo (PASSA num_iter!)
        custo = trepa_colinas_recristalizacao_simulada(sol, mat, vert, m, num_iter);

        // Resultados
        printf("  Solucao final:   ");
        escreve_sol(sol, vert);
        printf("  Custo: %.2f\n\n", custo);

        mbf += custo;

        // Guarda melhor
        if(k == 0 || best_custo < custo)
        {
            best_custo = custo;
            substitui(best, sol, vert);
        }
    }

    // Resultados finais
    printf("\n========== RESULTADOS FINAIS ==========\n");
    printf("MBF (Media): %.2f\n", mbf/runs);
    printf("Melhor custo: %.2f\n", best_custo);
    printf("Melhor solucao: ");
    escreve_sol(best, vert);
    printf("=======================================\n");

    // Liberta memória
    free(sol);
    free(best);
}

// EVOLUTIVO
void executa_algoritmo_evolutivo(float *mat, int vert, int m){
    info_ea param;
    pchrom pop, parents;
    chrom best_gen, best_ever;
    int gen, run, runs;
    float mbf = 0.0;

    // Pede parâmetros ao utilizador
    printf("\n--- PARAMETROS DO ALGORITMO EVOLUTIVO ---\n");
    printf("Tamanho da populacao [default=100]: ");
    scanf("%d", &param.popsize);
    if(param.popsize <= 0) param.popsize = 100;

    printf("Probabilidade de recombinacao (0-1) [default=0.7]: ");
    scanf("%f", &param.pr);
    if(param.pr < 0 || param.pr > 1) param.pr = 0.7;

    printf("Probabilidade de mutacao (0-1) [default=0.01]: ");
    scanf("%f", &param.pm);
    if(param.pm < 0 || param.pm > 1) param.pm = 0.01;

    printf("Numero de geracoes [default=100]: ");
    scanf("%d", &param.numGenerations);
    if(param.numGenerations <= 0) param.numGenerations = 100;

    printf("Numero de execucoes (runs) [default=10]: ");
    scanf("%d", &runs);
    if(runs <= 0) runs = 10;

    // Configura parâmetros
    param.numGenes = vert;
    param.m = m;
    param.tsize = 2;

    printf("\n========== CONFIGURACAO ==========\n");
    printf("Populacao: %d\n", param.popsize);
    printf("Geracoes: %d\n", param.numGenerations);
    printf("Prob. Recombinacao: %.2f\n", param.pr);
    printf("Prob. Mutacao: %.2f\n", param.pm);
    printf("Runs: %d\n", runs);
    printf("==================================\n\n");

    // Executa múltiplas vezes
    for(run = 0; run < runs; run++)
    {
        printf("Run %d/%d\n", run+1, runs);

        // Cria população inicial
        pop = init_pop_ea(param);

        // Avalia população inicial
        evaluate_pop(pop, param, mat);

        // Melhor da geração inicial
        best_gen = pop[0];
        best_gen = get_best_ea(pop, param, best_gen);

        // Aloca pais
        parents = malloc(sizeof(chrom) * param.popsize);
        if(parents == NULL)
        {
            printf("Erro na alocacao de memoria\n");
            exit(1);
        }

        // Loop de gerações
        for(gen = 1; gen <= param.numGenerations; gen++)
        {
            // Seleção por torneio
            tournament_ea(pop, param, parents);

            // Operadores genéticos
            genetic_operators_ea(parents, param, pop);

            // Avalia nova população
            evaluate_pop(pop, param, mat);

            // Atualiza melhor
            best_gen = get_best_ea(pop, param, best_gen);
        }

        // Resultados do run
        printf("  Melhor fitness: %.2f\n\n", best_gen.fitness);
        mbf += best_gen.fitness;

        // Atualiza melhor global
        if(run == 0 || best_gen.fitness > best_ever.fitness)
            best_ever = best_gen;

        // Liberta memória
        free(parents);
        free(pop);
    }

    // Resultados finais
    printf("\n========== RESULTADOS FINAIS ==========\n");
    printf("MBF (Media): %.2f\n", mbf/runs);
    write_best_ea(best_ever, param);
    printf("=======================================\n");
}

// HIBRIDO
void executa_algoritmo_hibrido(float *mat, int vert, int m)
{
   printf("\n[INFO] Algoritmo Hibrido ainda nao implementado.\n");
   printf("       Sera implementado apos o Algoritmo Evolutivo.\n");

   // TODO: Implementar híbrido
   // Opção 1: Evolutivo + Pesquisa Local no final
   // Opção 2: Pesquisa Local a cada X gerações
}