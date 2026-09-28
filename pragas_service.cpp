//Modulo responsavel pelas regras de negocio e acesso aos dados de pragas e ocorrencias sem utilizar printf ou scanf

#include "pragas_service.h"
#include "talhoes_service.h"

namespace {

ResultadoOperacao validarPraga(
    const PragaDTO &praga
)
{
    if (praga.codigo <= 0) {
        return {
            false,
            "O codigo da praga deve ser maior que zero."
        };
    }

    if (praga.nome.empty()) {
        return {
            false,
            "O nome da praga e obrigatorio."
        };
    }

    if (praga.descricao.empty()) {
        return {
            false,
            "A descricao da praga e obrigatoria."
        };
    }

    if (
        praga.nivelRisco < 1 ||
        praga.nivelRisco > 4
    ) {
        return {
            false,
            "O nivel de risco da praga deve estar entre 1 e 4."
        };
    }

    return {
        true,
        "Dados validos."
    };
}


ResultadoOperacao validarOcorrencia(
    sqlite3 *db,
    const OcorrenciaPragaDTO &ocorrencia
)
{
    if (ocorrencia.codigo <= 0) {
        return {
            false,
            "O codigo da ocorrencia deve ser maior que zero."
        };
    }

    PragaDTO praga;

    ResultadoOperacao buscaPraga =
        servicoBuscarPraga(
            db,
            ocorrencia.codigoPraga,
            praga
        );

    if (!buscaPraga.sucesso) {
        return {
            false,
            "A praga informada nao existe."
        };
    }

    TalhaoDTO talhao;

    ResultadoOperacao buscaTalhao =
        servicoBuscarTalhao(
            db,
            ocorrencia.codigoTalhao,
            talhao
        );

    if (!buscaTalhao.sucesso) {
        return {
            false,
            "O talhao informado nao existe."
        };
    }

    if (
        ocorrencia.nivelInfestacao < 1 ||
        ocorrencia.nivelInfestacao > 4
    ) {
        return {
            false,
            "O nivel de infestacao deve estar entre 1 e 4."
        };
    }

    if (
        ocorrencia.areaAfetada <= 0 ||
        ocorrencia.areaAfetada > talhao.area
    ) {
        return {
            false,
            "A area afetada deve ser maior que zero e nao pode ultrapassar a area total do talhao."
        };
    }

    if (ocorrencia.data.empty()) {
        return {
            false,
            "A data da ocorrencia e obrigatoria."
        };
    }

    return {
        true,
        "Dados validos."
    };
}

}


