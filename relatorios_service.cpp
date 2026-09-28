//Modulo responsavel pela geracao de relatorios, resumos e arquivos sem utilizar funcoes de entrada ou saida da interface

#include "relatorios_service.h"
#include "talhoes_service.h"
#include "pragas_service.h"
#include "clima_service.h"

#include <fstream>
#include <sstream>


//Consulta os registros climaticos utilizados pelo relatorio
ResultadoOperacao servicoRelatorioClima(
    sqlite3 *db,
    std::vector<RelatorioClimaDTO> &registros
)
{
    registros.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT "
        "t.codigo, "
        "t.nome, "
        "c.data, "
        "c.hora, "
        "c.temperatura, "
        "c.umidade "
        "FROM clima c "
        "INNER JOIN talhoes t "
        "ON c.codigo_talhao = t.codigo "
        "ORDER BY "
        "substr(c.data, 7, 4) || '-' || "
        "substr(c.data, 4, 2) || '-' || "
        "substr(c.data, 1, 2), "
        "c.hora;";

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        )
        != SQLITE_OK
    ) {
        return {
            false,
            sqlite3_errmsg(db)
        };
    }

    while (
        sqlite3_step(stmt)
        == SQLITE_ROW
    ) {

        RelatorioClimaDTO registro;

        registro.codigoTalhao =
            sqlite3_column_int(
                stmt,
                0
            );

        registro.nomeTalhao =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    1
                )
            );

        registro.data =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    2
                )
            );

        registro.hora =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    3
                )
            );

        registro.temperatura =
            sqlite3_column_double(
                stmt,
                4
            );

        registro.umidade =
            sqlite3_column_double(
                stmt,
                5
            );

        registros.push_back(
            registro
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Relatorio climatico gerado com sucesso."
    };
}


//Consulta ocorrencias de pragas e relaciona o ultimo registro climatico existente na mesma data
ResultadoOperacao servicoRelatorioPragasClima(
    sqlite3 *db,
    std::vector<RelatorioPragaClimaDTO> &registros
)
{
    registros.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT "
        "t.codigo, "
        "t.nome, "
        "p.codigo, "
        "p.nome, "
        "o.nivel_infestacao, "
        "o.area_afetada, "
        "o.data, "
        "c.temperatura, "
        "c.umidade, "
        "c.data "

        "FROM ocorrencias_pragas o "

        "INNER JOIN pragas p "
        "ON o.codigo_praga = p.codigo "

        "INNER JOIN talhoes t "
        "ON o.codigo_talhao = t.codigo "

        "LEFT JOIN clima c "
        "ON c.id = ("
            "SELECT MAX(c2.id) "
            "FROM clima c2 "
            "WHERE c2.codigo_talhao = o.codigo_talhao "
            "AND c2.data = o.data"
        ") "

        "ORDER BY "
        "substr(o.data, 7, 4) || '-' || "
        "substr(o.data, 4, 2) || '-' || "
        "substr(o.data, 1, 2);";

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        )
        != SQLITE_OK
    ) {
        return {
            false,
            sqlite3_errmsg(db)
        };
    }

    while (
        sqlite3_step(stmt)
        == SQLITE_ROW
    ) {

        RelatorioPragaClimaDTO registro;

        registro.codigoTalhao =
            sqlite3_column_int(
                stmt,
                0
            );

        registro.nomeTalhao =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    1
                )
            );

        registro.codigoPraga =
            sqlite3_column_int(
                stmt,
                2
            );

        registro.nomePraga =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    3
                )
            );

        registro.nivelInfestacao =
            sqlite3_column_int(
                stmt,
                4
            );

        registro.areaAfetada =
            sqlite3_column_double(
                stmt,
                5
            );

        registro.dataOcorrencia =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    6
                )
            );

        if (
            sqlite3_column_type(
                stmt,
                7
            )
            != SQLITE_NULL
        ) {

            registro.possuiClima =
                true;

            registro.temperatura =
                sqlite3_column_double(
                    stmt,
                    7
                );

            registro.umidade =
                sqlite3_column_double(
                    stmt,
                    8
                );

            registro.dataClima =
                reinterpret_cast<const char *>(
                    sqlite3_column_text(
                        stmt,
                        9
                    )
                );
        }

        registros.push_back(
            registro
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Relatorio de pragas e clima gerado com sucesso."
    };
}


