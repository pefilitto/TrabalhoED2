#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "TADTrabalho.h"

char *NossaStrStr(char *str, char *substr)
{
	int i, j;
	int strLen = strlen(str);
	int substrLen = strlen(substr);

	for (i = 0; i <= strLen - substrLen; i++)
	{
		for (j = 0; j < substrLen && str[i + j] == substr[j]; j++)
			;

		if (j == substrLen)
		{
			return (char *)(str + i);
		}
	}

	return NULL;
}

char buscarTabela(bd **bancoDeDados, char nomeTabela[])
{
	tabela *aux = (*bancoDeDados)->listaTabela;
	while (aux->prox != NULL && strcmp(nomeTabela, aux->nometabela) != 0)
		aux = aux->prox;

	if (aux == NULL)
		return 0;
	return 1;
}

int BuscaNivelDado(bd **bancoDeDados, char nomeTabela[], char atributo1[], char valor[])
{
	tabela *auxTabela;
	atributo *auxAtributo;
	ldados *dados;
	int cont = 1, valorAtributoInt;
	float valorAtributoFloat;

	auxTabela = (*bancoDeDados)->listaTabela;
	while (auxTabela != NULL && stricmp(auxTabela->nometabela, nomeTabela) != 0)
	{
		auxTabela = auxTabela->prox;
	}

	if (auxTabela != NULL)
	{
		auxAtributo = auxTabela->listaAtributos;
		while (auxAtributo != NULL && strcmp(auxAtributo->campo, atributo1) != 0)
		{
			auxAtributo = auxAtributo->prox;
		}

		if (auxAtributo != NULL)
		{
			if (auxAtributo->tipo == 'I')
			{
				dados = auxAtributo->listaDados;
				valorAtributoInt = atoi(valor);
				while (dados != NULL && dados->d.valorI != valorAtributoInt)
				{
					dados = dados->prox;
					cont++;
				}

				if (dados == NULL)
					return -1;
				return cont;
			}
			else if (auxAtributo->tipo == 'N')
			{
				ldados *dados;
				dados = auxAtributo->listaDados;
				valorAtributoFloat = atof(atributo1);
				while (dados != NULL && dados->d.valorN != valorAtributoFloat)
				{
					dados = dados->prox;
					cont++;
				}

				if (dados == NULL)
					cont = -1;
				return cont;
			}
			else if (auxAtributo->tipo == 'D')
				return cont;
			else if (auxAtributo->tipo == 'C')
			{
				ldados *dados;
				dados = auxAtributo->listaDados;
				while (dados != NULL && strcmp(dados->d.valorC, atributo1) != 0)
				{
					dados = dados->prox;
					cont++;
				}

				if (dados == NULL)
					cont = -1;
				return cont;
			}
			else
			{
				ldados *dados;
				dados = auxAtributo->listaDados;
				while (dados != NULL && strcmp(dados->d.valorT, atributo1) != 0)
				{
					dados = dados->prox;
					cont++;
				}

				if (dados == NULL)
					cont = -1;
				return cont;
			}
		}
	}
}

void gotoxy(int x, int y)
{
	printf("\033[%d;%dH", y, x);
}

void exibirBanco(bd *bancoDeDados)
{
	int totalLinhas = 25;
	int linhaInicial = (totalLinhas - 10) / 2;

	tabela *auxTabela = bancoDeDados->listaTabela;
	int linha = linhaInicial;

	while (auxTabela != NULL)
	{
		printf("\n=====================================");
		printf("\nTabela: %s", auxTabela->nometabela);
		printf("\n=====================================\n");

		atributo *auxAtributos = auxTabela->listaAtributos;

		while (auxAtributos != NULL)
		{
			printf("\nAtributo: %s\n", auxAtributos->campo);

			ldados *auxDados = auxAtributos->listaDados;
			int i = 1;

			while (auxDados != NULL)
			{
				printf("%d - ", i++);

				switch (auxAtributos->tipo)
				{
				case 'I':
					printf("%d\n", auxDados->d.valorI);
					break;
				case 'C':
					printf("%s\n", auxDados->d.valorT);
					break;
				case 'N':
					printf("%.1f\n", auxDados->d.valorN);
					break;
				default:
					printf("%s\n", auxDados->d.valorT);
				}

				auxDados = auxDados->prox;
			}

			auxAtributos = auxAtributos->prox;
		}

		linha += 3;

		auxTabela = auxTabela->prox;
	}
}
// Iniciando a criacao das estruturas

