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

void GetName (char * ponteiro, char &nome, char  comando [] ) {
	char auxNome[30];
	ponteiro += strlen(comando);
	while(*ponteiro == ' '){
		ponteiro++;
	}
	int tamanho_nome = strcspn(ponteiro, " ;\n");
	strncpy(auxNome, ponteiro, tamanho_nome);
	auxNome[tamanho_nome]='\0';
	strcpy(*nome , auxNome );

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
				GetName(ponteiroNomeDoBanco,auxNome,"CREATE DATABASE")
				CriarBancoDeDados(b_dados, auxNome);		
			}
			
			ponteiroNomeDaTabela = strstr(linha, "CREATE TABLE");
			if(ponteiroNomeDaTabela != NULL){
				GetName(ponteiroNomeDaTabela,auxNome,"CREATE TABLE");
				CriarTabela(b_dados,auxNome);
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
