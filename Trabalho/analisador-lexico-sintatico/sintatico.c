#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "symtable.h"
#include "lexer.h"
#include "sintatico.h"

extern FILE *arqErr;

static AST *parse_expr(Parser *p);

static int contadorId = 0;

TokenVec tokenizar() {
    TokenVec v;
    v.data = NULL;
    v.size = 0;
    v.cap = 0;

    for (;;) {
        Token t = proximoToken();

        if (t.type != TOKEN_EOF && t.type != TOKEN_ERROR)
            escreveToken(t);

        if (v.size + 1 > v.cap) {
            v.cap = v.cap ? v.cap * 2 : 16;
            v.data = realloc(v.data, v.cap * sizeof(Token));

            if (!v.data) {
                fprintf(stderr, "Sem memoria para o TokenVec\n");
                exit(1);
            }
        }

        v.data[v.size++] = t;

        if (t.type == TOKEN_EOF) break;
    }

    return v;
}

void tv_free(TokenVec *v) {
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}

AST *ast_criar(const char *rotulo) {
    AST *no = (AST *)calloc(1, sizeof(AST));

    if (!no) {
        fprintf(stderr, "Sem memoria para AST\n");
        exit(1);
    }

    strncpy(no->rotulo, rotulo, 255);
    no->rotulo[255] = '\0';
    no->filhos = NULL;
    no->id = contadorId++;

    return no;
}

void ast_add(AST *pai, AST *filho) {
    if (!filho)
        return;

    pai->filhos = realloc(pai->filhos, (pai->nFilhos + 1) * sizeof(AST *));

    if (!pai->filhos) {
        fprintf(stderr, "Sem memoria para filho AST\n");
        exit(1);
    }

    pai->filhos[pai->nFilhos++] = filho;
}

void ast_free(AST *t) {
    if (!t)
        return;

    for (int i = 0; i < t->nFilhos; i++) {
        ast_free(t->filhos[i]);
    }

    free(t->filhos);
    free(t);
}

void ast_print(const AST *t, int prof) {
    if (!t)
        return;

    for (int i = 0; i < prof; i++) {
        fputs(" ", stdout);
    }

    printf("[%s]\n", t->rotulo);

    for (int i = 0; i < t->nFilhos; i++) {
        ast_print(t->filhos[i], prof + 1);
    }
}

static void escape_dot(const char *src, char *dst, int max) {
    int j = 0;

    for (int i = 0; src[i] && j < max - 1; i++) {
        switch (src[i])
        {
        case '"':
            dst[j++] = '\'';
            break;
        case '<':
            dst[j++] = '[';
            break;
        case '>':
            dst[j++] = ']';
            break;
        case '\\':
            dst[j++] = '/';
            break;
        default:
            dst[j++] = src[i];
            break;
        }
    }
    dst[j] = '\0';
}

static void dot_nos(const AST *t, FILE *f) {
    if (!t)
        return;

    char safe[256];
    escape_dot(t->rotulo, safe, 256);

    if (t->nFilhos == 0) {
        fprintf(f, "  n%d [label=\"%s\", shape=box, "
                   "style=filled, fillcolor=\"#D6EAF8\", "
                   "fontname=\"Arial\"];\n",
                t->id, safe);
    } else {
        fprintf(f, "  n%d [label=\"%s\", shape=ellipse, "
                   "style=filled, fillcolor=\"#D5F5E3\", "
                   "fontname=\"Arial\"];\n",
                t->id, safe);
    }

    for (int i = 0; i < t->nFilhos; i++) {
        dot_nos(t->filhos[i], f);
    }
}

static void dot_arestas(const AST *t, FILE *f) {
    if (!t)
        return;

    for (int i = 0; i < t->nFilhos; i++) {
        fprintf(f, "  n%d -> n%d;\n", t->id, t->filhos[i]->id);
        dot_arestas(t->filhos[i], f);
    }
}