void InsereAtributoNaTabela(tabela **tabela, atributo **novoAtributo)
{
	(*novoAtributo)->prox = NULL;

	if ((*tabela)->listaAtributos == NULL)
	{
		(*tabela)->listaAtributos = *novoAtributo;
	}
	else
	{
		atributo *aux;
		aux = (*tabela)->listaAtributos;
		while (aux->prox != NULL)
		{
			aux = aux->prox;
		}
		aux->prox = *novoAtributo;
	}
	// printf("Atributo %s inserido na tabela %s\n", (*novoAtributo)->campo, (*tabela)->nometabela);
}

void CriarAtributo(tabela **tabela, char nomeAtributo[50], char tipo)
{
	atributo *novoAtributo;
	novoAtributo = (atributo *)malloc(sizeof(atributo));
	strcpy((novoAtributo->campo), nomeAtributo);
	novoAtributo->tipo = tipo;
	novoAtributo->FK = NULL;
	novoAtributo->listaDados = NULL;
	novoAtributo->prox = NULL;
	novoAtributo->PK = NULL;
	InsereAtributoNaTabela(&(*tabela), &novoAtributo);
}

void InsereTabelaNoBancoDeDados(bd **bancoDeDados, tabela **novaTabela)
{
	(*novaTabela)->ant = NULL;
	(*novaTabela)->prox = NULL;

	if ((*bancoDeDados)->listaTabela == NULL)
	{
		(*bancoDeDados)->listaTabela = *novaTabela;
	}
	else
	{
		tabela *aux = (*bancoDeDados)->listaTabela;
		while (aux->prox != NULL)
		{
			aux = aux->prox;
		}
		aux->prox = *novaTabela;
		(*novaTabela)->ant = aux;
	}

	// printf("\nTabela %s foi criada dentro do banco %s\n", (*novaTabela)->nometabela, (*bancoDeDados)->nome_banco);
}

void AtribuiPKaoAtributo(tabela **t, char nome[])
{
	atributo *aux = (*t)->listaAtributos;
	while (aux->prox != NULL & strcmp(nome, aux->campo) != 0)
		aux = aux->prox;

	if (aux != NULL)
	{
		aux->PK = 'S';
		// printf("Atributo %s recebeu PK\n", aux->campo);
	}
}

void LerAtributosDoArquivo(FILE **arquivo, tabela **tabelaCriada)
{
	char nome[50], tipo[20], linha[1000], linha2[1000];
	int flag = 0, i = 0, j, z = 0, m;
	if (*arquivo == NULL)
		printf("Ponteiro invalido na funcao leratributos\n");
	else
	{
		while (flag == 0)
		{
			i = 0;
			fscanf(*arquivo, "%[^\n]", &linha);
			fgetc(*arquivo);
			if (NossaStrStr(linha, "CONSTRAINT") != NULL)
			{
				if (NossaStrStr(linha, "PRIMARY KEY") != NULL)
				{
					// printf("Achou o primary key\n");
					for (i = 0; i < strlen(linha); i++)
					{
						if (linha[i] == '(')
						{
							for (j = i + 1; j < strlen(linha) && linha[j] != ')'; j++)
							{
								nome[z] = linha[j];
								z++;
							}

							nome[j - 1] = '\0';
							AtribuiPKaoAtributo(&(*tabelaCriada), nome);
						}
					}
				}
			}
			else
			{
				if (NossaStrStr(linha, ");") != NULL)
				{
					// Quando achar o );
					fgets(linha, sizeof(linha), (*arquivo));
					flag = 1;
				}
				else
				{
					while (linha[i] == ' ')
						i++;
					for (m = 0; linha[i] != ' '; i++, m++)
						nome[m] = linha[i];
					nome[m] = '\0';
					tipo[0] = linha[i + 1];
					CriarAtributo(&(*tabelaCriada), nome, tipo[0]);
				}
			}
		}
	}
}

void CriarTabela(bd **bancoDeDados, char nomeTabela[50], FILE **arquivo)
{
	tabela *novaTabela = (tabela *)malloc(sizeof(tabela));
	strcpy((novaTabela->nometabela), nomeTabela);
	novaTabela->listaAtributos = NULL;
	InsereTabelaNoBancoDeDados(bancoDeDados, &novaTabela);
	LerAtributosDoArquivo(&(*arquivo), &novaTabela);
}

