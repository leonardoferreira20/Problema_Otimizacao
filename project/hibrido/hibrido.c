//
// Created by Leonardo Ferreira on 06/12/2025.
//

#include "hibrido.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utils.h"
#include "../funcao_problema/funcao.h"
#include "../pesquisa_local/algoritmo_pesquisa_local.h"
#include "../evolutivo/algoritmo_evolutivo.h"

// =================================================================
// HÍBRIDO 1: Evolutivo + Pesquisa Local no FINAL
// Estratégia: Executa EA normalmente, depois aplica PL na população final
// =================================================================
float hibrido_evolutivo_depois_pl(float *mat, int vert, int m, info_ea EA_param, int num_iter_pl){
    pchrom pop, parents;
    chrom best;
    int gen;

    printf(">>> Híbrido 1: Evolutivo + Pesquisa Local No Final <<<\n\n");

    // 1) Criar população inicial
    pop = init_pop_ea(EA_param);
    evaluate_pop(pop, EA_param, mat);

    // Guardar melhor inicial
    best = pop[0];
    best = get_best_ea(pop, EA_param, best);

    // Alocar pais
    parents = malloc(sizeof(chrom) * EA_param.popsize);

    // 2) LOOP DO EVOLUTIVO NORMAL
    for (gen = 1; gen <= EA_param.numGenerations; gen++){
        // Seleção
        tournament_ea(pop, EA_param, parents);

        // Recombinação + Mutação
        genetic_operators_ea(parents, EA_param, pop);

        // Avaliação (inclui reparação)
        evaluate_pop(pop, EA_param, mat);

        // Atualizar melhor
        best = get_best_ea(pop, EA_param, best);
    }

    printf("Melhor após EA: %.2f\n", best.fitness);

    // 3) APLICAR PESQUISA LOCAL A CADA INDIVIDUO DA POPULAÇÃO FINAL
    for (int i = 0; i < EA_param.popsize; i++){
        pop[i].fitness = trepa_colinas_recristalizacao_simulada(pop[i].pontos, mat, vert, m, num_iter_pl);
    }

    // Reavaliar fitness real
    evaluate_pop(pop, EA_param, mat);

    // Melhor após PL
    best = get_best_ea(pop, EA_param, best);

    printf("Melhor após PL: %.2f\n", best.fitness);

    free(parents);
    free(pop);

    return best.fitness;
}

// =================================================================
// HÍBRIDO 2: Pesquisa Local DURANTE o Evolutivo
// Estratégia: A cada X gerações, aplica PL nos melhores N% da população
// =================================================================
float hibrido_pl_durante_evolutivo(float *mat, int vert, int m, info_ea EA_param, int num_iter_pl, int freq_pl){
    pchrom pop, parents;
    chrom best;
    int gen;

    printf(">>> Híbrido 2: Pesquisa Local Durante o Evolutivo <<<\n\n");

    pop = init_pop_ea(EA_param);
    evaluate_pop(pop, EA_param, mat);

    best = pop[0];
    best = get_best_ea(pop, EA_param, best);

    parents = malloc(sizeof(chrom) * EA_param.popsize);

    int num_melhores = (int)(EA_param.popsize * 0.2);
    if (num_melhores < 1) num_melhores = 1;

    for (gen = 1; gen <= EA_param.numGenerations; gen++){
        // ========== Etapa normal do evolutivo ==========
        tournament_ea(pop, EA_param, parents);
        genetic_operators_ea(parents, EA_param, pop);
        evaluate_pop(pop, EA_param, mat);

        // ========== Aplicar PL periodicamente ==========
        if (gen % freq_pl == 0){
            printf("Geracao %d: Aplicar PL nos melhores %d\n", gen, num_melhores);

            // Copiar pop temporariamente
            chrom *temp = malloc(sizeof(chrom) * EA_param.popsize);
            for (int i = 0; i < EA_param.popsize; i++)
                temp[i] = pop[i];

            // Ordenar por fitness desc (selection sort simples)
            for (int i = 0; i < EA_param.popsize - 1; i++){
                for (int j = i + 1; j < EA_param.popsize; j++){
                    if (temp[j].fitness > temp[i].fitness)
                    {
                        chrom aux = temp[i];
                        temp[i] = temp[j];
                        temp[j] = aux;
                    }
                }
            }

            // Aplicar PL aos melhores N
            for (int k = 0; k < num_melhores; k++){
                // Encontrar índice correspondente na população original
                float target_fitness = temp[k].fitness;
                int idx = -1;

                for (int i = 0; i < EA_param.popsize; i++){
                    if (pop[i].fitness == target_fitness){
                        idx = i;
                        break;
                    }
                }

                if (idx != -1){
                    pop[idx].fitness = trepa_colinas_recristalizacao_simulada(pop[idx].pontos, mat, vert, m, num_iter_pl);
                }
            }

            free(temp);
            evaluate_pop(pop, EA_param, mat);
        }

        best = get_best_ea(pop, EA_param, best);
    }

    free(parents);
    free(pop);

    return best.fitness;
}