//Modulo de cabecalho responsavel por disponibilizar a interface de terminal utilizada para apresentar os alertas
#ifndef ALERTAS_H
#define ALERTAS_H

#include <sqlite3.h>

//Declara a funcao responsavel por apresentar os alertas gerados pela camada de servicos
void gerarAlertas(sqlite3 *db);

#endif