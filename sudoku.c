#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

int jogo[9][9];
int jogoResolvido[9][9];
int elementos = 0; //Número de elementos que já foram completos na matriz jogo
int numJogo; //Variável para escolher o jogo
int jogos[5][9][9] = {{{9,7,3,8,1,6,4,5,2}, //Opções de jogos
			           {6,1,5,7,4,2,8,3,9},
			           {4,8,2,3,9,5,6,1,7},
			           {5,2,8,6,7,9,1,4,3},
			           {7,3,9,1,5,4,2,8,6},
			           {1,4,6,2,8,3,9,7,5},
			           {8,9,1,5,6,7,3,2,4},
			           {3,5,4,9,2,1,7,6,8},
			           {2,6,7,4,3,8,5,9,1}},
			          {{8,5,6,4,2,1,7,3,9},
			           {9,2,3,8,6,7,4,5,1},
			           {4,1,7,5,3,9,8,6,2},
			           {6,9,8,1,4,2,5,7,3},
			           {1,7,4,3,9,5,2,8,6},
			           {5,3,2,6,7,8,1,9,4},
			           {2,8,5,9,1,3,6,4,7},
			           {7,4,9,2,5,6,3,1,8},
			           {3,6,1,7,8,4,9,2,5}},
			          {{1,2,7,4,8,5,6,3,9},
			           {3,4,6,9,2,1,7,8,5},
			           {8,9,5,3,6,7,4,1,2},
			           {4,7,8,6,3,2,9,5,1},
			           {6,1,3,8,5,9,2,7,4},
			           {2,5,9,7,1,4,3,6,8},
			           {9,6,1,5,4,3,8,2,7},
			           {7,3,2,1,9,8,5,4,6},
			           {5,8,4,2,7,6,1,9,3}},
			          {{9,5,7,2,6,1,8,3,4},
			           {8,6,1,3,7,4,9,2,5},
			           {4,3,2,9,8,5,1,6,7},
			           {2,4,8,6,5,3,7,9,1},
			           {5,9,6,4,1,7,2,8,3},
			           {1,7,3,8,2,9,5,4,6},
			           {3,2,5,1,9,6,4,7,8},
			           {7,8,4,5,3,2,6,1,9},
			           {6,1,9,7,4,8,3,5,2}},
			          {{1,4,5,3,7,8,9,2,6},
			           {3,9,7,4,2,6,8,5,1},
			           {6,2,8,9,5,1,3,7,4},
			           {4,7,9,5,1,3,2,6,8},
			           {8,3,2,7,6,4,1,9,5},
			           {5,1,6,2,8,9,4,3,7},
			           {9,5,1,8,3,7,6,4,2},
			           {7,8,4,6,9,2,5,1,3},
			           {2,6,3,1,4,5,7,8,9}}};
					   
enum DOS_COLORS {
        BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
        LIGHT_GRAY, DARK_GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN,
        LIGHT_RED, LIGHT_MAGENTA, YELLOW, WHITE };

void textcolor (enum DOS_COLORS iColor)
{
        HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        BOOL b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
        bufferInfo.wAttributes &= 0x00F0;
        SetConsoleTextAttribute (hl, bufferInfo.wAttributes |= iColor);
}

void backcolor (enum DOS_COLORS iColor)
{
        HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        BOOL b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
        bufferInfo.wAttributes &= 0x000F;
        SetConsoleTextAttribute (hl, bufferInfo.wAttributes |= (iColor << 4));
}

void inserir(int lin, int col){ //Insere o número na posição especificada
	jogo[lin][col] = jogoResolvido[lin][col];
}

bool vPreenchido(int lin, int col){ //Verifica se a posição inserida já foi preenchida
	if(jogo[lin][col]==0){
		return false;
	}else{
		return true;
	}
}

bool verificar(int lin, int col, int x){ //Verifica se o número está correto
	if(vPreenchido(lin,col)==true){ //Se já foi preenchido, retorna false
		return false;
	}else{
		if(x==jogoResolvido[lin][col]){ //Se x estiver correto, retorna true
			return true;
		}else{
			return false;
		}
	}
}

bool vQtdNum(int x){ //Verifica a quantidade de números X já inseridos (não pode ultrapassar 9)
	int qtdX = 0;
	
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			if(jogo[i][j]==x){
				qtdX++;
			}
		}
	}
	
	if(qtdX==9){
		return false;
	}else{
		return true;
	}
}

bool vNumInserido(int lin, int col, int x){ //Verifica se linha, coluna e X inserido pelo usuário é valido
	if(((lin>=0)&&(lin<9))&&((col>=0)&&(col<9))&&((x>=1)&&(x<10))){
		return true;
	}else{
		return false;
	}
}

void criarJogo(int dif){ //Preenche certa quantidade de posições do jogo dependendo da dificuldade
	int lin, col, qtdElementos;
	
	switch (dif){ //Escolhe a dificuldade do jogo
		case 1:
		//42
			qtdElementos = 81;
			break;
		case 2:
			qtdElementos = 33;
			break;
		case 3:
			qtdElementos = 24;
			break;
	}
	
	for(int i=0;i<9;i++){ //Deixa a matriz jogo zerada
		for(int j=0;j<9;j++){
			jogo[i][j]=0;
		}
	}
	
	srand(time(NULL));
	numJogo = rand() % 5; //Escolhe aleatoriamente um jogo para o jogador
	
	for(int i=0;i<9;i++){ //Preenche a matriz jogoResolvido com o jogo sorteado
		for(int j=0;j<9;j++){
			jogoResolvido[i][j] = jogos[numJogo][i][j];
		}
	}
	
	while(elementos<qtdElementos){
		lin = rand() % 9;
		col = rand() % 9;
		if(vPreenchido(lin,col)==false){
			inserir(lin,col);
			elementos++;
		}
	}
}

