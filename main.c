#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "tokens.h"
#include "hash.h"


int linha_atual = 1, aux = 0, tamanho = 10;
int id_tabela = 0; 
char entry;
char *rersevadas[11] = {"char", "int", "real", "bool", "if", "then", "else", "while", "input", "output", "return"};
int tags[11] = {KW_CHAR, KW_INT, KW_REAL, KW_BOOL, KW_IF, KW_THEN, KW_ELSE, KW_LOOP, KW_INPUT, KW_OUTPUT, KW_RETURN};

Table_Hash tabela_simbolos;


struct Token {
    int tag;
    double value;
    char *name;
};

// retorna a linha atual que está sendo lida pelo analisador
int getLineNumber(){
    return linha_atual;
}


// retorna o proximo caractere do arquivo de entrada
char prox_char(FILE *f_in){
    return fgetc(f_in);
}


// ignora os espaços e identações como: ' ', '\n' e '\t'.
// é capaz de atualizar a contagem de linha incrementando linha_atual
void ignorar_espacos(FILE *f_in){

    while(isspace(entry)){
        if (entry == '\n') linha_atual++;
        entry = prox_char(f_in);
    }
}


// ignora linhas de comentário existentes no código como: \\oi \* oie */
// é capaz de atualizar a contagem de linha incrementando linha_atual quando:
// entra na etapa de ignorar comentário multilinha ou chama ignorar_espacos em comentários simples.
void verificar_comentário(FILE *f_in){
    char aux_entry;

    //ignora comentários de linha única verificando se existe "//"
    if (entry == '/'){
        entry =  prox_char(f_in);

        if (entry == '/'){
            while (entry != '\n' && !(feof(f_in))){
                entry = prox_char(f_in);
            }
            printf("Encontrado: comentario simples\n");
            ignorar_espacos(f_in);

        } else if (entry == '*'){
        entry = prox_char(f_in);
        aux_entry = prox_char(f_in);

        printf("Encontrado: comentario multilinha\n");

        while (entry != '*' && aux_entry != '/' && !(feof(f_in))){
            
            aux_entry = prox_char(f_in);

            if (entry == '\n') linha_atual++;
            entry = aux_entry;
        }
        printf("Encontrado: FIM comentario multilinha\n");
        entry = prox_char(f_in);
        entry = prox_char(f_in);

        ignorar_espacos(f_in);
        } else {
            fseek(f_in, -2, SEEK_CUR);
            entry = prox_char(f_in);
        }
        
    } 
}

