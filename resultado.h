//Modulo de cabecalho responsavel por padronizar os retornos da camada de servicos do Talhion
#ifndef RESULTADO_H
#define RESULTADO_H

#include <string>

//Estrutura utilizada para informar se uma operacao foi concluida e disponibilizar uma mensagem para a interface
struct ResultadoOperacao {
    bool sucesso = false;
    std::string mensagem;
};

#endif