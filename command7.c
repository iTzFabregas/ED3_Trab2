// A ideia dessa comando é pegar o arquivo .bin do trabalho 1 e criar uma árvore de indices para ele

// cada no(pagina de disco) possui 5 descendentes
// cada no, menos a raiz, possui no minimo 3 descendentes
// a raiz possui pelo menos 2 descentes
// todas as folhas aparecem no mesmo nível

#include "command7.h"

void recursive_next_node(FILE* index_file, Node* node, Data_reg* data) {

    if (node->folha == '1') { // né ???
        return;
    }

    for (size_t i = 0; i < 4; i++) {

        if (data->idConecta > node->key[i]->search_key && i != 3) {
            continue;
        }
        if (node->ponteiro[i] != -1) {
            fseek(index_file, ((node->ponteiro[i]*LEN_PAGDISC) + LEN_PAGDISC), SEEK_SET);
            read_node(index_file, node);
            recursive_next_node(index_file, node, data);
            break;
        }
    }
    return;
}

void shift_keys_node(Data_reg* data, Node* node, int j) {

    for (size_t i = 2; i <= j; i++) {
        node->key[i + 1] = node->key[i];
    }  
}

void recursive_spliting(FILE* file, Node* node, Data_reg* data) {
    // descobrir qual o termo do meio
    // cria um novo nó para alocar as 2 maiores chaves
    // nesse node atual, vai ficar as 2 menores chaves
    // a chave mediana sobe na árvore (necessário manter o RRN do no pai em registro)
    // ver se cabe ele nesse no (chaves !-= 4)
    // se couber, colocar ele na ordem e return
    // se nao, chamar a função recursive_spliting
}


// colocar 3 /// aparece
/// @brief 
/// @param data_name 
/// @param index_name 
/// @return 
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
    ind_header->noRaiz = 0; // ignorando o header
    ind_header->nroChavesTotal++;
    write_btheader(index_file, ind_header);
    write_node(index_file, node);

    for (size_t i = 0; i < reg_header->proxRRN; i++) {   
        if(!read_register(data_file, data)) { // se o registro tiver deletado
            continue;
        }

        fseek(index_file, ((ind_header->noRaiz*LEN_PAGDISC) + LEN_PAGDISC), SEEK_SET); // vai para a raiz
        read_node(index_file, node);

        recursive_next_node(index_file, node, data); // quando retornar, o node tera a folha na qual a chave melhor se encaixa
        
        if (node->nroChavesNo == 4) {
            // TO DO SPLIT
        } else {
            for (size_t j = 0; j < 4; j++) {
                if (data->idConecta > node->key[j]->search_key) {
                    continue;
                } else {
                    shift_keys_node(data, node, j);
                }
            }
            
        }

    }
    







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