// grava o token correspondente no arquivo de saída com base em sua tk->tag
void grava_token(FILE *out, struct Token *tk){
    char text[255];
    
    if (aux < linha_atual){
        aux = linha_atual;
        sprintf(text, "Tokens        ||    Lexemas --> linha: %d\n", linha_atual);
        fwrite(text, strlen(text), 1,out);
    }

    switch (tk->tag){
    case 33: 
        sprintf(text, "OP_NEG \t\t\t\t !\n");
        fwrite(text, strlen(text), 1, out);
        break;
    
    case 37:
        sprintf(text, "OP_MOD \t\t\t\t %%\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case 42:
        sprintf(text, "OP_MULT \t\t\t *\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case 43:
        sprintf(text, "OP_SOMA \t\t\t +\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case 45:
        sprintf(text, "OP_SUB \t\t\t\t -\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case 47:
        sprintf(text, "OP_DIV \t\t\t\t \\\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case 60: 
        sprintf(text, "OP_MENOR \t\t\t <\n");
        fwrite(text, strlen(text), 1, out);
        break;
        
    case 61:
        sprintf(text, "OP_ATRIB \t\t\t =\n");
        fwrite(text, strlen(text), 1, out);
        break;
    
    case 62:
        sprintf(text, "OP_MAIOR \t\t\t >\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_CHAR:
        sprintf(text, "KW_CHAR \t\t\t char\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_INT:
        sprintf(text, "KW_INT \t\t\t\t int\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_REAL:
        sprintf(text, "KW_REAL \t\t\t real\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_BOOL:
        sprintf(text, "KW_BOOL \t\t\t bool\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_IF:
        sprintf(text, "KW_IF \t\t\t\t if\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_THEN:
        sprintf(text, "KW_THEN \t\t\t then\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_ELSE:
        sprintf(text, "KW_ELSE \t\t\t else\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_LOOP:
        sprintf(text, "KW_LOOP \t\t\t loop\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_INPUT:
        sprintf(text, "KW_INPUT \t\t\t input\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_OUTPUT:
        sprintf(text, "KW_OUTPUT \t\t\t output\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case KW_RETURN:
        sprintf(text, "KW_RETURN \t\t\t return\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case OPERATOR_LE:
        sprintf(text, "OP_LE \t\t\t\t <=\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case OPERATOR_GE:
        sprintf(text, "OP_GE \t\t\t\t >=\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case OPERATOR_EQ:
        sprintf(text, "OP_EQ \t\t\t\t ==\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case OPERATOR_DIF:
        sprintf(text, "OP_DIF \t\t\t\t !=\n");
        fwrite(text, strlen(text), 1, out);
        break;

    case TK_IDENTIFIER: 
        sprintf(text, "TK_ID \t\t\t\t %s\n", tk->name);
        fwrite(text, strlen(text), 1, out);
        break;

    case LIT_INT:
        sprintf(text, "LIT_INT \t\t\t %d\n", (int) tk->value);
        fwrite(text, strlen(text), 1, out);
        break;
    
    case LIT_REAL:
        sprintf(text, "LIT_REAL \t\t\t %.5lf\n", tk->value);
        fwrite(text, strlen(text), 1, out);
        break;

    default:
        break;
    }
}


// verifica se a palavra passada está presenta na tabela hash
int verifica_reservada(char *palavra){
    int index = cria_chave(palavra) % tamanho;
    if (tabela_simbolos[index] != NULL){
        if (busca_lista(tabela_simbolos[index], cria_chave(palavra)) == 0){
            return 0;
        }
    }
    return 1;
}


// verifica o tipo de operador a ser tokenizado e passa para grava_token()
struct Token *verificar_operadores(FILE *f_in, FILE *f_out){
    struct Token *tk = (struct Token *)malloc(sizeof(struct Token));
    char entry_aux = entry;

    switch (entry){
    case '=':
        entry = prox_char(f_in);
        if (entry == '='){
            tk->tag = OPERATOR_EQ;
            grava_token(f_out, tk);
            return tk;
        } else {
            tk->tag = entry_aux;
            grava_token(f_out, tk);

            if (feof(f_in) == 0){
                fseek(f_in, -1, SEEK_CUR);
            }

            return tk;
        }

    case '<':
        entry = prox_char(f_in);
        if (entry == '='){
            tk->tag = OPERATOR_LE;
            grava_token(f_out, tk);
            return tk;
        } else {
            tk->tag = entry_aux;
            grava_token(f_out, tk);

            if (feof(f_in) == 0){
                fseek(f_in, -1, SEEK_CUR);
            }

            return tk;
        }
    
    case '>':
        entry = prox_char(f_in);
        if (entry == '='){
            tk->tag = OPERATOR_GE;
            grava_token(f_out, tk);
            return tk;
        } else {
            tk->tag = entry_aux;
            grava_token(f_out, tk);

            if (feof(f_in) == 0){
                fseek(f_in, -1, SEEK_CUR);
            }
            
            return tk;
        }

    case '!':
        entry = prox_char(f_in);
        if (entry == '='){
            tk->tag = OPERATOR_DIF;
            grava_token(f_out, tk);
            return tk;
        } else {
            tk->tag = entry_aux;
            grava_token(f_out, tk);

            if (feof(f_in) == 0){
                fseek(f_in, -1, SEEK_CUR);
            }
            
            return tk;
        }

    case '/':
        verificar_comentário(f_in);
        tk->tag = entry_aux;
        grava_token(f_out, tk);
        return tk;

    default:
        tk->tag = entry_aux;
        grava_token(f_out, tk);
        return tk;

    }
    
    return NULL;
    
}
 

// verifica o tipo de digito a ser tokenizado (int ou real) e passa para grava_token()
// é capaz de atualizar a contagem de linha incrementando linha_atual chamando ignorar_espaco()
struct Token *verificar_digitos(FILE *f_in, FILE *f_out){
    double value = 0, temp = 0.1;

    struct Token *tk = (struct Token *)malloc(sizeof(struct Token));

    if(isdigit(entry)){
        while (isdigit(entry)){
		    value = 10 * value + (entry - '0');
            entry = prox_char(f_in);

            if (entry == '.'){
                entry = prox_char(f_in);
                while(isdigit(entry)){
                    value = value + temp * (entry - '0');
                    temp = temp / 10;
                    entry = prox_char(f_in);
                }
                tk->tag = LIT_REAL;
                tk->value = value;
                grava_token(f_out, tk);

                if (feof(f_in) == 0){
                    fseek(f_in, -1, SEEK_CUR);
                }

                return tk;
            }
        }
        tk->tag = LIT_INT;
        tk->value = value;
        grava_token(f_out, tk);
        
        if (feof(f_in) == 0){
            fseek(f_in, -1, SEEK_CUR);
        }
        
        return tk;
    }
    return NULL;
};


// verifica se é um identificador ou uma palavra reservada e passa para grava_token()
// isso aqui tá uma gambiarra da porra, tem que provavelmente ajeitar a estrutura do hash
struct Token *verificar_identificador(FILE *f_in, FILE *f_out){
    int word_cap = 10, word_size = 0;

    struct Token *tk = (struct Token *)malloc(sizeof(struct Token));
    tk->name = (char *)malloc(sizeof(word_cap*sizeof(char)));

    if(isalpha(entry)){
        while (isalpha(entry)){
            if (word_size >= word_cap){
                word_cap += 10;  // Aumente a capacidade em incrementos de 10
                tk->name = (char *)realloc(tk->name, word_cap * sizeof(char));

                if (tk->name == NULL) {
                    fprintf(stderr, "Erro na alocação de memória\n");
                    exit(EXIT_FAILURE);
                }
            }
            tk->name[word_size++] = entry;
            entry = prox_char(f_in);
        }
        
        if (verifica_reservada(tk->name) == 0){
            int tag = getTagByKey(tabela_simbolos, 10, tk->name);
            int index = cria_chave(tk->name);

            switch (tag){
            case KW_CHAR:
                tk->tag = KW_CHAR;
                break;

            case KW_INT:
                tk->tag = KW_INT;
                break;
            
            case KW_REAL:
                tk->tag = KW_REAL;
                break;
            
            case KW_BOOL:
                tk->tag = KW_BOOL;
                break;
            
            case KW_IF:
                tk->tag = KW_IF;
                break;
            
            case KW_THEN:
                tk->tag = KW_THEN;
                break;
            
            case KW_ELSE:
                tk->tag = KW_ELSE;
                break;
                
            case KW_LOOP:
                tk->tag = KW_LOOP;
                break;
             
            case KW_INPUT:
                tk->tag = KW_INPUT;
                break;
            
            case KW_OUTPUT:
                tk->tag = KW_OUTPUT;
                break;
            
            case KW_RETURN:
                tk->tag = KW_RETURN;
                break;
            
            default:
                tk->tag = TK_IDENTIFIER;
                break;
            }

            grava_token(f_out, tk);

        } else {
            tk->tag = TK_IDENTIFIER;
            tabela_hash_adicionar(tabela_simbolos, cria_reservada(tk->name, tk->tag, id_tabela), tamanho);
            id_tabela++;
            grava_token(f_out, tk);
        }

        if (feof(f_in) == 0){
            fseek(f_in, -1, SEEK_CUR);
        }
        
        return tk;
    }
    return NULL;
}


void analex(FILE *f_in, FILE *f_out){

    struct Token *tk = NULL;
        
    ignorar_espacos(f_in);
    verificar_comentário(f_in);

    tk = verificar_operadores(f_in, f_out);
    tk = verificar_digitos(f_in, f_out);
    tk = verificar_identificador(f_in, f_out);
}

void main(){
    
    FILE *f_in = fopen("t.txt", "r");
    FILE *f_out = fopen("s.txt", "w");

    tabela_simbolos = tabela_hash_criar(tamanho);

    for (size_t i = 0; i < 11; i++){
        tabela_hash_adicionar(tabela_simbolos, cria_reservada(rersevadas[i], tags[i], id_tabela), tamanho);
        id_tabela++;
    }

    while (feof(f_in) == 0){
        entry = prox_char(f_in);
        analex(f_in, f_out);
    }
    
}