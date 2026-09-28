//Modulo responsavel pela interface de terminal utilizada para gerar e apresentar relatorios

#include <iostream>
#include <vector>
#include <string>

#include "relatorios.h"
#include "relatorios_service.h"
#include "modelos.h"
#include "resultado.h"


//Apresenta o relatorio climatico obtido atraves da camada de servicos
void gerarRelatorioClima(sqlite3 *db)
{
    std::vector<RelatorioClimaDTO> registros;

    ResultadoOperacao resultado =
        servicoRelatorioClima(
            db,
            registros
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "          RELATORIO DE CLIMA\n";
    std::cout << "====================================\n";

    if (registros.empty()) {

        std::cout
            << "\nNenhum registro encontrado.\n";

        return;
    }

    for (
        const RelatorioClimaDTO &registro
        : registros
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Talhao: "
            << registro.nomeTalhao
            << "\n";

        std::cout
            << "Data: "
            << registro.data
            << "\n";

        std::cout
            << "Hora: "
            << registro.hora
            << "\n";

        std::cout
            << "Temperatura: "
            << registro.temperatura
            << " C\n";

        std::cout
            << "Umidade: "
            << registro.umidade
            << "%\n";
    }

    std::cout << "-----------------------------\n";
}


//Apresenta o relatorio que relaciona ocorrencias de pragas e clima
void gerarRelatorioPragasClima(
    sqlite3 *db
)
{
    std::vector<RelatorioPragaClimaDTO> registros;

    ResultadoOperacao resultado =
        servicoRelatorioPragasClima(
            db,
            registros
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "       RELATORIO DE PRAGAS E CLIMA\n";
    std::cout << "====================================\n";

    if (registros.empty()) {

        std::cout
            << "\nNenhuma ocorrencia encontrada.\n";

        return;
    }

    for (
        const RelatorioPragaClimaDTO &registro
        : registros
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Talhao: "
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
            << "Data da ocorrencia: "
            << registro.dataOcorrencia
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

            std::cout
                << "Data do clima: "
                << registro.dataClima
                << "\n";
        }
        else {

            std::cout
                << "Nao existem dados climaticos para esta data.\n";
        }
    }

    std::cout << "-----------------------------\n";
}


//Solicita o periodo e apresenta o resumo calculado pela camada de servicos
void resumoPorPeriodo(sqlite3 *db)
{
    std::string dataInicial;
    std::string dataFinal;

    std::cout << "\n====================================\n";
    std::cout << "          RESUMO POR PERIODO\n";
    std::cout << "====================================\n";

    std::cout
        << "Digite a data inicial DD/MM/AAAA: ";

    std::cin
        >> dataInicial;

    std::cout
        << "Digite a data final DD/MM/AAAA: ";

    std::cin
        >> dataFinal;

    ResumoPeriodoDTO resumo;

    ResultadoOperacao resultado =
        servicoResumoPeriodo(
            db,
            dataInicial,
            dataFinal,
            resumo
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout
        << "\nQuantidade de registros: "
        << resumo.quantidadeRegistros
        << "\n";

    if (
        resumo.quantidadeRegistros == 0
    ) {

        std::cout
            << "Nenhum registro encontrado no periodo.\n";

        return;
    }

    std::cout
        << "Temperatura media: "
        << resumo.temperaturaMedia
        << " C\n";

    std::cout
        << "Umidade media: "
        << resumo.umidadeMedia
        << "%\n";

    std::cout
        << "Maior temperatura: "
        << resumo.maiorTemperatura
        << " C\n";

    std::cout
        << "Menor temperatura: "
        << resumo.menorTemperatura
        << " C\n";
}


//Solicita a exportacao dos dados do sistema para arquivos CSV
void exportarTalhoesCSV(sqlite3 *db)
{
    ResultadoOperacao resultado =
        servicoExportarCSV(
            db
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Apresenta no terminal o conteudo dos arquivos CSV previamente gerados
void lerTalhoesCSV()
{
    std::vector<std::string> linhas;

    ResultadoOperacao resultado =
        servicoLerArquivosCSV(
            linhas
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "          LEITURA DOS CSV\n";
    std::cout << "====================================\n";

    for (
        const std::string &linha
        : linhas
    ) {

        std::cout
            << linha
            << "\n";
    }
}


//Solicita a geracao do relatorio geral em arquivo TXT
void exportarRelatorioTXT(
    sqlite3 *db
)
{
    ResultadoOperacao resultado =
        servicoExportarRelatorioTXT(
            db
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}