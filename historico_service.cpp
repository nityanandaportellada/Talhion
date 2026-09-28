//Modulo responsavel pelas consultas e analises historicas sem utilizar funcoes da interface de terminal

#include "historico_service.h"
#include "talhoes_service.h"
#include "clima.h"

namespace {

//Classifica o risco numerico de acordo com as mesmas faixas utilizadas pelo restante do Talhion
std::string classificarRiscoHistorico(
    int riscoFinal
)
{
    if (riscoFinal <= 6) {
        return "BAIXO";
    }

    if (riscoFinal <= 16) {
        return "MEDIO";
    }

    if (riscoFinal <= 36) {
        return "ALTO";
    }

    return "MUITO ALTO";
}

}


//Consulta todas as ocorrencias registradas e devolve os dados para a interface
ResultadoOperacao servicoListarHistorico(
    sqlite3 *db,
    std::vector<HistoricoOcorrenciaDTO> &historico
)
{
    historico.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT "
        "o.codigo, "
        "o.codigo_talhao, "
        "t.nome, "
        "p.nome, "
        "o.nivel_infestacao, "
        "o.area_afetada, "
        "o.data "
        "FROM ocorrencias_pragas o "
        "INNER JOIN pragas p "
        "ON o.codigo_praga = p.codigo "
        "INNER JOIN talhoes t "
        "ON o.codigo_talhao = t.codigo "
        "ORDER BY "
        "o.codigo_talhao, "
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

        HistoricoOcorrenciaDTO registro;

        registro.codigoOcorrencia =
            sqlite3_column_int(
                stmt,
                0
            );

        registro.codigoTalhao =
            sqlite3_column_int(
                stmt,
                1
            );

        registro.nomeTalhao =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    2
                )
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

        registro.data =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    6
                )
            );

        historico.push_back(
            registro
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Historico consultado com sucesso."
    };
}


//Consulta somente as ocorrencias vinculadas ao talhao informado
ResultadoOperacao servicoHistoricoPorTalhao(
    sqlite3 *db,
    int codigoTalhao,
    std::vector<HistoricoOcorrenciaDTO> &historico
)
{
    TalhaoDTO talhao;

    ResultadoOperacao buscaTalhao =
        servicoBuscarTalhao(
            db,
            codigoTalhao,
            talhao
        );

    if (!buscaTalhao.sucesso) {
        return buscaTalhao;
    }

    historico.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT "
        "o.codigo, "
        "o.codigo_talhao, "
        "t.nome, "
        "p.nome, "
        "o.nivel_infestacao, "
        "o.area_afetada, "
        "o.data "
        "FROM ocorrencias_pragas o "
        "INNER JOIN pragas p "
        "ON o.codigo_praga = p.codigo "
        "INNER JOIN talhoes t "
        "ON o.codigo_talhao = t.codigo "
        "WHERE o.codigo_talhao = ? "
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

    sqlite3_bind_int(
        stmt,
        1,
        codigoTalhao
    );

    while (
        sqlite3_step(stmt)
        == SQLITE_ROW
    ) {

        HistoricoOcorrenciaDTO registro;

        registro.codigoOcorrencia =
            sqlite3_column_int(
                stmt,
                0
            );

        registro.codigoTalhao =
            sqlite3_column_int(
                stmt,
                1
            );

        registro.nomeTalhao =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    2
                )
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

        registro.data =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    6
                )
            );

        historico.push_back(
            registro
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Historico do talhao consultado com sucesso."
    };
}


//Calcula a evolucao historica do risco utilizando as datas em que existem ocorrencias de pragas
ResultadoOperacao servicoEvolucaoTalhao(
    sqlite3 *db,
    int codigoTalhao,
    std::vector<EvolucaoRiscoDTO> &evolucao
)
{
    TalhaoDTO talhao;

    ResultadoOperacao buscaTalhao =
        servicoBuscarTalhao(
            db,
            codigoTalhao,
            talhao
        );

    if (!buscaTalhao.sucesso) {
        return buscaTalhao;
    }

    evolucao.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT "
        "o.data, "
        "MAX(p.nivel_risco), "

        "(SELECT c.temperatura "
        "FROM clima c "
        "WHERE c.codigo_talhao = o.codigo_talhao "
        "AND c.data = o.data "
        "ORDER BY c.id DESC "
        "LIMIT 1), "

        "(SELECT c.umidade "
        "FROM clima c "
        "WHERE c.codigo_talhao = o.codigo_talhao "
        "AND c.data = o.data "
        "ORDER BY c.id DESC "
        "LIMIT 1) "

        "FROM ocorrencias_pragas o "

        "INNER JOIN pragas p "
        "ON o.codigo_praga = p.codigo "

        "WHERE o.codigo_talhao = ? "

        "GROUP BY o.data "

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

    sqlite3_bind_int(
        stmt,
        1,
        codigoTalhao
    );

    int riscoAnterior = -1;

    while (
        sqlite3_step(stmt)
        == SQLITE_ROW
    ) {

        EvolucaoRiscoDTO registro;

        registro.data =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    0
                )
            );

        registro.riscoPraga =
            sqlite3_column_int(
                stmt,
                1
            );

        if (
            sqlite3_column_type(
                stmt,
                2
            )
            != SQLITE_NULL
            &&
            sqlite3_column_type(
                stmt,
                3
            )
            != SQLITE_NULL
        ) {

            registro.possuiClima =
                true;

            registro.temperatura =
                sqlite3_column_double(
                    stmt,
                    2
                );

            registro.umidade =
                sqlite3_column_double(
                    stmt,
                    3
                );

            registro.riscoTemperatura =
                calcularRiscoTemperatura(
                    static_cast<float>(
                        registro.temperatura
                    )
                );

            registro.riscoUmidade =
                calcularRiscoUmidade(
                    static_cast<float>(
                        registro.umidade
                    )
                );
        }

        registro.riscoFinal =
            registro.riscoTemperatura
            *
            registro.riscoUmidade
            *
            registro.riscoPraga;

        registro.classificacao =
            classificarRiscoHistorico(
                registro.riscoFinal
            );

        if (riscoAnterior == -1) {

            registro.evolucao =
                "PRIMEIRO REGISTRO";
        }
        else if (
            registro.riscoFinal
            > riscoAnterior
        ) {

            registro.evolucao =
                "AUMENTOU";
        }
        else if (
            registro.riscoFinal
            < riscoAnterior
        ) {

            registro.evolucao =
                "DIMINUIU";
        }
        else {

            registro.evolucao =
                "PERMANECEU ESTAVEL";
        }

        riscoAnterior =
            registro.riscoFinal;

        evolucao.push_back(
            registro
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Evolucao de risco consultada com sucesso."
    };
}