ResultadoOperacao servicoCadastrarPraga(
    sqlite3 *db,
    const PragaDTO &praga
)
{
    ResultadoOperacao validacao =
        validarPraga(praga);

    if (!validacao.sucesso) {
        return validacao;
    }

    PragaDTO existente;

    if (
        servicoBuscarPraga(
            db,
            praga.codigo,
            existente
        ).sucesso
    ) {
        return {
            false,
            "Ja existe uma praga com este codigo."
        };
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "INSERT INTO pragas "
        "(codigo, nome, descricao, nivel_risco) "
        "VALUES (?, ?, ?, ?);";

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
        praga.codigo
    );

    sqlite3_bind_text(
        stmt,
        2,
        praga.nome.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        3,
        praga.descricao.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        4,
        praga.nivelRisco
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Praga cadastrada com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoEditarPraga(
    sqlite3 *db,
    const PragaDTO &praga
)
{
    ResultadoOperacao validacao =
        validarPraga(praga);

    if (!validacao.sucesso) {
        return validacao;
    }

    PragaDTO existente;

    ResultadoOperacao busca =
        servicoBuscarPraga(
            db,
            praga.codigo,
            existente
        );

    if (!busca.sucesso) {
        return busca;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "UPDATE pragas "
        "SET nome = ?, "
        "descricao = ?, "
        "nivel_risco = ? "
        "WHERE codigo = ?;";

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

    sqlite3_bind_text(
        stmt,
        1,
        praga.nome.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        2,
        praga.descricao.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        3,
        praga.nivelRisco
    );

    sqlite3_bind_int(
        stmt,
        4,
        praga.codigo
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Praga editada com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoBuscarPraga(
    sqlite3 *db,
    int codigo,
    PragaDTO &praga
)
{
    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT codigo, "
        "nome, "
        "descricao, "
        "nivel_risco "
        "FROM pragas "
        "WHERE codigo = ?;";

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
        codigo
    );

    if (
        sqlite3_step(stmt)
        != SQLITE_ROW
    ) {
        sqlite3_finalize(stmt);

        return {
            false,
            "Praga nao encontrada."
        };
    }

    praga.codigo =
        sqlite3_column_int(
            stmt,
            0
        );

    praga.nome =
        reinterpret_cast<const char *>(
            sqlite3_column_text(
                stmt,
                1
            )
        );

    praga.descricao =
        reinterpret_cast<const char *>(
            sqlite3_column_text(
                stmt,
                2
            )
        );

    praga.nivelRisco =
        sqlite3_column_int(
            stmt,
            3
        );

    sqlite3_finalize(stmt);

    return {
        true,
        "Praga encontrada."
    };
}


ResultadoOperacao servicoListarPragas(
    sqlite3 *db,
    std::vector<PragaDTO> &pragas
)
{
    pragas.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT codigo, "
        "nome, "
        "descricao, "
        "nivel_risco "
        "FROM pragas "
        "ORDER BY codigo;";

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
        PragaDTO praga;

        praga.codigo =
            sqlite3_column_int(
                stmt,
                0
            );

        praga.nome =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    1
                )
            );

        praga.descricao =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    2
                )
            );

        praga.nivelRisco =
            sqlite3_column_int(
                stmt,
                3
            );

        pragas.push_back(
            praga
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Pragas consultadas com sucesso."
    };
}