void ast_to_dot(const AST *t, const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) {
        perror("fopen dot");
        return;
    }

    fprintf(f, "digraph AST {\n");
    fprintf(f, "  rankdir=TB;\n");
    fprintf(f, "  node [fontname=\"Arial\", fontsize=11];\n");
    fprintf(f, "  edge [color=\"#555555\"];\n\n");

    dot_nos(t, f);
    dot_arestas(t, f);

    fprintf(f, "}\n");
    fclose(f);

    printf("Arquivo .dot gerado: %s\n", path);
}

static const Token *cur(Parser *p) {
    return (p->i < p->n) ? &p->toks[p->i] : &p->toks[p->n - 1];
}

static int match(Parser *p, TokenType tipo) {
    if (cur(p)->type == tipo) {
        p->i++;
        return 1;
    }
    return 0;
}

static void perr(Parser *p) {
    const Token *t = cur(p);
    if (t->type == TOKEN_EOF) {
        fprintf(stderr, "%d:fim de arquivo nao esperado.\n", t->linha);
    } else {
        fprintf(stderr, "%d:token nao esperado [%s].\n",
                t->linha, t->lexema);
    }
    fflush(arqErr);   
    fclose(arqErr);
    exit(1);
}

static AST *expect(Parser *p, TokenType tipo) {
    const Token *t = cur(p);
    if (t->type == tipo) {
        AST *folha = ast_criar(t->lexema);
        p->i++;
        return folha;
    }
    perr(p);
    return NULL;
}

static AST *parse_tipo(Parser *p) {
    AST *no = ast_criar("tipo");

    if (cur(p)->type == KW_INTEGER) {
        ast_add(no, expect(p, KW_INTEGER));
    } else if (cur(p)->type == KW_REAL) {
        ast_add(no, expect(p, KW_REAL));
    } else {
        perr(p);
    }

    return no;
}

static AST *parse_fator(Parser *p) {
    AST *no = ast_criar("fator");

    if (cur(p)->type == ID) {
        ast_add(no, expect(p, ID));
    } else if (cur(p)->type == NUM_INT || cur(p)->type == NUM_REAL) {
        AST *num = ast_criar(cur(p)->lexema);
        p->i++;
        ast_add(no, num);
    } else if (cur(p)->type == SMB_OPA) {
        ast_add(no, expect(p, SMB_OPA));
        ast_add(no, parse_expr(p));
        ast_add(no, expect(p, SMB_CPA));
    } else {
        perr(p);
    }

    return no;
}

static AST *parse_termo(Parser *p) {
    AST *no = ast_criar("termo");

    ast_add(no, parse_fator(p));

    while (cur(p)->type == OP_MUL || cur(p)->type == OP_DIV) {
        ast_add(no, ast_criar(cur(p)->lexema));
        p->i++;
        ast_add(no, parse_fator(p));
    }

    return no;
}

static AST *parse_expr_simples(Parser *p) {
    AST *no = ast_criar("expr_simples");

    if (cur(p)->type == OP_AD || cur(p)->type == OP_MIN) {
        ast_add(no, ast_criar(cur(p)->lexema));
        p->i++;
    }

    ast_add(no, parse_termo(p));

    while (cur(p)->type == OP_AD || cur(p)->type == OP_MIN) {
        ast_add(no, ast_criar(cur(p)->lexema));
        p->i++;
        ast_add(no, parse_termo(p));
    }

    return no;
}

static AST *parse_expr(Parser *p) {
    AST *no = ast_criar("expr");

    ast_add(no, parse_expr_simples(p));

    TokenType tp = cur(p)->type;
    if (tp == OP_EQ || tp == OP_NE || tp == OP_LT ||
        tp == OP_LE || tp == OP_GT || tp == OP_GE) {
        ast_add(no, ast_criar(cur(p)->lexema));
        p->i++;
        ast_add(no, parse_expr_simples(p));
    }

    return no;
}

