#ifndef TOKEN_H
#define TOKEN_H

typedef enum {

    /* Palavras reservadas do MicroPascal */
    KW_PROGRAM,
    KW_VAR,
    KW_INTEGER,
    KW_REAL,
    KW_BEGIN,
    KW_END,
    KW_IF,
    KW_THEN,
    KW_ELSE,
    KW_WHILE,
    KW_DO,

    /* Identificador e números */
    ID,
    NUM_INT,
    NUM_REAL,

    /* Operadores relacionais */
    OP_EQ, /* = */
    OP_NE, /* <> */
    OP_LT, /* < */
    OP_LE, /* <= */
    OP_GT, /* > */
    OP_GE, /* >= */

    /* Operadores aritméticos */
    OP_AD, /* + */
    OP_MIN, /* - */
    OP_MUL, /* * */
    OP_DIV, /* / */
    OP_ASS, /* := */

    /* Símbolos */
    SMB_SEM, /* ; */
    SMB_COM, /* , */
    SMB_OPA, /* ( */
    SMB_CPA, /* ) */
    SMB_COL, /* : */
    SMB_DOT, /* . */
    SMB_OBC, /* { */
    SMB_CBC, /* } */

    /* Especiais */
    TOKEN_EOF, /* Fim do arquivo */
    TOKEN_ERROR /* Erro léxico */

} TokenType;

typedef struct {
    TokenType type;
    char lexema[256];
    int linha;
    int col;
} Token;

typedef struct {
    char *nome;
    TokenType type;
} MapaToken;

#endif


