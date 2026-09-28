//Modulo de cabecalho responsavel por disponibilizar as operacoes de pragas e ocorrencias independentes da interface de terminal
#ifndef PRAGAS_SERVICE_H
#define PRAGAS_SERVICE_H

#include <sqlite3.h>
#include <vector>

#include "modelos.h"
#include "resultado.h"

ResultadoOperacao servicoCadastrarPraga(
    sqlite3 *db,
    const PragaDTO &praga
);

ResultadoOperacao servicoEditarPraga(
    sqlite3 *db,
    const PragaDTO &praga
);

ResultadoOperacao servicoExcluirPraga(
    sqlite3 *db,
    int codigo
);

ResultadoOperacao servicoBuscarPraga(
    sqlite3 *db,
    int codigo,
    PragaDTO &praga
);

ResultadoOperacao servicoListarPragas(
    sqlite3 *db,
    std::vector<PragaDTO> &pragas
);

ResultadoOperacao servicoCadastrarOcorrencia(
    sqlite3 *db,
    const OcorrenciaPragaDTO &ocorrencia
);

ResultadoOperacao servicoEditarOcorrencia(
    sqlite3 *db,
    const OcorrenciaPragaDTO &ocorrencia
);

ResultadoOperacao servicoExcluirOcorrencia(
    sqlite3 *db,
    int codigo
);

ResultadoOperacao servicoBuscarOcorrencia(
    sqlite3 *db,
    int codigo,
    OcorrenciaPragaDTO &ocorrencia
);

ResultadoOperacao servicoListarOcorrencias(
    sqlite3 *db,
    std::vector<OcorrenciaPragaDTO> &ocorrencias
);

#endif