// A ideia dessa comando é pegar o arquivo .bin do trabalho 1 e criar uma árvore de indices para ele

// cada no(pagina de disco) possui 5 descendentes
// cada no, menos a raiz, possui no minimo 3 descendentes
// a raiz possui pelo menos 2 descentes
// todas as folhas aparecem no mesmo nível

#include "command7.h"

int command7(char* data_name, char* index_name) {

    // abrindo arquivo de dados
    FILE* data_file = fopen(data_name, "rb"); // problema na funçao de abrir arquivo TO DO
    if (data_file == NULL) {
        return 0;
    }

    // abrindo arquivo de indices
    FILE* index_file = fopen(index_name, "wb+");
    if (index_file == NULL) {
        return 0;
    }
    

    // lendo header do arquivo de dados
    Header_reg* reg_header = malloc(sizeof(Header_reg));
    read_header(reg_header, data_file);

    // TO DO POSSÍVEIS CASOS QUE O PROGRAMA ACABA NA HORA
    /**
     * STATUS = 0
     * PROX_RRN = 0
    */


   // lendo o primeiro registro dos dados
    Data_reg* data = malloc(sizeof(Data_reg));
    fseek(data_file, LEN_DISC_PAG, SEEK_SET);
    read_register(data_file, data);


    // colocando esse registro lido como raiz
    Node* node = create_node();
    node->folha = '1'; // nao tem filhos
    node->nroChavesNo++; // vai ter uma chave
    node->alturaNo = 0; // ta na parte mais baixo da arvore
    node->RRNdoNo = 0; // ????
    node->key[0]->search_key = data->idConecta; // chave raiz no momento
    node->key[0]->RRN_key = 0; // RRN dessa chave no arquivo de dados


    // criando e escrevendo o header do arquivo de index
    BTHeader* ind_header = create_btheader();
    ind_header->noRaiz = 0;
    ind_header->nroChavesTotal++;
    write_btheader(index_file, ind_header);
    write_node(index_file, node);


    // colocando 1 no status e fechando o arquivo
    ind_header->status = '1';
    update_btheader(index_file, ind_header);

    fclose(data_file);
    fclose(index_file);

    free(reg_header);
    free(data);
    release_node(node);
    release_btheader(ind_header);

    return 1;
}