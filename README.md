# Talhion v0.2

## Sobre o Projeto

O **Talhion** é um sistema de gestão agrícola desenvolvido em C++, com o objetivo de auxiliar no gerenciamento e monitoramento das principais informações relacionadas a uma propriedade agrícola.

O sistema permite cadastrar talhões, registrar pragas e suas ocorrências, armazenar informações climáticas, analisar níveis de risco, consultar históricos, gerar alertas, emitir relatórios e visualizar graficamente os talhões da propriedade.

Os dados principais do sistema são armazenados em um banco de dados **SQLite**. O projeto também utiliza arquivos **CSV** e **TXT** para exportação, leitura e geração de relatórios.

A visualização gráfica da propriedade é realizada utilizando a biblioteca **Raylib**.

A partir da versão 0.2, o projeto também passa a possuir uma camada de serviços independente da interface de terminal, preparando a arquitetura para a implementação futura de um front-end.

---

## Versão

**Versão atual:** 0.2

A versão 0.2 do Talhion introduz uma nova camada de serviços responsável por separar progressivamente as regras de negócio e o acesso ao banco de dados da interface utilizada pelo usuário.

O sistema de terminal existente continua disponível e funcional, preservando as funcionalidades desenvolvidas anteriormente.

Nesta versão foram introduzidos modelos de transferência de dados, retornos padronizados de operações e serviços independentes de `printf()` e `scanf()` para os módulos de:

- Talhões;
- Pragas;
- Ocorrências de pragas;
- Clima;
- Classificação de risco.

A nova arquitetura permite que futuras interfaces gráficas, aplicações desktop, APIs ou front-ends possam utilizar as funcionalidades do Talhion sem depender diretamente dos menus executados no terminal.

Os novos módulos introduzidos na versão 0.2 são:

- `modelos.h`;
- `resultado.h`;
- `talhoes_service.cpp`;
- `talhoes_service.h`;
- `pragas_service.cpp`;
- `pragas_service.h`;
- `clima_service.cpp`;
- `clima_service.h`;
- `risco_service.cpp`;
- `risco_service.h`.

O banco principal utilizado pelo sistema é:

```text
talhion.db
```

O executável gerado é:

```text
talhion.exe
```

O projeto utiliza C++17, SQLite e Raylib.

---

## Objetivo

O objetivo do Talhion é centralizar informações agrícolas e permitir o acompanhamento das condições dos talhões por meio do cruzamento de dados relacionados a:

- Talhões;
- Plantações;
- Ocorrências de pragas;
- Níveis de infestação;
- Temperatura;
- Umidade;
- Histórico de registros;
- Classificação de risco.

A partir dessas informações, o sistema calcula o nível de risco de cada talhão e permite identificar áreas que necessitam de maior atenção.

---

## Tecnologias Utilizadas

O projeto utiliza:

- C++17;
- SQLite;
- Biblioteca SQLite3;
- Raylib;
- G++;
- CMake;
- Visual Studio Code;
- MSYS2 UCRT64;
- Arquivos CSV;
- Arquivos TXT.

---

## Arquitetura do Sistema

O Talhion utiliza uma arquitetura modular, onde diferentes responsabilidades são separadas em arquivos específicos.

A arquitetura geral do sistema pode ser representada da seguinte forma:

```text
Usuário
   |
   v
Interface de Terminal
   |
   v
Camada de Serviços
   |
   v
Regras de Negócio
   |
   v
SQLite
```

Além da interface de terminal atual, a camada de serviços foi criada para permitir que, futuramente, uma nova interface possa utilizar diretamente as funcionalidades do sistema.

A estrutura futura poderá seguir o modelo:

```text
              TALHION

          Front-end futuro
                 |
                 v
        Camada de Serviços
                 |
                 v
        Regras de Negócio
                 |
                 v
              SQLite
```

A principal vantagem dessa estrutura é evitar que as regras do sistema dependam diretamente de funções de terminal como:

