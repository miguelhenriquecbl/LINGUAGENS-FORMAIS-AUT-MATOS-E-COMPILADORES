#ifndef SINTATICO_H
#define SINTATICO_H

#include "token.h"
#include <stdio.h>

typedef struct {
    Token *data;
    int size;
    int cap;    
} TokenVec;

typedef struct AST {
    char rotulo[256];
    struct AST **filhos;
    int  nFilhos;
    int id;
} AST;

typedef struct {
    Token *toks;
    int i;
    int n;
} Parser;

TokenVec tokenizar(void);
void tv_free(TokenVec *v);

AST *ast_criar(const char *rotulo);
void ast_add(AST *pai, AST *filho);
void ast_free(AST *t);
void ast_print(const AST *t, int prof);
void ast_to_dot(const AST *t, const char *path);

AST *parse(TokenVec *tv);
 
#endif
