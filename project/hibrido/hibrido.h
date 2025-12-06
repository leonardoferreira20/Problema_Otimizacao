//
// Created by Leonardo Ferreira on 06/12/2025.
//
#include "../evolutivo/algoritmo_evolutivo.h"

#ifndef HIBRIDO_H
#define HIBRIDO_H

// Híbrido 1: Evolutivo + Pesquisa Local no FINAL
float hibrido_evolutivo_depois_pl(float *mat, int vert, int m, struct info_ea EA_param, int num_iter_pl);

// Híbrido 2: Pesquisa Local DURANTE o Evolutivo (nos melhores)
float hibrido_pl_durante_evolutivo(float *mat, int vert, int m, struct info_ea EA_param, int num_iter_pl, int freq_pl);
#endif //HIBRIDO_H
