//Modulo responsavel pela geracao dos alertas sem utilizar funcoes de entrada ou saida da interface

#include "alertas_service.h"
#include "risco_service.h"


//Consulta os riscos calculados e gera alertas somente para talhoes classificados com risco alto ou muito alto
ResultadoOperacao servicoListarAlertas(
    sqlite3 *db,
    std::vector<AlertaDTO> &alertas
)
{
    alertas.clear();

    std::vector<RiscoTalhaoDTO> riscos;

    ResultadoOperacao resultado =
        servicoListarRiscos(
            db,
            riscos
        );

    if (!resultado.sucesso) {
        return resultado;
    }

    for (
        const RiscoTalhaoDTO &risco
        : riscos
    ) {

        //Mantem a regra original do Talhion que gera alertas a partir do risco final 17
        if (risco.riscoFinal < 17) {
            continue;
        }

        AlertaDTO alerta;

        alerta.codigoTalhao =
            risco.codigoTalhao;

        alerta.nomeTalhao =
            risco.nomeTalhao;

        alerta.riscoTemperatura =
            risco.riscoTemperatura;

        alerta.riscoUmidade =
            risco.riscoUmidade;

        alerta.riscoPraga =
            risco.riscoPraga;

        alerta.riscoFinal =
            risco.riscoFinal;

        alerta.classificacao =
            risco.classificacao;

        if (risco.riscoFinal <= 36) {

            alerta.mensagem =
                "ALERTA: Talhao apresenta risco alto.";
        }
        else {

            alerta.mensagem =
                "ALERTA CRITICO: Talhao apresenta risco muito alto.";
        }

        alertas.push_back(
            alerta
        );
    }

    return {
        true,
        "Alertas consultados com sucesso."
    };
}