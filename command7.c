// A ideia dessa comando é pegar o arquivo .bin do trabalho 1 e criar uma árvore de indices para ele

// cada no(pagina de disco) possui 5 descendentes
// cada no, menos a raiz, possui no minimo 3 descendentes
// a raiz possui pelo menos 2 descentes
// todas as folhas aparecem no mesmo nível

#include "command7.h"

int recursive_next_node(FILE* index_file, Node* node, Key* data, int node_pai) {

    if (node->folha == '1') { // né ???
        return node_pai;
    }

    for (size_t i = 0; i < 4; i++) {

        if (data->search_key > node->key[i]->search_key && i != 3) {
            continue;
        }
        if (node->ponteiro[i] != -1) {
            node_pai = node->RRNdoNo;
            fseek(index_file, ((node->ponteiro[i]*LEN_PAGDISC) + LEN_PAGDISC), SEEK_SET);
            read_node(index_file, node);
            node_pai = recursive_next_node(index_file, node, data, node_pai);
            break;
        }
    }
    return node_pai;
}

void shift_keys_node(Node* node, int j) {

    for (int i = 2; i >= j; i--) {
        node->key[i + 1] = node->key[i];
    }  
}

Node* split(Key* key, Node* node, Key** promo_key) {
    
    Key* keys[5] = {node->key[0],
                    node->key[1],
                    node->key[2],
                    node->key[3],
                    key};

    for (int i = 0; i < 5; i++)
    {
        if (key->search_key <= keys[i]->search_key) {
            
            for (int j = 3; j >= i; j--) {
                keys[j + 1] = keys[j];
            }
            keys[i] = key;
            break;
        }
    }

    *promo_key = keys[2]; // elemento central do array
    
    Node* new_node = create_node();
    new_node->alturaNo = node->alturaNo;
    new_node->folha = node->folha;
    // node->RRNdoNo = 
    new_node->nroChavesNo = 2;
    // coloca os valores dps do centro para a nova pagina
    for (size_t i = 0; i < 3; i++)
    {
        new_node->ponteiro[i] = node->ponteiro[i+2];
        if (i != 2) {
            new_node->key[i]->search_key = node->key[i+2]->search_key;
            new_node->key[i]->RRN_key = node->key[i+2]->RRN_key;
        }
    }

    node->nroChavesNo = 2;
    
    //deleta os valores dps do centro desse pagina
    for (size_t i = 2; i < 5; i++)
    {
        node->ponteiro[i] = -1;
        if (i != 4) {
            node->key[i]->search_key = -1;
            node->key[i]->RRN_key = -1;
        }
    }

    return new_node;
    
}

int insert(FILE* file, int current_rrn, Key* key, Key** promo_key) {

    // como esse nó nao existe, a chave deverá ficar no nó anterior
    printf("entrou insert\n");
    if (current_rrn == -1) {
        (*promo_key) = key;
        //printf("-> atribuindo a promo_key: %d\n", (*promo_key)->search_key);
        return DO_PROMOTION;
        
    } else {
        Node* node = create_node();
        fseek(file, LEN_PAGDISC + (current_rrn*LEN_PAGDISC), SEEK_SET);
        read_node(file, node);
        

        int pos;
        for (size_t i = 0; i < 4; i++)
        {
            if (key->search_key <= node->key[i]->search_key || node->key[i]->search_key == -1) {
                if (node->key[i]->search_key == key->search_key) {
                    printf("Chave já inserida\n");
                    return EXIT;
                }
                pos = i;
                break;
            }
            pos = 4;
        }

        Key** local_promo = malloc(sizeof(Key*));
        int return_value = insert(file, node->ponteiro[pos], key, local_promo);
        //printf("valor local_promo: %d\n", *local_promo->search_key);
        if (return_value == EXIT, return_value == NO_PROMOTION) {
            return return_value;

        } else if (node->nroChavesNo < 4) {
            printf("tem espaço no nó - %d\n", node->nroChavesNo);
            // insert P_B_KEY and P_B_RRN in PAGE
            if (node->key[pos]->search_key == -1) {
                node->key[pos] = *local_promo;
            } else {
                shift_keys_node(node, pos);
                node->key[pos] = *local_promo;
            }            

            node->nroChavesNo++;
            fseek(file, LEN_PAGDISC + (current_rrn*LEN_PAGDISC), SEEK_SET);
            write_node(file, node);

            //printf("5.2\n");
            return NO_PROMOTION;
        } else {
            printf("nao tem espaço no nó, entra split\n");
            Node* new_node = split(key, node, local_promo);
            printf("sai split\n");
            fseek(file, LEN_PAGDISC + (current_rrn*LEN_PAGDISC), SEEK_SET);
            write_node(file, node);

            // verifica qual o proximo rrn livre
            BTHeader* header = malloc(sizeof(BTHeader));
            read_btheader(file, header);

            // escreve o novo no
            fseek(file, LEN_PAGDISC + (header->RRNproxNo*LEN_PAGDISC), SEEK_SET);
            write_node(file, new_node);

            // update no header com o novo proximo rrn
            header->RRNproxNo++;
            update_btheader(file, header);

            free(node);
            free(new_node);
            free(header);

            return DO_PROMOTION;
        }
        
    }
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
    Key* data_key = create_key();
    char aux;
    fseek(data_file, LEN_DISC_PAG, SEEK_SET);
    read_register(data_file, data);

    // colocando esse primeiro registro lido como raiz
    Node* node = create_node();
    node->folha = '1'; // nao tem filhos
    node->nroChavesNo = 1; // vai ter uma chave
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

    // for (size_t i = 1; i < reg_header->proxRRN; i++) {
    for (size_t i = 1; i < 20; i++) {
        fseek(data_file, LEN_DISC_PAG+(LEN_REG*i), SEEK_SET); 
        if(!read_register(data_file, data)) { // se o registro tiver deletado
            continue;
        }
        data_key->search_key = data->idConecta;
        data_key->RRN_key = i;
        //printf("%d - entrou\n", data_key->search_key);
        Key** promo_key = malloc(sizeof(Key*));
        //(*promo_key)->search_key = -2;
        if (insert(index_file, ind_header->noRaiz, data_key, promo_key) == DO_PROMOTION) {
            printf("entrou driver\n");
            printf("->valor promo_key: %d\n", (*promo_key)->search_key);
            Node* new_root = create_node();
            node->folha = '0';
            node->nroChavesNo = 1; // vai ter uma chave
            node->alturaNo = node->alturaNo++; // ta na parte mais baixo da arvore
            node->key[0] = *promo_key;

            // verifica qual o proximo rrn livre
            BTHeader* header = malloc(sizeof(BTHeader));
            read_btheader(index_file, header);

            // escreve o novo no
            node->RRNdoNo = header->RRNproxNo;
            fseek(index_file, LEN_PAGDISC + (header->RRNproxNo*LEN_PAGDISC), SEEK_SET);
            write_node(index_file, new_root);

            // update no header com o novo proximo rrn
            header->RRNproxNo++;
            update_btheader(index_file, header);
        }

    }

    // colocando 1 no status e fechando o arquivo
    ind_header->status = '1';
    update_btheader(index_file, ind_header);

    fclose(data_file);
    fclose(index_file);

    free(reg_header);
    free(data);
    release_key(data_key);
    release_node(node);
    release_btheader(ind_header);

    return 1;
}