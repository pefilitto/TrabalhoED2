#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TADTrabalho.h"

void InsereTabelaNoBancoDeDados (bd * * bancoDeDados, tabela * * tabela ) {
	tabela -> ant = NULL;
	tabela -> prox = NULL;
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


void CriarTabela (bd * * bancoDeDados, char nomeTabela[50], atributo * atributos ) {
	tabela * novaTabela = (tabela*)malloc(sizeof(tabela));
	strcpy((novaTabela -> nometabela), nomeTabela);
	novaTabela -> listaAtributos = atributos;
	InsereTabelaNoBancoDeDados(bancoDeDados, &novaTabela);
}




int main()
{
	bd *bancoDeDados;
	InicializaBanco(&bancoDeDados);
	exibir(bancoDeDados);
}
