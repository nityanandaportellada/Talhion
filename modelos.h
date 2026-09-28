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

//Estrutura utilizada para representar um alerta gerado pelo Talhion
struct AlertaDTO {
    int codigoTalhao = 0;
    std::string nomeTalhao;
    int riscoTemperatura = 1;
    int riscoUmidade = 1;
    int riscoPraga = 1;
    int riscoFinal = 1;
    std::string classificacao;
    std::string mensagem;
};

//Estrutura utilizada para apresentar ocorrencias no historico
struct HistoricoOcorrenciaDTO {
    int codigoOcorrencia = 0;
    int codigoTalhao = 0;
    std::string nomeTalhao;
    std::string nomePraga;
    int nivelInfestacao = 0;
    double areaAfetada = 0.0;
    std::string data;
};

//Estrutura utilizada para acompanhar a evolucao do risco de um talhao
struct EvolucaoRiscoDTO {
    std::string data;

    bool possuiClima = false;

    double temperatura = 0.0;
    double umidade = 0.0;

    int riscoTemperatura = 1;
    int riscoUmidade = 1;
    int riscoPraga = 1;
    int riscoFinal = 1;

    std::string classificacao;
    std::string evolucao;
};

//Estrutura utilizada pelo relatorio climatico
struct RelatorioClimaDTO {
    int codigoTalhao = 0;
    std::string nomeTalhao;
    std::string data;
    std::string hora;
    double temperatura = 0.0;
    double umidade = 0.0;
};

//Estrutura utilizada pelo relatorio que relaciona pragas e clima
struct RelatorioPragaClimaDTO {
    int codigoTalhao = 0;
    std::string nomeTalhao;

    int codigoPraga = 0;
    std::string nomePraga;

    int nivelInfestacao = 0;
    double areaAfetada = 0.0;

    std::string dataOcorrencia;

    bool possuiClima = false;

    double temperatura = 0.0;
    double umidade = 0.0;

    std::string dataClima;
};

//Estrutura utilizada para disponibilizar o resumo de um periodo
struct ResumoPeriodoDTO {
    std::string dataInicial;
    std::string dataFinal;

    int quantidadeRegistros = 0;

    double temperaturaMedia = 0.0;
    double umidadeMedia = 0.0;

    double maiorTemperatura = 0.0;
    double menorTemperatura = 0.0;
};

#endif