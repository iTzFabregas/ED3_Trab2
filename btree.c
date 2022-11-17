#include "btree.h"

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
int read_btheader( FILE* file, BTHeader* header){
    fread(&header->status, sizeof(char), 1, file);
    if(header->status == '0'){
        // error_file();
        return 0; //erro no arquivo    
    }
    
    fread(&header->noRaiz, sizeof(int), 1, file);
    fread(&header->nroChavesTotal, sizeof(int), 1, file);
    fread(&header->alturaArvore, sizeof(int), 1, file);
    fread(&header->RRNproxNo, sizeof(int), 1, file);

    fseek(file, 49, SEEK_CUR); //pula o lixo

    return 1; //sucesso
}

//escreve o cabecalho no arquivo indicado
void write_btheader(FILE* file, BTHeader* header) {
    fseek(file, 0, SEEK_SET);

    fwrite(&header->status, sizeof(char), 1, file);
    fwrite(&header->noRaiz, sizeof(int), 1, file);
    fwrite(&header->nroChavesTotal, sizeof(int), 1, file);
    fwrite(&header->alturaArvore, sizeof(int), 1, file);
    fwrite(&header->RRNproxNo, sizeof(int), 1, file);
    char garbage = GARBAGE;
    for(size_t i = 0; i < 49; i++){
        fwrite(&garbage, sizeof(char), 1, file);
    }
}

void update_btheader(FILE* file, BTHeader* header) {
    fseek(file, 0, SEEK_SET);

    fwrite(&header->status, sizeof(char), 1, file);
    fwrite(&header->noRaiz, sizeof(int), 1, file);
    fwrite(&header->nroChavesTotal, sizeof(int), 1, file);
    fwrite(&header->alturaArvore, sizeof(int), 1, file);
    fwrite(&header->RRNproxNo, sizeof(int), 1, file);
}


int read_node(FILE* file, Node* node) { // verificar se funciona
    fread(&node->folha, LEN_FOLHA, 1, file);
    fread(&node->nroChavesNo, LEN_NROCHAVESNO, 1, file);
    fread(&node->alturaNo, LEN_ALTURANO, 1, file);
    fread(&node->RRNdoNo, LEN_RRNDONO, 1, file);
    for (size_t i = 0; i < 5; i++)
    {
        fread(&node->ponteiro[i], LEN_PONTEIRO, 1, file);
        if (i != 4) {
            fread(&node->key[i]->search_key, LEN_SEARCHKEY, 1, file);
            fread(&node->key[i]->RRN_key, LEN_RRN_KEY, 1, file);
        }
    }
}

void write_node(FILE* file, Node* node) {
    fwrite(&node->folha, LEN_FOLHA, 1, file);
    fwrite(&node->nroChavesNo, LEN_NROCHAVESNO, 1, file);
    fwrite(&node->alturaNo, LEN_ALTURANO, 1, file);
    fwrite(&node->RRNdoNo, LEN_RRNDONO, 1, file);
    for (size_t i = 0; i < 5; i++)
    {
        fwrite(&node->ponteiro[i], LEN_PONTEIRO, 1, file);
        if (i != 4) {
            fwrite(&node->key[i]->search_key, LEN_SEARCHKEY, 1, file);
            fwrite(&node->key[i]->RRN_key, LEN_RRN_KEY, 1, file);
        }
    }
    
}


Key* create_key(){

    Key* key = malloc(sizeof(Key));
    key->search_key = -1;
    key->RRN_key = -1;
    
    return key;
}

void release_key(Key* key){
    free(key);
    key = NULL;
}


Node* create_node(){
    Node* node= malloc(sizeof(Node));
    node->nroChavesNo = 0;
    node->alturaNo = 0;
    // folha
    // RRn do no

    for (size_t i = 0; i < 5; i++)
    {
        node->ponteiro[i] = -1;
        if (i != 4) {
            node->key[i] = malloc(sizeof(Key*));
            node->key[i]->search_key = -1;
            node->key[i]->RRN_key = -1;
        }
    }

    return node;
}

void release_node(Node* node){

    for (size_t i = 0; i < 4; i++) {
        free(node->key[i]);
        node->key[i] = NULL;
    }
    free(node);
    node = NULL;
}

// PESQUISA do 'X'
/**
 * Olhar o nó raiz
 * Ver se o X está ali, compara com as chaves
 * Ver pelas chaves qual ponteiro deve-se pegar
 * Ver se algumas dessas novas chaves são iguais a X
 * Pegar um novo ponteiro
 * 
 * Fazer isso até achar uma chave igual X 
 * OU
 * Pegar um ponteiro que aponta para NULO
*/

