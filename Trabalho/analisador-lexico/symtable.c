#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symtable.h"

static EntradaTS *tabela = NULL;
static int quantidade = 0;
static int capacidade = 20;

void inicializaTS() {
    tabela = malloc(capacidade * sizeof(EntradaTS));

    if (!tabela) {
        printf("ERRO: sem memoria para a tabela");
        exit(1);
    }

    inserirTS("program", "KW_PROGRAM");
    inserirTS("var", "KW_VAR");
    inserirTS("integer", "KW_INTEGER");
    inserirTS("real", "KW_REAL");
    inserirTS("begin", "KW_BEGIN");
    inserirTS("end", "KW_END");
    inserirTS("if", "KW_IF");
    inserirTS("then", "KW_THEN");
    inserirTS("else", "KW_ELSE");
    inserirTS("while", "KW_WHILE");
    inserirTS("do", "KW_DO");
}

int buscaTS(char *lexema) {
    char lower[256];
    int i;

    for (i = 0; lexema[i]; i++) lower[i] = tolower(lexema[i]);
    lower[i] = '\0';

    for (int j = 0; j < quantidade; j++) {
        if (strcmp(tabela[j].lexema, lower) == 0 ) return j;
    }

    return -1;
}

void inserirTS(char *lexema, char *tokenName) {
    char lower[256];
    int i;

    for (i = 0; lexema[i]; i++) lower[i] = tolower(lexema[i]);
    lower[i] = '\0';

    if (buscaTS(lower) != -1) return;

    if (quantidade == capacidade) {
        capacidade *= 2;
        tabela = realloc(tabela, capacidade * sizeof(EntradaTS));

        if (!tabela) {
            printf("ERRO: sem memoria para a tabela");
            exit(1);
        }
    }

    strcpy(tabela[quantidade].lexema, lower);
    strcpy(tabela[quantidade].tokenName, tokenName);
    quantidade++;
}

void escreverTS(FILE *ts) {
    fprintf(ts, "%-20s %s\n", "LEXEMA", "TIPO");
    fprintf(ts, "%-20s %s\n", "------", "----");

    for (int i = 0; i < quantidade; i++) fprintf(ts, "%-20s %s\n", tabela[i].lexema, tabela[i].tokenName);
}

char *pegarTokenNome (int idx) {
    return tabela[idx].tokenName;
}

void liberaTS() {
    free(tabela);
    tabela = NULL;
}

