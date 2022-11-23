#include<stdio.h>
#include<string.h>

#include "search.h"
#include "btree.h"
#include "register.h"
#include "funcoesFornecidas.h"

//recebe os inputs do teclado e guarda qual sera o campo usado na busca
int receive_inputs(char fieldname[][20], char field[][20], int type_field[], int num_searches){
   for(int i = 0; i < num_searches; i++){
        scanf("%s ", fieldname[i]);

        if(strcmp(fieldname[i], "idConecta") == 0){
            scanf("%s", field[i]);
            type_field[i] = 1;
        } else if(strcmp(fieldname[i], "siglaPais") == 0){
            scan_quote_string(field[i]);
            type_field[i] = 2;
        } else if(strcmp(fieldname[i], "idPoPsConectado") == 0){
            scanf("%s", field[i]);
            type_field[i] = 3;
        } else if(strcmp(fieldname[i], "unidadeMedida") == 0){
            scan_quote_string(field[i]);
            type_field[i] = 4;
        } else if(strcmp(fieldname[i], "velocidade") == 0){
            scanf("%s", field[i]);
            type_field[i] = 5;
        } else if(strcmp(fieldname[i], "nomePoPs") == 0){
            scan_quote_string(field[i]);
            type_field[i] = 6;
        } else if(strcmp(fieldname[i], "nomePais") == 0){
            scan_quote_string(field[i]);
            type_field[i] = 7;
        } else {
            //error_file();
            return 0;
        }
    }
    return 1;
}

int recursive_search_BT(FILE* index_file, int rrn, int key_value, int* num_pag_disc){
    fseek(index_file, ((rrn + 1) * LEN_PAGDISC), SEEK_SET); //vai p/ pagina indicada

    Node* node = create_node();
    read_node(index_file, node);
    (*num_pag_disc)++;

    int i;
    for(i = 0; i < node->nroChavesNo; i++){
        if(node->key[i].search_key == key_value){
            int aux = node->key[i].RRN_key;
            release_node(node);
            return aux; //elemento encontrado
        }
        if(node->key[i].search_key > key_value){
            break;
        }
    }

    int aux = node->ponteiro[i];
    //release_node(node);mAK
    if(aux == -1){ //nao ha filho na direita
        return -1; //elemento nao esta presente na arvore
    }

    return recursive_search_BT(index_file, aux, key_value, num_pag_disc);
}

int search_BT(FILE* index_file, BTHeader* ind_header, int key_value, int* num_pag_disc){
    //primeiro busca na raiz
    if(ind_header->noRaiz == -1){ //arvore vazia
        return -1;
    }
    fseek(index_file, ((ind_header->noRaiz + 1) * LEN_PAGDISC), SEEK_SET); //vai p/ posicao do no raiz
    Node* root_node = create_node();
    read_node(index_file, root_node);
    (*num_pag_disc)++;

    int i;
    for(i = 0; i < root_node->nroChavesNo; i++){
        if(root_node->key[i].search_key == key_value){
            int aux = root_node->key[i].RRN_key;
            release_node(root_node);
            return aux; //elemento encontrado
        }
        if(root_node->key[i].search_key > key_value){ //buscar elemento no no esquerdo
            break;
        }
    }

    int aux = root_node->ponteiro[i];
    //release_node(root_node);
    if(aux == -1){ //nao ha filho na direita
        return -1; //elemento nao esta presente na arvore
    }
    return recursive_search_BT(index_file, aux, key_value, num_pag_disc);
}

void command8(char* data_name, char* index_name, int num_searches){
    //variaveis para guardar os inputs
    char fieldname[num_searches][20];
    char field[num_searches][20];
    int type_field[num_searches];

    if(!receive_inputs(fieldname, field, type_field, num_searches)){
        return;
    }

    //abre o arquivo de dados e cria um cabecalho
    FILE* data_file = fopen(data_name, "rb");
    Header_reg* header = create_header();
    read_header(header, data_file);
    if(header->status == '0'){
        release_header(header);
        fclose(data_file);
        return;
    }

    //abre o arquivo de indice e cria um cabecalho
    FILE* index_file = fopen(index_name, "rb");
    BTHeader* ind_header = create_btheader();
    read_btheader(index_file, ind_header);
    if(ind_header->status == '0'){
        release_header(header);
        fclose(data_file);
        release_btheader(ind_header);
        fclose(index_file);
        return;
    }

    //realiza as n buscas
    int flag;
    int num_pag_disc;
    for(int i = 0; i < num_searches; i++){
        flag = 1;
        num_pag_disc = 0;
        Data_reg* reg = create_reg();

        printf("Busca %d\n", i + 1);

        if(type_field[i] == 1){ //busca usando arvore-B
            num_pag_disc = 2; //contando os cabecalhos
            int key_value = atoi(field[i]);
            int reg_rrn = search_BT(index_file, ind_header, key_value, &num_pag_disc);

            if(reg_rrn != -1){
                fseek(data_file, (reg_rrn * LEN_REG), SEEK_CUR);
                num_pag_disc++;
                if(read_register(data_file, reg)){
                    printar_registros(reg);
                    flag = 0;
                }
            }

        } else { //busca pela funcionalidade 3
            //percorre todos os registros de dados
            for (size_t j = 0; j < header->proxRRN; j++) {
                fseek(data_file, LEN_DISC_PAG + (j*LEN_REG), SEEK_SET); 

                //regitro removido deve ser desconsiderado
                if(!read_register(data_file, reg)){
                    continue;
                }

                //verifica se o registro possui o campo igual ao especificado na busca
                int value;
                switch (type_field[i]){
                case 2:
                    if(strcmp(reg->siglaPais, field[i]) == 0){
                        printar_registros(reg);
                        flag = 0;
                        num_pag_disc = header->nroPagDisco;
                    }
                    break;
                case 3:
                    value = atoi(field[i]);

                    if(reg->idPoPsConectado == value){
                        printar_registros(reg);
                        flag = 0;
                        num_pag_disc = header->nroPagDisco;
                    }
                    break;
                case 4:
                    if(reg->unidadeMedida == field[i][0]){
                        printar_registros(reg);
                        flag = 0;
                        num_pag_disc = header->nroPagDisco;
                    }
                    break;
                case 5:
                    value = atoi(field[i]);

                    if(reg->velocidade == value){
                        printar_registros(reg);
                        flag = 0;
                        num_pag_disc = header->nroPagDisco;
                    }
                    break;
                case 6:
                    if(strcmp(reg->nomePoPs, field[i]) == 0){
                        printar_registros(reg);
                        flag = 0;
                        num_pag_disc = header->nroPagDisco;
                    }
                    break;
                case 7:
                    if(strcmp(reg->nomePais, field[i]) == 0){
                        printar_registros(reg);
                        flag = 0;
                        num_pag_disc = header->nroPagDisco;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        //se nenhum registro coincidiu com a busca
        if(flag){
            printf("Nenhum registro encontrado.\n");
        }
        
        printf("Numero de paginas de disco: %d\n\n", num_pag_disc);

        //desaloca memoria e volta o arquivo para o primeiro reg de dados
        release_reg(reg);
        fseek(data_file, 960, SEEK_SET);
    }

    release_header(header);
    release_btheader(ind_header);

    fclose(data_file);
    fclose(index_file);
}