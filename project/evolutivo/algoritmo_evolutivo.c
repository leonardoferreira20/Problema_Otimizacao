//
// Created by Leonardo Ferreira on 06/12/2025.
//
#include "algoritmo_evolutivo.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utils.h"
#include "../funcao_problema/funcao.h"

// ==================== INICIALIZAÇÃO ====================

// Cria população inicial aleatória
pchrom init_pop_ea(info_ea d)
{
    int i, j, count, pos;
    pchrom pop;

    pop = malloc(sizeof(chrom) * d.popsize);
    if (pop == NULL)
    {
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }

    // Gera cada indivíduo
    for (i = 0; i < d.popsize; i++)
    {
        // Inicializa tudo a 0
        for (j = 0; j < d.numGenes; j++)
            pop[i].pontos[j] = 0;

        // Seleciona aleatoriamente m pontos
        count = 0;
        while (count < d.m)
        {
            pos = random_l_h(0, d.numGenes - 1);
            if (pop[i].pontos[pos] == 0)
            {
                pop[i].pontos[pos] = 1;
                count++;
            }
        }

        pop[i].fitness = 0.0;
        pop[i].valido = 1;
    }

    return pop;
}

// ==================== AVALIAÇÃO ====================

// Avalia um indivíduo
float eval_individual(int sol[], info_ea d, float *mat)
{
    return calcula_fit(sol, mat, d.numGenes, d.m);
}

// Avalia toda a população
void evaluate_pop(pchrom pop, info_ea d, float *mat)
{
    int i;

    for (i = 0; i < d.popsize; i++)
    {
        // Repara solução se necessário
        reparacao_ea(pop[i].pontos, d);

        // Avalia
        pop[i].fitness = eval_individual(pop[i].pontos, d, mat);
        pop[i].valido = 1;
    }
}

// Encontra o melhor indivíduo
chrom get_best_ea(pchrom pop, info_ea d, chrom best)
{
    int i;

    for (i = 0; i < d.popsize; i++)
    {
        if (pop[i].fitness > best.fitness)  // MAXIMIZAÇÃO
            best = pop[i];
    }

    return best;
}

// ==================== SELEÇÃO ====================

// Seleção por Torneio Binário
void tournament_ea(pchrom pop, info_ea d, pchrom parents)
{
    int i, x1, x2;

    for (i = 0; i < d.popsize; i++)
    {
        // Escolhe 2 indivíduos aleatórios
        x1 = random_l_h(0, d.popsize - 1);
        do
            x2 = random_l_h(0, d.popsize - 1);
        while (x1 == x2);

        // O melhor vai para os pais
        if (pop[x1].fitness > pop[x2].fitness)  // MAXIMIZAÇÃO
            parents[i] = pop[x1];
        else
            parents[i] = pop[x2];
    }
}

// Seleção por Roleta
void roleta(pchrom pop, info_ea d, pchrom parents)
{
    int i, j;
    float total_fitness = 0.0, partial_sum, r;

    // Calcula fitness total
    for (i = 0; i < d.popsize; i++)
        total_fitness += pop[i].fitness;

    // Seleciona d.popsize indivíduos
    for (i = 0; i < d.popsize; i++)
    {
        r = rand_01() * total_fitness;
        partial_sum = 0.0;

        for (j = 0; j < d.popsize; j++)
        {
            partial_sum += pop[j].fitness;
            if (partial_sum >= r)
            {
                parents[i] = pop[j];
                break;
            }
        }
    }
}

// ==================== OPERADORES GENÉTICOS ====================

// Aplica recombinação e mutação
void genetic_operators_ea(pchrom parents, info_ea d, pchrom offspring)
{
    // Recombinação
    crossover_uniforme(parents, d, offspring);

    // Mutação
    mutation_swap(offspring, d);
}

