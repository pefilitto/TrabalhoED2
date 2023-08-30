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
}

void CriarAtributo (tabela * * tabela, char nomeAtributo[50], char tipo, char PK) {
	atributo * novoAtributo;
	novoAtributo = (atributo *)malloc(sizeof(atributo));
	strcpy((novoAtributo->campo), nomeAtributo);
	novoAtributo -> tipo = tipo;
	novoAtributo -> FK = NULL;
	novoAtributo -> listaDados = NULL;
	novoAtributo -> prox = NULL; 
	novoAtributo -> PK = PK;
	InsereAtributoNaTabela (tabela, &novoAtributo);
}


void InsereTabelaNoBancoDeDados (bd * * bancoDeDados, tabela * * novaTabela ) {
	(*novaTabela) -> ant = NULL;
	(*novaTabela) -> prox = NULL;
	if((* bancoDeDados) -> listaTabela  == NULL) {
		(* bancoDeDados) -> listaTabela = novaTabela;
	}
	else {
		tabela * aux;
		aux = (* bancoDeDados) -> listaTabela;
		while(aux -> prox != NULL) {
			aux = aux -> prox;
		}
		aux -> prox = * novaTabela;
		(* novaTabela) -> ant = aux;
	}
}


void CriarTabela (bd * * bancoDeDados, char nomeTabela[50]) {
	tabela * novaTabela = (tabela*)malloc(sizeof(tabela));
	strcpy((novaTabela -> nometabela), nomeTabela);
	novaTabela -> listaAtributos = NULL;
	InsereTabelaNoBancoDeDados(bancoDeDados, &novaTabela);
}



void LeArquivo(bd * * b_dados){
	FILE *ptr = fopen("ComandoSQL.txt", "r+");
	char linha[1000], *ponteiroNomeDoBanco, *ponteiroNomeDaTabela;
	char auxNome[30];
	int cont = 0;
	while(!feof(ptr)){
		if(fgets(linha, sizeof(linha), ptr) != NULL){
			
			ponteiroNomeDoBanco = strstr(linha, "CREATE DATABASE");
			if(ponteiroNomeDoBanco != NULL){
		
				ponteiroNomeDoBanco += strlen("CREATE DATABASE");
					
 				while(*ponteiroNomeDoBanco == ' ')
 					ponteiroNomeDoBanco++;
				
 				int tamanho_nome = strcspn(ponteiroNomeDoBanco, " ;\n");
 				strncpy(auxNome, ponteiroNomeDoBanco, tamanho_nome);
 				auxNome[tamanho_nome] = '\0';
				 
				CriarBancoDeDados(b_dados, auxNome);		
			}
			
			ponteiroNomeDaTabela = strstr(linha, "CREATE TABLE");
			if(ponteiroNomeDaTabela != NULL){
				tabela *novaTabela;
				ponteiroNomeDaTabela += strlen()
			}	
		}
	}
}


int main()
{
	bd *bancoDeDados;
	LeArquivo(&bancoDeDados);
	exibir(bancoDeDados);
}