void GetName(char *ponteiro, char *nome, const char *comando)
{
	char auxNome[30];
	ponteiro += strlen(comando);

	while (*ponteiro == ' ' || *ponteiro == '(')
	{
		ponteiro++;
	}

	int tamanho_nome = strcspn(ponteiro, " ;\n");

	if (ponteiro[tamanho_nome - 1] == '(')
	{
		tamanho_nome--;
	}

	strncpy(auxNome, ponteiro, tamanho_nome);
	auxNome[tamanho_nome] = '\0';

	strcpy(nome, auxNome);
}

void LeArquivo(bd **b_dados)
{
	FILE *ptr = fopen("ComandoSQL.txt", "r+");
	char linha[1000], auxNome[45];
	int cont = 0;

	while (fgets(linha, sizeof(linha), ptr) != NULL)
	{
		char *ponteiroNomeDoBanco = NossaStrStr(linha, "CREATE DATABASE");
		if (ponteiroNomeDoBanco != NULL)
		{
			GetName(ponteiroNomeDoBanco, auxNome, "CREATE DATABASE");
			CriarBancoDeDados(b_dados, auxNome);
		}
		char *ponteiroNomeDaTabela = NossaStrStr(linha, "CREATE TABLE");
		if (ponteiroNomeDaTabela != NULL)
		{
			GetName(ponteiroNomeDaTabela, auxNome, "CREATE TABLE");
			CriarTabela(b_dados, auxNome, &ptr);
		}

		char *ponteiroAlterTable = NossaStrStr(linha, "ALTER TABLE");
		if (ponteiroAlterTable != NULL)
		{
			// printf("\nAchou o alter table\n");
			GetName(ponteiroAlterTable, auxNome, "ALTER TABLE");
			if (buscarTabela(b_dados, auxNome))
			{
				// Colocar aqui a funcao para manipular o ALTER TABLE
			}
		}
	}
	fclose(ptr);
}

// Finalizando a criaï¿½ï¿½o das estruturas

// Iniciando os comandos INSERT, DELETE, SELECT e UPDATE

void AtribuiTipoDeDadoAoAtributo(ldados **dados, char valor[], char tipo)
{
	if (tipo == 'I')
	{
		(*dados)->d.valorI = atoi(valor);
	}
	else if (tipo == 'N')
	{
		(*dados)->d.valorN = atof(valor);
	}
	else if (tipo == 'D')
	{
		strcpy((*dados)->d.valorT, valor);
	}
	else if (tipo == 'C')
	{
		strcpy((*dados)->d.valorT, valor);
	}
	else
	{
		strcpy((*dados)->d.valorT, valor);
	}
}

void Insert(bd **bancoDeDados, char nomeTabela[], char campo[], char valor[])
{
	tabela *aux;
	int x = 0, y = 0;
	aux = (*bancoDeDados)->listaTabela;

	while (aux != NULL && stricmp(aux->nometabela, nomeTabela) != 0)
	{
		aux = aux->prox;
	}

	atributo *auxAtr;
	auxAtr = aux->listaAtributos;

	while (auxAtr != NULL && stricmp(auxAtr->campo, campo) != 0)
	{
		auxAtr = auxAtr->prox;
	}

	ldados *auxDados;
	auxDados = auxAtr->listaDados;

	if (auxDados == NULL)
	{
		auxAtr->listaDados = (ldados *)malloc(sizeof(ldados));
		auxDados = auxAtr->listaDados;
		auxDados->prox = NULL;
		auxDados->terminal = 1;
		AtribuiTipoDeDadoAoAtributo(&auxDados, valor, auxAtr->tipo);
	}
	else
	{
		while (auxDados->prox != NULL)
		{
			auxDados = auxDados->prox;
		}
		auxDados->prox = (ldados *)malloc(sizeof(ldados));
		auxDados = auxDados->prox;
		auxDados->prox = NULL;
		auxDados->terminal = 1;
		AtribuiTipoDeDadoAoAtributo(&auxDados, valor, auxAtr->tipo);
	}

	y++;
	// printf("\nInseriu\n");
}

void CortarSQLAtributos(bd **bancoDeDados, char nomeTabela[], char campos[], char valores[])
{
	int i, c = 0, v = 0, k, flagParenteses = 0, l;
	char campo[50], valor[50];

	while (!flagParenteses)
	{
		if (campos[c] == ' ')
		{
			c++;
		}
		for (i = 0; campos[c] != ',' && campos[c] != ')' && c < strlen(campos); i++, c++)
		{
			campo[i] = campos[c];
		}
		c++;
		campo[i] = '\0';

		if (valores[v] == ' ')
		{
			v++;
		}
		for (l = 0; valores[v] != ',' && valores[v] != ')' && v < strlen(valores); v++, l++)
		{
			valor[l] = valores[v];
		}
		valor[l] = '\0';
		v++;
		// printf("Inserindo campo %s, valor %s\n", campo, valor);
		Insert(&(*bancoDeDados), nomeTabela, campo, valor);

		if (c >= strlen(campos) || v >= strlen(valores))
		{
			flagParenteses = 1;
		}
	}
}