```cpp
printf()
scanf()
```

Dessa forma, o mesmo núcleo do Talhion poderá ser utilizado por diferentes interfaces.

---

## Camada de Serviços

A partir da versão 0.2, o Talhion possui uma camada de serviços responsável por receber dados, realizar validações, executar operações no banco de dados e retornar resultados para a interface.

Os serviços não utilizam diretamente comandos de entrada e saída do terminal.

As operações retornam uma estrutura padronizada:

```cpp
struct ResultadoOperacao {
    bool sucesso;
    std::string mensagem;
};
```

Isso permite que uma interface utilize:

```cpp
resultado.sucesso
```

para verificar se uma operação foi concluída corretamente e:

```cpp
resultado.mensagem
```

para apresentar informações ao usuário.

---

## Modelos de Dados

O arquivo:

```text
modelos.h
```

possui estruturas utilizadas para transportar os dados entre interfaces e serviços.

Os principais modelos são:

```text
TalhaoDTO
PragaDTO
OcorrenciaPragaDTO
RegistroClimaDTO
RiscoTalhaoDTO
```

Essas estruturas utilizam tipos do C++, como:

```cpp
std::string
```

permitindo reduzir a dependência de arrays de caracteres utilizados anteriormente em C.

---

## Estrutura do Projeto

A estrutura principal do projeto é:

```text
Talhion/
|
|-- main.cpp
|
|-- banco.cpp
|-- banco.h
|
|-- menu.cpp
|-- menu.h
|
|-- talhoes.cpp
|-- talhoes.h
|
|-- pragas.cpp
|-- pragas.h
|
|-- clima.cpp
|-- clima.h
|
|-- risco.cpp
|-- risco.h
|
|-- alertas.cpp
|-- alertas.h
|
|-- historico.cpp
|-- historico.h
|
|-- relatorios.cpp
|-- relatorios.h
|
|-- visualizacao.cpp
|-- visualizacao.h
|
|-- modelos.h
|-- resultado.h
|
|-- talhoes_service.cpp
|-- talhoes_service.h
|
|-- pragas_service.cpp
|-- pragas_service.h
|
|-- clima_service.cpp
|-- clima_service.h
|
|-- risco_service.cpp
|-- risco_service.h
|
|-- talhion.db
|
|-- talhoes.csv
|-- clima.csv
|-- pragas.csv
|-- ocorrencias_pragas.csv
|-- relatorio_fazenda.txt
|
|-- CMakeLists.txt
|-- README.md
|
`-- .vscode/
    |-- tasks.json
    |-- launch.json
    `-- c_cpp_properties.json
```

---

## main.cpp

O arquivo `main.cpp` representa o ponto inicial de execução do Talhion.

Ele é responsável por:

- Abrir o banco de dados;
- Inicializar as tabelas;
- Exibir o menu principal;
- Direcionar o usuário para os módulos do sistema;
- Encerrar corretamente a conexão com o banco de dados.

Na versão 0.2, o menu principal identifica o sistema como:

```text
TALHION v0.2
```

---

## banco.cpp / banco.h

Esses arquivos são responsáveis pela conexão e inicialização do banco de dados.

O banco utilizado pelo sistema é:

```text
talhion.db
```

As principais tabelas são:

- `talhoes`;
- `pragas`;
- `ocorrencias_pragas`;
- `clima`.

Também são configuradas chaves estrangeiras utilizadas para manter os relacionamentos entre os registros.

---

## menu.cpp / menu.h

Responsáveis pelos menus utilizados na interface de terminal.

Os menus permitem acessar as funcionalidades de:

- Talhões;
- Pragas e ocorrências;
- Clima;
- Classificação de risco;
- Relatórios;
- Histórico;
- Alertas;
- Visualização da propriedade.

O terminal será mantido durante a evolução da aplicação para permitir que o sistema continue funcional enquanto o novo front-end é desenvolvido.

---

## talhoes.cpp / talhoes.h

