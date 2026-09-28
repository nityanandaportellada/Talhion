//Modulo de cabecalho responsavel por disponibilizar as operacoes climaticas independentes da interface de terminal
#ifndef CLIMA_SERVICE_H
#define CLIMA_SERVICE_H

#include <sqlite3.h>
#include <vector>

#include "modelos.h"
#include "resultado.h"

ResultadoOperacao servicoRegistrarClima(
    sqlite3 *db,
    const RegistroClimaDTO &registro
);

ResultadoOperacao servicoEditarClima(
    sqlite3 *db,
    const RegistroClimaDTO &registro
);

ResultadoOperacao servicoExcluirClima(
    sqlite3 *db,
    int id
);

ResultadoOperacao servicoBuscarClima(
    sqlite3 *db,
    int id,
    RegistroClimaDTO &registro
);

ResultadoOperacao servicoListarClima(
    sqlite3 *db,
    std::vector<RegistroClimaDTO> &registros
);

ResultadoOperacao servicoListarClimaPorTalhao(
    sqlite3 *db,
    int codigoTalhao,
    std::vector<RegistroClimaDTO> &registros
);

#endif