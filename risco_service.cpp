//Modulo responsavel pelo calculo de risco dos talhoes sem utilizar printf ou scanf

#include "risco_service.h"
#include "clima.h"
#include "talhoes_service.h"

namespace {

//Converte o valor numerico do risco final na classificacao utilizada pelo Talhion
std::string classificarRiscoServico(
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


ResultadoOperacao servicoCalcularRiscoTalhao(
    sqlite3 *db,
    int codigoTalhao,
    RiscoTalhaoDTO &risco
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

    risco = {};

    risco.codigoTalhao =
        talhao.codigo;

    risco.nomeTalhao =
        talhao.nome;

    sqlite3_stmt *stmt = nullptr;

    //Busca os dados climaticos mais recentes do talhao
    const char *sqlClima =
        "SELECT temperatura, umidade "
        "FROM clima "
        "WHERE codigo_talhao = ? "
        "ORDER BY id DESC "
        "LIMIT 1;";

    if (
        sqlite3_prepare_v2(
            db,
            sqlClima,
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

    if (
        sqlite3_step(stmt)
        == SQLITE_ROW
    ) {
        double temperatura =
            sqlite3_column_double(
                stmt,
                0
            );

        double umidade =
            sqlite3_column_double(
                stmt,
                1
            );

        risco.riscoTemperatura =
            calcularRiscoTemperatura(
                static_cast<float>(
                    temperatura
                )
            );

        risco.riscoUmidade =
            calcularRiscoUmidade(
                static_cast<float>(
                    umidade
                )
            );
    }

    sqlite3_finalize(stmt);

    //Busca a praga de maior risco na ultima data registrada para o talhao
    const char *sqlPraga =
        "SELECT MAX(p.nivel_risco) "
        "FROM ocorrencias_pragas o "
        "INNER JOIN pragas p "
        "ON o.codigo_praga = p.codigo "
        "WHERE o.codigo_talhao = ? "
        "AND o.data = ("
        "SELECT o2.data "
        "FROM ocorrencias_pragas o2 "
        "WHERE o2.codigo_talhao = ? "
        "ORDER BY "
        "substr(o2.data, 7, 4) || '-' || "
        "substr(o2.data, 4, 2) || '-' || "
        "substr(o2.data, 1, 2) DESC "
        "LIMIT 1"
        ");";

    if (
        sqlite3_prepare_v2(
            db,
            sqlPraga,
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

    sqlite3_bind_int(
        stmt,
        2,
        codigoTalhao
    );

    if (
        sqlite3_step(stmt)
        == SQLITE_ROW &&
        sqlite3_column_type(
            stmt,
            0
        )
        != SQLITE_NULL
    ) {
        risco.riscoPraga =
            sqlite3_column_int(
                stmt,
                0
            );
    }

    sqlite3_finalize(stmt);

    //Calcula o risco final mantendo a mesma regra utilizada atualmente pelo sistema
    risco.riscoFinal =
        risco.riscoTemperatura
        *
        risco.riscoUmidade
        *
        risco.riscoPraga;

    risco.classificacao =
        classificarRiscoServico(
            risco.riscoFinal
        );

    return {
        true,
        "Risco calculado com sucesso."
    };
}


ResultadoOperacao servicoListarRiscos(
    sqlite3 *db,
    std::vector<RiscoTalhaoDTO> &riscos
)
{
    riscos.clear();

    std::vector<TalhaoDTO> talhoes;

    ResultadoOperacao resultadoTalhoes =
        servicoListarTalhoes(
            db,
            talhoes
        );

    if (!resultadoTalhoes.sucesso) {
        return resultadoTalhoes;
    }

    for (
        const TalhaoDTO &talhao
        : talhoes
    ) {
        RiscoTalhaoDTO risco;

        ResultadoOperacao resultado =
            servicoCalcularRiscoTalhao(
                db,
                talhao.codigo,
                risco
            );

        if (!resultado.sucesso) {
            return resultado;
        }

        riscos.push_back(
            risco
        );
    }

    return {
        true,
        "Riscos consultados com sucesso."
    };
}