void CortarSQLInsert(bd **bancoDeDados, char comando[])
{
	char nomeTabela[50];
	int i, a;
	for (i = strlen("INSERT INTO "), a = 0; i < strlen(comando) && comando[i] != ' '; i++, a++)
	{
		nomeTabela[a] = comando[i];
	}
	nomeTabela[a] = '\0';

	char *inicio_campos = strchr(comando, '(');
	inicio_campos++; // Avanï¿½ar para o primeiro caractere apï¿½s o parï¿½ntese

	char *fim_campos = strchr(inicio_campos, ')');

	char campos[100];
	int tamanho_campos = fim_campos - inicio_campos;
	strncpy(campos, inicio_campos, tamanho_campos);
	campos[tamanho_campos] = '\0';

	char *inicio_valores = NossaStrStr(comando, "values") ? NossaStrStr(comando, "values") : NossaStrStr(comando, "VALUES");
	inicio_valores = strchr(inicio_valores, '(');
	inicio_valores++; // Avanï¿½ar para o primeiro caractere apï¿½s o parï¿½ntese

	char *fim_valores = strrchr(comando, ')'); // Usar strrchr para encontrar o ï¿½ltimo ')'

	char valores[100];
	int tamanho_valores = fim_valores - inicio_valores;
	strncpy(valores, inicio_valores, tamanho_valores);
	valores[tamanho_valores] = '\0';

	printf("Campos %s \nValores %s \n", campos, valores);
	CortarSQLAtributos(&(*bancoDeDados), nomeTabela, campos, valores);
}

void Delete(bd **bancoDeDados, char nomeTabela[], int nivel)
{
	tabela *auxTabela;
	atributo *auxAtributo;
	ldados *dados, *anterior, *elementoLiberar;
	int nivelAtual;
	auxTabela = (*bancoDeDados)->listaTabela;
	while (auxTabela != NULL && stricmp(auxTabela->nometabela, nomeTabela) != 0)
	{
		auxTabela = auxTabela->prox;
	}

	if (auxTabela != NULL)
	{
		auxAtributo = auxTabela->listaAtributos;
		while (auxAtributo != NULL)
		{
			nivelAtual = 1;
			dados = auxAtributo->listaDados;
			while (dados != NULL && nivelAtual != nivel)
			{
				dados = dados->prox;
				nivelAtual++;
			}

			if (nivelAtual == nivel)
			{
				elementoLiberar = dados;

				if (dados == auxAtributo->listaDados)
				{
					auxAtributo->listaDados = dados->prox;
				}
				else
				{
					anterior = auxAtributo->listaDados;
					while (anterior->prox != dados)
					{
						anterior = anterior->prox;
					}

					anterior->prox = dados->prox;
				}

				free(elementoLiberar);
			}
			auxAtributo = auxAtributo->prox;
		}
	}
}

void CortarSQLDelete(bd **bancoDeDados, char comando[])
{
	char nomeTabela[50], atributo[50], operador, valorAtributoStr[50];
	ldados valor;
	int i, a, n, nivel = 0;
	for (i = strlen("DELETE FROM "), a = 0; i < strlen(comando) && comando[i] != ' '; i++, a++)
	{
		nomeTabela[a] = comando[i];
	}
	nomeTabela[a] = '\0';

	i = i + strlen(" WHERE ");

	for (a = 0; comando[i] != ' '; i++, a++)
	{
		atributo[a] = comando[i];
	}
	atributo[a] = '\0';
	i += 3;

	for (a = 0; comando[i] != ';'; i++, a++)
	{
		valorAtributoStr[a] = comando[i];
	}
	valorAtributoStr[a] = '\0';

	nivel = BuscaNivelDado(&(*bancoDeDados), nomeTabela, atributo, valorAtributoStr);
	Delete(&(*bancoDeDados), nomeTabela, nivel);
}