static AST *parse_atrib(Parser *p) {
    AST *no = ast_criar("atribuicao");

    ast_add(no, expect(p, ID));
    ast_add(no, expect(p, OP_ASS));
    ast_add(no, parse_expr(p));

    return no;
}

static AST *parse_cmd(Parser *p);

static AST *parse_cmd_composto(Parser *p) {
    AST *no = ast_criar("cmd_composto");

    ast_add(no, expect(p, KW_BEGIN));
    ast_add(no, parse_cmd(p));

    while (cur(p)->type == SMB_SEM) {
        ast_add(no, expect(p, SMB_SEM));
        ast_add(no, parse_cmd(p));
    }

    ast_add(no, expect(p, KW_END));

    return no;
}

static AST *parse_condicional(Parser *p) {
    AST *no = ast_criar("cmd_condicional");

    ast_add(no, expect(p, KW_IF));
    ast_add(no, parse_expr(p));
    ast_add(no, expect(p, KW_THEN));
    ast_add(no, parse_cmd(p));

    if (match(p, KW_ELSE)) {
        ast_add(no, ast_criar("else"));
        ast_add(no, parse_cmd(p));
    }

    return no;
}

static AST *parse_repetitivo(Parser *p) {
    AST *no = ast_criar("cmd_repetitivo");

    ast_add(no, expect(p, KW_WHILE));
    ast_add(no, parse_expr(p));
    ast_add(no, expect(p, KW_DO));
    ast_add(no, parse_cmd(p));

    return no;
}

static AST *parse_cmd(Parser *p) {
    AST *no = ast_criar("cmd");

    switch (cur(p)->type) {
        case ID:
            ast_add(no, parse_atrib(p));
            break;
        case KW_BEGIN:
            ast_add(no, parse_cmd_composto(p));
            break;
        case KW_IF:
            ast_add(no, parse_condicional(p));
            break;
        case KW_WHILE:
            ast_add(no, parse_repetitivo(p));
            break;
        default:
            perr(p);
    }

    return no;
}

static AST *parse_lista_ids(Parser *p) {
    AST *no = ast_criar("lista_ids");

    ast_add(no, expect(p, ID));

    while (cur(p)->type == SMB_COM) {
        ast_add(no, expect(p, SMB_COM));
        ast_add(no, expect(p, ID));
    }

    return no;
}

static AST *parse_decl_var(Parser *p) {
    AST *no = ast_criar("decl_var");

    ast_add(no, parse_lista_ids(p));
    ast_add(no, expect(p, SMB_COL));
    ast_add(no, parse_tipo(p));

    return no;
}

static AST *parse_parte_decl(Parser *p) {
    AST *no = ast_criar("parte_decl");

    if (cur(p)->type == KW_VAR) {
        ast_add(no, expect(p, KW_VAR));
        ast_add(no, parse_decl_var(p));
        ast_add(no, expect(p, SMB_SEM));

        while (cur(p)->type == ID) {
            ast_add(no, parse_decl_var(p));
            ast_add(no, expect(p, SMB_SEM));
        }
    } else {
        ast_add(no, ast_criar("epsilon"));
    }

    return no;
}

static AST *parse_bloco(Parser *p) {
    AST *no = ast_criar("bloco");

    ast_add(no, parse_parte_decl(p));
    ast_add(no, parse_cmd_composto(p));

    return no;
}

static AST *parse_programa(Parser *p) {
    AST *no = ast_criar("programa");

    ast_add(no, expect(p, KW_PROGRAM));
    ast_add(no, expect(p, ID));
    ast_add(no, expect(p, SMB_SEM));
    ast_add(no, parse_bloco(p));
    ast_add(no, expect(p, SMB_DOT));

    return no;
}

AST *parse(TokenVec *tv) {
    Parser p = {tv->data, 0, tv->size};

    AST *raiz = parse_programa(&p);

    if (!match(&p, TOKEN_EOF)) {
        perr(&p);
    }

    return raiz;
}
