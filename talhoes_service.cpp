//Modulo responsavel pelas regras de negocio e acesso aos dados de talhoes sem utilizar printf ou scanf

#include "talhoes_service.h"

namespace {

//Verifica os dados obrigatorios recebidos pela camada de servicos antes de acessar o banco
ResultadoOperacao validarTalhao(
    const TalhaoDTO &talhao
)
{
    if (talhao.codigo <= 0) {
        return {
            false,
            "O codigo do talhao deve ser maior que zero."
        };
    }

    if (talhao.nome.empty()) {
        return {
            false,
            "O nome do talhao e obrigatorio."
        };
    }

    if (talhao.area <= 0) {
        return {
            false,
            "A area do talhao deve ser maior que zero."
        };
    }

    if (talhao.plantacao.empty()) {
        return {
            false,
            "A plantacao do talhao e obrigatoria."
        };
    }

    if (talhao.localizacao.empty()) {
        return {
            false,
            "A localizacao do talhao e obrigatoria."
        };
    }

    return {
        true,
        "Dados validos."
    };
}

}

//Cadastra um talhao utilizando dados recebidos de qualquer tipo de interface
ResultadoOperacao servicoCadastrarTalhao(
    sqlite3 *db,
    const TalhaoDTO &talhao
)
{
    ResultadoOperacao validacao =
        validarTalhao(talhao);

    if (!validacao.sucesso) {
        return validacao;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sqlBusca =
        "SELECT codigo "
        "FROM talhoes "
        "WHERE codigo = ?;";

    if (
        sqlite3_prepare_v2(
            db,
            sqlBusca,
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
        talhao.codigo
    );

    if (
        sqlite3_step(stmt)
        == SQLITE_ROW
    ) {
        sqlite3_finalize(stmt);

        return {
            false,
            "Ja existe um talhao com este codigo."
        };
    }

    sqlite3_finalize(stmt);

    const char *sql =
        "INSERT INTO talhoes "
        "(codigo, nome, area, plantacao, localizacao) "
        "VALUES (?, ?, ?, ?, ?);";

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
        talhao.codigo
    );

    sqlite3_bind_text(
        stmt,
        2,
        talhao.nome.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_double(
        stmt,
        3,
        talhao.area
    );

    sqlite3_bind_text(
        stmt,
        4,
        talhao.plantacao.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        5,
        talhao.localizacao.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Talhao cadastrado com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}

//Edita um talhao existente sem solicitar dados diretamente ao usuario
ResultadoOperacao servicoEditarTalhao(
    sqlite3 *db,
    const TalhaoDTO &talhao
)
{
    ResultadoOperacao validacao =
        validarTalhao(talhao);

    if (!validacao.sucesso) {
        return validacao;
    }

    TalhaoDTO existente;

    ResultadoOperacao busca =
        servicoBuscarTalhao(
            db,
            talhao.codigo,
            existente
        );

    if (!busca.sucesso) {
        return busca;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "UPDATE talhoes "
        "SET nome = ?, "
        "area = ?, "
        "plantacao = ?, "
        "localizacao = ? "
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
        talhao.nome.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_double(
        stmt,
        2,
        talhao.area
    );

    sqlite3_bind_text(
        stmt,
        3,
        talhao.plantacao.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        4,
        talhao.localizacao.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        5,
        talhao.codigo
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Talhao editado com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}

//Exclui um talhao desde que nao existam registros dependentes vinculados a ele
ResultadoOperacao servicoExcluirTalhao(
    sqlite3 *db,
    int codigo
)
{
    if (codigo <= 0) {
        return {
            false,
            "Codigo de talhao invalido."
        };
    }

    TalhaoDTO existente;

    ResultadoOperacao busca =
        servicoBuscarTalhao(
            db,
            codigo,
            existente
        );

    if (!busca.sucesso) {
        return busca;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sqlDependencias =
        "SELECT "
        "(SELECT COUNT(*) "
        "FROM clima "
        "WHERE codigo_talhao = ?) + "
        "(SELECT COUNT(*) "
        "FROM ocorrencias_pragas "
        "WHERE codigo_talhao = ?);";

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

    sqlite3_bind_int(
        stmt,
        2,
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
            "O talhao possui registros de clima ou ocorrencias de pragas vinculados."
        };
    }

    const char *sql =
        "DELETE FROM talhoes "
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
            ? "Talhao excluido com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}

//Busca um unico talhao e devolve seus dados para a interface que realizou a chamada
ResultadoOperacao servicoBuscarTalhao(
    sqlite3 *db,
    int codigo,
    TalhaoDTO &talhao
)
{
    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT codigo, "
        "nome, "
        "area, "
        "plantacao, "
        "localizacao "
        "FROM talhoes "
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
            "Talhao nao encontrado."
        };
    }

    talhao.codigo =
        sqlite3_column_int(
            stmt,
            0
        );

    talhao.nome =
        reinterpret_cast<const char *>(
            sqlite3_column_text(
                stmt,
                1
            )
        );

    talhao.area =
        sqlite3_column_double(
            stmt,
            2
        );

    talhao.plantacao =
        reinterpret_cast<const char *>(
            sqlite3_column_text(
                stmt,
                3
            )
        );

    talhao.localizacao =
        reinterpret_cast<const char *>(
            sqlite3_column_text(
                stmt,
                4
            )
        );

    sqlite3_finalize(stmt);

    return {
        true,
        "Talhao encontrado."
    };
}

//Lista todos os talhoes e devolve os registros em um vetor que pode ser utilizado pelo terminal ou pelo futuro front-end
ResultadoOperacao servicoListarTalhoes(
    sqlite3 *db,
    std::vector<TalhaoDTO> &talhoes
)
{
    talhoes.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT codigo, "
        "nome, "
        "area, "
        "plantacao, "
        "localizacao "
        "FROM talhoes "
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
        TalhaoDTO talhao;

        talhao.codigo =
            sqlite3_column_int(
                stmt,
                0
            );

        talhao.nome =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    1
                )
            );

        talhao.area =
            sqlite3_column_double(
                stmt,
                2
            );

        talhao.plantacao =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    3
                )
            );

        talhao.localizacao =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    4
                )
            );

        talhoes.push_back(
            talhao
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Talhoes consultados com sucesso."
    };
}