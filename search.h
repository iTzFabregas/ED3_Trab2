#ifndef __SEARCH_H__
#define __SEARCH_H__

#include<stdio.h>
#include "btree.h"

/**
 * @brief recebe os inputs do teclado e guarda qual sera o campo usado na busca
 * 
 * @param fieldname nome do campo a ser buscado
 * @param field valor do campo a ser buscado
 * @param type_field guarda um valor numerico referente a qual campo sera buscado
 * @param num_searches numero de buscas a serem realizadas
 * @return int 1 para sucesso e 0 para erro
 */
int receive_inputs(char fieldname[][20], char field[][20], int type_field[], int num_searches);

/**
 * @brief execucao recursiva da busca em arvore-B
 * 
 * @param index_file arquivo da arvore-B
 * @param rrn numero rrn do registro de dados cuja chave seja igual ao valor de
 * referencia da busca
 * @param key_value valor de referencia para a busca
 * @param num_pag_disc numero de paginas de disco percorridas durante a busca
 * @return int retorna o rrn do registro de dados cuja chave seja igual ao valor de
 * referencia da busca e -1 caso contratio
 */
int recursive_search_BT(FILE* index_file, int rrn, int key_value, int* num_pag_disc);

/**
 * @brief realiza uma busca em arvore-B comecando pela raiz e fazendo chamadas recursivas
 * 
 * @param index_file arquivo da arvore-B
 * @param ind_header cabecalho da arvore-B
 * @param key_value valor de referencia para a busca
 * @param num_pag_disc numero de paginas de disco percorridas durante a busca
 * @return int retorna o rrn do registro de dados cuja chave seja igual ao valor de
 * referencia da busca e -1 caso contratio
 */
int search_BT(FILE* index_file, BTHeader* ind_header, int key_value, int* num_pag_disc);

/**
 * @brief funcao que executa o comando 8, realiza n buscas, podendo ser a partir
 * do arquivo de indice, caso o campo usado na busca seja o utilizado como chave 
 * de indice ou diretamente no arquivo de dados, para todos os outros campos 
 * 
 * @param data_name nome do arquivo de dados a ser usado
 * @param index_name nome do arquivo de indice a ser usado
 * @param num_searches numero de buscas a serem realizadas
 */
void command8(char* data_name, char* index_name, int num_searches);

/**
 * @brief imprime no terminal os campos de dois registros de dados
 * 
 * @param reg1 primeiro registro de dados
 * @param reg2 segundo registro de dados
 */
void print_two_reg(Data_reg* reg1, Data_reg* reg2);

/**
 * @brief realiza uma busca no arquivo de indice tendo como referencia suas
 * chaves e um campo dos registros presentes no primeiro arquivo de dados, caso
 * os valores sejam iguais, sao printados os dois registros
 * 
 * @param data_name1 nome do primeiro arquivo de dados a ser usado
 * @param data_name2 nome do segundo arquivo de dados a ser usado
 * @param index_name nome do arquivo de indice a ser usado, refe-se
 * ao segundo arquivo de dados
 */
void command10(char* data_name1, char* data_name2, char* index_name);

#endif