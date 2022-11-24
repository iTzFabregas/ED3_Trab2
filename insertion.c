#include "insertion.h"


void shift_keys_node(Node* node, int pos) {

    for (int i = 3; i >= pos; i--) {

        if (i != pos) {
            node->ponteiro[i + 1] = node->ponteiro[i];
        }

        if(i != 3) {
            node->key[i + 1] = node->key[i];
        }
    }  
}


void split(Key* local_key, int* local_right_rrn, Node* node, Key* promo_key, int* right_rrn, Node* new_node) {
    
    // CRIA UMA LISTA DE CHAVES E PONTEIROS QUE CABE UM A MAIS QUE O NO
    Key_list* keys = malloc(sizeof(Key_list));
    for (size_t i = 0; i < 5; i++)
    {
        keys->ponteiro[i] = node->ponteiro[i];
        if (i != 4) {
            keys->key[i]= node->key[i];
        }
    }
    keys->key[4] = *(local_key);
    keys->ponteiro[5] = *(local_right_rrn);
    
    
    // ORDENA ESSA LISTA COM A NOVA CHAVE NELA
    for (int j = 0; j < 6; j++)
    {
        if (local_key->search_key <= keys->key[j].search_key) {
            
            for (int i = 4; i >= j; i--) {
                if (i != j) {
                    keys->ponteiro[i+1] = keys->ponteiro[i];
                }
                if (i != 4) {
                    keys->key[i+1] = keys->key[i];
                }
            }
            
            keys->key[j] = *(local_key);
            keys->ponteiro[j+1] = *(local_right_rrn);
            break;

        }
    }

    // ELEMENTO CENTRAL DESSA LISTA SERÁ O PROMO
    *promo_key = keys->key[2]; 

    new_node->alturaNo = node->alturaNo;
    new_node->folha = node->folha;
    new_node->nroChavesNo = 2;

    // COLOCA OS VALORES DEPOIS DO CENTRO NO NOVO NO
    for (size_t i = 0; i < 3; i++)
    {
        new_node->ponteiro[i] = keys->ponteiro[i+3];
        if (i != 2) {
            new_node->key[i]  = keys->key[i+3];
        }
    }


    // DELETA TODOS OS VALORES E COLOCA OS VALORES ANTES DO CENTRO NO NO ATUAL
    delete_keys(node);
    node->nroChavesNo = 2;
    for (size_t i = 0; i < 3; i++)
    {
        node->ponteiro[i] = keys->ponteiro[i];
        if (i != 2) {
            node->key[i] = keys->key[i];
        }
    }
    
        free(keys);
}


int insert(FILE* file, int current_rrn, Key* key, Key* promo_key, int* right_rrn) {

    // SE ESSE RRN NAO EXISITR, RETORNA A CHAVE ATUAL COMO PROMO
    if (current_rrn == -1) {
        *promo_key = *key;
        *right_rrn = -1;
        return DO_PROMOTION;
        
    } else {
        Node* node = create_node();
        fseek(file, LEN_PAGDISC + (current_rrn*LEN_PAGDISC), SEEK_SET);
        read_node(file, node);

        // ACHA A POSIÇÃO CERTA DA CHAVE NESSE NO
        int pos;
        for (size_t i = 0; i < 4; i++)
        {
            if (key->search_key <= node->key[i].search_key || node->key[i].search_key == -1) {
                if (node->key[i].search_key == key->search_key) {
                    printf("Chave já inserida\n");
                    free(node);

                    return EXIT;
                }
                pos = i;
                break;
            }
            pos = 4;
        }

        Key* local_promo = malloc(sizeof(Key));
        int* local_right_rrn = malloc(sizeof(int));
        int return_value = insert(file, node->ponteiro[pos], key, local_promo, local_right_rrn);

        // SE NAO PRECISAR DE PROMOÇÃO OU TIVER RETORNADO ERRO, SAIR DA RECURSSÃO
        if (return_value == EXIT || return_value == NO_PROMOTION) {
            free(node);
            free(local_promo);
            free(local_right_rrn);

            return return_value;

        // SE TIVER ESPAÇO NESSE NO, COLOCAR A CHAVE EM SEU LUGAR CERTO
        } else if (node->nroChavesNo < 4) {

            if (node->key[pos].search_key == -1) {
                node->key[pos] = *local_promo;
                node->ponteiro[pos+1] = *local_right_rrn;
            } else {
                shift_keys_node(node, pos);
                node->key[pos] = *local_promo;
                node->ponteiro[pos+1] = *local_right_rrn;
            }            
            
            node->nroChavesNo++;
            fseek(file, LEN_PAGDISC + (current_rrn*LEN_PAGDISC), SEEK_SET);
            write_node(file, node);

            free(node);
            free(local_promo);
            free(local_right_rrn);

            return NO_PROMOTION;

        // SE NÃO TIVER ESPAÇO NESSE NO, FAZER SPLIT E COLOCAR A CHAVE
        } else {
            Node* new_node = create_node();

            split(local_promo, local_right_rrn, node, promo_key, right_rrn, new_node);
            
            // ESCREVENDO O NO
            fseek(file, LEN_PAGDISC + (current_rrn*LEN_PAGDISC), SEEK_SET);
            write_node(file, node);

            // VERIFICA QUAL PROX RRN LIVRE
            BTHeader* header = create_btheader();
            read_btheader(file, header);

            // ESCREVENDO O NOVO NO
            new_node->RRNdoNo = header->RRNproxNo;
            fseek(file, LEN_PAGDISC + (header->RRNproxNo*LEN_PAGDISC), SEEK_SET);
            write_node(file, new_node);

            // UPDATE NO HEADER COM AS INFOS ATUALIZADAS
            *right_rrn = header->RRNproxNo;
            header->RRNproxNo++;
            update_btheader(file, header);

            free(node);
            free(local_promo);
            free(local_right_rrn);
            free(new_node);
            free(header);

            return DO_PROMOTION;
        }
        
    }
}


