#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct reservada Reservada;
typedef struct lista T_lista;
typedef T_lista ** Table_Hash;


//char *rersevadas[16] = {"void", "int", "float", "char", "bool", "if", "else", "for", "while", "do", "return", "break", "continue", "goto", "true", "false"};


struct reservada{
    int chave;
    int tag;
    int id;
    char reservada[1000];
};



struct lista{ 
    Reservada *reservada;
    T_lista *proximo;
};

int cria_chave(char * dado){
    int pesos[] = {19, 17, 11, 13, 23, 31};
    int soma = 0;
    for (int i = 0; i < strlen(dado); i++){
        if (i < 6){
            soma = soma + dado[i] * pesos[i];
        }else{
            soma = soma + dado[i];
        }
        
    }
    return soma;
}

Reservada *cria_reservada(char* palavra, int tag, int id){
    Reservada *r = malloc(sizeof(Reservada));
    r->chave = cria_chave(palavra);
    r->tag = tag;
    r->id = id;
    
    strcpy(r->reservada, palavra);
    return r;

}


bool busca_lista(T_lista *lista, int chave){
    for (T_lista *i = lista; i != NULL ; i = i->proximo){
        if (chave == i->reservada->chave){
            return 0;
        }
        
    }
    return 1;
    
}


// função para adicionar o elemento no inicio da lista (de acordo com o pdf do professor).
bool lista_adicionar_inicio(T_lista ** lista, Reservada *reservada){
    T_lista *item_novo = malloc(sizeof(T_lista));
    // verifica se foi alocado a memória.
    if (item_novo == NULL) return false;

    // armazena o conteudo no NO e aponta para o inicio da lista (para null caso esteja vazia).
    // Sobrescreve o inicio com ela mesma.
    item_novo->reservada = reservada; 
    item_novo->proximo = *lista; 
    *lista = item_novo;

    return true;

}


// // função para listar os elementos da lista.
void lista_listar(T_lista *lista, int pos){
    // Caminha na lista usando o for iniciando no primeiro item da lista.
    if (lista == NULL){
        return;
    }
    for (T_lista * i = lista; i != NULL; i = i->proximo){
        
        printf("\t--> Elemento (tipo, nome): %s\n", i->reservada->reservada);
    }
    
}

Table_Hash tabela_hash_criar(int tamanho){
    Table_Hash hash = malloc(sizeof(Table_Hash) * tamanho);
    for (int i = 0; i < tamanho; i++){
        hash[i] = NULL;
    }
    return hash;

}

bool tabela_hash_adicionar(Table_Hash tabela_hash, Reservada *reser, int tamanho, int tag, int id){
    int indx = cria_chave(reser->reservada) % tamanho;
    if (tabela_hash[indx] != NULL){
        if (busca_lista(tabela_hash[indx],cria_chave(reser->reservada)) == 0){
            return false;
        }
    }
    
    lista_adicionar_inicio(&tabela_hash[indx], reser);
    return true;
    
}

int getTagByKey(Table_Hash tabela_hash, int tamanho, char* palavra) {
    int chave = cria_chave(palavra);
    int indice = chave % tamanho;
    T_lista *lista = tabela_hash[indice];
    
    while (lista != NULL) {
        if (lista->reservada->chave == chave) {
            return lista->reservada->tag;
        }
        lista = lista->proximo;
    }
    
    return -1; // Retorna -1 se a chave não for encontrada
}


void tabela_hash_listar(Table_Hash tabela_hash, int tamanho){
    for (int i = 0; i < tamanho; i++){
        lista_listar(tabela_hash[i], i);
        //printf("----------------\n");
    }
    
}

