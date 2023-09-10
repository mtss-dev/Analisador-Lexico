#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "tokens.h"
#include "hash.h"

// Variaveis globais
int linha_atual = 1, aux = 0, tamanho = 10;
bool jumpLine = false;
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

// retorna o proximo caractere do arquivo de entrada
char prox_char(FILE *f_in){
    return fgetc(f_in);
}

// função auxiliar para remover os zeros desnecessários de um número real
char *remove_zeros_and_concat(double value, const char *str) {
    char num_str[50];
    snprintf(num_str, sizeof(num_str), "%g", value);
    size_t len = strlen(num_str) + strlen(str) + 1;
    char *result = (char *)malloc(len);
    if (result == NULL) {
        return NULL;
    }
    strcpy(result, num_str);
    strcat(result, str);
    return result;
}

// ignora os espaços e identações como: ' ', '\n' e '\t'.
// é capaz de atualizar a contagem de linha incrementando linha_atual
void ignorar_espacos(FILE *f_in){

    while(isspace(entry)){
        if (entry == '\n') linha_atual++;
        entry = prox_char(f_in);
    }
}

// grava o token correspondente no arquivo de saída com base em sua tk->tag
void grava_token(FILE *out, struct Token *tk){
    char text[255];
    
    if (aux < linha_atual){
        aux = linha_atual;
        if(linha_atual > 1 && jumpLine){
            sprintf(text, "\n");
            fwrite(text, strlen(text), 1,out);
        }
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
        jumpLine = true;
        break;
    
    case 38:
        sprintf(text, "OP_AND \t\t\t\t &\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 40:
        sprintf(text, "SG_ABREPAR \t\t\t (\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;
    
    case 41:
        sprintf(text, "SG_FECHAPAR \t\t )\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 42:
        sprintf(text, "OP_MULT \t\t\t *\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 43:
        sprintf(text, "OP_SOMA \t\t\t +\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 44:
        sprintf(text, "SG_COMMA \t\t\t ,\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 45:
        sprintf(text, "OP_SUB \t\t\t\t -\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 47:
        sprintf(text, "OP_DIV \t\t\t\t /\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 59:
        sprintf(text, "SG_SEMICOLON \t\t ;\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 60: 
        sprintf(text, "OP_MENOR \t\t\t <\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;
        
    case 61:
        sprintf(text, "OP_ATRIB \t\t\t =\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;
    
    case 62:
        sprintf(text, "OP_MAIOR \t\t\t >\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;
    
    case 91:
        sprintf(text, "SG_ABRECOL \t\t\t [\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;
    
    case 93:
        sprintf(text, "SG_FECHACOL \t\t ]\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 123:
        sprintf(text, "SG_ABRECHV \t\t\t {\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 124:
        sprintf(text, "OP_OR \t\t\t\t |\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 125:
        sprintf(text, "SG_FECHACHV \t\t }\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case 126:
        sprintf(text, "OP_NOT \t\t\t\t ~\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
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
        jumpLine = true;
        break;

    case OPERATOR_GE:
        sprintf(text, "OP_GE \t\t\t\t >=\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
        break;

    case OPERATOR_EQ:
        sprintf(text, "OP_EQ \t\t\t\t ==\n");
        fwrite(text, strlen(text), 1, out);
        jumpLine = true;
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

    case LIT_CHAR:
        sprintf(text, "LIT_CHAR \t\t\t %s\n", tk->name);
        fwrite(text, strlen(text), 1, out);
        break;

    case LIT_STRING:
        sprintf(text, "LIT_STRING \t\t\t %s\n", tk->name);
        fwrite(text, strlen(text), 1, out);
        break;

    default:
        jumpLine = true;
        break;
    }
}

// ignora linhas de comentário existentes no código como: \\oi \* oie */
// é capaz de atualizar a contagem de linha incrementando linha_atual quando:
// entra na etapa de ignorar comentário multilinha e quando encontra um '\n'
bool verificar_comentário(FILE *f_in){
    char aux_entry;
    int flag = 0;
    bool encontrou = false, multi = false;

    //ignora comentários de linha única verificando se existe "//"
    if (entry == '\\'){
        entry =  prox_char(f_in);

        if (entry == '\\'){
            encontrou = true;
            entry = prox_char(f_in);

            if (entry == '\\'){
                multi = true;
                encontrou = true;
                entry = prox_char(f_in);
                aux_entry = prox_char(f_in);

                //printf("Encontrado: comentario multilinha\n");

                while (flag != 2 && !(feof(f_in))){
                    
                    if (aux_entry == '\n') linha_atual++;

                    if(entry == '/' && aux_entry == '/'){
                        flag++;
                    }

                    entry = aux_entry;
                    aux_entry = prox_char(f_in);
                }
                //printf("Encontrado: FIM comentario multilinha\n");
                entry = aux_entry;
                //ignorar_espacos(f_in);
                //fseek(f_in, -1, SEEK_CUR);
            }

            if (multi == false){
                while (entry != '\n' && !(feof(f_in))){
                    entry = prox_char(f_in);
                }
                //printf("Encontrado: comentario simples\n");
                //ignorar_espacos(f_in);
                //fseek(f_in, -1, SEEK_CUR);
            }

        } else {
            fseek(f_in, -1, SEEK_CUR);
            entry = prox_char(f_in);
        }
        
    }
    return encontrou; 
}

//funcao para verificar se entry é um caractere especial (,;()[]{}=+-*/%<>&|~)
bool caractere_especial(char entry){
    switch (entry){
    case '(':
        return true;
        break;
    
    case ')':
        return true;
        break;
    
    case '[':
        return true;
        break;
    
    case ']':
        return true;
        break;
    
    case '{':
        return true;
        break;
    
    case '}':
        return true;
        break;
    
    case ';':
        return true;
        break;
    
    case '=':
        return true;
        break;
    
    case '+':
        return true;
        break;
    
    case '-':
        return true;
        break;
    
    case '*':
        return true;
        break;
    
    case '/':
        return true;
        break;
    
    case '%':
        return true;
        break;
    
    case '<':
        return true;
        break;
    
    case '>':
        return true;
        break;
    
    case '&':
        return true;
        break;
    
    case '|':
        return true;
        break;
    
    case '~':
        return true;
        break;

    case ',':
        return true;
        break;
    
    default:
        return false;
        break;
    }
}

// verifica se o identificador é válido
bool identificador_invalido(char entry, FILE *f_in){
    return (!isalpha(entry) && !isspace(entry) && feof(f_in) == 0 && !caractere_especial(entry)) ? true : false;
}

// verifica se o número é válido
bool numero_invalido(char entry, FILE *f_in){
    return (!isdigit(entry) && !isspace(entry) && feof(f_in) == 0 && entry != ';') ? true : false;
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
        if(verificar_comentário(f_in)){
            return NULL;
        } else {
            tk->tag = entry_aux;
            grava_token(f_out, tk);
            return tk;
        }

    default:
        if (!(feof(f_in)) && caractere_especial(entry)){
            tk->tag = entry_aux;
            grava_token(f_out, tk);
            
            return tk;
        }
        return NULL;
    }
}

// Função que deve ser chamada para imprimir uma mensagem de erro e finalizar o analisador
void erro (FILE *f_out, struct Token *tk){
    grava_token(f_out, tk);
    fprintf(f_out,"TOKEN_ERROR \t\t %s\n", tk->name);
    printf("Erro léxico na linha %d, verificar o arquivo de saída\n", getLineNumber());
    exit(EXIT_FAILURE);
}
 
// verifica o tipo de digito a ser tokenizado (int ou real) e passa para grava_token()
// é capaz de atualizar a contagem de linha incrementando linha_atual chamando ignorar_espaco()
struct Token *verificar_digitos(FILE *f_in, FILE *f_out){
    double value = 0, temp = 0.1;
    bool isReal = false;

    struct Token *tk = (struct Token *)malloc(sizeof(struct Token));

    if(isdigit(entry)){
        while (isdigit(entry)){
		    value = 10 * value + (entry - '0');
            entry = prox_char(f_in);

            //Se encontrar uma letra no meio do número, é um erro
            if (entry == '.'){
                entry = prox_char(f_in);
                while(isdigit(entry)){
                    value = value + temp * (entry - '0');
                    temp = temp / 10;
                    entry = prox_char(f_in);
                }
                isReal = true;
            }

            if(numero_invalido(entry, f_in) && !caractere_especial(entry)){
                tk->name = (char *)malloc(sizeof(char));
                //continua a leitura até encontrar um espaço salvando o identificador completo
                while (isspace(entry) == 0 && feof(f_in) == 0 && entry != ';'){
                    sprintf(tk->name, "%s%c",tk->name ,entry);
                    entry = prox_char(f_in);
                }
                
                //coloca o value no inicio do nome
                char *aux = (char *)malloc(sizeof(char));
                if(isReal){
                    tk->name = remove_zeros_and_concat(value, tk->name);
                } else {
                    sprintf(aux, "%d", (int) value);
                    strcat(aux, tk->name);
                    tk->name = aux;
                }
                erro(f_out, tk);
            }
        }
        tk->tag = isReal ? LIT_REAL : LIT_INT;
        tk->value = value;
        jumpLine = true;    
        grava_token(f_out, tk);
        
        if (feof(f_in) == 0){
            fseek(f_in, -1, SEEK_CUR);
        }
        
        return tk;
    }
    return NULL;
};

// verifica se é um identificador ou uma palavra reservada e passa para grava_token()
struct Token *verificar_identificador(FILE *f_in, FILE *f_out){
    int word_cap = 10, word_size = 0;

    struct Token *tk = (struct Token *)malloc(sizeof(struct Token));
    tk->name = (char *)malloc(sizeof(word_cap*sizeof(char)));

    if(isalpha(entry)){
        while (isalpha(entry) || entry == '_' || entry == '.'){
            if (word_size >= word_cap){
                word_cap += 10;  
                tk->name = (char *)realloc(tk->name, word_cap * sizeof(char));

                if (tk->name == NULL) {
                    fprintf(stderr, "Erro na alocação de memória\n");
                    exit(EXIT_FAILURE);
                }
            }
            tk->name[word_size++] = entry;
            entry = prox_char(f_in);
        }
        //printf("\n");
        //Se encontrar um digito no meio do identificador, é um erro
        if(identificador_invalido(entry, f_in)){
            while(isspace(entry) == 0 && feof(f_in) == 0 && entry != ';'){
                if (word_size >= word_cap){
                    word_cap += 10;  
                    tk->name = (char *)realloc(tk->name, word_cap * sizeof(char));

                    if (tk->name == NULL) {
                        fprintf(stderr, "Erro na alocação de memória\n");
                        exit(EXIT_FAILURE);
                    }
                }
                tk->name[word_size++] = entry;
                entry = prox_char(f_in);
            }
            erro(f_out, tk);
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
            jumpLine = true;
            grava_token(f_out, tk);
        }

        if (feof(f_in) == 0){
            fseek(f_in, -1, SEEK_CUR);
        }
        
        return tk;
    }
    return NULL;
}

struct Token *verificar_caractere(FILE *f_in, FILE *f_out){
    struct Token *tk = (struct Token *)malloc(sizeof(struct Token));

    if(entry == 39){
        tk->name = (char *)malloc(sizeof(char));
        entry = prox_char(f_in);
        tk->name[0] = entry;

        entry = prox_char(f_in);
        if (entry != 39){
            // continua a leitura até encontrar um espaço salvando o identificador completo
            while (isspace(entry) == 0 && feof(f_in) == 0 && entry != ';'){
                sprintf(tk->name, "%s%c",tk->name ,entry);
                entry = prox_char(f_in);
            }
            grava_token(f_out, tk);
            fprintf(f_out,"TOKEN_ERROR \t\t \'%s\n", tk->name);
            exit(EXIT_FAILURE);

        }
        tk->tag = LIT_CHAR;
        jumpLine = true;
        grava_token(f_out, tk);
        return tk;
    }

    return NULL;
}

struct Token *verificar_string(FILE *f_in, FILE *f_out){
    int word_cap = 10, word_size = 0;

    struct Token *tk = (struct Token *)malloc(sizeof(struct Token));
    tk->name = (char *)malloc(sizeof(word_cap*sizeof(char)));

    if(entry == 34){
        entry = prox_char(f_in);
        while (entry != 34){

            if (entry == '\n' || feof(f_in) == 1){
                //printf("erro! não finalizou a string\n");
                erro(f_out, tk);
                //fprintf(f_out,"TOKEN_ERROR \t\t \"%s\n", tk->name);
                exit(EXIT_FAILURE);
            }

            if (word_size >= word_cap){
                word_cap += 10;  
                tk->name = (char *)realloc(tk->name, word_cap * sizeof(char));

                if (tk->name == NULL) {
                    fprintf(stderr, "Erro na alocação de memória\n");
                    exit(EXIT_FAILURE);
                }
            }
            tk->name[word_size++] = entry;
            entry = prox_char(f_in);
        }
        
        tk->tag = LIT_STRING;
        jumpLine = true;
        grava_token(f_out, tk);
        return tk;
    }

    return NULL;
   
}

void analex(FILE *f_in, FILE *f_out){

    struct Token *tk = NULL;
    
    ignorar_espacos(f_in);
    
    while (entry == '\\'){
        verificar_comentário(f_in);
        ignorar_espacos(f_in);
    }

    tk = verificar_operadores(f_in, f_out);
    tk = verificar_identificador(f_in, f_out);
    tk = verificar_digitos(f_in, f_out);
    tk = verificar_caractere(f_in, f_out);
    tk = verificar_string(f_in, f_out);
}

void main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s entrada.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    const char *nome_arquivo_entrada = argv[1];
    const char *nome_arquivo_saida = "saida.txt";

    FILE *f_in = fopen(nome_arquivo_entrada, "r");
    if (f_in == NULL) {
        perror("Erro ao abrir arquivo de entrada");
        exit(EXIT_FAILURE);
    }

    FILE *f_out = fopen(nome_arquivo_saida, "w");
    if (f_out == NULL) {
        perror("Erro ao abrir arquivo de saída");
        fclose(f_in);
        exit(EXIT_FAILURE);
    }

    tabela_simbolos = tabela_hash_criar(tamanho);

    for (size_t i = 0; i < 11; i++){
        tabela_hash_adicionar(tabela_simbolos, cria_reservada(rersevadas[i], tags[i], id_tabela), tamanho);
        id_tabela++;
    }

    while (feof(f_in) == 0){
        entry = prox_char(f_in);
        analex(f_in, f_out);
    }

    printf("Análise léxica concluída com sucesso!\n");

    fclose(f_in);
    fclose(f_out);   
}