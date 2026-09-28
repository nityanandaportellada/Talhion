//Modulo de cabecalho responsavel por disponibilizar as funcoes da interface utilizadas nos relatorios
#ifndef RELATORIOS_H
#define RELATORIOS_H

#include <sqlite3.h>

//Declara as funcoes utilizadas pela interface de terminal
void gerarRelatorioClima(sqlite3 *db);
void gerarRelatorioPragasClima(sqlite3 *db);
void resumoPorPeriodo(sqlite3 *db);
void exportarTalhoesCSV(sqlite3 *db);
void lerTalhoesCSV();
void exportarRelatorioTXT(sqlite3 *db);

#endif