//Modulo de cabecalho responsavel por disponibilizar as funcoes da interface de terminal utilizadas no gerenciamento dos talhoes
#ifndef TALHOES_H
#define TALHOES_H

#include <sqlite3.h>

//Declara as funcoes utilizadas pela interface de terminal para cadastrar, listar, buscar, editar e excluir talhoes
void cadastrarTalhao(sqlite3 *db);
void listarTalhoes(sqlite3 *db);
int buscarTalhao(sqlite3 *db, int codigo);
void editarTalhao(sqlite3 *db);
void excluirTalhao(sqlite3 *db);

#endif