#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TADTrabalho.h"

void InsereAtributoNaTabela (tabela * * tabela , atributo * * novoAtributo ) {
	(* novoAtributo) -> prox = NULL;

	if((* tabela) -> listaAtributos == NULL) {
		(* tabela) -> listaAtributos = * novoAtributo;
	}
	else {
		atributo * aux;
		aux = (* tabela) -> listaAtributos;
		while( aux -> prox != NULL ) {
			aux = aux -> prox;
		}
		aux -> prox = novoAtributo;
	}
	printf("Atributo %s inserido na tabela %s\n", (*novoAtributo) -> campo, (*tabela) -> nometabela);
}

void CriarAtributo (tabela * * tabela, char nomeAtributo[50], char tipo) {
	atributo * novoAtributo;
	novoAtributo = (atributo *)malloc(sizeof(atributo));
	strcpy((novoAtributo->campo), nomeAtributo);
	novoAtributo -> tipo = tipo;
	novoAtributo -> FK = NULL;
	novoAtributo -> listaDados = NULL;
	novoAtributo -> prox = NULL; 
	novoAtributo -> PK = NULL;
	InsereAtributoNaTabela (&(*tabela), &novoAtributo);
}


void InsereTabelaNoBancoDeDados(bd **bancoDeDados, tabela **novaTabela) {
    (*novaTabela)->ant = NULL;
    (*novaTabela)->prox = NULL;
    
    if ((*bancoDeDados)->listaTabela == NULL) {
        (*bancoDeDados)->listaTabela = *novaTabela;
    } 
	else {
        tabela *aux = (*bancoDeDados)->listaTabela;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = *novaTabela;
        (*novaTabela)->ant = aux;
    }
    
    printf("\nTabela %s foi criada dentro do banco %s\n", (*novaTabela)->nometabela, (*bancoDeDados)->nome_banco);
}

void AtribuiPKaoAtributo(tabela * * t, char nome[]){
	atributo *aux = (*t) -> listaAtributos;
	while(aux -> prox != NULL & strcmp(nome, aux -> campo) != 0)
		aux = aux -> prox;
	
	if(aux != NULL){
		aux -> PK = 'S';
		printf("Atributo %s recebeu PK\n", aux -> campo);
	}
		
}

void LerAtributosDoArquivo(FILE * * arquivo, tabela * * tabelaCriada){
	char nome[50], tipo[20], linha[1000], linha2[1000];
	int flag=0, i, j, z=0;
	if(*arquivo == NULL)
		printf("Ponteiro invalido na funcao leratributos\n");
	else{
		while (flag == 0)
		{
			fscanf(*arquivo, "%[^\n]", &linha);
			fgetc(*arquivo);
			if(strstr(linha, "CONSTRAINT") != NULL){
				if(strstr(linha, "PRIMARY KEY") != NULL){
					printf("Achou o primary key\n");
					for(i=0; i<strlen(linha); i++){
						if(linha[i] == '('){
							for(j=i+1; j<strlen(linha) && linha[j] != ')'; j++){
								nome[z] = linha[j];
								z++;	
							}
							
							nome[j-1] = '\0';
							AtribuiPKaoAtributo(&(*tabelaCriada), nome);
						}
					}
				}
			}
			else{
				if(strstr(linha, ");") != NULL){
					//Quando achar o );
					fgets(linha, sizeof(linha), (*arquivo));
					flag=1;
				}
				else{
					for(i=0; i<strlen(linha) && linha[i] != ' '; i++)
						nome[i] = linha[i];
					nome[i] = '\0';
					tipo[0] = linha[i+1];
					CriarAtributo(&(*tabelaCriada), nome, tipo[0]);	
				}	
			} 
		}		
	}
}


void CriarTabela (bd * * bancoDeDados, char nomeTabela[50], FILE * * arquivo) {
	tabela * novaTabela = (tabela*)malloc(sizeof(tabela));
	strcpy((novaTabela -> nometabela), nomeTabela);
	novaTabela -> listaAtributos = NULL;
	InsereTabelaNoBancoDeDados(bancoDeDados, &novaTabela);
	LerAtributosDoArquivo(&(*arquivo), &novaTabela);
}

void GetName(char *ponteiro, char *nome, const char *comando) {
    char auxNome[30];
    ponteiro += strlen(comando);

    while (*ponteiro == ' ' || *ponteiro == '(') {
        ponteiro++;
    }

    int tamanho_nome = strcspn(ponteiro, " ;\n");

    if (ponteiro[tamanho_nome - 1] == '(') {
        tamanho_nome--; 
    }

    strncpy(auxNome, ponteiro, tamanho_nome);
    auxNome[tamanho_nome] = '\0';

    strcpy(nome, auxNome);
}


void LeArquivo(bd * * b_dados) {
    FILE *ptr = fopen("ComandoSQL.txt", "r+");
    char linha[1000], auxNome[45];
    int cont = 0;

    while (fgets(linha, sizeof(linha), ptr) != NULL) {
        char *ponteiroNomeDoBanco = strstr(linha, "CREATE DATABASE");
        if (ponteiroNomeDoBanco != NULL) {
            GetName(ponteiroNomeDoBanco, auxNome, "CREATE DATABASE");
            CriarBancoDeDados(b_dados, auxNome);        
        }
        char *ponteiroNomeDaTabela = strstr(linha, "CREATE TABLE");
        if (ponteiroNomeDaTabela != NULL) {
            GetName(ponteiroNomeDaTabela, auxNome, "CREATE TABLE");
            CriarTabela(b_dados, auxNome, &ptr);
        }
        
        char *ponteiroAlterTable = strstr(linha, "ALTER TABLE");
        if(ponteiroAlterTable != NULL){
        	printf("\nAchou o alter table\n");
        	GetName(ponteiroAlterTable, auxNome, "ALTER TABLE");
        	if(buscarTabela(b_dados, auxNome)){
        		//Colocar aqui a funcao para manipular o ALTER TABLE
        	}
        }
    }
    fclose(ptr);
}



int main()
{
	bd *bancoDeDados;
	LeArquivo(&bancoDeDados);
	exibir(bancoDeDados);
}
