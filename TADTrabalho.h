struct BancoDeDados{
	char nome_banco[20];
	struct Tabela *listaTabela;
};
typedef struct BancoDeDados bd;

struct Tabela{
	struct Tabela *ant, *prox;
	char nometabela[50];
	struct atributo *listaAtributos;
};
typedef struct Tabela tabela;

struct Atributo{
	struct Atributos *prox;
	char campo[50], tipo, PK;
	atributo * FK;
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
};
typedef struct ListaDados ldados;


// void InicializaBanco(bd **b_dados){
	
// 	char *ponteiroNomeDoBanco;
// 	FILE *ptr = fopen("ComandoSQL.txt", "r");
// 	char linha[1000];
// 	if(*b_dados == NULL)
// 		printf("Erro ao alocar memoria para a criacao do banco de dados\n");
// 	else{
// 		if(ptr == NULL)
// 			printf("Erro ao abrir arquivo de script de comandos\n");
// 		else{
// 			//Lendo a primeira linha do arquivo
// 			if(fgets(linha, sizeof(linha), ptr) != NULL){
// 				//Guardando o nome do banco que esta no arquivo na variavel ponteiro 'nomeBanco'
// 				ponteiroNomeDoBanco = strstr(linha, "CREATE DATABASE");s
				
// 				if(ponteiroNomeDoBanco != NULL){
// 					ponteiroNomeDoBanco += strlen("CREATE DATABASE");
// 					while(*ponteiroNomeDoBanco == ' ')
// 						ponteiroNomeDoBanco++;
					
// 					//Guardando na variavel 'tamanho_nome' o tamanho do nome do banco de dados para usar na funcao strncpy
// 					int tamanho_nome = strcspn(ponteiroNomeDoBanco, " ;\n");
					
// 					//Parametros da funcao strncpy -> (destino, origem, tamanho).
// 					//Estamos utilizando essa funcao para garantir que o ; e o \n nao sejam copiados
// 					strncpy((*b_dados) -> nome_banco, ponteiroNomeDoBanco, tamanho_nome);
					
// 					//Assegurando que a string terminara com o '\0'
// 					(*b_dados) -> nome_banco[tamanho_nome] = '\0';
// 					(*b_dados) -> listaTabela == NULL;			
// 				}	
// 			}
// 		}
	
// 		printf("Banco com o nome %s foi criado com sucesso\n", (*b_dados) -> nome_banco);
// 	}
// }

// ---------------->>>>>> O certo aqui é fazer uma função geralzona que vai ler linha - a - linha
// do arquivo txt, e dentro da condicional, quando achar um CREATE DATABASE, ela chama essa funcao de criar
// o Banco de Dados

void CriarBancoDeDados (bd * * bancoDeDados, char nome [50]) {
	* bancoDeDados = (bd *)malloc(sizeof(bd));
	strcpy((* bancoDeDados) ->  nome_banco), nome);
	(* bancoDeDados) -> listaTabela = NULL;
}


void exibir(bd *b_dados){
	if(b_dados != NULL)
		printf("\n\nNome do banco: %s", b_dados->nome_banco);
}
















