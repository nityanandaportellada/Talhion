//Modulo responsavel pela interface de terminal utilizada para consultar o historico e a evolucao dos riscos

#include <iostream>
#include <limits>
#include <vector>

#include "historico.h"
#include "historico_service.h"
#include "talhoes_service.h"
#include "modelos.h"
#include "resultado.h"

namespace {

//Limpa os dados restantes da entrada utilizada pelo terminal
void limparEntradaHistorico()
{
    std::cin.clear();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
}

}


//Consulta o historico geral atraves da camada de servicos
void listarHistorico(sqlite3 *db)
{
    std::vector<HistoricoOcorrenciaDTO> historico;

    ResultadoOperacao resultado =
        servicoListarHistorico(
            db,
            historico
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "          HISTORICO GERAL\n";
    std::cout << "====================================\n";

    if (historico.empty()) {

        std::cout
            << "\nNenhum registro encontrado.\n";

        return;
    }

    for (
        const HistoricoOcorrenciaDTO &registro
        : historico
    ) {

        std::cout << "\n-----------------------------\n";
        std::cout << "Tipo: Registro de Ocorrencia\n";

        std::cout
            << "Codigo da ocorrencia: "
            << registro.codigoOcorrencia
            << "\n";

        std::cout
            << "Talhao: "
            << registro.codigoTalhao
            << "\n";

        std::cout
            << "Nome do talhao: "
            << registro.nomeTalhao
            << "\n";

        std::cout
            << "Praga: "
            << registro.nomePraga
            << "\n";

        std::cout
            << "Nivel de infestacao: "
            << registro.nivelInfestacao
            << "\n";

        std::cout
            << "Area afetada: "
            << registro.areaAfetada
            << " hectares\n";

        std::cout
            << "Data: "
            << registro.data
            << "\n";
    }

    std::cout << "-----------------------------\n";
}


//Solicita o talhao e apresenta somente seu historico
void historicoPorTalhao(sqlite3 *db)
{
    int codigoTalhao;

    std::cout
        << "\nDigite o codigo do talhao: ";

    if (!(std::cin >> codigoTalhao)) {

        limparEntradaHistorico();

        std::cout
            << "\nDigite somente numeros.\n";

        return;
    }

    limparEntradaHistorico();

    std::vector<HistoricoOcorrenciaDTO> historico;

    ResultadoOperacao resultado =
        servicoHistoricoPorTalhao(
            db,
            codigoTalhao,
            historico
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "        HISTORICO DO TALHAO\n";
    std::cout << "====================================\n";

    if (historico.empty()) {

        std::cout
            << "\nNenhuma ocorrencia encontrada para este talhao.\n";

        return;
    }

    for (
        const HistoricoOcorrenciaDTO &registro
        : historico
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Ocorrencia: "
            << registro.codigoOcorrencia
            << "\n";

        std::cout
            << "Praga: "
            << registro.nomePraga
            << "\n";

        std::cout
            << "Nivel de infestacao: "
            << registro.nivelInfestacao
            << "\n";

        std::cout
            << "Area afetada: "
            << registro.areaAfetada
            << " hectares\n";

        std::cout
            << "Data: "
            << registro.data
            << "\n";
    }

    std::cout << "-----------------------------\n";
}


//Consulta a evolucao historica do risco utilizando a camada de servicos
void evolucaoTalhao(sqlite3 *db)
{
    int codigoTalhao;

    std::cout
        << "\nDigite o codigo do talhao: ";

    if (!(std::cin >> codigoTalhao)) {

        limparEntradaHistorico();

        std::cout
            << "\nDigite somente numeros.\n";

        return;
    }

    limparEntradaHistorico();

    std::vector<EvolucaoRiscoDTO> evolucao;

    ResultadoOperacao resultado =
        servicoEvolucaoTalhao(
            db,
            codigoTalhao,
            evolucao
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "         EVOLUCAO DO RISCO\n";
    std::cout << "====================================\n";

    if (evolucao.empty()) {

        std::cout
            << "\nNao existem dados suficientes para analisar a evolucao.\n";

        return;
    }

    for (
        const EvolucaoRiscoDTO &registro
        : evolucao
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Data: "
            << registro.data
            << "\n";

        if (registro.possuiClima) {

            std::cout
                << "Temperatura: "
                << registro.temperatura
                << " C\n";

            std::cout
                << "Umidade: "
                << registro.umidade
                << "%\n";
        }
        else {

            std::cout
                << "Nao existem dados climaticos para esta data.\n";
        }

        std::cout
            << "Risco temperatura: "
            << registro.riscoTemperatura
            << "\n";

        std::cout
            << "Risco umidade: "
            << registro.riscoUmidade
            << "\n";

        std::cout
            << "Risco praga: "
            << registro.riscoPraga
            << "\n";

        std::cout
            << "Risco final: "
            << registro.riscoFinal
            << "\n";

        std::cout
            << "Classificacao: "
            << registro.classificacao
            << "\n";

        std::cout
            << "Evolucao: "
            << registro.evolucao
            << "\n";
    }

    std::cout << "-----------------------------\n";
}