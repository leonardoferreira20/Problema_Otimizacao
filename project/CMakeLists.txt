cmake_minimum_required(VERSION 3.30)
project(project C)

set(CMAKE_C_STANDARD 11)

add_executable(project main.c
        pesquisa_local/algoritmo_pesquisa_local.c
        pesquisa_local/algoritmo_pesquisa_local.h
        evolutivo/algoritmo_evolutivo.c
        evolutivo/algoritmo_evolutivo.h
        funcao_problema/funcao.c
        funcao_problema/funcao.h
        utils.c
        utils.h)
