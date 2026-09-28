//Modulo responsavel pela interface de terminal utilizada no gerenciamento dos talhoes

#include <iostream>
#include <limits>
#include <vector>

#include "talhoes.h"
#include "talhoes_service.h"
#include "modelos.h"
#include "resultado.h"

namespace {

//Limpa os dados restantes da entrada utilizada pelo terminal
void limparEntradaTalhao()
{
    std::cin.clear();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
}

}


//Solicita os dados do novo talhao ao usuario e encaminha o cadastro para a camada de servicos
void cadastrarTalhao(sqlite3 *db)
{
    TalhaoDTO talhao;

    std::cout << "\n====================================\n";
    std::cout << "        CADASTRO DE TALHAO\n";
    std::cout << "====================================\n";

    std::cout << "Codigo: ";

    if (!(std::cin >> talhao.codigo)) {

        limparEntradaTalhao();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    limparEntradaTalhao();

    std::cout << "Nome: ";
    std::getline(
        std::cin,
        talhao.nome
    );

    std::cout << "Area em hectares: ";

    if (!(std::cin >> talhao.area)) {

        limparEntradaTalhao();

        std::cout << "\nArea invalida.\n";
        return;
    }

    limparEntradaTalhao();

    std::cout << "Plantacao: ";
    std::getline(
        std::cin,
        talhao.plantacao
    );

    std::cout << "Localizacao: ";
    std::getline(
        std::cin,
        talhao.localizacao
    );

    ResultadoOperacao resultado =
        servicoCadastrarTalhao(
            db,
            talhao
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Consulta os talhoes atraves da camada de servicos e apresenta os registros no terminal
void listarTalhoes(sqlite3 *db)
{
    std::vector<TalhaoDTO> talhoes;

    ResultadoOperacao resultado =
        servicoListarTalhoes(
            db,
            talhoes
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "         TALHOES CADASTRADOS\n";
    std::cout << "====================================\n";

    if (talhoes.empty()) {

        std::cout
            << "\nNenhum talhao cadastrado.\n";

        return;
    }

    for (
        const TalhaoDTO &talhao
        : talhoes
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Codigo: "
            << talhao.codigo
            << "\n";

        std::cout
            << "Nome: "
            << talhao.nome
            << "\n";

        std::cout
            << "Area: "
            << talhao.area
            << " hectares\n";

        std::cout
            << "Plantacao: "
            << talhao.plantacao
            << "\n";

        std::cout
            << "Localizacao: "
            << talhao.localizacao
            << "\n";
    }

    std::cout << "-----------------------------\n";
}


//Busca um talhao utilizando a nova camada de servicos
int buscarTalhao(
    sqlite3 *db,
    int codigo
)
{
    TalhaoDTO talhao;

    ResultadoOperacao resultado =
        servicoBuscarTalhao(
            db,
            codigo,
            talhao
        );

    return resultado.sucesso
        ? 1
        : 0;
}


//Solicita os novos dados do talhao e encaminha a alteracao para a camada de servicos
void editarTalhao(sqlite3 *db)
{
    int codigo;

    std::cout << "\n====================================\n";
    std::cout << "          EDITAR TALHAO\n";
    std::cout << "====================================\n";

    std::cout << "Codigo do talhao: ";

    if (!(std::cin >> codigo)) {

        limparEntradaTalhao();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    limparEntradaTalhao();

    TalhaoDTO talhao;

    ResultadoOperacao busca =
        servicoBuscarTalhao(
            db,
            codigo,
            talhao
        );

    if (!busca.sucesso) {

        std::cout
            << "\n"
            << busca.mensagem
            << "\n";

        return;
    }

    std::cout << "\nDados atuais:\n";

    std::cout
        << "Nome: "
        << talhao.nome
        << "\n";

    std::cout
        << "Area: "
        << talhao.area
        << "\n";

    std::cout
        << "Plantacao: "
        << talhao.plantacao
        << "\n";

    std::cout
        << "Localizacao: "
        << talhao.localizacao
        << "\n";

    std::cout << "\nNovo nome: ";

    std::getline(
        std::cin,
        talhao.nome
    );

    std::cout << "Nova area em hectares: ";

    if (!(std::cin >> talhao.area)) {

        limparEntradaTalhao();

        std::cout << "\nArea invalida.\n";
        return;
    }

    limparEntradaTalhao();

    std::cout << "Nova plantacao: ";

    std::getline(
        std::cin,
        talhao.plantacao
    );

    std::cout << "Nova localizacao: ";

    std::getline(
        std::cin,
        talhao.localizacao
    );

    ResultadoOperacao resultado =
        servicoEditarTalhao(
            db,
            talhao
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Solicita o codigo e encaminha a exclusao do talhao para a camada de servicos
void excluirTalhao(sqlite3 *db)
{
    int codigo;

    std::cout << "\n====================================\n";
    std::cout << "         EXCLUIR TALHAO\n";
    std::cout << "====================================\n";

    std::cout << "Codigo do talhao: ";

    if (!(std::cin >> codigo)) {

        limparEntradaTalhao();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    limparEntradaTalhao();

    ResultadoOperacao resultado =
        servicoExcluirTalhao(
            db,
            codigo
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}