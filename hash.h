typedef struct reservada Reservada;
struct reservada{
    int chave;
    char reservada[100];
};
typedef struct lista T_lista;
struct lista{ 
    Reservada *reservada;
    T_lista *proximo;
};
typedef T_lista ** Table_Hash;
int cria_chave(char * dado);
Reservada *cria_reservada(char* palavra, int tag, int id);
bool lista_adicionar_inicio(T_lista ** lista, Reservada *reservada);
void lista_listar(T_lista *lista, int pos);
Table_Hash tabela_hash_criar(int tamanho);
bool tabela_hash_adicionar(Table_Hash tabela_hash, Reservada *reser, int tamanho);
void tabela_hash_listar(Table_Hash tabela_hash, int tamanho);
bool busca_lista(T_lista *lista, int chave);
int getTagByKey(Table_Hash tabela_hash, int tamanho, char* palavra);