void Update(bd **bancoDeDados, char nomeTabela[], char nomeAtributoParaAtualizar[], int valorAtributoNovo, char pkDoAtributoParaAtualizar[], char valorDaPk[])
{
	tabela *auxTabela;
	atributo *auxAtributo;
	ldados *dados;
	int nivel, valor, nivelAtual = 1;
	auxTabela = (*bancoDeDados)->listaTabela;
	while (auxTabela != NULL && stricmp(auxTabela->nometabela, nomeTabela) != 0)
	{
		auxTabela = auxTabela->prox;
	}

	if (auxTabela != NULL)
	{
		auxAtributo = auxTabela->listaAtributos;
		while (auxAtributo != NULL && strcmp(pkDoAtributoParaAtualizar, auxAtributo->campo) != 0)
			auxAtributo = auxAtributo->prox;

		if (auxAtributo != NULL)
		{
			dados = auxAtributo->listaDados;
			valor = atoi(valorDaPk);
			while (dados != NULL && valor != dados->d.valorI)
				dados = dados->prox;

			if (dados != NULL)
			{
				nivel = BuscaNivelDado(&(*bancoDeDados), nomeTabela, pkDoAtributoParaAtualizar, valorDaPk);
				auxAtributo = auxTabela->listaAtributos;
				while (auxAtributo != NULL && strcmp(nomeAtributoParaAtualizar, auxAtributo->campo) != 0)
					auxAtributo = auxAtributo->prox;

				if (auxAtributo != NULL)
				{
					dados = auxAtributo->listaDados;
					while (dados != NULL && nivel != nivelAtual)
					{
						dados = dados->prox;
						nivelAtual++;
					}

					if (nivel == nivelAtual)
						dados->d.valorI = valorAtributoNovo;
				}
			}
		}
	}
}

void CortarSQLUpdate(bd **bancoDeDados, char comando[])
{
	char nomeTabela[50], nomeAtributoParaAtualizar[50], valorAtributoParaAtualizar[10], pkDoAtributoParaAtualizar[50], valorDaPkParaAtualizar[10];
	int i, a, valorAtributoNovo, valorDaPk;
	for (i = strlen("UPDATE "), a = 0; i < strlen(comando) && comando[i] != ' '; i++, a++)
	{
		nomeTabela[a] = comando[i];
	}
	nomeTabela[a] = '\0';

	i = i + strlen(" SET ");
	for (a = 0; comando[i] != ' '; a++, i++)
	{
		nomeAtributoParaAtualizar[a] = comando[i];
	}
	nomeAtributoParaAtualizar[a] = '\0';

	i += 3;
	for (a = 0; comando[i] != ' '; i++, a++)
	{
		valorAtributoParaAtualizar[a] = comando[i];
	}
	valorAtributoParaAtualizar[a] = '\0';

	i = i + strlen(" WHERE ");

	for (a = 0; comando[i] != ' '; a++, i++)
	{
		pkDoAtributoParaAtualizar[a] = comando[i];
	}
	pkDoAtributoParaAtualizar[a] = '\0';

	i += 3;
	for (a = 0; comando[i] != ';'; a++, i++)
	{
		valorDaPkParaAtualizar[a] = comando[i];
	}
	valorDaPkParaAtualizar[a] = '\0';

	valorAtributoNovo = atoi(valorAtributoParaAtualizar);

	Update(&(*bancoDeDados), nomeTabela, nomeAtributoParaAtualizar, valorAtributoNovo, pkDoAtributoParaAtualizar, valorDaPkParaAtualizar);
}

void SelectAll(bd *bancoDeDados, char nomeTabela[])
{
	tabela *auxTabela = bancoDeDados->listaTabela;
	while (auxTabela != NULL && strcmp(auxTabela->nometabela, nomeTabela) != 0)
	{
		auxTabela = auxTabela->prox;
	}

	printf("\nExibindo os dados da tabela: %s\n", nomeTabela);
	if (auxTabela != NULL)
	{
		atributo *auxAtributos = auxTabela->listaAtributos;
		while (auxAtributos != NULL)
		{
			printf("\nAtributo: %s\n", auxAtributos->campo);
			int i = 1;
			ldados *auxDados = auxAtributos->listaDados;
			while (auxDados != NULL)
			{
				printf("%d - ", i++);

				switch (auxAtributos->tipo)
				{
				case 'I':
					printf("%d\n", auxDados->d.valorI);
					break;
				case 'C':
					printf("%s\n", auxDados->d.valorT);
					break;
				case 'N':
					printf("%.1f\n", auxDados->d.valorN);
					break;
				default:
					printf("%s\n", auxDados->d.valorT);
				}

				auxDados = auxDados->prox;
			}
			auxAtributos = auxAtributos->prox;
		}
	}
}

