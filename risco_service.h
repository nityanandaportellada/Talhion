//Modulo de cabecalho responsavel por disponibilizar o calculo de risco sem dependencia da interface de terminal
#ifndef RISCO_SERVICE_H
#define RISCO_SERVICE_H

#include <sqlite3.h>
#include <vector>

#include "modelos.h"
#include "resultado.h"

ResultadoOperacao servicoCalcularRiscoTalhao(
    sqlite3 *db,
    int codigoTalhao,
    RiscoTalhaoDTO &risco
);

ResultadoOperacao servicoListarRiscos(
    sqlite3 *db,
    std::vector<RiscoTalhaoDTO> &riscos
);

#endif