//Modulo responsavel pela interface de terminal utilizada no gerenciamento das pragas e suas ocorrencias

#include <iostream>
#include <limits>
#include <vector>

#include "pragas.h"
#include "pragas_service.h"
#include "talhoes_service.h"
#include "modelos.h"
#include "resultado.h"

namespace {

//Limpa os dados restantes da entrada utilizada pelo terminal
void limparEntradaPraga()
{
    std::cin.clear();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
}

}


//Consulta as pragas atraves da camada de servicos e apresenta os resultados no terminal
void listarPragas(sqlite3 *db)
{
    std::vector<PragaDTO> pragas;

    ResultadoOperacao resultado =
        servicoListarPragas(
            db,
            pragas
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "          PRAGAS CADASTRADAS\n";
    std::cout << "====================================\n";

    if (pragas.empty()) {

        std::cout
            << "\nNenhuma praga cadastrada.\n";

        return;
    }

    for (
        const PragaDTO &praga
        : pragas
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Codigo: "
            << praga.codigo
            << "\n";

        std::cout
            << "Nome: "
            << praga.nome
            << "\n";

        std::cout
            << "Descricao: "
            << praga.descricao
            << "\n";

        std::cout
            << "Nivel de risco: "
            << praga.nivelRisco
            << "\n";
    }

    std::cout << "-----------------------------\n";
}


//Solicita os dados de uma nova praga e utiliza a camada de servicos para realizar o cadastro
void cadastrarPraga(sqlite3 *db)
{
    PragaDTO praga;

    std::cout << "\n====================================\n";
    std::cout << "          CADASTRO DE PRAGA\n";
    std::cout << "====================================\n";

    std::cout << "Codigo: ";

    if (!(std::cin >> praga.codigo)) {

        limparEntradaPraga();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    limparEntradaPraga();

    std::cout << "Nome: ";

    std::getline(
        std::cin,
        praga.nome
    );

    std::cout << "Descricao: ";

    std::getline(
        std::cin,
        praga.descricao
    );

    std::cout << "Nivel de risco entre 1 e 4: ";

    if (!(std::cin >> praga.nivelRisco)) {

        limparEntradaPraga();

        std::cout
            << "\nNivel de risco invalido.\n";

        return;
    }

    limparEntradaPraga();

    ResultadoOperacao resultado =
        servicoCadastrarPraga(
            db,
            praga
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Verifica se uma praga existe utilizando a camada de servicos
int buscarPraga(
    sqlite3 *db,
    int codigo
)
{
    PragaDTO praga;

    ResultadoOperacao resultado =
        servicoBuscarPraga(
            db,
            codigo,
            praga
        );

    return resultado.sucesso
        ? 1
        : 0;
}


//Solicita os novos dados da praga e encaminha a alteracao para a camada de servicos
void editarPraga(sqlite3 *db)
{
    int codigo;

    std::cout << "\n====================================\n";
    std::cout << "           EDITAR PRAGA\n";
    std::cout << "====================================\n";

    std::cout << "Codigo da praga: ";

    if (!(std::cin >> codigo)) {

        limparEntradaPraga();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    limparEntradaPraga();

    PragaDTO praga;

    ResultadoOperacao busca =
        servicoBuscarPraga(
            db,
            codigo,
            praga
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
        << praga.nome
        << "\n";

    std::cout
        << "Descricao: "
        << praga.descricao
        << "\n";

    std::cout
        << "Nivel de risco: "
        << praga.nivelRisco
        << "\n";

    std::cout << "\nNovo nome: ";

    std::getline(
        std::cin,
        praga.nome
    );

    std::cout << "Nova descricao: ";

    std::getline(
        std::cin,
        praga.descricao
    );

    std::cout << "Novo nivel de risco entre 1 e 4: ";

    if (!(std::cin >> praga.nivelRisco)) {

        limparEntradaPraga();

        std::cout
            << "\nNivel de risco invalido.\n";

        return;
    }

    limparEntradaPraga();

    ResultadoOperacao resultado =
        servicoEditarPraga(
            db,
            praga
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Solicita o codigo e encaminha a exclusao da praga para a camada de servicos
void excluirPraga(sqlite3 *db)
{
    int codigo;

    std::cout << "\n====================================\n";
    std::cout << "          EXCLUIR PRAGA\n";
    std::cout << "====================================\n";

    std::cout << "Codigo da praga: ";

    if (!(std::cin >> codigo)) {

        limparEntradaPraga();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    limparEntradaPraga();

    ResultadoOperacao resultado =
        servicoExcluirPraga(
            db,
            codigo
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Solicita os dados de uma nova ocorrencia e utiliza a camada de servicos para efetuar o cadastro
void cadastrarOcorrencia(sqlite3 *db)
{
    OcorrenciaPragaDTO ocorrencia;

    std::cout << "\n====================================\n";
    std::cout << "       CADASTRO DE OCORRENCIA\n";
    std::cout << "====================================\n";

    std::cout << "Codigo da ocorrencia: ";

    if (!(std::cin >> ocorrencia.codigo)) {

        limparEntradaPraga();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    std::cout << "Codigo da praga: ";

    if (!(std::cin >> ocorrencia.codigoPraga)) {

        limparEntradaPraga();

        std::cout
            << "\nCodigo da praga invalido.\n";

        return;
    }

    std::cout << "Codigo do talhao: ";

    if (!(std::cin >> ocorrencia.codigoTalhao)) {

        limparEntradaPraga();

        std::cout
            << "\nCodigo do talhao invalido.\n";

        return;
    }

    std::cout << "Nivel de infestacao entre 1 e 4: ";

    if (!(std::cin >> ocorrencia.nivelInfestacao)) {

        limparEntradaPraga();

        std::cout
            << "\nNivel de infestacao invalido.\n";

        return;
    }

    std::cout << "Area afetada em hectares: ";

    if (!(std::cin >> ocorrencia.areaAfetada)) {

        limparEntradaPraga();

        std::cout
            << "\nArea afetada invalida.\n";

        return;
    }

    limparEntradaPraga();

    std::cout << "Data no formato DD/MM/AAAA: ";

    std::getline(
        std::cin,
        ocorrencia.data
    );

    ResultadoOperacao resultado =
        servicoCadastrarOcorrencia(
            db,
            ocorrencia
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Consulta as ocorrencias atraves da camada de servicos e apresenta os registros no terminal
void listarOcorrencias(sqlite3 *db)
{
    std::vector<OcorrenciaPragaDTO> ocorrencias;

    ResultadoOperacao resultado =
        servicoListarOcorrencias(
            db,
            ocorrencias
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "       OCORRENCIAS DE PRAGAS\n";
    std::cout << "====================================\n";

    if (ocorrencias.empty()) {

        std::cout
            << "\nNenhuma ocorrencia cadastrada.\n";

        return;
    }

    for (
        const OcorrenciaPragaDTO &ocorrencia
        : ocorrencias
    ) {

        PragaDTO praga;
        TalhaoDTO talhao;

        servicoBuscarPraga(
            db,
            ocorrencia.codigoPraga,
            praga
        );

        servicoBuscarTalhao(
            db,
            ocorrencia.codigoTalhao,
            talhao
        );

        std::cout << "\n-----------------------------\n";

        std::cout
            << "Codigo: "
            << ocorrencia.codigo
            << "\n";

        std::cout
            << "Praga: "
            << praga.nome
            << " ("
            << ocorrencia.codigoPraga
            << ")\n";

        std::cout
            << "Talhao: "
            << talhao.nome
            << " ("
            << ocorrencia.codigoTalhao
            << ")\n";

        std::cout
            << "Nivel de infestacao: "
            << ocorrencia.nivelInfestacao
            << "\n";

        std::cout
            << "Area afetada: "
            << ocorrencia.areaAfetada
            << " hectares\n";

        std::cout
            << "Data: "
            << ocorrencia.data
            << "\n";
    }

    std::cout << "-----------------------------\n";
}


//Verifica se determinada ocorrencia existe utilizando a camada de servicos
int buscarOcorrencia(
    sqlite3 *db,
    int codigo
)
{
    OcorrenciaPragaDTO ocorrencia;

    ResultadoOperacao resultado =
        servicoBuscarOcorrencia(
            db,
            codigo,
            ocorrencia
        );

    return resultado.sucesso
        ? 1
        : 0;
}


//Solicita os novos dados e encaminha a alteracao da ocorrencia para a camada de servicos
void editarOcorrencia(sqlite3 *db)
{
    int codigo;

    std::cout << "\n====================================\n";
    std::cout << "        EDITAR OCORRENCIA\n";
    std::cout << "====================================\n";

    std::cout << "Codigo da ocorrencia: ";

    if (!(std::cin >> codigo)) {

        limparEntradaPraga();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    limparEntradaPraga();

    OcorrenciaPragaDTO ocorrencia;

    ResultadoOperacao busca =
        servicoBuscarOcorrencia(
            db,
            codigo,
            ocorrencia
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
        << "Praga: "
        << ocorrencia.codigoPraga
        << "\n";

    std::cout
        << "Talhao: "
        << ocorrencia.codigoTalhao
        << "\n";

    std::cout
        << "Nivel de infestacao: "
        << ocorrencia.nivelInfestacao
        << "\n";

    std::cout
        << "Area afetada: "
        << ocorrencia.areaAfetada
        << "\n";

    std::cout
        << "Data: "
        << ocorrencia.data
        << "\n";

    std::cout << "\nNovo codigo da praga: ";

    if (!(std::cin >> ocorrencia.codigoPraga)) {

        limparEntradaPraga();
        return;
    }

    std::cout << "Novo codigo do talhao: ";

    if (!(std::cin >> ocorrencia.codigoTalhao)) {

        limparEntradaPraga();
        return;
    }

    std::cout << "Novo nivel de infestacao: ";

    if (!(std::cin >> ocorrencia.nivelInfestacao)) {

        limparEntradaPraga();
        return;
    }

    std::cout << "Nova area afetada: ";

    if (!(std::cin >> ocorrencia.areaAfetada)) {

        limparEntradaPraga();
        return;
    }

    limparEntradaPraga();

    std::cout << "Nova data DD/MM/AAAA: ";

    std::getline(
        std::cin,
        ocorrencia.data
    );

    ResultadoOperacao resultado =
        servicoEditarOcorrencia(
            db,
            ocorrencia
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Solicita o codigo e encaminha a exclusao da ocorrencia para a camada de servicos
void excluirOcorrencia(sqlite3 *db)
{
    int codigo;

    std::cout << "\n====================================\n";
    std::cout << "       EXCLUIR OCORRENCIA\n";
    std::cout << "====================================\n";

    std::cout << "Codigo da ocorrencia: ";

    if (!(std::cin >> codigo)) {

        limparEntradaPraga();

        std::cout << "\nCodigo invalido.\n";
        return;
    }

    limparEntradaPraga();

    ResultadoOperacao resultado =
        servicoExcluirOcorrencia(
            db,
            codigo
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}