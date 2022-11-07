#include <stdio.h>
#include <stdlib.h>

#include "btree.h"
#include "print_msg.h"

//aloca memoria e inicializa variaveis para o cabecalho
BTHeader* create_btheader(){
    BTHeader* header = malloc(sizeof(BTHeader));
    header->status = '0';
    header->noRaiz = -1;
    header->nroChavesTotal = 0;
    header->alturaArvore = 0;
    header->RRNproxNo = 0;

    return header;
}

//libera memoria usada no cabecalho
void release_btheader(BTHeader* header){
    free(header);
    header = NULL;
}

//le os campos do cabecalho
int read_btheader(BTHeader* header, FILE* file){
    fread(&header->status, sizeof(char), 1, file);
    if(header->status == '0'){
        error_file();
        return 0; //erro no arquivo    
    }
    
    fread(&header->noRaiz, sizeof(int), 1, file);
    fread(&header->nroChavesTotal, sizeof(int), 1, file);
    fread(&header->alturaArvore, sizeof(int), 1, file);
    fread(&header->RRNproxNo, sizeof(int), 1, file);

    fseek(file, 56, SEEK_CUR); //pula o lixo

    return 1; //sucesso
}

//escreve o cabecalho no arquivo indicado
void write_btheader(FILE* file, BTHeader* header) {
    fseek(file, 0, SEEK_SET);

    fwrite(&header->noRaiz, sizeof(int), 1, file);
    fwrite(&header->nroChavesTotal, sizeof(int), 1, file);
    fwrite(&header->alturaArvore, sizeof(int), 1, file);
    fwrite(&header->RRNproxNo, sizeof(int), 1, file);
    char garbage = GARBAGE;
    for(size_t i = 0; i < 56; i++){
        fwrite(&garbage, sizeof(char), 1, file);
    }
}

Key* create_key(){
    Key* key = malloc(sizeof(Key));

    return key;
}

void release_key(Key* key){
    free(key);
    key = NULL;
}

No* create_no(){
    No* no = malloc(sizeof(No));
    no->nroChavesNo = 0;
    no->alturaNo = 0;

    return no;
}

void release_no(No* no){
    free(no);
    no = NULL;
}