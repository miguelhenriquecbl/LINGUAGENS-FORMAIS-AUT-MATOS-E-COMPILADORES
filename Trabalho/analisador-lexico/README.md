# Analisador Léxico - MicroPascal (µ-Pascal)

Trabalho prático da disciplina **Linguagens Formais, Autômatos e Compiladores**  
Universidade Católica de Brasília — 1º semestre de 2026  
Professor: Marcelo Eustáquio

---

## Descrição

Implementação de um analisador léxico para a linguagem **MicroPascal (µ-Pascal)**, desenvolvido em linguagem C. O analisador é baseado em um **Autômato Finito Determinístico (AFD)** e é capaz de:

- Reconhecer os tokens da linguagem
- Identificar e reportar erros léxicos
- Manter uma Tabela de Símbolos (TS)
- Registrar a posição (linha e coluna) de cada token
- Gerar arquivos de saída `.lex`, `.ts` e `.err`

---

## Estrutura do Projeto

```
projeto/
├── main.c                            ← ponto de entrada, abre arquivos e executa o analisador
├── lexer.h                           ← declarações das funções do analisador léxico
├── lexer.c                           ← implementação do analisador léxico (AFD)
├── token.h                           ← enum TokenType, struct Token e MapaToken
├── symtable.h                        ← declarações da Tabela de Símbolos
├── symtable.c                        ← implementação da Tabela de Símbolos
├── README.md                         ← este arquivo
├── analisador-lexico.exe             ← executável gerado pelo compilador
│
├── AFDs/                             ← diagramas do AFD
│   ├── codigo/                       ← código fonte Graphviz (.dot)
│   │   ├── afd_completo.dot
│   │   ├── afd_tokens_simples.dot
│   │   ├── afd_comentario.dot
│   │   ├── afd_operador_atribuicao.dot
│   │   ├── afd_operador_menor.dot
│   │   ├── afd_operador_maior.dot
│   │   ├── afd_numeros_e_sinais.dot
│   │   ├── afd_identificadores.dot
│   │   ├── afd_fim_arquivo_eof.dot
│   │   └── afd_erro_lexico.dot
│   ├── png/                          ← imagens PNG geradas
│   │   ├── afd_completo.png
│   │   ├── afd_tokens_simples.png
│   │   ├── afd_comentario.png
│   │   ├── afd_operador_atribuicao.png
│   │   ├── afd_operador_menor.png
│   │   ├── afd_operador_maior.png
│   │   ├── afd_numeros_e_sinais.png
│   │   ├── afd_identificadores.png
│   │   ├── afd_fim_arquivo_eof.png
│   │   └── afd_erro_lexico.png
│   └── svg/                          ← imagens SVG geradas (vetorial)
│       ├── afd_completo.svg
│       ├── afd_tokens_simples.svg
│       ├── afd_comentario.svg
│       ├── afd_operador_atribuicao.svg
│       ├── afd_operador_menor.svg
│       ├── afd_operador_maior.svg
│       ├── afd_numeros_e_sinais.svg
│       ├── afd_identificadores.svg
│       ├── afd_fim_arquivo_eof.svg
│       └── afd_erro_lexico.svg
│
├── codigo-teste-pascal/              ← programas de teste em MicroPascal
│   ├── teste001.pas                  ← correto — exemplo do professor com sinais e E
│   ├── teste002.pas                  ← correto — operadores relacionais com sinais
│   ├── teste003.pas                  ← correto — comentários com sinais e E
│   ├── teste004.pas                  ← correto — case-insensitive com E e e
│   ├── teste005.pas                  ← erro — caractere inválido '@' com sinais
│   ├── teste006.pas                  ← erro — comentário não fechado
│   ├── teste007.pas                  ← erro — E sem dígito depois
│   └── teste008.pas                  ← erro — múltiplos erros com novas regras
│
├── exemplos-saida/                   ← saídas pré-geradas de cada teste
│   ├── lex/
│   │   ├── saidaLex-teste001.lex
│   │   ├── saidaLex-teste002.lex
│   │   ├── saidaLex-teste003.lex
│   │   ├── saidaLex-teste004.lex
│   │   ├── saidaLex-teste005.lex
│   │   ├── saidaLex-teste006.lex
│   │   ├── saidaLex-teste007.lex
│   │   └── saidaLex-teste008.lex
│   ├── ts/
│   │   ├── saidaTS-teste001.ts
│   │   ├── saidaTS-teste002.ts
│   │   ├── saidaTS-teste003.ts
│   │   ├── saidaTS-teste004.ts
│   │   ├── saidaTS-teste005.ts
│   │   ├── saidaTS-teste006.ts
│   │   ├── saidaTS-teste007.ts
│   │   └── saidaTS-teste008.ts
│   └── err/
│       ├── saidaErro-teste001.err
│       ├── saidaErro-teste002.err
│       ├── saidaErro-teste003.err
│       ├── saidaErro-teste004.err
│       ├── saidaErro-teste005.err
│       ├── saidaErro-teste006.err
│       ├── saidaErro-teste007.err
│       └── saidaErro-teste008.err
│
└── resultado/                        ← gerado ao rodar o analisador
    ├── saidaLex-teste.lex            ← tokens reconhecidos
    ├── saidaTS-teste.ts              ← tabela de símbolos
    └── saidaErro-teste.err           ← erros léxicos encontrados
```

