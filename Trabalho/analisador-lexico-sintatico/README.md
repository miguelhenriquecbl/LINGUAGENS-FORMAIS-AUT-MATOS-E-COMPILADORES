# Analisador Léxico e Sintático - MicroPascal (µ-Pascal)

Trabalho prático da disciplina **Linguagens Formais, Autômatos e Compiladores**  
Universidade Católica de Brasília - 1º semestre de 2026  
Professor: Marcelo Eustáquio

---

## Descrição

Implementação de um compilador front-end para a linguagem **MicroPascal (µ-Pascal)**, desenvolvido em linguagem C, composto por duas etapas:

**Etapa 1 - Analisador Léxico:** lê o programa fonte caractere por caractere, reconhece tokens, detecta erros léxicos e mantém a Tabela de Símbolos.

**Etapa 2 - Analisador Sintático:** recebe os tokens do léxico, verifica se a estrutura segue a gramática do MicroPascal, constrói a Árvore Sintática Abstrata (AST) e gera o diagrama da árvore em Graphviz.

---

## Estrutura do Projeto

```
analisador-lexico-sintatico/
├── main.c                                   ← ponto de entrada
├── lexer.h / lexer.c                        ← analisador léxico (AFD)
├── sintatico.h / sintatico.c                ← analisador sintático (AST + Graphviz)
├── token.h                                  ← enum TokenType, struct Token e MapaToken
├── symtable.h / symtable.c                  ← Tabela de Símbolos
├── README.md                                ← este arquivo
├── analisador.exe                           ← executável gerado pelo compilador
│
├── AFDs/analise-lexica/                     ← diagramas do AFD léxico
│   ├── codigo/                              ← código fonte Graphviz (.dot)
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
│   ├── png/                                 ← imagens PNG
│   └── svg/                                 ← imagens SVG (vetorial)
│
├── codigo-teste-pascal-lexico/              ← programas de teste do léxico
│   ├── teste001.pas                         ← correto - exemplo do professor com sinais e E
│   ├── teste002.pas                         ← correto - operadores relacionais com sinais
│   ├── teste003.pas                         ← correto - comentários com sinais e E
│   ├── teste004.pas                         ← correto - case-insensitive com E e e
│   ├── teste005.pas                         ← erro - caractere inválido '@' com sinais
│   ├── teste006.pas                         ← erro - comentário não fechado
│   ├── teste007.pas                         ← erro - E sem dígito depois
│   └── teste008.pas                         ← erro - múltiplos erros
│
├── codigo-teste-pascal-sintatico/           ← programas de teste do sintático
│   ├── teste001.pas                         ← correto - programa simples com atribuição
│   ├── teste002.pas                         ← correto - if/else com expressões relacionais
│   ├── teste003.pas                         ← correto - while + begin/end aninhado
│   ├── teste004.pas                         ← erro sintático - falta o ponto final
│   ├── teste005.pas                         ← erro sintático - falta o then no if
│   └── teste006.pas                         ← erro sintático - falta o begin
│
├── exemplos-saida/
│   ├── saida-lexico/                        ← saídas pré-geradas dos testes léxicos
│   │   ├── lex/
│   │   │   ├── saidaLex-teste001.lex ... saidaLex-teste008.lex
│   │   ├── ts/
│   │   │   ├── saidaTS-teste001.ts   ... saidaTS-teste008.ts
│   │   └── err/
│   │       ├── saidaErro-teste001.err ... saidaErro-teste008.err
│   │
│   └── saida-sintatico/                     ← saídas pré-geradas dos testes sintáticos
│       ├── afd_codigo/
│       │   ├── saidaAFD_teste001.dot
│       │   ├── saidaAFD_teste002.dot
│       │   └── saidaAFD_teste003.dot
│       ├── afd_png/
│       │   ├── saidaAFD_teste001.png
│       │   ├── saidaAFD_teste002.png
│       │   └── saidaAFD_teste003.png
│       └── afd_svg/
│           ├── saidaAFD_teste001.svg
│           ├── saidaAFD_teste002.svg
│           └── saidaAFD_teste003.svg
│
└── resultado/                               ← gerado ao rodar o compilador
    ├── saidaLex-teste.lex                   ← tokens reconhecidos
    ├── saidaTS-teste.ts                     ← tabela de símbolos
    ├── saidaErro-teste.err                  ← erros léxicos
    ├── saidaAFD-teste.dot                   ← árvore sintática (Graphviz)
    ├── saidaAFD-teste.png                   ← imagem PNG da árvore
    └── saidaAFD-teste.svg                   ← imagem SVG da árvore
```

---

## Pré-requisitos

- Compilador GCC: https://www.mingw-w64.org/downloads/
- Graphviz (para gerar as imagens da árvore): https://graphviz.org/download/

```bash
gcc --version
dot --version
```

---

## Como Compilar

```bash
gcc main.c lexer.c symtable.c sintatico.c -o analisador
```

---

## Como Executar

**Rodar com um arquivo específico:**
```bash
./analisador codigo-teste-pascal-sintatico/teste001.pas
```

