//Modulo responsavel pela interface de terminal utilizada no gerenciamento dos dados climaticos e pelos calculos de risco relacionados ao clima

#include <iostream>
#include <limits>
#include <vector>
#include <cstdio>

#include "clima.h"
#include "clima_service.h"
#include "talhoes_service.h"
#include "modelos.h"
#include "resultado.h"

namespace {

//Limpa os dados restantes da entrada utilizada pelo terminal
void limparEntradaClima()
{
    std::cin.clear();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
}

}


//Valida se a data informada possui um dia e mes compativeis com o calendario
int validarData(char data[])
{
    int dia;
    int mes;
    int ano;

    if (
        std::sscanf(
            data,
            "%d/%d/%d",
            &dia,
            &mes,
            &ano
        )
        != 3
    ) {
        return 0;
    }

    if (
        ano < 1900 ||
        ano > 2100
    ) {
        return 0;
    }

    if (
        mes < 1 ||
        mes > 12
    ) {
        return 0;
    }

    int diasMes[] = {
        31,
        28,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31
    };

    //Verifica se o ano e bissexto para permitir 29 dias no mes de fevereiro
    if (
        ano % 400 == 0 ||
        (
            ano % 4 == 0 &&
            ano % 100 != 0
        )
    ) {
        diasMes[1] = 29;
    }

    if (
        dia < 1 ||
        dia > diasMes[mes - 1]
    ) {
        return 0;
    }

    return 1;
}


//Valida se a hora informada esta no intervalo permitido
int validarHora(char hora[])
{
    int horas;
    int minutos;

    if (
        std::sscanf(
            hora,
            "%d:%d",
            &horas,
            &minutos
        )
        != 2
    ) {
        return 0;
    }

    if (
        horas < 0 ||
        horas > 23
    ) {
        return 0;
    }

    if (
        minutos < 0 ||
        minutos > 59
    ) {
        return 0;
    }

    return 1;
}


//Calcula o fator de risco associado a temperatura registrada
int calcularRiscoTemperatura(
    float temperatura
)
{
    if (
        temperatura >= 18 &&
        temperatura <= 26
    ) {
        return 1;
    }

    if (
        temperatura >= 15 &&
        temperatura < 18
    ) {
        return 2;
    }

    if (
        temperatura > 26 &&
        temperatura <= 30
    ) {
        return 2;
    }

    if (
        temperatura >= 10 &&
        temperatura < 15
    ) {
        return 3;
    }

    if (
        temperatura > 30 &&
        temperatura <= 35
    ) {
        return 3;
    }

    return 4;
}


//Calcula o fator de risco associado a umidade registrada
int calcularRiscoUmidade(
    float umidade
)
{
    if (
        umidade >= 50 &&
        umidade <= 70
    ) {
        return 1;
    }

    if (
        umidade >= 40 &&
        umidade < 50
    ) {
        return 2;
    }

    if (
        umidade > 70 &&
        umidade <= 80
    ) {
        return 2;
    }

    if (
        umidade >= 30 &&
        umidade < 40
    ) {
        return 3;
    }

    if (
        umidade > 80 &&
        umidade <= 90
    ) {
        return 3;
    }

    return 4;
}


//Calcula simultaneamente os fatores de temperatura e umidade
void calcularRiscosClimaticos(
    float temperatura,
    float umidade,
    int *riscoTemperatura,
    int *riscoUmidade
)
{
    *riscoTemperatura =
        calcularRiscoTemperatura(
            temperatura
        );

    *riscoUmidade =
        calcularRiscoUmidade(
            umidade
        );
}