---

## Pré-requisitos

- Compilador GCC instalado  
  Download: https://www.mingw-w64.org/downloads/

Para verificar se o GCC está instalado, abra o terminal e digite:
```bash
gcc --version
```

---

## Como Compilar

Abra o terminal na pasta do projeto e execute:

```bash
gcc main.c lexer.c symtable.c -o analisador-lexico
```

---

## Como Executar

**Rodar com um arquivo específico:**
```bash
./analisador-lexico codigo-teste-pascal/teste001.pas
```

**Rodar todos os 8 testes de uma vez:**

No `main.c` existe um modo de execução em lote — basta descomentar o bloco do loop `for` e comentar as linhas de arquivo único. Os arquivos serão gerados automaticamente na pasta `resultado/`:
```
resultado/saidaLex-teste001.lex ... resultado/saidaLex-teste008.lex
resultado/saidaTS-teste001.ts   ... resultado/saidaTS-teste008.ts
resultado/saidaErro-teste001.err ... resultado/saidaErro-teste008.err
```

---

## Tokens Reconhecidos

### Identificadores e Números

| Token | Descrição | Exemplos |
|---|---|---|
| ID | Sequência iniciada por letra `[a-zA-Z]` seguida de letras e/ou dígitos `[a-zA-Z0-9]*` | `x`, `contador`, `valor1` |
| NUM_INT | Sequência de dígitos `[0-9]+`, com sinal opcional `+` ou `-` | `10`, `+42`, `-100` |
| NUM_REAL | Inteiro com parte decimal `.` e/ou fator de escala `E` | `1.5`, `10E5`, `1.5E+3`, `-2.0E-2` |

> A linguagem é **case-insensitive** — `Exemplo`, `EXEMPLO` e `exemplo` são o mesmo identificador, sempre armazenado em minúsculo na Tabela de Símbolos.

#### Formatos válidos de NUM_REAL

| Formato | Exemplo |
|---|---|
| Inteiro com ponto decimal | `1.5`, `3.14` |
| Inteiro com fator de escala | `10E5`, `10e5` |
| Inteiro com fator de escala e sinal | `10E+5`, `10E-5` |
| Real com fator de escala | `1.5E10`, `1.5e10` |
| Real com fator de escala e sinal | `1.5E+10`, `2.0E-3` |

#### Sinal nos números

O sinal `+` ou `-` antes de um número é reconhecido como parte do número quando aparece diretamente antes de dígitos. Caso contrário é reconhecido como operador `OP_AD` ou `OP_MIN`.

| Entrada | Tokens gerados |
|---|---|
| `+10` | `NUM_INT "+10"` |
| `-1.5` | `NUM_REAL "-1.5"` |
| `x + 10` | `ID "x"`, `OP_AD "+"`, `NUM_INT "10"` |