//Calcula o resumo dos registros climaticos existentes dentro do periodo informado
ResultadoOperacao servicoResumoPeriodo(
    sqlite3 *db,
    const std::string &dataInicial,
    const std::string &dataFinal,
    ResumoPeriodoDTO &resumo
)
{
    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT "
        "COUNT(*), "
        "AVG(temperatura), "
        "AVG(umidade), "
        "MAX(temperatura), "
        "MIN(temperatura) "

        "FROM clima "

        "WHERE "

        "substr(data, 7, 4) || '-' || "
        "substr(data, 4, 2) || '-' || "
        "substr(data, 1, 2) "

        "BETWEEN "

        "substr(?, 7, 4) || '-' || "
        "substr(?, 4, 2) || '-' || "
        "substr(?, 1, 2) "

        "AND "

        "substr(?, 7, 4) || '-' || "
        "substr(?, 4, 2) || '-' || "
        "substr(?, 1, 2);";

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        )
        != SQLITE_OK
    ) {
        return {
            false,
            sqlite3_errmsg(db)
        };
    }

    for (
        int i = 1;
        i <= 3;
        ++i
    ) {

        sqlite3_bind_text(
            stmt,
            i,
            dataInicial.c_str(),
            -1,
            SQLITE_TRANSIENT
        );
    }

    for (
        int i = 4;
        i <= 6;
        ++i
    ) {

        sqlite3_bind_text(
            stmt,
            i,
            dataFinal.c_str(),
            -1,
            SQLITE_TRANSIENT
        );
    }

    resumo = {};

    resumo.dataInicial =
        dataInicial;

    resumo.dataFinal =
        dataFinal;

    if (
        sqlite3_step(stmt)
        == SQLITE_ROW
    ) {

        resumo.quantidadeRegistros =
            sqlite3_column_int(
                stmt,
                0
            );

        if (
            resumo.quantidadeRegistros > 0
        ) {

            resumo.temperaturaMedia =
                sqlite3_column_double(
                    stmt,
                    1
                );

            resumo.umidadeMedia =
                sqlite3_column_double(
                    stmt,
                    2
                );

            resumo.maiorTemperatura =
                sqlite3_column_double(
                    stmt,
                    3
                );

            resumo.menorTemperatura =
                sqlite3_column_double(
                    stmt,
                    4
                );
        }
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Resumo calculado com sucesso."
    };
}


//Exporta as principais tabelas para arquivos CSV utilizando os servicos da aplicacao
ResultadoOperacao servicoExportarCSV(
    sqlite3 *db
)
{
    std::vector<TalhaoDTO> talhoes;
    std::vector<PragaDTO> pragas;
    std::vector<OcorrenciaPragaDTO> ocorrencias;
    std::vector<RegistroClimaDTO> clima;

    ResultadoOperacao resultado;

    resultado =
        servicoListarTalhoes(
            db,
            talhoes
        );

    if (!resultado.sucesso) {
        return resultado;
    }

    resultado =
        servicoListarPragas(
            db,
            pragas
        );

    if (!resultado.sucesso) {
        return resultado;
    }

    resultado =
        servicoListarOcorrencias(
            db,
            ocorrencias
        );

    if (!resultado.sucesso) {
        return resultado;
    }

    resultado =
        servicoListarClima(
            db,
            clima
        );

    if (!resultado.sucesso) {
        return resultado;
    }

    std::ofstream arquivoTalhoes(
        "talhoes.csv"
    );

    if (!arquivoTalhoes.is_open()) {

        return {
            false,
            "Nao foi possivel criar talhoes.csv."
        };
    }

    arquivoTalhoes
        << "Codigo;Nome;Area;Plantacao;Localizacao\n";

    for (
        const TalhaoDTO &talhao
        : talhoes
    ) {

        arquivoTalhoes
            << talhao.codigo << ";"
            << talhao.nome << ";"
            << talhao.area << ";"
            << talhao.plantacao << ";"
            << talhao.localizacao
            << "\n";
    }

    arquivoTalhoes.close();

    std::ofstream arquivoPragas(
        "pragas.csv"
    );

    if (!arquivoPragas.is_open()) {

        return {
            false,
            "Nao foi possivel criar pragas.csv."
        };
    }

    arquivoPragas
        << "Codigo;Nome;Descricao;NivelRisco\n";

    for (
        const PragaDTO &praga
        : pragas
    ) {

        arquivoPragas
            << praga.codigo << ";"
            << praga.nome << ";"
            << praga.descricao << ";"
            << praga.nivelRisco
            << "\n";
    }

    arquivoPragas.close();

    std::ofstream arquivoOcorrencias(
        "ocorrencias_pragas.csv"
    );

    if (!arquivoOcorrencias.is_open()) {

        return {
            false,
            "Nao foi possivel criar ocorrencias_pragas.csv."
        };
    }

    arquivoOcorrencias
        << "Codigo;CodigoPraga;CodigoTalhao;"
        << "NivelInfestacao;AreaAfetada;Data\n";

    for (
        const OcorrenciaPragaDTO &ocorrencia
        : ocorrencias
    ) {

        arquivoOcorrencias
            << ocorrencia.codigo << ";"
            << ocorrencia.codigoPraga << ";"
            << ocorrencia.codigoTalhao << ";"
            << ocorrencia.nivelInfestacao << ";"
            << ocorrencia.areaAfetada << ";"
            << ocorrencia.data
            << "\n";
    }

    arquivoOcorrencias.close();

    std::ofstream arquivoClima(
        "clima.csv"
    );

    if (!arquivoClima.is_open()) {

        return {
            false,
            "Nao foi possivel criar clima.csv."
        };
    }

    arquivoClima
        << "ID;CodigoTalhao;Temperatura;"
        << "Umidade;Data;Hora\n";

    for (
        const RegistroClimaDTO &registro
        : clima
    ) {

        arquivoClima
            << registro.id << ";"
            << registro.codigoTalhao << ";"
            << registro.temperatura << ";"
            << registro.umidade << ";"
            << registro.data << ";"
            << registro.hora
            << "\n";
    }

    arquivoClima.close();

    return {
        true,
        "Arquivos CSV exportados com sucesso."
    };
}