void CortarSQLSelectAll(bd **bancoDeDados, char comando[])
{
	int i, a;
	char nomeTabela[50];
	for (i = strlen("SELECT * FROM "), a = 0; comando[i] != ';'; i++, a++)
	{
		nomeTabela[a] = comando[i];
	}
	nomeTabela[a] = '\0';

	SelectAll(*bancoDeDados, nomeTabela);
}

void Select(bd **bancoDeDados, char nomeTabela[], char nomeAtributo[])
{
	tabela *auxTabela;
	atributo *auxAtributo;
	ldados *auxDados;
	int i = 1;
	auxTabela = (*bancoDeDados)->listaTabela;
	while (auxTabela != NULL && strcmp(auxTabela->nometabela, nomeTabela) != 0)
	{
		auxTabela = auxTabela->prox;
	}

	if (auxTabela != NULL)
	{
		auxAtributo = auxTabela->listaAtributos;
		while (auxAtributo != NULL && strcmp(auxAtributo->campo, nomeAtributo) != 0)
		{
			auxAtributo = auxAtributo->prox;
		}

		if (auxAtributo != NULL)
		{
			auxDados = auxAtributo->listaDados;
			while (auxDados != NULL)
			{
				printf("%d - ", i++);

				switch (auxAtributo->tipo)
				{
					case 'I':
						printf("%d\n", auxDados->d.valorI);
						break;
					case 'C':
						printf("%s\n", auxDados->d.valorT);
						break;
					case 'N':
						printf("%.1f\n", auxDados->d.valorN);
						break;
					default:
						printf("%s\n", auxDados->d.valorT);
				}
				auxDados = auxDados->prox;
			}
		}
	}
}

void CortarSQLSelect(bd **bancoDeDados, char comando[])
{
	int i, a;
	char nomeTabela[20] = "", nomeAtributo[20];

	char *ponteiroFrom = NossaStrStr(comando, "FROM");
	if (ponteiroFrom != NULL)
	{
		i = ponteiroFrom - comando + strlen("FROM ");

		while (comando[i] == ' ')
		{
			i++;
		}

		for (a = 0; comando[i] != ' ' && comando[i] != ';'; a++, i++)
		{
			nomeTabela[a] = comando[i];
		}
		nomeTabela[a] = '\0';
	}

	for (i = strlen("SELECT "), a = 0; comando[i] != ';'; i++, a++)
	{
		if (comando[i] == ',' || comando[i] == ' ')
		{
			if (a > 0 && strcmp(nomeAtributo, "ROM") != 0)
			{ // Verifique se hï¿½ algum caractere vï¿½lido no atributo
				nomeAtributo[a] = '\0';
				a = 0;
				i += 2;
				printf("\nNome do atributo selecionado: %s\n", nomeAtributo);
				Select(&(*bancoDeDados), nomeTabela, nomeAtributo);
			}
		}
		nomeAtributo[a] = comando[i];
	}
}

void ExibirAtributoNoNivel(tabela **auxTabela, int nivel, char nomeAtributo[], int i)
{
	atributo *auxAtributo;
	ldados *auxDados;
	auxAtributo = (*auxTabela)->listaAtributos;
	int nivelAtual = 1;
	while (auxAtributo != NULL && strcmp(auxAtributo->campo, nomeAtributo) != 0)
	{
		auxAtributo = auxAtributo->prox;
	}
	
	if(auxAtributo != NULL){
		
		auxDados = auxAtributo->listaDados;
		while (nivelAtual != nivel)
		{
			auxDados = auxDados->prox;
			nivelAtual++;
		}
	
		if (nivel == nivelAtual)
		{
			switch (auxAtributo->tipo)
			{
				case 'I':
					printf("%d - %d\n", i, auxDados->d.valorI);
					break;
				case 'C':
					printf("%d - %s\n", i, auxDados->d.valorT);
					break;
				case 'N':
					printf("%d - %.1f\n", i, auxDados->d.valorN);
					break;
				default:
					printf("%d - %s\n", i, auxDados->d.valorT);
			}
		}	
	}
}