// Recombinação Uniforme
void crossover_uniforme(pchrom parents, info_ea d, pchrom offspring)
{
    int i, j;

    for (i = 0; i < d.popsize; i += 2)
    {
        if (rand_01() < d.pr)
        {
            // Recombinação uniforme
            for (j = 0; j < d.numGenes; j++)
            {
                if (rand_01() < 0.5)
                {
                    offspring[i].pontos[j] = parents[i].pontos[j];
                    offspring[i + 1].pontos[j] = parents[i + 1].pontos[j];
                }
                else
                {
                    offspring[i].pontos[j] = parents[i + 1].pontos[j];
                    offspring[i + 1].pontos[j] = parents[i].pontos[j];
                }
            }
        }
        else
        {
            // Copia pais para filhos sem alterações
            offspring[i] = parents[i];
            offspring[i + 1] = parents[i + 1];
        }
    }
}

// Recombinação de 1 Ponto
void crossover_1ponto(pchrom parents, info_ea d, pchrom offspring)
{
    int i, j, point;

    for (i = 0; i < d.popsize; i += 2)
    {
        if (rand_01() < d.pr)
        {
            point = random_l_h(1, d.numGenes - 1);

            // Copia até ao ponto de corte
            for (j = 0; j < point; j++)
            {
                offspring[i].pontos[j] = parents[i].pontos[j];
                offspring[i + 1].pontos[j] = parents[i + 1].pontos[j];
            }

            // Troca depois do ponto de corte
            for (j = point; j < d.numGenes; j++)
            {
                offspring[i].pontos[j] = parents[i + 1].pontos[j];
                offspring[i + 1].pontos[j] = parents[i].pontos[j];
            }
        }
        else
        {
            offspring[i] = parents[i];
            offspring[i + 1] = parents[i + 1];
        }
    }
}

// Mutação por Troca (Swap)
void mutation_swap(pchrom offspring, info_ea d)
{
    int i, pos1, pos2;

    for (i = 0; i < d.popsize; i++)
    {
        if (rand_01() < d.pm)
        {
            // Encontra posição com 0
            do
                pos1 = random_l_h(0, d.numGenes - 1);
            while (offspring[i].pontos[pos1] != 0);

            // Encontra posição com 1
            do
                pos2 = random_l_h(0, d.numGenes - 1);
            while (offspring[i].pontos[pos2] != 1);

            // Troca
            offspring[i].pontos[pos1] = 1;
            offspring[i].pontos[pos2] = 0;
        }
    }
}

// Mutação por Flip (bit a bit)
void mutation_flip(pchrom offspring, info_ea d)
{
    int i, j;

    for (i = 0; i < d.popsize; i++)
    {
        for (j = 0; j < d.numGenes; j++)
        {
            if (rand_01() < d.pm)
            {
                offspring[i].pontos[j] = !(offspring[i].pontos[j]);
            }
        }
    }
}

// ==================== REPARAÇÃO ====================

// Repara solução para ter exatamente m pontos
void reparacao_ea(int sol[], info_ea d)
{
    int count = 0, i, pos;

    // Conta quantos estão selecionados
    for (i = 0; i < d.numGenes; i++)
        if (sol[i] == 1)
            count++;

    // Se tem poucos, adiciona
    while (count < d.m)
    {
        pos = random_l_h(0, d.numGenes - 1);
        if (sol[pos] == 0)
        {
            sol[pos] = 1;
            count++;
        }
    }

    // Se tem muitos, remove
    while (count > d.m)
    {
        pos = random_l_h(0, d.numGenes - 1);
        if (sol[pos] == 1)
        {
            sol[pos] = 0;
            count--;
        }
    }
}

// ==================== ESCRITA ====================

// Escreve melhor solução
void write_best_ea(chrom x, info_ea d)
{
    int i;

    printf("\nMelhor individuo: %.2f\n", x.fitness);
    printf("Solucao: [ ");
    for (i = 0; i < d.numGenes; i++)
        if (x.pontos[i] == 1)
            printf("e%d ", i + 1);
    printf("]\n");
}