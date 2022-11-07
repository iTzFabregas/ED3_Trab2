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

//imprime todos os campos de um registro do arquivo de dados
/*void printar_registros(Data_reg *registro){
    if (registro->idConecta != -1) { // se for diferente de NULL
        printf("Identificador do ponto: %d\n", registro->idConecta);
    }
    
    if (registro->nomePoPs[0] != DELIMITER && strlen(registro->nomePoPs) > 0) {  // se for diferente de NULL
        printf("Nome do ponto: %s\n", registro->nomePoPs);    
    } 
    
    if (registro->nomePais[0] != DELIMITER && strlen(registro->nomePais) > 0) { // se for diferente de NULL
        printf("Pais de localizacao: %s\n", registro->nomePais);
    }
    
    if (registro->siglaPais[0] != GARBAGE && strlen(registro->siglaPais) > 0) { // se for diferente de NULL
        printf("Sigla do pais: %s\n", registro->siglaPais);
    }
    
    if (registro->idPoPsConectado != -1) { // se for diferente de NULL
        printf("Identificador do ponto conectado: %d\n", registro->idPoPsConectado);
    }
    
    if (registro->velocidade != -1 && registro->unidadeMedida != '$') {  // se for diferente de NULL
        printf("Velocidade de transmissao: %d %cbps\n", registro->velocidade, registro->unidadeMedida);
    }
    printf("\n");
}*/