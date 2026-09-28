//Modulo de cabecalho responsavel por disponibilizar as consultas de historico sem dependencia da interface de terminal
#ifndef HISTORICO_SERVICE_H
#define HISTORICO_SERVICE_H

#include <sqlite3.h>
#include <vector>

#include "modelos.h"
#include "resultado.h"

//Declara a consulta do historico geral de ocorrencias
ResultadoOperacao servicoListarHistorico(
    sqlite3 *db,
    std::vector<HistoricoOcorrenciaDTO> &historico
);

//Declara a consulta do historico de um unico talhao
ResultadoOperacao servicoHistoricoPorTalhao(
    sqlite3 *db,
    int codigoTalhao,
    std::vector<HistoricoOcorrenciaDTO> &historico
);

//Declara a analise da evolucao do risco de um determinado talhao
ResultadoOperacao servicoEvolucaoTalhao(
    sqlite3 *db,
    int codigoTalhao,
    std::vector<EvolucaoRiscoDTO> &evolucao
);

#endif