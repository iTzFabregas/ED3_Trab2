/*
Fabricio Sampaio
Pedro Lucas Castro de Andrade
*/

#include <stdio.h>

#include "insertion.h"
#include "search.h"
#include "funcoesFornecidas.h"

int main(void) {
    //variaveis p/ entrada do teclado
    int command;
    char data_file1[30];
    char data_file2[30];
    char index_file[30];
    int num_suarches;

    //le do teclado qual o comando sera acionado
    scanf("%d ", &command);

    //aciona a execucao do comando lido
    switch (command){
        case 7:
            scanf("%s %s", data_file1, index_file);
            if(command7(data_file1, index_file)){
                binarioNaTela(index_file);
            }
            break;
        case 8:
            scanf("%s %s %d", data_file1, index_file, &num_suarches);
            command8(data_file1, index_file, num_suarches);
            break;
        case 9:
            scanf("%s %s", data_file1, index_file);
            if(command9(data_file1, index_file)){
                binarioNaTela(data_file1);
                binarioNaTela(index_file);
            }
            break;
        case 10:
            scanf("%s %s %*s %*s %s", data_file1, data_file2, index_file);
            command10(data_file1, data_file2, index_file);
            break;
    }
}
