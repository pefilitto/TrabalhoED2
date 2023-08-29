struct BancoDeDados{
	char nome_banco[20];
	struct Tabela *listatabela;
};
typedef struct BancoDeDados bd;

struct Tabela{
	struct Tabela *ant, *prox;
	char nometabela[50];
	struct atributo *listaatributos;
};
typedef struct Tabela tabela;

struct Atributo{
	struct Atributos *prox;
	char campo[50], tipo, PK;
	struct ldados *listadados; //No PDF do trabalho está representado como um campo dentro de T1, T2 que aponta para uma lista de dados
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


void InicializaBanco(bd **b_dados){
	*b_dados = (bd *)malloc(sizeof(bd));
	char *achou;
	FILE *ptr = fopen("ComandoSQL.txt", "r");
	char linha[1000];
	if(*b_dados == NULL)
		printf("Erro ao alocar memoria para a criacao do banco de dados\n");
	else{
		if(ptr == NULL)
			printf("Erro ao abrir arquivo de script de comandos\n");
		else{
			//Lendo a primeira linha do arquivo
			if(fgets(linha, sizeof(linha), ptr) != NULL){
				//Guardando o nome do banco que esta no arquivo na variavel ponteiro 'nomeBanco'
				achou = strstr(linha, "CREATE DATABASE");
				
				if(achou != NULL){
					achou += strlen("CREATE DATABASE");
					while(*achou == ' ')
						achou++;
					
					//Guardando na variavel 'tamanho_nome' o tamanho do nome do banco de dados para usar na funcao strncpy
					int tamanho_nome = strcspn(achou, " ;\n");
					
					//Parametros da funcao strncpy -> (destino, origem, tamanho).
					//Estamos utilizando essa funcao para garantir que o ; e o \n nao sejam copiados
					strncpy((*b_dados) -> nome_banco, achou, tamanho_nome);
					
					//Assegurando que a string terminara com o '\0'
					(*b_dados) -> nome_banco[tamanho_nome] = '\0';
					(*b_dados) -> listatabela == NULL;			
				}	
			}
		}
	
		printf("Banco com o nome %s foi criado com sucesso\n", (*b_dados) -> nome_banco);
	}
}


//Funcao teste para ver se realmente criou o banco
void exibir(bd *b_dados){
	if(b_dados != NULL)
		printf("\n\nNome do banco: %s", b_dados->nome_banco);
}
















