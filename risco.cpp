//Modulo responsavel pela interface de terminal utilizada para apresentar a classificacao de risco calculada pela camada de servicos

#include <iostream>
#include <vector>

#include "risco.h"
#include "risco_service.h"
#include "modelos.h"
#include "resultado.h"


//Consulta a classificacao de risco atraves da camada de servicos e apresenta os resultados no terminal
void classificarRisco(sqlite3 *db)
{
    std::vector<RiscoTalhaoDTO> riscos;

    ResultadoOperacao resultado =
        servicoListarRiscos(
            db,
            riscos
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "        CLASSIFICACAO DE RISCO\n";
    std::cout << "====================================\n";

    if (riscos.empty()) {

        std::cout
            << "\nNenhum talhao cadastrado.\n";

        return;
    }

    for (
        const RiscoTalhaoDTO &risco
        : riscos
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Talhao: "
            << risco.codigoTalhao
            << "\n";

        std::cout
            << "Nome: "
            << risco.nomeTalhao
            << "\n";

        std::cout
            << "Risco temperatura: "
            << risco.riscoTemperatura
            << "\n";

        std::cout
            << "Risco umidade: "
            << risco.riscoUmidade
            << "\n";

        std::cout
            << "Risco praga: "
            << risco.riscoPraga
            << "\n";

        std::cout
            << "Risco final: "
            << risco.riscoFinal
            << "\n";

        std::cout
            << "Classificacao: "
            << risco.classificacao
            << "\n";
    }

    std::cout << "-----------------------------\n";
}