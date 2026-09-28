//Modulo responsavel pelas regras de negocio e acesso aos dados climaticos sem utilizar printf ou scanf

#include "clima_service.h"
#include "clima.h"
#include "talhoes_service.h"

namespace {

ResultadoOperacao validarRegistroClima(
    sqlite3 *db,
    const RegistroClimaDTO &registro
)
{
    TalhaoDTO talhao;

    if (
        !servicoBuscarTalhao(
            db,
            registro.codigoTalhao,
            talhao
        ).sucesso
    ) {
        return {
            false,
            "O talhao informado nao existe."
        };
    }

    if (
        registro.temperatura < -20 ||
        registro.temperatura > 50
    ) {
        return {
            false,
            "A temperatura deve estar entre -20 e 50 graus Celsius."
        };
    }

    if (
        registro.umidade < 0 ||
        registro.umidade > 100
    ) {
        return {
            false,
            "A umidade deve estar entre 0 e 100 por cento."
        };
    }

    if (
        !validarData(
            const_cast<char *>(
                registro.data.c_str()
            )
        )
    ) {
        return {
            false,
            "Data invalida. Utilize o formato DD/MM/AAAA."
        };
    }

    if (
        !validarHora(
            const_cast<char *>(
                registro.hora.c_str()
            )
        )
    ) {
        return {
            false,
            "Hora invalida. Utilize o formato HH:MM."
        };
    }

    return {
        true,
        "Dados validos."
    };
}

}


ResultadoOperacao servicoRegistrarClima(
    sqlite3 *db,
    const RegistroClimaDTO &registro
)
{
    ResultadoOperacao validacao =
        validarRegistroClima(
            db,
            registro
        );

    if (!validacao.sucesso) {
        return validacao;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "INSERT INTO clima "
        "(codigo_talhao, temperatura, umidade, data, hora) "
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
        registro.codigoTalhao
    );

    sqlite3_bind_double(
        stmt,
        2,
        registro.temperatura
    );

    sqlite3_bind_double(
        stmt,
        3,
        registro.umidade
    );

    sqlite3_bind_text(
        stmt,
        4,
        registro.data.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        5,
        registro.hora.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Registro climatico cadastrado com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoBuscarClima(
    sqlite3 *db,
    int id,
    RegistroClimaDTO &registro
)
{
    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT id, "
        "codigo_talhao, "
        "temperatura, "
        "umidade, "
        "data, "
        "hora "
        "FROM clima "
        "WHERE id = ?;";

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
        id
    );

    if (
        sqlite3_step(stmt)
        != SQLITE_ROW
    ) {
        sqlite3_finalize(stmt);

        return {
            false,
            "Registro climatico nao encontrado."
        };
    }

    registro.id =
        sqlite3_column_int(
            stmt,
            0
        );

    registro.codigoTalhao =
        sqlite3_column_int(
            stmt,
            1
        );

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

    registro.data =
        reinterpret_cast<const char *>(
            sqlite3_column_text(
                stmt,
                4
            )
        );

    registro.hora =
        reinterpret_cast<const char *>(
            sqlite3_column_text(
                stmt,
                5
            )
        );

    sqlite3_finalize(stmt);

    return {
        true,
        "Registro climatico encontrado."
    };
}


ResultadoOperacao servicoEditarClima(
    sqlite3 *db,
    const RegistroClimaDTO &registro
)
{
    if (registro.id <= 0) {
        return {
            false,
            "ID do registro climatico invalido."
        };
    }

    ResultadoOperacao validacao =
        validarRegistroClima(
            db,
            registro
        );

    if (!validacao.sucesso) {
        return validacao;
    }

    RegistroClimaDTO existente;

    ResultadoOperacao busca =
        servicoBuscarClima(
            db,
            registro.id,
            existente
        );

    if (!busca.sucesso) {
        return busca;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "UPDATE clima "
        "SET codigo_talhao = ?, "
        "temperatura = ?, "
        "umidade = ?, "
        "data = ?, "
        "hora = ? "
        "WHERE id = ?;";

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
        registro.codigoTalhao
    );

    sqlite3_bind_double(
        stmt,
        2,
        registro.temperatura
    );

    sqlite3_bind_double(
        stmt,
        3,
        registro.umidade
    );

    sqlite3_bind_text(
        stmt,
        4,
        registro.data.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        5,
        registro.hora.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        6,
        registro.id
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Registro climatico editado com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoExcluirClima(
    sqlite3 *db,
    int id
)
{
    RegistroClimaDTO existente;

    ResultadoOperacao busca =
        servicoBuscarClima(
            db,
            id,
            existente
        );

    if (!busca.sucesso) {
        return busca;
    }

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "DELETE FROM clima "
        "WHERE id = ?;";

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
        id
    );

    bool sucesso =
        sqlite3_step(stmt)
        == SQLITE_DONE;

    std::string mensagem =
        sucesso
            ? "Registro climatico excluido com sucesso."
            : sqlite3_errmsg(db);

    sqlite3_finalize(stmt);

    return {
        sucesso,
        mensagem
    };
}


ResultadoOperacao servicoListarClima(
    sqlite3 *db,
    std::vector<RegistroClimaDTO> &registros
)
{
    registros.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT id, "
        "codigo_talhao, "
        "temperatura, "
        "umidade, "
        "data, "
        "hora "
        "FROM clima "
        "ORDER BY id;";

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
        RegistroClimaDTO registro;

        registro.id =
            sqlite3_column_int(
                stmt,
                0
            );

        registro.codigoTalhao =
            sqlite3_column_int(
                stmt,
                1
            );

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

        registro.data =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    4
                )
            );

        registro.hora =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    5
                )
            );

        registros.push_back(
            registro
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Registros climaticos consultados com sucesso."
    };
}


ResultadoOperacao servicoListarClimaPorTalhao(
    sqlite3 *db,
    int codigoTalhao,
    std::vector<RegistroClimaDTO> &registros
)
{
    TalhaoDTO talhao;

    if (
        !servicoBuscarTalhao(
            db,
            codigoTalhao,
            talhao
        ).sucesso
    ) {
        return {
            false,
            "Talhao nao encontrado."
        };
    }

    registros.clear();

    sqlite3_stmt *stmt = nullptr;

    const char *sql =
        "SELECT id, "
        "codigo_talhao, "
        "temperatura, "
        "umidade, "
        "data, "
        "hora "
        "FROM clima "
        "WHERE codigo_talhao = ? "
        "ORDER BY "
        "substr(data, 7, 4) || '-' || "
        "substr(data, 4, 2) || '-' || "
        "substr(data, 1, 2), "
        "hora;";

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
        RegistroClimaDTO registro;

        registro.id =
            sqlite3_column_int(
                stmt,
                0
            );

        registro.codigoTalhao =
            sqlite3_column_int(
                stmt,
                1
            );

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

        registro.data =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    4
                )
            );

        registro.hora =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    stmt,
                    5
                )
            );

        registros.push_back(
            registro
        );
    }

    sqlite3_finalize(stmt);

    return {
        true,
        "Serie climatica consultada com sucesso."
    };
}