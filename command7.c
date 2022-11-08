// A ideia dessa comando é pegar o arquivo .bin do trabalho 1 e criar uma árvore de indices para ele

// cada no(pagina de disco) possui 5 descendentes
// cada no, menos a raiz, possui no minimo 3 descendentes
// a raiz possui pelo menos 2 descentes
// todas as folhas aparecem no mesmo nível

#include "command7.h"

int command7(char data_name[], char index_name[]) {

    FILE* data_file = open_file(data_name, FILE_READB);
    FILE* index_file = open_file(index_file, FILE_READWRITEB);
    
    Header_reg* reg_header = malloc(sizeof(Header_reg));
    read_header(reg_header, data_file);

    // TO DO POSSÍVEIS CASOS QUE O PROGRAMA ACABA NA HORA
    /**
     * STATUS = 0
     * PROX_RRN = 0
    */

    Data_reg* data = malloc(sizeof(Data_reg));
    fseek(data_file, LEN_PAGDISC, SEEK_SET);
    read_register(data_file, data);

    // no raiz
    No* node = create_no();
    node->folha = '1'; // nao tem filhos
    node->nroChavesNo++; // vai ter uma chave
    node->alturaNo = 0; // ta na parte mais baixo da arvore
    node->RRNdoNo = 0; // ????
    node->key[0]->search_key = data->idConecta; // chave raiz no momento
    node->key[0]->RRN_key = 0; // RRN dessa chave no arquivo de dados


    BTHeader* header = create_btheader();
    header->noRaiz = 0;
    header->nroChavesTotal++;
    write_btheader(index_file, header);

    



   

    return 1;
}