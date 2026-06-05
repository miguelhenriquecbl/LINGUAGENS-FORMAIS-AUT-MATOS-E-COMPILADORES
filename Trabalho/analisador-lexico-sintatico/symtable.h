#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdio.h>

typedef struct {
    char lexema[256];
    char tokenName[32];
} EntradaTS;

void inicializaTS();

int buscaTS(char *lexema);

void inserirTS(char *lexema, char *tokenName);

void escreverTS(FILE *ts);

void liberaTS();

char *pegarTokenNome (int idx);

#endif


