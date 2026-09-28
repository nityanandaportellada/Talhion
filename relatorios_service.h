//Modulo de cabecalho responsavel por disponibilizar as operacoes de relatorios e exportacoes sem dependencia da interface
#ifndef RELATORIOS_SERVICE_H
#define RELATORIOS_SERVICE_H

#include <sqlite3.h>
#include <string>
#include <vector>

#include "modelos.h"
#include "resultado.h"

ResultadoOperacao servicoRelatorioClima(
    sqlite3 *db,
    std::vector<RelatorioClimaDTO> &registros
);

ResultadoOperacao servicoRelatorioPragasClima(
    sqlite3 *db,
    std::vector<RelatorioPragaClimaDTO> &registros
);

ResultadoOperacao servicoResumoPeriodo(
    sqlite3 *db,
    const std::string &dataInicial,
    const std::string &dataFinal,
    ResumoPeriodoDTO &resumo
);

ResultadoOperacao servicoExportarCSV(
    sqlite3 *db
);

ResultadoOperacao servicoLerArquivosCSV(
    std::vector<std::string> &linhas
);

ResultadoOperacao servicoExportarRelatorioTXT(
    sqlite3 *db
);

#endif