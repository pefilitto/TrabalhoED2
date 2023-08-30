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


void InsereTabelaNoBancoDeDados (bd * * bancoDeDados, tabela * * tabela ) {
	(*tabela) -> ant = NULL;
	(*tabela) -> prox = NULL;
	if((* bancoDeDados) -> listaTabela  == NULL) {
		(* bancoDeDados) -> listaTabela = tabela;
	}
	else {
		tabela * aux;
		aux = (* bancoDeDados) -> listaTabela;
		while( aux -> prox != NULL) {
			aux = aux -> prox;
		}
		aux -> prox = * tabela;
		(* tabela) -> ant = aux;
	}
}


void CriarTabela (bd * * bancoDeDados, char nomeTabela[50]) {
	tabela * novaTabela = (tabela*)malloc(sizeof(tabela));
	strcpy((novaTabela -> nometabela), nomeTabela);
	novaTabela -> listaAtributos = NULL;
	InsereTabelaNoBancoDeDados(bancoDeDados, &novaTabela);
}




int main()
{
	bd *bancoDeDados;
	InicializaBanco(&bancoDeDados);
	exibir(bancoDeDados);
}
