//Modulo de cabecalho responsavel por disponibilizar a geracao de alertas sem dependencia da interface
#ifndef ALERTAS_SERVICE_H
#define ALERTAS_SERVICE_H

#include <sqlite3.h>
#include <vector>

#include "modelos.h"
#include "resultado.h"

//Declara a funcao responsavel por consultar os riscos e gerar os alertas dos talhoes
ResultadoOperacao servicoListarAlertas(
    sqlite3 *db,
    std::vector<AlertaDTO> &alertas
);

#endif