Responsáveis pelo gerenciamento dos talhões utilizando a interface tradicional do sistema.

Permitem:

- Cadastrar talhão;
- Listar talhões;
- Buscar talhão;
- Editar talhão;
- Excluir talhão.

Cada talhão possui:

- Código;
- Nome;
- Área;
- Tipo de plantação;
- Localização.

O sistema também realiza validações para evitar códigos duplicados e áreas inválidas.

---

## talhoes_service.cpp / talhoes_service.h

Responsáveis pelas operações de talhões independentes da interface de terminal.

As principais funções disponíveis são:

```cpp
servicoCadastrarTalhao()
servicoEditarTalhao()
servicoExcluirTalhao()
servicoBuscarTalhao()
servicoListarTalhoes()
```

Essas funções recebem os dados diretamente por parâmetros e estruturas C++, sem utilizar `scanf()`.

Exemplo:

```cpp
TalhaoDTO talhao;

talhao.codigo = 1;
talhao.nome = "Talhao Norte";
talhao.area = 25.5;
talhao.plantacao = "Soja";
talhao.localizacao = "Setor Norte";

ResultadoOperacao resultado =
    servicoCadastrarTalhao(
        db,
        talhao
    );
```

---

## pragas.cpp / pragas.h

Responsáveis pelo gerenciamento das pragas e suas ocorrências por meio da interface de terminal.

Permitem:

- Cadastrar pragas;
- Listar pragas;
- Buscar pragas;
- Editar pragas;
- Excluir pragas;
- Registrar ocorrências;
- Listar ocorrências;
- Editar ocorrências;
- Excluir ocorrências.

Cada praga possui um nível de risco entre 1 e 4.

As ocorrências registram informações como:

- Código;
- Praga;
- Talhão;
- Nível de infestação;
- Área afetada;
- Data.

Antes de registrar uma ocorrência, o sistema verifica se a praga e o talhão informados existem.

A área afetada também é validada para impedir que seja maior que a área total do talhão.

---

## pragas_service.cpp / pragas_service.h

Responsáveis pelas operações de pragas e ocorrências sem dependência da interface de terminal.

As principais funções são:

```cpp
servicoCadastrarPraga()
servicoEditarPraga()
servicoExcluirPraga()
servicoBuscarPraga()
servicoListarPragas()
```

Para ocorrências:

```cpp
servicoCadastrarOcorrencia()
servicoEditarOcorrencia()
servicoExcluirOcorrencia()
servicoBuscarOcorrencia()
servicoListarOcorrencias()
```

As validações de existência do talhão, existência da praga e área afetada são realizadas pela camada de serviços.

---

## clima.cpp / clima.h

Responsáveis pelo gerenciamento dos dados climáticos.

Permitem:

- Registrar dados climáticos;
- Listar histórico climático;
- Visualizar série climática de um talhão;
- Editar registros climáticos;
- Excluir registros climáticos.

Os registros armazenam:

- Talhão;
- Temperatura;
- Umidade;
- Data;
- Hora.

O módulo também possui funções utilizadas no cálculo dos fatores de risco relacionados à temperatura e à umidade.

---

## clima_service.cpp / clima_service.h

Responsáveis pelas operações climáticas independentes da interface de terminal.

As principais funções são:

```cpp
servicoRegistrarClima()
servicoEditarClima()
servicoExcluirClima()
servicoBuscarClima()
servicoListarClima()
servicoListarClimaPorTalhao()
```

A camada valida:

- Existência do talhão;
- Temperatura;
- Umidade;
- Data;
- Hora.

---

## risco.cpp / risco.h

Responsáveis pelo cálculo tradicional da classificação de risco dos talhões.

O sistema considera três fatores:

- Risco de temperatura;
- Risco de umidade;
- Risco de praga.

O risco final é calculado através da multiplicação:

```text
Risco Final =
Risco de Temperatura
×
Risco de Umidade
×
Risco de Praga
```
