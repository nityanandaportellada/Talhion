//Modulo responsavel pela interface de terminal utilizada para apresentar os alertas gerados pela camada de servicos

#include <iostream>
#include <vector>

#include "alertas.h"
#include "alertas_service.h"
#include "modelos.h"
#include "resultado.h"


//Consulta os alertas atraves da camada de servicos e apresenta os resultados no terminal
void gerarAlertas(sqlite3 *db)
{
    std::vector<AlertaDTO> alertas;

    ResultadoOperacao resultado =
        servicoListarAlertas(
            db,
            alertas
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "              ALERTAS\n";
    std::cout << "====================================\n";

    if (alertas.empty()) {

        std::cout
            << "\nNenhum alerta encontrado.\n";

        return;
    }

    for (
        const AlertaDTO &alerta
        : alertas
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Talhao: "
            << alerta.codigoTalhao
            << "\n";

        std::cout
            << "Nome: "
            << alerta.nomeTalhao
            << "\n";

        std::cout
            << "Risco temperatura: "
            << alerta.riscoTemperatura
            << "\n";

        std::cout
            << "Risco umidade: "
            << alerta.riscoUmidade
            << "\n";

        std::cout
            << "Risco praga: "
            << alerta.riscoPraga
            << "\n";

        std::cout
            << "Risco final: "
            << alerta.riscoFinal
            << "\n";

        std::cout
            << "Classificacao: "
            << alerta.classificacao
            << "\n";

        std::cout
            << alerta.mensagem
            << "\n";
    }

    std::cout << "-----------------------------\n";
}