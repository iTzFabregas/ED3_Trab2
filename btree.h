#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "print_msg.h"

#define LEN_PAGDISC 73

//defines de tamanhos bastante usados
#define LEN_FOLHA 1
#define LEN_NROCHAVESNO 4
#define LEN_ALTURANO 4
#define LEN_RRNDONO 4
#define LEN_PONTEIRO 4
#define LEN_SEARCHKEY 4
#define LEN_RRN_KEY 4

//define a estrutura do cabecalho
typedef struct{
    char status;
    int noRaiz;
    int nroChavesTotal;
    int alturaArvore;
    int RRNproxNo;
} BTHeader;

// define estrutura de uma chave e o ponteiro referente
typedef struct{
    int search_key; // IdConecta
    int RRN_key; // RRN no arquivo de dados que essa chave esta localizada
} Key;

//define estutura de um no
typedef struct{
    int ponteiro[5]; // RRN dos nos filhos
    Key* key[4]; // chaves

    char folha;
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;
} Node;

// criação e liberação do header
BTHeader* create_btheader();
void release_btheader(BTHeader* header);

// leitura e escrita do header
int read_btheader(FILE* file, BTHeader* header);
void write_btheader(FILE* file, BTHeader* header);
void update_btheader(FILE* file, BTHeader* header);

// leitura e escrita do no
int read_node(FILE* file, Node* node);
void write_node(FILE* file, Node* node);

// criação e liberação da chave
Key* create_key();
void release_key(Key* key);

// criação e liberação do nó
Node* create_node();
void release_node(Node* node);

#endif