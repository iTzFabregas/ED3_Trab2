#include<stdio.h>
#include<string.h>

#include "register.h"

//mensagem de erro para erro ao tentar abrir um arquivo
void error_file(){
    printf("Falha no processamento do arquivo.\n");
}

//mensagem de erro para erro p/ registro inexistente
void error_reg(){
    printf("Registro inexistente.\n\n");
}