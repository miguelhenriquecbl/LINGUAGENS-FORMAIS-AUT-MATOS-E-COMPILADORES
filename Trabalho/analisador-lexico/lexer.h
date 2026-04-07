#ifndef LEXER_H
#define LEXER_H
#include "token.h"

extern int linha;
extern int col;
extern int temErro;

extern MapaToken mapa[];

char *nomeDoToken(TokenType type);

TokenType tokenPorNome(char *nome);

int proximoChar();

void devolverChar(int c);

void ignoraComentario(int linhaInicio, int colInicio);

void reconheceExpoente(char c, int i, Token *t);

void ehNumero(char c, int i, Token *t);

void ehLetra(char c, Token *t);

Token proximoToken();

void escreveToken(Token t);

#endif

