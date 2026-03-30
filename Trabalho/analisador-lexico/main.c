#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "symtable.h"

FILE *fonte;
FILE *arqLex;
FILE *arqTS;
FILE *arqErr;

int linha = 1;
int col = 1;
int temErro = 0;

MapaToken mapa[] = {
    { "KW_PROGRAM", KW_PROGRAM},
    { "KW_VAR", KW_VAR},
    { "KW_INTEGER", KW_INTEGER},
    { "KW_REAL", KW_REAL},
    { "KW_BEGIN", KW_BEGIN},
    { "KW_END", KW_END},
    { "KW_IF", KW_IF},
    { "KW_THEN", KW_THEN},
    { "KW_ELSE", KW_ELSE},
    { "KW_WHILE", KW_WHILE},
    { "KW_DO", KW_DO},

    { "ID", ID},
    { "NUM_INT", NUM_INT},
    { "NUM_REAL", NUM_REAL},

    { "OP_EQ", OP_EQ},
    { "OP_NE", OP_NE},
    { "OP_LT", OP_LT},
    { "OP_LE", OP_LE},
    { "OP_GT", OP_GT},
    { "OP_GE", OP_GE},

    { "OP_AD", OP_AD},
    { "OP_MIN", OP_MIN},
    { "OP_MUL", OP_MUL},
    { "OP_DIV", OP_DIV},
    { "OP_ASS", OP_ASS},

    { "SMB_SEM", SMB_SEM},
    { "SMB_COM", SMB_COM},
    { "SMB_OPA", SMB_OPA},
    { "SMB_CPA", SMB_CPA},
    { "SMB_COL", SMB_COL},
    { "SMB_DOT", SMB_DOT},
    { "SMB_OBC", SMB_OBC},
    { "SMB_CBC", SMB_CBC},

    { "TOKEN_EOF", TOKEN_EOF},
    { "TOKEN_ERROR", TOKEN_ERROR},
};

char *nomeDoToken(TokenType type) {
    int tamanho = sizeof(mapa) / sizeof(mapa[0]);
    for (int i = 0; i < tamanho; i++) {
        if(mapa[i].type == type) return mapa[i].nome;
    }
    
    return "DESCONHECIDO";
}

TokenType tokenPorNome(char *nome) {
    int tamanho = sizeof(mapa) / sizeof(mapa[0]);
    for (int i = 0; i < tamanho; i++) {
        if(strcmp(mapa[i].nome, nome) == 0) return mapa[i].type;
    }

    return TOKEN_ERROR;
}

int proximoChar() {
    int c = fgetc(fonte);

    if (c == '\n') {
        linha++;
        col = 1;
    } else {
        col++;
    }

    return c;

}

void devolverChar(int c) {
    ungetc(c, fonte);

    if (c != '\n') col--;
}

void ignoraComentario(int linhaInicio, int colInicio) {
    int c;

    while((c = proximoChar()) != EOF) {
        if (c == '}') return;
    }

    fprintf(arqErr, "ERRO linha %d, coluna %d: comentario nao fechado\n", linhaInicio, colInicio);
    temErro = 1;
}


Token proximoToken() {
    Token t;
    int c;
    int prox;

    while (1) {
        c = proximoChar();
        
        if (c == EOF) {
            t.type = TOKEN_EOF;
            strcpy(t.lexema, "EOF");
            t.linha = linha;
            t.col = col;
            
            return t;
        }

        if (isspace(c)) continue;
        if (c == '{') {
            ignoraComentario(linha, col - 1);
            continue;
        }

        break;
    }

    t.linha = linha;
    t.col = col - 1;

    if (isalpha(c)) {
        int i = 0;

        t.lexema[i++] = tolower(c);

        while ((c = proximoChar()) != EOF && (isalpha(c) || isdigit(c))) {
            t.lexema[i++] = tolower(c);
        }

        t.lexema[i] = '\0';

        if (c != EOF) devolverChar(c);

        int idx = buscaTS(t.lexema);
        if (idx != -1) {
            char *nome = pegarTokenNome(idx);
            t.type = tokenPorNome(nome); 

        } else {
            t.type = ID;
            inserirTS(t.lexema, "ID");
        }

        return t;
    }

    if (isdigit(c)) {
        int i = 0;
        t.lexema[i++] = c;
        
        while ((c = proximoChar()) != EOF && isdigit(c)) {
            t.lexema[i++] = c;
        }

        if (c == '.') {
            t.lexema[i++] = '.';
            while ((c = proximoChar()) != EOF && isdigit(c)) {
                t.lexema[i++] = c;
            }

            t.lexema[i] = '\0';
            if (c != EOF) devolverChar(c);

            t.type = NUM_REAL;
        } else {
            t.lexema[i] = '\0';
            if (c != EOF) devolverChar(c);

            t.type = NUM_INT;
        }

        return t;        
    }

    t.lexema[0] = c;
    t.lexema[1] = '\0';

    switch (c) {
        case '=': t.type = OP_EQ; break;
        case '+': t.type = OP_AD; break;
        case '-': t.type = OP_MIN; break;
        case '*': t.type = OP_MUL; break;
        case '/': t.type = OP_DIV; break;
        case ';': t.type = SMB_SEM; break;
        case ',': t.type = SMB_COM; break;
        case '(': t.type = SMB_OPA; break;
        case ')': t.type = SMB_CPA; break;
        case '.': t.type = SMB_DOT; break;
        
        case '<': 
            prox = proximoChar();
            if (prox == '=') {
                strcpy(t.lexema, "<=");
                t.type = OP_LE; 
            } else if (prox == '>') {
                strcpy(t.lexema, "<>");
                t.type = OP_NE; 
            } else {
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
            if (prox == '=') {
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

void escreveToken(Token t) {
    fprintf(arqLex, "<%s, %s> %d %d\n", nomeDoToken(t.type), t.lexema, t.linha, t.col);
}

int main (int argc, char *argv[]) {
    if (argc < 2 ) {
        printf("Você não passou nenhum arquivo.\n");
        return 1;
    }

    fonte = fopen(argv[1], "r");
    arqLex = fopen("resultado/saidaLex-teste008.lex", "w");
    arqTS = fopen("resultado/saidaTS-teste008.ts", "w");
    arqErr = fopen("resultado/saidaErro-teste008.err", "w");

    if (!fonte) {
        printf("ERRO: Arquivo nao encontrado\n");
        return 1;
    }

    if (!arqLex) {
        printf("ERRO: Erro ao criar o arquivo saidaLex.lex\n");
        return 1;
    }

    if (!arqTS) {
        printf("ERRO: Erro ao criar o arquivo saidaTS.ts\n");
        return 1;
    }

    if (!arqErr) {
        printf("ERRO: Erro ao criar o arquivo saidaErro.err\n");
        return 1;
    }

    inicializaTS();

    Token t;
    do {

        t = proximoToken();

        if (t.type != TOKEN_EOF && t.type != TOKEN_ERROR) {
            escreveToken(t);
        }
        
    } while (t.type != TOKEN_EOF);

    escreverTS(arqTS);

    if (temErro == 1) {
        printf("Analise concluida com erros. Veja saidaErro.err");
    } else {
        printf("Analise concluida com sucesso!");
    }

    liberaTS();

    fclose(fonte);
    fclose(arqLex);
    fclose(arqTS);
    fclose(arqErr);

    return 0;
}

