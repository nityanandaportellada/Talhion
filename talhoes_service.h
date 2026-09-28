//Modulo de cabecalho responsavel por disponibilizar as operacoes de talhoes independentes da interface de terminal
#ifndef TALHOES_SERVICE_H
#define TALHOES_SERVICE_H

#include <sqlite3.h>
#include <vector>

#include "modelos.h"
#include "resultado.h"

ResultadoOperacao servicoCadastrarTalhao(
    sqlite3 *db,
    const TalhaoDTO &talhao
);

ResultadoOperacao servicoEditarTalhao(
    sqlite3 *db,
    const TalhaoDTO &talhao
);

ResultadoOperacao servicoExcluirTalhao(
    sqlite3 *db,
    int codigo
);

ResultadoOperacao servicoBuscarTalhao(
    sqlite3 *db,
    int codigo,
    TalhaoDTO &talhao
);

ResultadoOperacao servicoListarTalhoes(
    sqlite3 *db,
    std::vector<TalhaoDTO> &talhoes
);

#endif