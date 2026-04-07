#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "symtable.h"
#include "lexer.h"

// Verifica o Sistema Operacional antes de compilar
#ifdef _WIN32
    #include <direct.h> // Biblioteca para Windows
    #define CRIAR_PASTA(nome) _mkdir(nome)
#else
    #include <sys/stat.h> // Biblioteca para Linux/Unix
    #include <sys/types.h>
    #define CRIAR_PASTA(nome) _mkdir(nome, 0777)
#endif

extern int temErro;

FILE *fonte;
FILE *arqLex;
FILE *arqTS;
FILE *arqErr;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Você não passou nenhum arquivo.\n");
        return 1;
    }

    if (CRIAR_PASTA("resultado") == 0)
    {
        printf("Pasta 'resultado' criada com sucesso.\n");
    }
    else
    {
        printf("A pasta 'resultado' ja existe ou ocorreu um erro ao criar.\n");
    }

    fonte = fopen(argv[1], "r");
    arqLex = fopen("resultado/saidaLex-teste.lex", "w");
    arqTS = fopen("resultado/saidaTS-teste.ts", "w");
    arqErr = fopen("resultado/saidaErro-teste.err", "w");

    if (!fonte)
    {
        printf("ERRO: Arquivo nao encontrado\n");
        return 1;
    }

    if (!arqLex)
    {
        printf("ERRO: Erro ao criar o arquivo saidaLex.lex\n");
        return 1;
    }

    if (!arqTS)
    {
        printf("ERRO: Erro ao criar o arquivo saidaTS.ts\n");
        return 1;
    }

    if (!arqErr)
    {
        printf("ERRO: Erro ao criar o arquivo saidaErro.err\n");
        return 1;
    }

    inicializaTS();

    Token t;
    do
    {

        t = proximoToken();

        if (t.type != TOKEN_EOF && t.type != TOKEN_ERROR)
        {
            escreveToken(t);
        }

    } while (t.type != TOKEN_EOF);

    escreverTS(arqTS);

    if (temErro == 1)
    {
        printf("Analise concluida com erros. Veja saidaErro.err\n");
    }
    else
    {
        printf("Analise concluida com sucesso!\n");
    }

    liberaTS();

    fclose(fonte);
    fclose(arqLex);
    fclose(arqTS);
    fclose(arqErr);

    return 0;
}

/* 
======================================================================
Codigo para Executar os 8 codigos teste pascal

Observação! Siga esses passos:

1 - Na função main, comente as linhas do fonte, arqLex, arqTS, arqErr.
2 - Copie e cole o codigo abaixo na função main.
3 - Coloque o "for" e a chave aberta('{') antes do "if (!fonte)"" e feche a chave ('}') dele antes do "return 0;".

======================================================================

char nomeArqFonte[50];
char nomeArqLex[50];
char nomeArqTS[50];
char nomeArqErr[50];

for (int i = 1; i < 9; i++)
{

    linha = 1;
    col = 1;
    temErro = 0;

    sprintf(nomeArqFonte, "codigo-teste-pascal/teste00%d.pas", i);
    fonte = fopen(nomeArqFonte, "r");

    sprintf(nomeArqLex, "resultado/saidaLex-teste00%d.lex", i);
    arqLex = fopen(nomeArqLex, "w");

    sprintf(nomeArqTS, "resultado/saidaTS-teste00%d.ts", i);
    arqTS = fopen(nomeArqTS, "w");

    sprintf(nomeArqErr, "resultado/saidaErro-teste00%d.err", i);
    arqErr = fopen(nomeArqErr, "w");
}
*/