Arquivos gerados na pasta `resultado/`:
- `saidaLex-teste.lex` - tokens reconhecidos
- `saidaTS-teste.ts` - tabela de símbolos
- `saidaErro-teste.err` - erros léxicos
- `saidaAFD-teste.dot` - árvore sintática em Graphviz
- `saidaAFD-teste.png` - imagem PNG da árvore
- `saidaAFD-teste.svg` - imagem SVG da árvore

---

## Gramática do MicroPascal

```
<programa>    ::= program <id> ; <bloco> .
<bloco>       ::= <parteDecl> <cmdComposto>
<parteDecl>   ::= { var <declVar> {; <declVar>} ; }
<declVar>     ::= <listaIds> : <tipo>
<listaIds>    ::= <id> { , <id> }
<tipo>        ::= integer | real
<cmdComposto> ::= begin <cmd> { ; <cmd> } end
<cmd>         ::= <atrib> | <cmdComposto> | <condicional> | <repetitivo>
<atrib>       ::= <var> := <expr>
<condicional> ::= if <expr> then <cmd> [ else <cmd> ]
<repetitivo>  ::= while <expr> do <cmd>
<expr>        ::= <exprSimples> [ <relacao> <exprSimples> ]
<relacao>     ::= = | <> | < | <= | >= | >
<exprSimples> ::= [ +|- ] <termo> { (+|-) <termo> }
<termo>       ::= <fator> { (*|/) <fator> }
<fator>       ::= <var> | <num> | ( <expr> )
```

---

## Tokens Reconhecidos

### Identificadores e Números

| Token | Descrição | Exemplos |
|---|---|---|
| ID | Letra `[a-zA-Z]` seguida de letras e/ou dígitos `[a-zA-Z0-9]*` | `x`, `contador`, `valor1` |
| NUM_INT | Dígitos `[0-9]+`, com sinal opcional `+` ou `-` | `10`, `+42`, `-100` |
| NUM_REAL | Inteiro com parte decimal `.` e/ou fator de escala `E` | `1.5`, `10E5`, `1.5E+3` |

> A linguagem é **case-insensitive** - `Exemplo`, `EXEMPLO` e `exemplo` são o mesmo identificador.

### Palavras Reservadas
| Token | Lexema | Token | Lexema |
|---|---|---|---|
| KW_PROGRAM | program | KW_IF | if |
| KW_VAR | var | KW_THEN | then |
| KW_INTEGER | integer | KW_ELSE | else |
| KW_REAL | real | KW_WHILE | while |
| KW_BEGIN | begin | KW_DO | do |
| KW_END | end | | |

### Operadores e Símbolos
| Token | Lexema | Token | Lexema |
|---|---|---|---|
| OP_EQ | = | SMB_SEM | ; |
| OP_NE | <> | SMB_COM | , |
| OP_LT | < | SMB_OPA | ( |
| OP_LE | <= | SMB_CPA | ) |
| OP_GT | > | SMB_COL | : |
| OP_GE | >= | SMB_DOT | . |
| OP_AD | + | OP_ASS | := |
| OP_MIN | - | OP_MUL | * |
| OP_DIV | / | | |

---

## Erros Detectados

### Erros Léxicos
| Tipo | Exemplo |
|---|---|
| Caractere inválido | `@`, `%`, `$`, `#`, `!` |
| Comentário não fechado | `{ comentário sem fechar` + EOF |
| Expoente inválido | `1.5E`, `10E+`, `10E-` |

### Erros Sintáticos
| Formato | Exemplo |
|---|---|
| `nn:token nao esperado [lex].` | `7:token nao esperado [x].` |
| `nn:fim de arquivo nao esperado.` | `11:fim de arquivo nao esperado.` |

---

## AFD - Analisador Léxico

O AFD léxico foi dividido em 10 diagramas na pasta `AFDs/analise-lexica/`.

| Arquivo | Descrição |
|---|---|
| `afd_completo` | visão geral de todos os estados e transições |
| `afd_tokens_simples` | tokens de um único caractere: `= * / ; , ( ) .` |
| `afd_comentario` | reconhecimento de comentários `{ }` |
| `afd_operador_atribuicao` | distinção entre `:` e `:=` |
| `afd_operador_menor` | distinção entre `<`, `<=` e `<>` |
| `afd_operador_maior` | distinção entre `>` e `>=` |
| `afd_numeros_e_sinais` | `NUM_INT`, `NUM_REAL`, fator de escala `E` e sinais |
| `afd_identificadores` | identificadores e palavras reservadas |
| `afd_fim_arquivo_eof` | reconhecimento do fim de arquivo |
| `afd_erro_lexico` | `TOKEN_ERROR` e conceito de lookahead |

Para gerar todos os PNG e SVG no Windows:
```bat
for %f in (AFDs\analise-lexica\codigo\*.dot) do (
    dot -Tpng "%f" -o "AFDs\analise-lexica\png\%~nf.png"
    dot -Tsvg "%f" -o "AFDs\analise-lexica\svg\%~nf.svg"
)
```

---

## Árvore Sintática

O analisador sintático gera automaticamente a árvore sintática em Graphviz ao rodar. Os arquivos ficam em `resultado/`.

- Nós **elípticos verdes** - não-terminais da gramática (`programa`, `bloco`, `cmd`, etc.)
- Nós **retangulares azuis** - terminais/tokens (`program`, `x`, `:=`, `10`, etc.)
