// A ideia dessa comando é pegar o arquivo .bin do trabalho 1 e criar uma árvore de indices para ele

// cada no(pagina de disco) possui 5 descendentes
// cada no, menos a raiz, possui no minimo 3 descendentes
// a raiz possui pelo menos 2 descentes
// todas as folhas aparecem no mesmo nível

#include "command7.h"

int command7(char* data_name, char* index_name) {

    FILE* data_file = fopen(data_name, "rb");
    if (data_file == NULL) {
        return 0;
    }

    FILE* index_file = fopen(index_name, "wb+");
    if (index_file == NULL) {
        return 0;
    }
    
    Header_reg* reg_header = malloc(sizeof(Header_reg));
    read_header(reg_header, data_file);

    // TO DO POSSÍVEIS CASOS QUE O PROGRAMA ACABA NA HORA
    /**
     * STATUS = 0
     * PROX_RRN = 0
    */

    Data_reg* data = malloc(sizeof(Data_reg));
    fseek(data_file, LEN_DISC_PAG, SEEK_SET);
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
    write_node(index_file, node);

    header->status = '1';
    update_btheader(index_file, header);


    free(data);
    release_btheader(header);
    release_no(node);
    fclose(data_file);
    fclose(index_file);


   

    return 1;
}