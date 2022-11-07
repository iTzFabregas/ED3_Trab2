/*
Fabricio Sampaio [runcodes] - 12547423 - participacao: 100%
Pedro Lucas Castro de Andrade - 11212289 - participacao: 100%
*/

#include<stdio.h>

//#include "command7.h"
//#include "command8.h"
//#include "command9.h"
//#include "command10.h"
#include "funcoesFornecidas.h"

int main(void) {
    //variaveis p/ entrada do teclado
    int command;
    char data_file1[30];
    char data_file2[30];
    char index_file[30];

    //le do teclado qual o comando sera acionado
    scanf("%d ", &command);

    //aciona a execucao do comando lido
    switch (command){
        case 7:
            scanf("%s %s", data_file1, index_file);
            /*if(command7(file1, file2)){
                binarioNaTela(file2);
            }*/
            break;
        case 8:
            scanf("%s %s", data_file1, index_file);
            //command8(file1, file2);
            break;
        case 9:
            scanf("%s %s", data_file1, index_file);
            /*if(command7(file1, file2)){
                binarioNaTela(file2);
            }*/
            break;
        case 10:
            
            break;
    }
}