int command7(char* data_name, char* index_name) {

    // ABRINDO OS ARQUIVOS DE DADOS
    FILE* data_file = fopen(data_name, "rb");
    if (data_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    Header_reg* reg_header = malloc(sizeof(Header_reg));
    read_header(reg_header, data_file);
    if (reg_header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        free(reg_header);
        fclose(data_file);
        return 0; 
    }

    // ABRINDO OS ARQUIVOS DE INDEXES
    FILE* index_file = fopen(index_name, "wb+");
    if (index_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        free(reg_header);
        fclose(data_file);
        return 0;
    }    

    // SE O ARQUIVO DE DADOS NÃO TIVER REGISTROS, ESCREVER APENAS O HEADER NO ARQUIVO DE INDEXES 
    if (reg_header->proxRRN == 0) {
        BTHeader* ind_header = create_btheader();
        write_btheader(index_file, ind_header);
        fclose(data_file);
        fclose(index_file);
        free(ind_header);
        return 1;
    }

   // LENDO OS REGISTROS ATÉ CHEGAR NO PRIMEIRO REGISTRO NÃO DELETADO
    Data_reg* data = malloc(sizeof(Data_reg));
    Key* data_key = create_key();

    int i = 0;
    for (; i < reg_header->proxRRN; i++) {
        fseek(data_file, LEN_DISC_PAG+(LEN_REG*i), SEEK_SET); 
        if(read_register(data_file, data)) { // se o registro tiver deletado
            break;
        }
    }

    // COLOCANDO ESSE PRIMEIRO REGISTRO LIDO COMO RAIZ
    Node* node = create_node();
    node->folha = '1'; // nao tem filhos
    node->nroChavesNo = 1; // vai ter uma chave
    node->alturaNo = 1; // ta na parte mais baixo da arvore
    node->RRNdoNo = 0; // primeiro RRN do arquivo de indexes
    node->key[0].search_key = data->idConecta; // chave raiz no momento
    node->key[0].RRN_key = i; // RRN dessa chave no arquivo de dados

    // CRIANDO E ESCREVENDO O HEADER DO ARQUIVIO DE INDEX
    BTHeader* ind_header = create_btheader();
    ind_header->noRaiz = 0;
    ind_header->nroChavesTotal = 1;
    ind_header->RRNproxNo = 1;
    ind_header->alturaArvore = 1;
    write_btheader(index_file, ind_header);
    write_node(index_file, node);

    i++;
    for (; i < reg_header->proxRRN; i++) {
        fseek(data_file, LEN_DISC_PAG+(LEN_REG*i), SEEK_SET); 
        if(!read_register(data_file, data)) {
            continue;
        }
        
        data_key->search_key = data->idConecta;
        data_key->RRN_key = i;

        Key* promo_key = malloc(sizeof(Key));
        int* right_rrn = malloc(sizeof(int));

        if (insert(index_file, ind_header->noRaiz, data_key, promo_key, right_rrn) == DO_PROMOTION) {    
            read_btheader(index_file, ind_header);
            Node* new_root = create_node();
            new_root->folha = '0';
            new_root->nroChavesNo = 1;
            new_root->alturaNo = ind_header->alturaArvore + 1;
            new_root->ponteiro[0] = ind_header->noRaiz;
            new_root->key[0] = *promo_key;
            new_root->ponteiro[1] = *right_rrn;

            // ESCREVE A NOVA RAIZ
            new_root->RRNdoNo = ind_header->RRNproxNo;
            fseek(index_file, LEN_PAGDISC + (ind_header->RRNproxNo*LEN_PAGDISC), SEEK_SET);
            write_node(index_file, new_root);

            // UPDATE NO HEADER COM AS INFORMAÇÕES ATUALIZADAS
            read_btheader(index_file, ind_header);
            ind_header->noRaiz = ind_header->RRNproxNo;
            ind_header->RRNproxNo++;
            ind_header->alturaArvore = new_root->alturaNo;
            ind_header->nroChavesTotal++;

            free(new_root);

        } else {
            // LE O HEADER E ATUALIZA
            read_btheader(index_file, ind_header);
            ind_header->nroChavesTotal++;
        }

        update_btheader(index_file, ind_header);
        free(promo_key);
        free(right_rrn);

    }

    // COLOCANDO 1 NO STATUS E ATUALIZANDO
    read_btheader(index_file, ind_header);
    ind_header->status = '1';
    update_btheader(index_file, ind_header);

    fclose(data_file);
    fclose(index_file);

    free(reg_header);
    free(data);
    free(data_key);
    free(node);
    free(ind_header);

    return 1;
}

int command9(char* data_name, char* index_name) {

    // ABRE O ARQUIVO DE DADOS, VERIFICA O STATUS E COLOCA 0 NO STATUS
    FILE* data_file = fopen(data_name, "rb+");
    if (data_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    Header_reg* reg_header = malloc(sizeof(Header_reg));
    read_header(reg_header, data_file);
    if (reg_header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    reg_header->status = '0';
    write_header(data_file,reg_header);

    // ABRE O ARQUIVO DE INDEXES, VERIFICA O STATUS E COLOCA 0 NO STATUS
    FILE* index_file = fopen(index_name, "rb+");
    if (index_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    BTHeader* ind_header = malloc(sizeof(BTHeader));
    read_btheader(index_file, ind_header);
    if (ind_header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    ind_header->status = '0';
    update_btheader(index_file, ind_header);

    // VERIFICA O NUMERO DE INSERÇÕES
    int n;
    scanf("%d", &n);

    Data_reg* new_reg = create_reg();
    Key* new_key = create_key();
    char str[50];
    for (size_t i = 0; i < n; i++) {

        // LE OS HEADERS
        read_header(reg_header, data_file);
        read_btheader(index_file, ind_header);

        // LE TODOS OS INPUTS DO TECLADO DESSA INSERÇÃO
        scanf("%s ", str);
        if(strcmp(str, "NULO") != 0) {
            new_reg->idConecta = atoi(str);
        } else {
            new_reg->idConecta = -1;
        }
        scan_quote_string(new_reg->nomePoPs);
        scan_quote_string(new_reg->nomePais);   
        scan_quote_string(new_reg->siglaPais);

        scanf(" %s", str);
        if(strcmp(str, "NULO") != 0) {
            new_reg->idPoPsConectado = atoi(str);
        } else {
            new_reg->idPoPsConectado = -1;
        }

        scan_quote_string(&new_reg->unidadeMedida);

        scanf(" %s", str);
        if(strcmp(str, "NULO") != 0) {
            new_reg->velocidade = atoi(str);
        } else {
            new_reg->velocidade = -1;
        }
        
        // CRIANDO A CHAVE PARA O ARQUIVO DE INDEXES
        new_key->search_key = new_reg->idConecta;
        new_key->RRN_key = reg_header->proxRRN;

        // PULA PARA O PROXRRN DO ARQUIVO DE DADOS E ESCREVE O NOVO RESGISTRO E ATUALIZA O HEADER
        fseek(data_file, LEN_DISC_PAG+(reg_header->proxRRN*LEN_REG), SEEK_SET);
        write_register(data_file, new_reg);
        reg_header->proxRRN++;
        write_header(data_file, reg_header);


        // FAZ A INSERÇÃO DA CHAVE NO ARQUIVO DE INDEXES
        Key* promo_key = create_key();
        int* right_rrn = malloc(sizeof(int));
        if (insert(index_file, ind_header->noRaiz, new_key, promo_key, right_rrn) == DO_PROMOTION) {
            Node* new_root = create_node();
            new_root->folha = '0';
            new_root->nroChavesNo = 1;
            new_root->alturaNo = ind_header->alturaArvore;
            new_root->ponteiro[0] = ind_header->noRaiz;
            new_root->key[0] = *promo_key;
            new_root->ponteiro[1] = *right_rrn;

            // VERIFICA QUAL O PROXIMO RRN LIVRE
            read_btheader(index_file, ind_header);

            // ESCREVE A NOVA RAIZ
            new_root->RRNdoNo = ind_header->RRNproxNo;
            fseek(index_file, LEN_PAGDISC + (ind_header->RRNproxNo*LEN_PAGDISC), SEEK_SET);
            write_node(index_file, new_root);

            // UPDATE NO HJEADER COM O NOVO PROXIMO RRN
            read_btheader(index_file, ind_header);
            ind_header->noRaiz = ind_header->RRNproxNo;
            ind_header->RRNproxNo++;
            ind_header->alturaArvore = new_root->alturaNo;
            ind_header->nroChavesTotal++;

            free(new_root);
        } else {
            read_btheader(index_file, ind_header);
            ind_header->nroChavesTotal++;
        }

        update_btheader(index_file, ind_header);
        free(promo_key);
        free(right_rrn);
    }

    // ATUALIZA O STATUS DOS ARQUIVOS
    reg_header->status = '1';
    write_header(data_file, reg_header);
    ind_header->status = '1';
    update_btheader(index_file, ind_header);

    free(reg_header);
    free(ind_header);
    free(new_reg);
    free(new_key);
    fclose(data_file);
    fclose(index_file);

    return 1;
}