ResultadoOperacao servicoExcluirPraga(
    sqlite3 *db,
    int codigo
)
{
    PragaDTO existente;

    ResultadoOperacao busca =
        servicoBuscarPraga(
            db,
            codigo,
            existente
        );

    if (!busca.sucesso) {
        return busca;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sqlDependencias =
        "SELECT COUNT(*) "
        "FROM ocorrencias_pragas "
        "WHERE codigo_praga = ?;";

    if (
        sqlite3_prepare_v2(
            db,
            sqlDependencias,
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
        codigo
    );

    int dependencias = 0;

    if (
        sqlite3_step(stmt)
        == SQLITE_ROW
    ) {
        dependencias =
            sqlite3_column_int(
                stmt,
                0
            );
    }

    sqlite3_finalize(stmt);

    if (dependencias > 0) {
        return {
            false,
            "A praga possui ocorrencias vinculadas e nao pode ser excluida."
        };
    }

    const char *sql =
        "DELETE FROM pragas "
        "WHERE codigo = ?;";

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
        codigo
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Praga excluida com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoCadastrarOcorrencia(
    sqlite3 *db,
    const OcorrenciaPragaDTO &ocorrencia
)
{
    ResultadoOperacao validacao =
        validarOcorrencia(
            db,
            ocorrencia
        );

    if (!validacao.sucesso) {
        return validacao;
    }

    OcorrenciaPragaDTO existente;

    if (
        servicoBuscarOcorrencia(
            db,
            ocorrencia.codigo,
            existente
        ).sucesso
    ) {
        return {
            false,
            "Ja existe uma ocorrencia com este codigo."
        };
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "INSERT INTO ocorrencias_pragas "
        "(codigo, codigo_praga, codigo_talhao, "
        "nivel_infestacao, area_afetada, data) "
        "VALUES (?, ?, ?, ?, ?, ?);";

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
        ocorrencia.codigo
    );

    sqlite3_bind_int(
        stmt,
        2,
        ocorrencia.codigoPraga
    );

    sqlite3_bind_int(
        stmt,
        3,
        ocorrencia.codigoTalhao
    );

    sqlite3_bind_int(
        stmt,
        4,
        ocorrencia.nivelInfestacao
    );

    sqlite3_bind_double(
        stmt,
        5,
        ocorrencia.areaAfetada
    );

    sqlite3_bind_text(
        stmt,
        6,
        ocorrencia.data.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Ocorrencia cadastrada com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoBuscarOcorrencia(
    sqlite3 *db,
    int codigo,
    OcorrenciaPragaDTO &ocorrencia
)
{
    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT codigo, "
        "codigo_praga, "
        "codigo_talhao, "
        "nivel_infestacao, "
        "area_afetada, "
        "data "
        "FROM ocorrencias_pragas "
        "WHERE codigo = ?;";

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
        codigo
    );

    if (
        sqlite3_step(stmt)
        != SQLITE_ROW
    ) {
        sqlite3_finalize(stmt);

        return {
            false,
            "Ocorrencia nao encontrada."
        };
    }

    ocorrencia.codigo =
        sqlite3_column_int(
            stmt,
            0
        );

    ocorrencia.codigoPraga =
        sqlite3_column_int(
            stmt,
            1
        );

    ocorrencia.codigoTalhao =
        sqlite3_column_int(
            stmt,
            2
        );

    ocorrencia.nivelInfestacao =
        sqlite3_column_int(
            stmt,
            3
        );

    ocorrencia.areaAfetada =
        sqlite3_column_double(
            stmt,
            4
        );

    ocorrencia.data =
        reinterpret_cast<const char *>(
            sqlite3_column_text(
                stmt,
                5
            )
        );

    sqlite3_finalize(stmt);

    return {
        true,
        "Ocorrencia encontrada."
    };
}


ResultadoOperacao servicoEditarOcorrencia(
    sqlite3 *db,
    const OcorrenciaPragaDTO &ocorrencia
)
{
    ResultadoOperacao validacao =
        validarOcorrencia(
            db,
            ocorrencia
        );

    if (!validacao.sucesso) {
        return validacao;
    }

    OcorrenciaPragaDTO existente;

    ResultadoOperacao busca =
        servicoBuscarOcorrencia(
            db,
            ocorrencia.codigo,
            existente
        );

    if (!busca.sucesso) {
        return busca;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "UPDATE ocorrencias_pragas "
        "SET codigo_praga = ?, "
        "codigo_talhao = ?, "
        "nivel_infestacao = ?, "
        "area_afetada = ?, "
        "data = ? "
        "WHERE codigo = ?;";

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
        ocorrencia.codigoPraga
    );

    sqlite3_bind_int(
        stmt,
        2,
        ocorrencia.codigoTalhao
    );

    sqlite3_bind_int(
        stmt,
        3,
        ocorrencia.nivelInfestacao
    );

    sqlite3_bind_double(
        stmt,
        4,
        ocorrencia.areaAfetada
    );

    sqlite3_bind_text(
        stmt,
        5,
        ocorrencia.data.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        6,
        ocorrencia.codigo
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Ocorrencia editada com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoExcluirOcorrencia(
    sqlite3 *db,
    int codigo
)
{
    OcorrenciaPragaDTO existente;

    ResultadoOperacao busca =
        servicoBuscarOcorrencia(
            db,
            codigo,
            existente
        );

    if (!busca.sucesso) {
        return busca;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "DELETE FROM ocorrencias_pragas "
        "WHERE codigo = ?;";

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
        codigo
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Ocorrencia excluida com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoListarOcorrencias(
    sqlite3 *db,
    std::vector<OcorrenciaPragaDTO> &ocorrencias
)
{
    ocorrencias.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT codigo, "
        "codigo_praga, "
        "codigo_talhao, "
        "nivel_infestacao, "
        "area_afetada, "
        "data "
        "FROM ocorrencias_pragas "
        "ORDER BY codigo;";

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
        OcorrenciaPragaDTO ocorrencia;

        ocorrencia.codigo =
            sqlite3_column_int(
                stmt,
                0
            );

        ocorrencia.codigoPraga =
            sqlite3_column_int(
                stmt,
                1
            );

        ocorrencia.codigoTalhao =
            sqlite3_column_int(
                stmt,
                2
            );

        ocorrencia.nivelInfestacao =
            sqlite3_column_int(
                stmt,
                3
            );

        ocorrencia.areaAfetada =
            sqlite3_column_double(
                stmt,
                4
            );

        ocorrencia.data =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    5
                )
            );

        ocorrencias.push_back(
            ocorrencia
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Ocorrencias consultadas com sucesso."
    };
}