void SelectBetween(bd **bancoDeDados, char nomeTabela[], char nomeAtributoChave[], char nomeAtributo[], char valor1[], char valor2[])
{
	tabela *auxTabela;
	atributo *auxAtributo, *auxAtributo2;
	ldados *dados;
	int nivel, i=1;
	char intToChar[50];
	auxTabela = (*bancoDeDados)->listaTabela;
	while (auxTabela != NULL && strcmp(auxTabela->nometabela, nomeTabela) != 0)
	{
		auxTabela = auxTabela->prox;
	}

	if (auxTabela != NULL)
	{
		auxAtributo = auxTabela->listaAtributos;
		auxAtributo2 = auxTabela->listaAtributos;
		while (auxAtributo != NULL && strcmp(auxAtributo->campo, nomeAtributoChave) != 0)
		{
			auxAtributo = auxAtributo->prox;
		}
		if (strcmp(nomeAtributo, "ALL") == 0)
		{
			dados = auxAtributo->listaDados;
			while (dados != NULL)
			{
				i=1;
				if (auxAtributo->tipo == 'I')
				{
					if (dados->d.valorI >= atoi(valor1) && dados->d.valorI <= atoi(valor2))
					{
						itoa(dados->d.valorI, intToChar, 10);
						nivel = BuscaNivelDado(&(*bancoDeDados), nomeTabela, nomeAtributoChave, intToChar);
						while(auxAtributo2!=NULL){
							ExibirAtributoNoNivel(&auxTabela, nivel, auxAtributo2 -> campo, i++);
							auxAtributo2 = auxAtributo2->prox;
						}
						auxAtributo2 = auxTabela->listaAtributos;
						
					}
				}
				else if (auxAtributo->tipo == 'T')
				{
					if (strcmp(dados->d.valorT, valor1) >= 0 && strcmp(dados->d.valorT, valor2) <= 0)
					{
						nivel = BuscaNivelDado(&(*bancoDeDados), nomeTabela, nomeAtributoChave, dados->d.valorT);
						while(auxAtributo2!=NULL){
							ExibirAtributoNoNivel(&auxTabela, nivel, auxAtributo2 -> campo, i++);
							auxAtributo2 = auxAtributo2->prox;
						}
						auxAtributo2 = auxTabela->listaAtributos;
					}
				}
				dados = dados->prox;
			}
		}
		else{
			if (auxAtributo != NULL)
			{
				dados = auxAtributo->listaDados;
				while (dados != NULL)
				{
					if (auxAtributo->tipo == 'I')
					{
						if (dados->d.valorI >= atoi(valor1) && dados->d.valorI <= atoi(valor2))
						{
							itoa(dados->d.valorI, intToChar, 10);
							nivel = BuscaNivelDado(&(*bancoDeDados), nomeTabela, nomeAtributoChave, intToChar);
							ExibirAtributoNoNivel(&auxTabela, nivel, nomeAtributo, i++);
						}
					}
					else if (auxAtributo->tipo == 'T')
					{
						if (strcmp(dados->d.valorT, valor1) >= 0 && strcmp(dados->d.valorT, valor2) <= 0)
						{
							nivel = BuscaNivelDado(&(*bancoDeDados), nomeTabela, nomeAtributoChave, dados->d.valorT);
							ExibirAtributoNoNivel(&auxTabela, nivel, nomeAtributo, i++);
						}
					}
					dados = dados->prox;
				}
			}
		}
	}
}

