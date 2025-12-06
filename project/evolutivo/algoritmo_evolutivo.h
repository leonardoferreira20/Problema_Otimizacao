//
// Created by Leonardo Ferreira on 06/12/2025.
//

#ifndef ALGORITMO_EVOLUTIVO_H
#define ALGORITMO_EVOLUTIVO_H

#define MAX_VERTICES 500  // Número máximo de candidaturas

// Estrutura de um indivíduo
typedef struct individuo
{
    int pontos[MAX_VERTICES];  // Array binário (0/1)
    float fitness;              // Qualidade da solução
    int valido;                 // 1=válido, 0=inválido
} chrom, *pchrom;

// Estrutura com parâmetros do algoritmo
typedef struct info_ea
{
    int popsize;           // Tamanho da população
    float pm;              // Probabilidade de mutação
    float pr;              // Probabilidade de recombinação
    int tsize;             // Tamanho do torneio
    int numGenerations;    // Número de gerações
    int numGenes;          // Número de genes (C - candidaturas)
    int m;                 // Número de locais a selecionar
    int sel_method;        // 1 = torneio, 2 = roleta
    int crossover_method;  // 1 = uniforme, 2 = 1-ponto
    int mutation_method;   // 1 = swap, 2 = flip
} info_ea;

// Funções principais
pchrom init_pop_ea(info_ea d);
void evaluate_pop(pchrom pop, info_ea d, float *mat);
float eval_individual(int sol[], info_ea d, float *mat);
chrom get_best_ea(pchrom pop, info_ea d, chrom best);

// Seleção
void tournament_ea(pchrom pop, info_ea d, pchrom parents);
void roleta(pchrom pop, info_ea d, pchrom parents);

// Operadores genéticos
void genetic_operators_ea(pchrom parents, info_ea d, pchrom offspring);
void crossover_uniforme(pchrom parents, info_ea d, pchrom offspring);
void crossover_1ponto(pchrom parents, info_ea d, pchrom offspring);
void mutation_swap(pchrom offspring, info_ea d);
void mutation_flip(pchrom offspring, info_ea d);

// Reparação
void reparacao_ea(int sol[], info_ea d);

// Escrita
void write_best_ea(chrom x, info_ea d);

#endif //ALGORITMO_EVOLUTIVO_H
