//Modulo de cabecalho responsavel por disponibilizar as funcoes da interface de terminal utilizadas no gerenciamento de pragas e ocorrencias
#ifndef PRAGAS_H
#define PRAGAS_H

#include <sqlite3.h>

//Declara as funcoes utilizadas pela interface de terminal para gerenciamento das pragas
void listarPragas(sqlite3 *db);
void cadastrarPraga(sqlite3 *db);
int buscarPraga(sqlite3 *db, int codigo);
void editarPraga(sqlite3 *db);
void excluirPraga(sqlite3 *db);

//Declara as funcoes utilizadas pela interface de terminal para gerenciamento das ocorrencias de pragas
void cadastrarOcorrencia(sqlite3 *db);
void listarOcorrencias(sqlite3 *db);
int buscarOcorrencia(sqlite3 *db, int codigo);
void editarOcorrencia(sqlite3 *db);
void excluirOcorrencia(sqlite3 *db);

#endif