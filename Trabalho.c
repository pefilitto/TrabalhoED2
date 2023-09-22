#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TADTrabalho.h"

char buscarTabela(bd **bancoDeDados, char nomeTabela[])
{
	tabela *aux = (*bancoDeDados)->listaTabela;
	while (aux->prox != NULL && strcmp(nomeTabela, aux->nometabela) != 0)
		aux = aux->prox;

	if (aux == NULL)
		return 0;
	return 1;
}

void InsereValorNoAtributo(char nomeAtributo[], char valorAtributo[])
{
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
		aux->prox = novoAtributo;
	}
	printf("Atributo %s inserido na tabela %s\n", (*novoAtributo)->campo, (*tabela)->nometabela);
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

	printf("\nTabela %s foi criada dentro do banco %s\n", (*novaTabela)->nometabela, (*bancoDeDados)->nome_banco);
}

void AtribuiPKaoAtributo(tabela **t, char nome[])
{
	atributo *aux = (*t)->listaAtributos;
	while (aux->prox != NULL & strcmp(nome, aux->campo) != 0)
		aux = aux->prox;

	if (aux != NULL)
	{
		aux->PK = 'S';
		printf("Atributo %s recebeu PK\n", aux->campo);
	}
}

void LerAtributosDoArquivo(FILE **arquivo, tabela **tabelaCriada)
{
	char nome[50], tipo[20], linha[1000], linha2[1000];
	int flag = 0, i, j, z = 0;
	if (*arquivo == NULL)
		printf("Ponteiro invalido na funcao leratributos\n");
	else
	{
		while (flag == 0)
		{
			fscanf(*arquivo, "%[^\n]", &linha);
			fgetc(*arquivo);
			if (strstr(linha, "CONSTRAINT") != NULL)
			{
				if (strstr(linha, "PRIMARY KEY") != NULL)
				{
					printf("Achou o primary key\n");
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
				if (strstr(linha, ");") != NULL)
				{
					// Quando achar o );
					fgets(linha, sizeof(linha), (*arquivo));
					flag = 1;
				}
				else
				{
					for (i = 0; i < strlen(linha) && linha[i] != ' '; i++)
						nome[i] = linha[i];
					nome[i] = '\0';
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
		char *ponteiroNomeDoBanco = strstr(linha, "CREATE DATABASE");
		if (ponteiroNomeDoBanco != NULL)
		{
			GetName(ponteiroNomeDoBanco, auxNome, "CREATE DATABASE");
			CriarBancoDeDados(b_dados, auxNome);
		}
		char *ponteiroNomeDaTabela = strstr(linha, "CREATE TABLE");
		if (ponteiroNomeDaTabela != NULL)
		{
			GetName(ponteiroNomeDaTabela, auxNome, "CREATE TABLE");
			CriarTabela(b_dados, auxNome, &ptr);
		}

		char *ponteiroAlterTable = strstr(linha, "ALTER TABLE");
		if (ponteiroAlterTable != NULL)
		{
			printf("\nAchou o alter table\n");
			GetName(ponteiroAlterTable, auxNome, "ALTER TABLE");
			if (buscarTabela(b_dados, auxNome))
			{
				// Colocar aqui a funcao para manipular o ALTER TABLE
			}
		}
	}
	fclose(ptr);
}

// Finalizando a cria��o das estruturas

// Iniciando os comandos INSERT, DELETE, SELECT e UPDATE

void Insert(bd **bancoDeDados, char nomeTabela[], char campo[], char valor[])
{
	tabela *aux;
	int x = 0, y = 0;
	aux = (*bancoDeDados)->listaTabela;
	while (aux->nometabela != NULL && stricmp(aux->nometabela, nomeTabela) != 0)
	{
		aux = aux->prox;
	}
	atributo *auxAtr;
	auxAtr = aux->listaAtributos;
	while (auxAtr != NULL && stricmp(auxAtr->campo, campo) != 0)
		auxAtr = auxAtr->prox;
		
	ldados *auxDados;
	auxDados = auxAtr->listaDados;
	if (auxDados == NULL)
	{
		auxDados = (ldados *)malloc(sizeof(ldados));
		auxDados->prox = NULL;
		auxDados->terminal = 1;
		strcpy(auxDados->d.valorT, valor);
	}
	else
	{
		while (auxDados -> prox != NULL)
		{
			auxDados = auxDados->prox;
		}
		auxDados->prox = (ldados *)malloc(sizeof(ldados));
		auxDados = auxDados->prox;
		auxDados->prox = NULL;
		auxDados->terminal = 1;
		strcpy(auxDados->d.valorT, valor);
	}
	y++;
	printf("\nInseriu\n");
}

void CortarSQLAtributos(bd **bancoDeDados, char nomeTabela[], char campos[], char valores[])
{
	int i, c = 0, v = 0, k, flagParenteses = 0, l;
	char campo[15], valor[15];

	while (!flagParenteses)
	{

		for (i = 0; campos[c] != ',' && campos[c] != ')' && c < strlen(campos); i++, c++)
		{
			campo[i] = campos[c];
		}
		c++;
		campo[i] = '\0';
		
		for (l = 0; valores[v] != ',' && valores[v] != ')' && v < strlen(valores); v++, l++)
		{
			valor[l] = valores[v];
		}
		valor[l] = '\0';
		v++;
		printf("Inserindo campo %s, valor %s\n", campo, valor);
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
	inicio_campos++; // Avan�ar para o primeiro caractere ap�s o par�ntese

	char *fim_campos = strchr(inicio_campos, ')');

	char campos[100];
	int tamanho_campos = fim_campos - inicio_campos;
	strncpy(campos, inicio_campos, tamanho_campos);
	campos[tamanho_campos] = '\0';

	char *inicio_valores = strstr(comando, "VALUES");
	inicio_valores = strchr(inicio_valores, '(');
	inicio_valores++; // Avan�ar para o primeiro caractere ap�s o par�ntese

	char *fim_valores = strrchr(comando, ')'); // Usar strrchr para encontrar o �ltimo ')'

	char valores[100];
	int tamanho_valores = fim_valores - inicio_valores;
	strncpy(valores, inicio_valores, tamanho_valores);
	valores[tamanho_valores] = '\0';

	printf("Campos %s \nValores %s \n", campos, valores);
	CortarSQLAtributos(&(*bancoDeDados), nomeTabela, campos, valores);
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
		char *ponteiroInsert = strstr(comando, "INSERT INTO");
		if (ponteiroInsert != NULL)
		{
			CortarSQLInsert(&(*bancoDeDados), comando);
		}
		char *ponteiroUpdate = strstr(comando, "UPDATE");
		if (ponteiroUpdate != NULL)
		{
			// Atualizar(comando);
		}
		char *ponteiroSelect = strstr(comando, "SELECT");
		if (ponteiroSelect != NULL)
		{
			// Selecionar(comando);
		}
		char *ponteiroDelete = strstr(comando, "DELETE");
		if (ponteiroDelete != NULL)
		{
			// Deletar(comando);
		}
		return '1';
	}
}

int main()
{
	char comando[2000];
	bd *bancoDeDados;
	LeArquivo(&bancoDeDados);
	printf("Digite o comando: ");
	fflush(stdin);
	while (LeComando(&bancoDeDados, gets(comando)) != '0')
	{
		printf("Digite o comando: ");
		fflush(stdin);
	}
	exibir(bancoDeDados);
}
