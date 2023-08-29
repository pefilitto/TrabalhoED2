#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio2.h>
#include "TADTrabalho.h"

int main()
{
	bd *banco_dados;
	InicializaBanco(&banco_dados);
	exibir(banco_dados);
}