### Palavras Reservadas
| Token | Lexema |
|---|---|
| KW_PROGRAM | program |
| KW_VAR | var |
| KW_INTEGER | integer |
| KW_REAL | real |
| KW_BEGIN | begin |
| KW_END | end |
| KW_IF | if |
| KW_THEN | then |
| KW_ELSE | else |
| KW_WHILE | while |
| KW_DO | do |

### Operadores
| Token | Lexema |
|---|---|
| OP_EQ | = |
| OP_NE | <> |
| OP_LT | < |
| OP_LE | <= |
| OP_GT | > |
| OP_GE | >= |
| OP_AD | + |
| OP_MIN | - |
| OP_MUL | * |
| OP_DIV | / |
| OP_ASS | := |

### Símbolos
| Token | Lexema |
|---|---|
| SMB_SEM | ; |
| SMB_COM | , |
| SMB_OPA | ( |
| SMB_CPA | ) |
| SMB_COL | : |
| SMB_DOT | . |

---

## Erros Léxicos Detectados

| Tipo de Erro | Exemplo |
|---|---|
| Caractere inválido | `@`, `%`, `$`, `#`, `!` |
| Comentário não fechado | `{ comentário sem fechar` + EOF |
| Expoente inválido | `1.5E`, `10E+`, `10E-` |

Para cada erro, o analisador registra o **tipo do erro**, a **linha** e a **coluna** no arquivo `saidaErro.err`.

---

## Formato dos Arquivos de Saída

### `saidaLex-teste.lex`
```
<KW_PROGRAM, program> 1 1
<ID, exemplo> 1 9
<SMB_SEM, ;> 1 16
```
Formato: `<TIPO_TOKEN, lexema> linha coluna`

### `saidaTS-teste.ts`
```
LEXEMA               TIPO
------               ----
program              KW_PROGRAM
exemplo              ID
x                    ID
```

### `saidaErro-teste.err`
```
ERRO linha 5, coluna 9: caractere invalido '@'
ERRO linha 8, coluna 3: comentario nao fechado
ERRO linha 12, coluna 7: expoente invalido
```

---

## Tabela de Símbolos

A Tabela de Símbolos é inicializada com todas as **11 palavras reservadas** da linguagem. Durante a análise, os **identificadores** encontrados são inseridos automaticamente. As regras são:

- Não permite duplicatas
- A linguagem é **case-insensitive** — `Program`, `PROGRAM` e `program` são o mesmo lexema
- Apenas identificadores e palavras reservadas são armazenados
- Números, operadores e símbolos **não** são armazenados na TS

---

## AFD — Autômato Finito Determinístico

O AFD foi dividido em diagramas menores para facilitar a leitura e explicação. Todos os arquivos estão na pasta `AFDs/`.

| Arquivo | Descrição |
|---|---|
| `afd_completo` | visão geral de todos os estados e transições |
| `afd_tokens_simples` | tokens de um único caractere: `= * / ; , ( ) .` |
| `afd_comentario` | reconhecimento de comentários `{ }` e erro de EOF |
| `afd_operador_atribuicao` | distinção entre `:` e `:=` |
| `afd_operador_menor` | distinção entre `<`, `<=` e `<>` |
| `afd_operador_maior` | distinção entre `>` e `>=` |
| `afd_numeros_e_sinais` | `NUM_INT`, `NUM_REAL`, fator de escala `E` e sinais `+` `-` |
| `afd_identificadores` | identificadores e palavras reservadas |
| `afd_fim_arquivo_eof` | reconhecimento do fim de arquivo |
| `afd_erro_lexico` | `TOKEN_ERROR` e conceito de lookahead |

Para gerar o PNG ou SVG a partir do `.dot` (requer Graphviz instalado):

Para gerar o SVG a partir do `.dot` (requer Graphviz instalado):

```bash
Exemplos: 

dot -Tsvg .\AFDs\codigo\afd_completo.dot -o .\AFDs\png\afd_completo.svg
dot -Tpng .\AFDs\codigo\afd_completo.dot -o .\AFDs\png\afd_completo.png
```

Download do Graphviz: https://graphviz.org/download/
