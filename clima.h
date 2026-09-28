//Modulo de cabecalho responsavel por disponibilizar as funcoes utilizadas pela interface de clima e pelos calculos climaticos
#ifndef CLIMA_H
#define CLIMA_H

#include <sqlite3.h>

//Declara as funcoes da interface de terminal utilizadas no gerenciamento dos registros climaticos
void registrarClima(sqlite3 *db);
void listarHistoricoClima(sqlite3 *db);
void visualizarSerieTalhao(sqlite3 *db);
void editarClima(sqlite3 *db);
void excluirClima(sqlite3 *db);

//Declara as funcoes utilizadas para calcular os fatores climaticos de risco
int calcularRiscoTemperatura(float temperatura);
int calcularRiscoUmidade(float umidade);

void calcularRiscosClimaticos(
    float temperatura,
    float umidade,
    int *riscoTemperatura,
    int *riscoUmidade
);

//Declara as funcoes de validacao utilizadas nos registros climaticos
int validarData(char data[]);
int validarHora(char hora[]);

#endif