//Solicita os dados climaticos e encaminha o registro para a camada de servicos
void registrarClima(sqlite3 *db)
{
    RegistroClimaDTO registro;

    std::cout << "\n====================================\n";
    std::cout << "       REGISTRO DE DADOS CLIMATICOS\n";
    std::cout << "====================================\n";

    std::cout << "Codigo do talhao: ";

    if (!(std::cin >> registro.codigoTalhao)) {

        limparEntradaClima();

        std::cout
            << "\nCodigo do talhao invalido.\n";

        return;
    }

    std::cout << "Temperatura em graus Celsius: ";

    if (!(std::cin >> registro.temperatura)) {

        limparEntradaClima();

        std::cout
            << "\nTemperatura invalida.\n";

        return;
    }

    std::cout << "Umidade em porcentagem: ";

    if (!(std::cin >> registro.umidade)) {

        limparEntradaClima();

        std::cout
            << "\nUmidade invalida.\n";

        return;
    }

    limparEntradaClima();

    std::cout << "Data DD/MM/AAAA: ";

    std::getline(
        std::cin,
        registro.data
    );

    std::cout << "Hora HH:MM: ";

    std::getline(
        std::cin,
        registro.hora
    );

    ResultadoOperacao resultado =
        servicoRegistrarClima(
            db,
            registro
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Consulta todos os registros climaticos atraves da camada de servicos
void listarHistoricoClima(sqlite3 *db)
{
    std::vector<RegistroClimaDTO> registros;

    ResultadoOperacao resultado =
        servicoListarClima(
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
    std::cout << "        HISTORICO CLIMATICO\n";
    std::cout << "====================================\n";

    if (registros.empty()) {

        std::cout
            << "\nNenhum registro climatico encontrado.\n";

        return;
    }

    for (
        const RegistroClimaDTO &registro
        : registros
    ) {

        TalhaoDTO talhao;

        servicoBuscarTalhao(
            db,
            registro.codigoTalhao,
            talhao
        );

        std::cout << "\n-----------------------------\n";

        std::cout
            << "ID: "
            << registro.id
            << "\n";

        std::cout
            << "Talhao: "
            << talhao.nome
            << " ("
            << registro.codigoTalhao
            << ")\n";

        std::cout
            << "Temperatura: "
            << registro.temperatura
            << " C\n";

        std::cout
            << "Umidade: "
            << registro.umidade
            << "%\n";

        std::cout
            << "Data: "
            << registro.data
            << "\n";

        std::cout
            << "Hora: "
            << registro.hora
            << "\n";
    }

    std::cout << "-----------------------------\n";
}


//Consulta a serie climatica de um talhao utilizando a camada de servicos
void visualizarSerieTalhao(sqlite3 *db)
{
    int codigoTalhao;

    std::cout << "\n====================================\n";
    std::cout << "       SERIE CLIMATICA DO TALHAO\n";
    std::cout << "====================================\n";

    std::cout << "Codigo do talhao: ";

    if (!(std::cin >> codigoTalhao)) {

        limparEntradaClima();

        std::cout
            << "\nCodigo invalido.\n";

        return;
    }

    limparEntradaClima();

    std::vector<RegistroClimaDTO> registros;

    ResultadoOperacao resultado =
        servicoListarClimaPorTalhao(
            db,
            codigoTalhao,
            registros
        );

    if (!resultado.sucesso) {

        std::cout
            << "\n"
            << resultado.mensagem
            << "\n";

        return;
    }

    if (registros.empty()) {

        std::cout
            << "\nNenhum registro climatico encontrado para este talhao.\n";

        return;
    }

    TalhaoDTO talhao;

    servicoBuscarTalhao(
        db,
        codigoTalhao,
        talhao
    );

    std::cout
        << "\nTalhao: "
        << talhao.nome
        << "\n";

    for (
        const RegistroClimaDTO &registro
        : registros
    ) {

        std::cout << "\n-----------------------------\n";

        std::cout
            << "ID: "
            << registro.id
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


//Solicita novos dados para um registro climatico e utiliza a camada de servicos para realizar a alteracao
void editarClima(sqlite3 *db)
{
    int id;

    std::cout << "\n====================================\n";
    std::cout << "      EDITAR REGISTRO CLIMATICO\n";
    std::cout << "====================================\n";

    std::cout << "ID do registro: ";

    if (!(std::cin >> id)) {

        limparEntradaClima();

        std::cout << "\nID invalido.\n";
        return;
    }

    limparEntradaClima();

    RegistroClimaDTO registro;

    ResultadoOperacao busca =
        servicoBuscarClima(
            db,
            id,
            registro
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
        << "Talhao: "
        << registro.codigoTalhao
        << "\n";

    std::cout
        << "Temperatura: "
        << registro.temperatura
        << "\n";

    std::cout
        << "Umidade: "
        << registro.umidade
        << "\n";

    std::cout
        << "Data: "
        << registro.data
        << "\n";

    std::cout
        << "Hora: "
        << registro.hora
        << "\n";

    std::cout << "\nNovo codigo do talhao: ";

    if (!(std::cin >> registro.codigoTalhao)) {

        limparEntradaClima();
        return;
    }

    std::cout << "Nova temperatura: ";

    if (!(std::cin >> registro.temperatura)) {

        limparEntradaClima();
        return;
    }

    std::cout << "Nova umidade: ";

    if (!(std::cin >> registro.umidade)) {

        limparEntradaClima();
        return;
    }

    limparEntradaClima();

    std::cout << "Nova data DD/MM/AAAA: ";

    std::getline(
        std::cin,
        registro.data
    );

    std::cout << "Nova hora HH:MM: ";

    std::getline(
        std::cin,
        registro.hora
    );

    ResultadoOperacao resultado =
        servicoEditarClima(
            db,
            registro
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}


//Solicita o identificador e encaminha a exclusao do registro climatico para a camada de servicos
void excluirClima(sqlite3 *db)
{
    int id;

    std::cout << "\n====================================\n";
    std::cout << "      EXCLUIR REGISTRO CLIMATICO\n";
    std::cout << "====================================\n";

    std::cout << "ID do registro: ";

    if (!(std::cin >> id)) {

        limparEntradaClima();

        std::cout << "\nID invalido.\n";
        return;
    }

    limparEntradaClima();

    ResultadoOperacao resultado =
        servicoExcluirClima(
            db,
            id
        );

    std::cout
        << "\n"
        << resultado.mensagem
        << "\n";
}