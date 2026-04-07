#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "symtable.h"

extern FILE *fonte;
extern FILE *arqLex;
extern FILE *arqErr;

int linha = 1;
int col = 1;
int temErro = 0;

MapaToken mapa[] = {
    {"KW_PROGRAM", KW_PROGRAM},
    {"KW_VAR", KW_VAR},
    {"KW_INTEGER", KW_INTEGER},
    {"KW_REAL", KW_REAL},
    {"KW_BEGIN", KW_BEGIN},
    {"KW_END", KW_END},
    {"KW_IF", KW_IF},
    {"KW_THEN", KW_THEN},
    {"KW_ELSE", KW_ELSE},
    {"KW_WHILE", KW_WHILE},
    {"KW_DO", KW_DO},

    {"ID", ID},
    {"NUM_INT", NUM_INT},
    {"NUM_REAL", NUM_REAL},

    {"OP_EQ", OP_EQ},
    {"OP_NE", OP_NE},
    {"OP_LT", OP_LT},
    {"OP_LE", OP_LE},
    {"OP_GT", OP_GT},
    {"OP_GE", OP_GE},

    {"OP_AD", OP_AD},
    {"OP_MIN", OP_MIN},
    {"OP_MUL", OP_MUL},
    {"OP_DIV", OP_DIV},
    {"OP_ASS", OP_ASS},

    {"SMB_SEM", SMB_SEM},
    {"SMB_COM", SMB_COM},
    {"SMB_OPA", SMB_OPA},
    {"SMB_CPA", SMB_CPA},
    {"SMB_COL", SMB_COL},
    {"SMB_DOT", SMB_DOT},
    {"SMB_OBC", SMB_OBC},
    {"SMB_CBC", SMB_CBC},

    {"TOKEN_EOF", TOKEN_EOF},
    {"TOKEN_ERROR", TOKEN_ERROR},
};

char *nomeDoToken(TokenType type)
{
    int tamanho = sizeof(mapa) / sizeof(mapa[0]);
    for (int i = 0; i < tamanho; i++)
    {
        if (mapa[i].type == type)
            return mapa[i].nome;
    }

    return "DESCONHECIDO";
}

TokenType tokenPorNome(char *nome)
{
    int tamanho = sizeof(mapa) / sizeof(mapa[0]);
    for (int i = 0; i < tamanho; i++)
    {
        if (strcmp(mapa[i].nome, nome) == 0)
            return mapa[i].type;
    }

    return TOKEN_ERROR;
}

int proximoChar()
{
    int c = fgetc(fonte);

    if (c == '\n')
    {
        linha++;
        col = 1;
    }
    else
    {
        col++;
    }

    return c;
}

void devolverChar(int c)
{
    ungetc(c, fonte);

    if (c != '\n')
        col--;
}

void ignoraComentario(int linhaInicio, int colInicio)
{
    int c;

    while ((c = proximoChar()) != EOF)
    {
        if (c == '}')
            return;
    }

    fprintf(arqErr, "ERRO linha %d, coluna %d: comentario nao fechado\n", linhaInicio, colInicio);
    temErro = 1;
}

void ehLetra(char c, Token *t)
{
    int i = 0;

    t->lexema[i++] = tolower(c);

    while ((c = proximoChar()) != EOF && (isalpha(c) || isdigit(c)))
    {
        t->lexema[i++] = tolower(c);
    }

    t->lexema[i] = '\0';

    if (c != EOF)
        devolverChar(c);

    int idx = buscaTS(t->lexema);
    if (idx != -1)
    {
        char *nome = pegarTokenNome(idx);
        t->type = tokenPorNome(nome);
    }
    else
    {
        t->type = ID;
        inserirTS(t->lexema, "ID");
    }
}