//Le os arquivos CSV sem apresentar diretamente os dados na interface
ResultadoOperacao servicoLerArquivosCSV(
    std::vector<std::string> &linhas
)
{
    linhas.clear();

    const std::string arquivos[] = {
        "talhoes.csv",
        "pragas.csv",
        "ocorrencias_pragas.csv",
        "clima.csv"
    };

    for (
        const std::string &nomeArquivo
        : arquivos
    ) {

        std::ifstream arquivo(
            nomeArquivo
        );

        if (!arquivo.is_open()) {

            linhas.push_back(
                "Arquivo nao encontrado: "
                + nomeArquivo
            );

            continue;
        }

        linhas.push_back(
            "===== "
            + nomeArquivo
            + " ====="
        );

        std::string linha;

        while (
            std::getline(
                arquivo,
                linha
            )
        ) {

            linhas.push_back(
                linha
            );
        }

        arquivo.close();

        linhas.push_back("");
    }

    return {
        true,
        "Arquivos CSV lidos com sucesso."
    };
}


//Gera o relatorio geral do Talhion em formato TXT
ResultadoOperacao servicoExportarRelatorioTXT(
    sqlite3 *db
)
{
    std::vector<TalhaoDTO> talhoes;
    std::vector<RelatorioClimaDTO> clima;
    std::vector<RelatorioPragaClimaDTO> pragasClima;

    ResultadoOperacao resultado =
        servicoListarTalhoes(
            db,
            talhoes
        );

    if (!resultado.sucesso) {
        return resultado;
    }

    resultado =
        servicoRelatorioClima(
            db,
            clima
        );

    if (!resultado.sucesso) {
        return resultado;
    }

    resultado =
        servicoRelatorioPragasClima(
            db,
            pragasClima
        );

    if (!resultado.sucesso) {
        return resultado;
    }

    std::ofstream arquivo(
        "relatorio_fazenda.txt"
    );

    if (!arquivo.is_open()) {

        return {
            false,
            "Nao foi possivel criar relatorio_fazenda.txt."
        };
    }

    arquivo
        << "====================================\n"
        << "       RELATORIO GERAL TALHION\n"
        << "====================================\n\n";

    arquivo
        << "TALHOES\n"
        << "------------------------------------\n";

    for (
        const TalhaoDTO &talhao
        : talhoes
    ) {

        arquivo
            << "Codigo: " << talhao.codigo << "\n"
            << "Nome: " << talhao.nome << "\n"
            << "Area: " << talhao.area << " hectares\n"
            << "Plantacao: " << talhao.plantacao << "\n"
            << "Localizacao: " << talhao.localizacao << "\n"
            << "------------------------------------\n";
    }

    arquivo
        << "\nDADOS CLIMATICOS\n"
        << "------------------------------------\n";

    for (
        const RelatorioClimaDTO &registro
        : clima
    ) {

        arquivo
            << "Talhao: " << registro.nomeTalhao << "\n"
            << "Data: " << registro.data << "\n"
            << "Hora: " << registro.hora << "\n"
            << "Temperatura: " << registro.temperatura << " C\n"
            << "Umidade: " << registro.umidade << "%\n"
            << "------------------------------------\n";
    }

    arquivo
        << "\nOCORRENCIAS DE PRAGAS\n"
        << "------------------------------------\n";

    for (
        const RelatorioPragaClimaDTO &registro
        : pragasClima
    ) {

        arquivo
            << "Talhao: " << registro.nomeTalhao << "\n"
            << "Praga: " << registro.nomePraga << "\n"
            << "Nivel de infestacao: "
            << registro.nivelInfestacao << "\n"
            << "Area afetada: "
            << registro.areaAfetada << " hectares\n"
            << "Data: "
            << registro.dataOcorrencia << "\n"
            << "------------------------------------\n";
    }

    arquivo.close();

    return {
        true,
        "Relatorio TXT exportado com sucesso."
    };
}