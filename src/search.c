#include<string.h>

#include "search.h"
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

//execucao recursiva da busca em arvore-B
int recursive_search_BT(FILE* index_file, int rrn, int key_value, int* num_pag_disc){
    fseek(index_file, ((rrn + 1) * LEN_BT_DISC_PAG), SEEK_SET); //vai p/ pagina indicada

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
    release_node(node);
    if(aux == -1){ //nao ha filho na direita
        return -1; //elemento nao esta presente na arvore
    }

    return recursive_search_BT(index_file, aux, key_value, num_pag_disc);
}

//executa uma busca em arvore-B
int search_BT(FILE* index_file, BTHeader* ind_header, int key_value, int* num_pag_disc){
    //primeiro busca na raiz
    if(ind_header->noRaiz == -1){ //arvore vazia
        return -1;
    }
    fseek(index_file, ((ind_header->noRaiz + 1) * LEN_BT_DISC_PAG), SEEK_SET); //vai p/ posicao do no raiz
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
    release_node(root_node);
    if(aux == -1){ //nao ha filho na direita
        return -1; //elemento nao esta presente na arvore
    }
    return recursive_search_BT(index_file, aux, key_value, num_pag_disc);
}

//funcao que executa o comando 8
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
    if(data_file == NULL){
        error_file();
        return;
    }
    Header_reg* header = create_header();
    read_header(header, data_file);
    if(header->status == '0'){
        release_header(header);
        fclose(data_file);
        return;
    }

    //abre o arquivo de indice e cria um cabecalho
    FILE* index_file = fopen(index_name, "rb");
    if(index_file == NULL){
        error_file();
        fclose(data_file);
        release_header(header);
        return;
    }
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

        } else { //busca direto no arquivo de dados
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
            error_reg();
            printf("\n");
        }
        
        printf("Numero de paginas de disco: %d\n\n", num_pag_disc);

        //desaloca memoria e volta o arquivo para o primeiro reg de dados
        release_reg(reg);
        fseek(data_file, 960, SEEK_SET);
    }

    //libera memoria usada
    release_header(header);
    release_btheader(ind_header);

    //fecha arquivos usados
    fclose(data_file);
    fclose(index_file);
}

//imprime no terminal os campos de dois registros de dados
void print_two_reg(Data_reg* reg1, Data_reg* reg2){
    if (reg1->idConecta != -1) { // se for diferente de NULL
        printf("Identificador do ponto: %d\n", reg1->idConecta);
    }
    
    if (reg1->nomePoPs[0] != DELIMITER && strlen(reg1->nomePoPs) > 0) {  // se for diferente de NULL
        printf("Nome do ponto: %s\n", reg1->nomePoPs);    
    } 
    
    if (reg1->nomePais[0] != DELIMITER && strlen(reg1->nomePais) > 0) { // se for diferente de NULL
        printf("Pais de localizacao: %s\n", reg1->nomePais);
    }
    
    if (reg1->siglaPais[0] != GARBAGE && strlen(reg1->siglaPais) > 0) { // se for diferente de NULL
        printf("Sigla do pais: %s\n", reg1->siglaPais);
    }
    
    if (reg1->idPoPsConectado != -1) { // se for diferente de NULL
        printf("Identificador do ponto conectado: %d\n", reg1->idPoPsConectado);
    }

    if (reg2->nomePoPs[0] != DELIMITER && strlen(reg2->nomePoPs) > 0) {  // se for diferente de NULL
        printf("Nome do ponto conectado: %s\n", reg2->nomePoPs);    
    }

    if (reg2->nomePais[0] != DELIMITER && strlen(reg2->nomePais) > 0) { // se for diferente de NULL
        printf("Nome do pais conectado: %s\n", reg2->nomePais);
    }

    if (reg2->siglaPais[0] != GARBAGE && strlen(reg2->siglaPais) > 0) { // se for diferente de NULL
        printf("Sigla do pais: %s\n", reg2->siglaPais);
    }
    
    if (reg1->velocidade != -1 && reg1->unidadeMedida != '$') {  // se for diferente de NULL
        printf("Velocidade de transmissao: %d %cbps\n", reg1->velocidade, reg1->unidadeMedida);
    }
    printf("\n");
}

//funcao que executa o comando 10
void command10(char* data_name1, char* data_name2, char* index_name){
    //abre os arquivos de dados e cria cabecalhos
    FILE* data_file1 = fopen(data_name1, "rb");
    if(data_file1 == NULL){
        error_file();
        return;
    }
    Header_reg* header1 = create_header();
    read_header(header1, data_file1);
    if(header1->status == '0'){
        release_header(header1);
        fclose(data_file1);
        return;
    }

    FILE* data_file2 = fopen(data_name2, "rb");
    if(data_file2 == NULL){
        error_file();
        fclose(data_file1);
        release_header(header1);
        return;
    }
    Header_reg* header2 = create_header();
    read_header(header2, data_file2);
    if(header2->status == '0'){
        release_header(header1);
        fclose(data_file1);
        release_header(header2);
        fclose(data_file2);
        return;
    }

    //abre o arquivo de indice e cria um cabecalho
    FILE* index_file2 = fopen(index_name, "rb");
    if(index_file2 == NULL){
        error_file();
        release_header(header1);
        fclose(data_file1);
        release_header(header2);
        fclose(data_file2);
        return;
    }
    BTHeader* ind_header = create_btheader();
    read_btheader(index_file2, ind_header);
    if(ind_header->status == '0'){
        release_header(header1);
        fclose(data_file1);
        release_header(header2);
        fclose(data_file2);
        release_btheader(ind_header);
        fclose(index_file2);
        return;
    }

    //percorre todos os registros de dados
    int flag = 1;
    for (int j = 0; j < header1->proxRRN; j++) {
        Data_reg* reg1 = create_reg();
        Data_reg* reg2 = create_reg();
        
        fseek(data_file1, LEN_DISC_PAG + (j*LEN_REG), SEEK_SET); 

        //regitro removido deve ser desconsiderado
        if(!read_register(data_file1, reg1)){
            continue;
        }

        //verifica a igualdade file1.idPoPsConectado = file2.idConecta
        int aux1 = reg1->idPoPsConectado;
        int aux2 = 0;
        int rrn = search_BT(index_file2, ind_header, aux1, &aux2);

        if(rrn != -1){ //encontrou um registro valido
            fseek(data_file2, LEN_DISC_PAG + (rrn * LEN_REG), SEEK_SET);
            if(!read_register(data_file2, reg2)){ //verifica se ha inconsistencia no status do registro2
                continue;
            }

            print_two_reg(reg1, reg2);
            flag = 0;
        }
    }

    //se nenhum registro coincidiu com a busca
    if(flag){
        error_reg();
    }

    //fecha arquvios usados
    fclose(data_file1);
    fclose(data_file2);
    fclose(index_file2);

    //libera memoria usada
    release_header(header1);
    release_header(header2);
    release_btheader(ind_header);
}