void reconheceExpoente(char c, int i, Token *t)
{
    t->lexema[i++] = 'e';

    c = proximoChar();

    if (c == '+' || c == '-')
    {
        t->lexema[i++] = c;
        c = proximoChar();
    }

    if (isdigit(c))
    {
        t->lexema[i++] = c;
        while ((c = proximoChar()) != EOF && isdigit(c))
        {
            t->lexema[i++] = c;
        }

        t->lexema[i] = '\0';
        if (c != EOF)
            devolverChar(c);

        t->type = NUM_REAL;
    }
    else
    {
        t->lexema[i] = '\0';
        if (c != EOF)
            devolverChar(c);

        fprintf(arqErr, "ERRO linha %d, coluna %d: expoente invalido '%c'\n", linha, col, c);
        temErro = 1;
        t->type = TOKEN_ERROR;
    }
}

void ehNumero(char c, int i, Token *t)
{

    t->lexema[i++] = c;

    while ((c = proximoChar()) != EOF && isdigit(c))
    {
        t->lexema[i++] = c;
    }

    if (c == '.')
    {
        t->lexema[i++] = '.';
        while ((c = proximoChar()) != EOF && isdigit(c))
        {
            t->lexema[i++] = c;
        }

        if (tolower(c) == 'e')
        {
            reconheceExpoente(c, i, t);
        }
        else
        {
            t->lexema[i] = '\0';
            if (c != EOF)
                devolverChar(c);

            t->type = NUM_REAL;
        }
    }
    else if (tolower(c) == 'e')
    {
        reconheceExpoente(c, i, t);
    }
    else
    {
        t->lexema[i] = '\0';
        if (c != EOF)
            devolverChar(c);

        t->type = NUM_INT;
    }
}

Token proximoToken() {
    Token t;
    int c;
    int prox;
    int i = 0;

    while (1) {
        c = proximoChar();

        if (c == EOF) {
            t.type = TOKEN_EOF;
            strcpy(t.lexema, "EOF");
            t.linha = linha;
            t.col = col;
            return t;
        }

        if (isspace(c))
            continue;
        if (c == '{') {
            ignoraComentario(linha, col - 1);
            continue;
        }

        break;
    }

    t.linha = linha;
    t.col = col - 1;

    if (isalpha(c)) {
        ehLetra(c, &t);
        return t;
    }

    if (isdigit(c)) {
        ehNumero(c, i, &t);
        return t;
    }

    t.lexema[0] = c;
    t.lexema[1] = '\0';

    switch (c) {
    case '=': t.type = OP_EQ; break;
    case '*': t.type = OP_MUL; break;
    case '/': t.type = OP_DIV; break;
    case ';': t.type = SMB_SEM; break;
    case ',': t.type = SMB_COM; break;
    case '(': t.type = SMB_OPA; break;
    case ')': t.type = SMB_CPA; break;
    case '.': t.type = SMB_DOT; break;

    case '+':
        prox = proximoChar();
        if (isdigit(prox)){
            ehNumero(prox, ++i, &t);
            return t;
        } else{
            t.type = OP_AD;
        }
        break;

    case '-':
        prox = proximoChar();
        if (isdigit(prox)) {
            ehNumero(prox, ++i, &t);
            return t;
        } else {
            t.type = OP_MIN;
        }
        break;

    case '<':
        prox = proximoChar();
        if (prox == '=') {
            strcpy(t.lexema, "<=");
            t.type = OP_LE;
        } else if (prox == '>') {
            strcpy(t.lexema, "<>");
            t.type = OP_NE;
        } else{
            devolverChar(prox);
            t.type = OP_LT;
        }

        break;

    case '>':
        prox = proximoChar();
        if (prox == '=') {
            strcpy(t.lexema, ">=");
            t.type = OP_GE;
        } else {
            devolverChar(prox);
            t.type = OP_GT;
        }
        break;

    case ':':
        prox = proximoChar();
        if (prox == '=')
        {
            strcpy(t.lexema, ":=");
            t.type = OP_ASS;
        } else {
            devolverChar(prox);
            t.type = SMB_COL;
        }
        break;

    default:
        fprintf(arqErr, "ERRO linha %d, coluna %d: caractere invalido '%c'\n", linha, col, c);
        temErro = 1;
        t.type = TOKEN_ERROR;
        break;
    }

    return t;
}

void escreveToken(Token t)
{
    fprintf(arqLex, "<%s, %s> %d %d\n", nomeDoToken(t.type), t.lexema, t.linha, t.col);
}