void CortarSQLSelectBetween(bd **bancoDeDados, char comando[])
{
	int i, a;
	char nomeTabela[20] = "", nomeAtributo[50], nomeAtributoChave[20], valor1[10], valor2[10];
	tabela *auxTabela;
	atributo *auxAtributo;
	ldados *auxDados;
	char *ponteiroFrom = NossaStrStr(comando, "FROM");
	if (ponteiroFrom != NULL)
	{
		i = ponteiroFrom - comando + strlen("FROM ");

		while (comando[i] == ' ')
		{
			i++;
		}

		for (a = 0; comando[i] != ' ' && comando[i] != ';'; a++, i++)
		{
			nomeTabela[a] = comando[i];
		}
		nomeTabela[a] = '\0';
	}

	char *ponteiroWhere = NossaStrStr(comando, "WHERE");
	if (ponteiroWhere != NULL)
	{
		i = ponteiroWhere - comando + strlen("WHERE ");

		while (comando[i] == ' ')
		{
			i++;
		}

		for (a = 0; comando[i] != ' ' && comando[i] != ';'; a++, i++)
		{
			nomeAtributoChave[a] = comando[i];
		}
		nomeAtributoChave[a] = '\0';
	}

	char *ponteiroBetween = NossaStrStr(comando, "BETWEEN ");
	if (ponteiroBetween != NULL)
	{
		i = ponteiroBetween - comando + strlen("BETWEEN ");

		for (a = 0; comando[i] != ' ' && comando[i] != ';'; a++, i++)
		{
			valor1[a] = comando[i];
		}
		valor1[a] = '\0';

		i += strlen("AND ");

		for (a = 0; comando[i] != ';'; a++, i++)
		{
			valor2[a] = comando[i];
		}
		valor2[a] = '\0';
	}

	i = strlen("SELECT ");
	if (comando[i] == '*'){
		auxTabela = (*bancoDeDados) -> listaTabela;
		while(auxTabela != NULL && strcmp(auxTabela -> nometabela, nomeTabela) != 0){
			auxTabela = auxTabela -> prox;
		}
		
		if(auxTabela != NULL){
			auxAtributo = auxTabela -> listaAtributos;
			while(auxAtributo != NULL){
				printf("\nAtributo: %s\n", auxAtributo -> campo);
				SelectBetween(&(*bancoDeDados), nomeTabela, nomeAtributoChave, auxAtributo->campo, valor1, valor2);
				auxAtributo = auxAtributo -> prox;
			}
		}
	}
	else
	{
		while (i < strlen(comando) && comando[i] != 'F')
		{
			while (comando[i] == ',' || comando[i] == ' ')
				i++;
			for (a = 0; comando[i] != ' ' && comando[i] != ';' && comando[i] != 'F' && comando[i] != ','; i++, a++)
			{
				nomeAtributo[a] = comando[i];
			}
			nomeAtributo[a] = '\0';
			if (comando[i] != 'F')
			{
				printf("\nAtributo: %s\n", nomeAtributo);
				SelectBetween(&(*bancoDeDados), nomeTabela, nomeAtributoChave, nomeAtributo, valor1, valor2);
			}
		}
	}
}

char LeComando(bd **bancoDeDados, char comando[])
{
	if (!strcmp(comando, "sair"))
	{
		printf("saiu\n");
		return '0';
	}
	else
	{
		char *ponteiroClear = NossaStrStr(comando, "clear");
		if (ponteiroClear != NULL)
		{
			system("cls");
		}

		char *ponteiroExibir = NossaStrStr(comando, "exibir");
		if (ponteiroExibir != NULL)
		{
			exibirBanco((*bancoDeDados));
		}

		char *ponteiroInsert = NossaStrStr(comando, "INSERT INTO");
		if (ponteiroInsert != NULL)
		{
			CortarSQLInsert(&(*bancoDeDados), comando);
		}

		char *ponteiroUpdate = NossaStrStr(comando, "UPDATE");
		if (ponteiroUpdate != NULL)
		{
			CortarSQLUpdate(&(*bancoDeDados), comando);
		}

		char *ponteiroSelect = NossaStrStr(comando, "SELECT");
		if (ponteiroSelect != NULL)
		{
			char *SelectAll = NossaStrStr(comando, "*");
			if (SelectAll != NULL)
			{
				char *SelectAllBetween = NossaStrStr(comando, "BETWEEN");
				char *SelectAllAnd = NossaStrStr(comando, "AND");

				if (SelectAllBetween != NULL)
				{
					CortarSQLSelectBetween(&(*bancoDeDados), comando);
				}
				else if (SelectAllAnd != NULL)
				{
					// chamar a funï¿½ï¿½o do SelectAllAnd
				}
				else
				{
					CortarSQLSelectAll(&(*bancoDeDados), comando);
				}
			}
			else
			{
				char *SelectBetween = NossaStrStr(comando, "BETWEEN");
				if (SelectBetween != NULL)
				{
					CortarSQLSelectBetween(&(*bancoDeDados), comando);
				}
				else
				{
					CortarSQLSelect(&(*bancoDeDados), comando);
				}
			}
		}

		char *ponteiroDelete = NossaStrStr(comando, "DELETE");
		if (ponteiroDelete != NULL)
		{
			CortarSQLDelete(&(*bancoDeDados), comando);
		}
		return '1';
	}
}

int main()
{
	char comando[4000];
	bd *bancoDeDados;
	LeArquivo(&bancoDeDados);
	printf("Digite o comando: ");
	fflush(stdin);
	gets(comando);
	while (LeComando(&bancoDeDados, comando) != '0')
	{
		printf("\nDigite o comando: ");
		fflush(stdin);
		gets(comando);
	}
	exibir(bancoDeDados);
}
