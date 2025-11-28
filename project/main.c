#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pesquisa_local/algoritmo_pesquisa_local.h"
#include "utils.h"

#define DEFAULT_ITER 10000

int main(int argc, char *argv[])
{
   char    nome_fich[100];
   int     vert, m, k;
   float   *mat;
   int     *sol, *best;
   float   custo, best_custo = 0.0;
   float   mbf = 0.0;

   printf("Nome do Ficheiro: ");
   scanf("%s", nome_fich);

   init_rand();

   // Lê dados do ficheiro (C e m vêm do ficheiro)
   mat = init_dados(nome_fich, &vert, &m);

   printf("\n========== CONFIGURACAO ==========\n");
   printf("Ficheiro: %s\n", nome_fich);
   printf("Candidaturas (C): %d\n", vert);
   printf("Locais a construir (m): %d\n", m);
   printf("==================================\n\n");

   // Aloca soluções
   sol = malloc(sizeof(int) * vert);
   best = malloc(sizeof(int) * vert);
   if(sol == NULL || best == NULL)
   {
      printf("Erro na alocacao de memoria\n");
      exit(1);
   }

   // Executa múltiplas vezes
   for(k = 0; k < m; k++)
   {
      // Gera solução inicial aleatória com exatamente m pontos
      gera_sol_inicial(sol, vert, m);

      printf("Repeticao %d\n", k+1);
      printf("Solucao inicial: ");
      escreve_sol(sol, vert);

      // Aplica algoritmo de otimização
      custo = trepa_colinas_recristalizacao_simulada(sol, mat, vert, m);

      // Escreve resultados da repetição k
      printf("Solucao final: ");
      escreve_sol(sol, vert);
      printf("Custo final: %.2f\n\n", custo);

      mbf += custo;

      // Guarda melhor solução (MAXIMIZAÇÃO)
      if(k == 0 || best_custo < custo)
      {
         best_custo = custo;
         substitui(best, sol, vert);
      }
   }

   // Escreve resultados globais
   printf("\n========== RESULTADOS FINAIS ==========\n");
   printf("MBF (Media): %.2f\n", mbf/m);
   printf("Melhor custo: %.2f\n", best_custo);
   printf("Melhor solucao: ");
   escreve_sol(best, vert);
   printf("=======================================\n");

   // Liberta memória
   free(mat);
   free(sol);
   free(best);

   return 0;
}