void mostrar(){ //Mostra a matriz sendo preenchida pelo jogador
	printf("\n 1 2 3 4 5 6 7 8 9");
	printf("\n-------------------");
	for(int i=0;i<9;i++){
		printf("\n");
		printf("|");
		if(i%3==0 && i!=0){
			printf("-----------------|\n|");
		}
		for(int j=0;j<9;j++){
			if(j%3==0 && j!=0){
				printf("|");
			}
			if(j==2 || j==5 || j==8){
				printf("%d", jogo[i][j]);
			}else{
				printf("%d ", jogo[i][j]);
			}	
		}
		printf("|");
		printf(" %d", i+1);
	}
	printf("\n-------------------");
}

void jogada(){
	int lin, col, x;
	bool vNums = false;
	
	mostrar();
	
	while(vNums==false){	
		printf("\nRealize sua jogada.\n\n");
		printf("Insira a linha (1 ate 9): ");
		scanf("%d", &lin);
		printf("Insira a coluna (1 ate 9): ");
		scanf("%d", &col);
		printf("Insira o numero desejado (1 ate 9): ");
		scanf("%d", &x);
		
		vNums = vNumInserido(lin-1, col-1, x);
		if(vNums==false){
			printf("\nInsira posicoes e numeros validos.\n");
		}
	}
	
	system("cls");
	
	if(vQtdNum(x)==false){
		printf("Quantidade maxima de numeros %d ja foi atingida.\n", x);
	}else{
		if(verificar(lin-1, col-1, x)==true){
			inserir(lin-1, col-1);
			elementos++;
			printf("Muito bem, voce acertou!\n");
		}else{
			printf("Poxa, o numero %d nao pode estar na posicao [%d,%d]. Tente novamente.\n", x, lin, col);
		}
	}
}

int jNovamente(){ //Pergunta se o jogador deseja jogar novamente
	int jogarNovamente;
	while(jogarNovamente!=1 || jogarNovamente!=2){
		printf("\nDeseja jogar novamente?\n1- Sim\n2- Nao\n");
		printf("Sua resposta: ");
		scanf("%d", &jogarNovamente);
		
		if(jogarNovamente==1){
			elementos = 0;
			return 1;
		}else{
			if(jogarNovamente==2){
				printf("\nAte a proxima!");
				return 2;
				system("taskkill /f /im SUDOKU");
			}
		}
	}
}

int main(){
	int dif;
	bool validarDif;
	int jogarNovamente = 1;
	int gostou = 0;
	
	int fundo, texto;
    enum DOS_COLORS c[16] = {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHT_GRAY, DARK_GRAY,
                        LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, LIGHT_MAGENTA, YELLOW, WHITE};
    
    //comando de regionalização
    setlocale(LC_ALL, "Portuguese");
    //título do programa
    SetConsoleTitle("SUDOKU");
    
	do{
	
	system("cls");
	printf("Seja bem-vindo ao nosso Sudoku!\n");
	printf ("\nAntes de iniciar o jogo, vamos alterar a cor do texto?\n");
    printf ("\nTabela de cores:\n\n");
    printf ("\t0 - Preto\t\t8 - Cinza Escuro\n");
    printf ("\t1 - Azul\t\t9 - Azul Claro\n");
    printf ("\t2 - Verde\t\t10 - Verde Claro\n");
    printf ("\t3 - Ciano\t\t11 - Ciano Claro\n");
    printf ("\t4 - Vermelho\t\t12 - Vermelho Claro\n");
    printf ("\t5 - Magenta\t\t13 - Magenta Claro\n");
    printf ("\t6 - Marrom\t\t14 - Amarelo\n");
    printf ("\t7 - Cinza Claro\t\t15 - Branco\n");
    
    //printf ("\n\nCor de fundo: ");
    //scanf("%d", &fundo);
    
	printf ("\nObs.: Caso nao queira alterar, entre com o numero 7 - Cinza Claro\n");
    printf ("\nCor do texto: ");
    scanf("%d", &texto);
    
    system ("cls");
    
    //backcolor(c[fundo]);
    textcolor(c[texto]);
    
    printf ("Muito bem, a cor do texto foi alterada!\n\n");
	printf ("Gostou?\n1- Sim\n2- Nao, quero alterar novamente\n");
	printf ("\nSua resposta: ");
	scanf("%d",&gostou);
	
	if(gostou==1){
		break;
	}else{ 
		if(gostou==2){
			//backcolor(c[BLACK]);
			textcolor(c[LIGHT_GRAY]);
			continue;
		}else{
			printf("Digite uma opcao valida.\n");
		}
	}
	}while(gostou!=1 || gostou!=2);
	
	system ("cls");
	
	while(jogarNovamente==1){
		validarDif = false;
		while(validarDif==false){
			printf("*****Sudoku*****\n\n");
			printf("Selecione a dificuldade:\n1- Facil\n2- Medio\n3- Dificil\n");
			printf("Sua resposta: ");
			scanf("%d", &dif);
			
			if(dif<=0 || dif>3){
				printf("Digite uma opcao valida.\n");
			}else{
				validarDif = true;
			}
		}
		
		system("cls");
		
		criarJogo(dif);
		
		while(elementos<81){ //Enquanto a matriz não estiver comleta, a função jogada será chamada
			jogada();
		}
		
		mostrar();

		printf("\n***FIM DE JOGO***\n\nParabens, voce VENCEU!\n");

		jogarNovamente = jNovamente();
	}
	
	return 0;
}