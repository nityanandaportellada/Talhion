//Modulo de cabecalho responsavel por definir os modelos de dados utilizados pela camada de servicos do Talhion
#ifndef MODELOS_H
#define MODELOS_H

#include <string>

//Estrutura utilizada para transportar os dados de um talhao sem dependencia da interface de terminal
struct TalhaoDTO {
    int codigo = 0;
    std::string nome;
    double area = 0.0;
    std::string plantacao;
    std::string localizacao;
};

//Estrutura utilizada para transportar os dados de uma praga sem dependencia da interface de terminal
struct PragaDTO {
    int codigo = 0;
    std::string nome;
    std::string descricao;
    int nivelRisco = 1;
};

//Estrutura utilizada para transportar os dados de uma ocorrencia de praga
struct OcorrenciaPragaDTO {
    int codigo = 0;
    int codigoPraga = 0;
    int codigoTalhao = 0;
    int nivelInfestacao = 1;
    double areaAfetada = 0.0;
    std::string data;
};

//Estrutura utilizada para transportar os dados de um registro climatico
struct RegistroClimaDTO {
    int id = 0;
    int codigoTalhao = 0;
    double temperatura = 0.0;
    double umidade = 0.0;
    std::string data;
    std::string hora;
};

//Estrutura utilizada para disponibilizar o resultado calculado de risco de um talhao
struct RiscoTalhaoDTO {
    int codigoTalhao = 0;
    std::string nomeTalhao;
    int riscoTemperatura = 1;
    int riscoUmidade = 1;
    int riscoPraga = 1;
    int riscoFinal = 1;
    std::string classificacao;
};

#endif