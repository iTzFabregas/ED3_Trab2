#ifndef __BTREE_H__
#define __BTREE_H__

#include "register.h"

#define LEN_PAGDISC 73

//define a estrutura do cabecalho
typedef struct{
    char status;
    int noRaiz;
    int nroChavesTotal;
    int alturaArvore;
    int RRNproxNo;
} BTHeader;

typedef struct{
    int search_key; // IdConecta
    int RRN_key; // RRN no arquivo de dados que essa chave esta localizada
} Key;

typedef struct{
    No* p[5]; // ponteiros
    Key* key[4]; // chaves

    char folha;
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;
} No;

// criação e liberação do header
BTHeader* create_btheader();
void release_btheader(BTHeader* header);

// leitura e escrita do header
int read_btheader(FILE* file, BTHeader* header);
void write_btheader(FILE* file, BTHeader* header);

// leitura e escrita do no
int read_node(FILE* file, No* node);
int write_node(FILE* file, No* node);

// criação e liberação da chave
Key* create_key();
void release_key(Key* key);

// criação e liberação do nó
No* create_no();
void release_no(No* no);

#endif