#ifndef __BTREE_H__
#define __BTREE_H__

#include "register.h"

#define LEN_BTHEADER 73

//define a estrutura do cabecalho
typedef struct{
    char status;
    int noRaiz;
    int nroChavesTotal;
    int alturaArvore;
    int RRNproxNo;
} BTHeader;

typedef struct{
    int search_key;
    Data_reg* pr;
} Key;

typedef struct{
    No* p[5];
    Key* key[4];

    char folha;
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;
} No;

BTHeader* create_btheader();

void release_btheader(BTHeader* header);

int read_btheader(BTHeader* header, FILE* file);

void write_btheader(FILE* file, BTHeader* header);

Key* create_key();

void release_key(Key* key);

No* create_no();

void release_no(No* no);

#endif