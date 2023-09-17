struct BancoDeDados{
	char nome_banco[20];
	struct Tabela *listaTabela;
};
typedef struct BancoDeDados bd;

struct Tabela{
	struct Tabela *ant, *prox;
	char nometabela[50];
	struct Atributo *listaAtributos;
};
typedef struct Tabela tabela;

struct Atributo{
	struct Atributo *prox, *FK;
	char campo[50], tipo, PK;
	struct ldados *listaDados; //No PDF do trabalho est� representado como um campo dentro de T1, T2 que aponta para uma lista de dados
};
typedef struct Atributo atributo;

union Dados{
	int valorI; //Se o dado for um inteiro
	float valorN; //Se o dado for um decimal (float)
	char valorD[10]; //Se o dado for uma data
	char valorC; //Se o dado for apenas um caracter
	char valorT[20]; //Se o dado for um texto
};

struct ListaDados{
	union Dados d;
	char terminal; 
	struct ListaDados * prox;
};
typedef struct ListaDados ldados;

// ---------------->>>>>> O certo aqui é fazer uma função geralzona que vai ler linha - a - linha
// do arquivo txt, e dentro da condicional, quando achar um CREATE DATABASE, ela chama essa funcao de criar
// o Banco de Dados

void CriarBancoDeDados (bd * * bancoDeDados, char nome [50]) {
	* bancoDeDados = (bd *)malloc(sizeof(bd));
	strcpy(((* bancoDeDados) ->  nome_banco), nome);
	(* bancoDeDados) -> listaTabela = NULL;
	printf("Banco de dados com o nome de %s criado com sucesso\n", (* bancoDeDados) -> nome_banco);
}

void exibir(bd *b_dados){
	if(b_dados != NULL)
		printf("Banco %s", b_dados -> nome_banco);
}


















