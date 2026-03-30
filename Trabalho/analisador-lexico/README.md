# Analisador Léxico — MicroPascal (µ-Pascal)

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
├── main.c                            ← analisador léxico (AFD + geração de arquivos)
├── token.h                           ← enum TokenType e struct Token
├── symtable.h                        ← declarações da Tabela de Símbolos
├── symtable.c                        ← implementação da Tabela de Símbolos
├── AFD.dot                           ← diagrama do AFD em Graphviz (código fonte)
├── AFD.svg                           ← diagrama do AFD gerado
├── README.md                         ← este arquivo
├── analisador-lexico.exe             ← executável gerado pelo compilador
│
├── codigo-teste-pascal/              ← programas de teste em MicroPascal
│   ├── teste001.pas                  ← correto — exemplo do professor
│   ├── teste002.pas                  ← correto — operadores relacionais
│   ├── teste003.pas                  ← correto — comentários
│   ├── teste004.pas                  ← correto — case-insensitive
│   ├── teste005.pas                  ← erro — caractere inválido '@'
│   ├── teste006.pas                  ← erro — comentário não fechado
│   ├── teste007.pas                  ← erro — múltiplos erros '@' '%' '$'
│   └── teste008.pas                  ← erro — caracteres inválidos '#' '!'
│
├── exemplos-saida/                   ← saídas pré-geradas de cada teste
│   ├── saidaLex-teste001.lex         
│   ├── saidaLex-teste002.lex         
│   ├── saidaLex-teste003.lex         
│   ├── saidaLex-teste004.lex         
│   ├── saidaLex-teste005.lex         
│   ├── saidaLex-teste006.lex         
│   ├── saidaLex-teste007.lex         
│   ├── saidaLex-teste008.lex         
│   ├── saidaTS-teste001.ts           
│   ├── saidaTS-teste002.ts           
│   ├── saidaTS-teste003.ts           
│   ├── saidaTS-teste004.ts           
│   ├── saidaTS-teste005.ts           
│   ├── saidaTS-teste006.ts           
│   ├── saidaTS-teste007.ts           
│   ├── saidaTS-teste008.ts           
│   ├── saidaErro-teste001.err        
│   ├── saidaErro-teste002.err        
│   ├── saidaErro-teste003.err        
│   ├── saidaErro-teste004.err        
│   ├── saidaErro-teste005.err        
│   ├── saidaErro-teste006.err        
│   ├── saidaErro-teste007.err        
│   └── saidaErro-teste008.err        
│
└── resultado/                        ← gerado ao rodar o analisador
    ├── saidaLex.lex                  ← tokens reconhecidos
    ├── saidaTS.ts                    ← tabela de símbolos
    └── saidaErro.err                 ← erros léxicos encontrados
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
gcc main.c symtable.c -o a
```

---

## Como Executar

```bash
./a codigo-teste-pascal/teste001.pas
```

Os arquivos de saída serão gerados automaticamente na pasta `resultado/`:
- `resultado/saidaLex.lex` — tokens reconhecidos
- `resultado/saidaTS.ts` — tabela de símbolos
- `resultado/saidaErro.err` — erros léxicos (vazio se não houver erros)

---

## Tokens Reconhecidos

### Identificadores e Números

| Token | Descrição | Exemplos |
|---|---|---|
| ID | Sequência iniciada por letra `[a-zA-Z]` seguida de letras e/ou dígitos `[a-zA-Z0-9]*` | `x`, `contador`, `valor1` |
| NUM_INT | Sequência de dígitos `[0-9]+` | `10`, `42`, `100` |
| NUM_REAL | Dígitos seguidos de `.` e mais dígitos `[0-9]+.[0-9]+` | `1.5`, `3.14`, `2.5` |

> A linguagem é **case-insensitive** — `Exemplo`, `EXEMPLO` e `exemplo` são o mesmo identificador, sempre armazenado em minúsculo na Tabela de Símbolos.

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

Para cada erro, o analisador registra o **tipo do erro**, a **linha** e a **coluna** no arquivo `saidaErro.err`.

---

## Formato dos Arquivos de Saída

### `saidaLex.lex`
```
<KW_PROGRAM, program> 1 1
<ID, exemplo> 1 9
<SMB_SEM, ;> 1 16
```
Formato: `<TIPO_TOKEN, lexema> linha coluna`

### `saidaTS.ts`
```
LEXEMA               TIPO
------               ----
program              KW_PROGRAM
exemplo              ID
x                    ID
```

### `saidaErro.err`
```
ERRO linha 5, coluna 9: caractere invalido '@'
ERRO linha 8, coluna 3: comentario nao fechado
```

---

## Tabela de Símbolos

A Tabela de Símbolos é inicializada com todas as **11 palavras reservadas** da linguagem. Durante a análise, os **identificadores** encontrados são inseridos automaticamente. As regras são:

- Não permite duplicatas
- A linguagem é **case-insensitive** — `Program`, `PROGRAM` e `program` são o mesmo lexema
- Apenas identificadores e palavras reservadas são armazenados

---

## AFD — Autômato Finito Determinístico

O diagrama do AFD está disponível nos arquivos `AFD.dot` e `afd.svg`.

Para gerar o SVG a partir do `.dot` (requer Graphviz instalado):
```bash
dot -Tsvg AFD.dot -o afd.svg
```

Download do Graphviz: https://graphviz.org/download/

---

## Exemplo de Execução

Entrada (`teste001.pas`):
```pascal
program Exemplo;
var
   x : integer;
begin
   x := 10
end.
```

Saída (`saidaLex.lex`):
```
<KW_PROGRAM, program> 1 1
<ID, exemplo> 1 9
<SMB_SEM, ;> 1 16
<KW_VAR, var> 3 1
<ID, x> 4 4
<SMB_COL, :> 4 6
<KW_INTEGER, integer> 4 8
<KW_BEGIN, begin> 5 1
<ID, x> 6 4
<OP_ASS, :=> 6 6
<NUM_INT, 10> 6 9
<KW_END, end> 7 1
<SMB_DOT, .> 7 4
```
