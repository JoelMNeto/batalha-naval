// Joel Marçola Neto e João Pedro da Silva Vieira
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#ifdef _WIN64
    #include <windows.h>
#elif __linux
    #include <unistd.h>
#endif

void limparTela();
void mostrarTitulo();
void mostrarRegras();
void timeout(int tempoMs);
void trocarJogador(char jogador1[], char jogador2[], int pontos[]);
void mostrarMatriz(char matriz[][8], int linhas, int colunas);
void posicionarNavio(char matriz[][8], int linhas, int colunas);
void inicializarMatriz(char matriz[][8], int linhas, int colunas);
void mostrarPlacar(char jogador1[], char jogador2[], int pontos[]);
void mostrarJogadorPontuou(char jogador1[], char jogador2[], int pontos[], int destruiuNavio);
void acertouTiro(char matriz[][8], char matrizGabarito[][8], int tiro[], int retorno[], int linhas, int colunas);
int  verificaVencedor(char jogador1[], char jogador2[], int pontos[]);
int  inserirNavio(char matriz[][8], int posicao[], int linhas, int colunas);
int  inserirCasa(char matriz[][8], char jogada, int posicao[], int linhas, int colunas);
int  inserirJogada(char matriz[][8], char matrizGabarito[][8], int linhas, int colunas);

int main(){ 
    setlocale(LC_ALL, "Portuguese_Brazil");

    char nomeJogador1[20], nomeJogador2[20], 
        matrizGabarito[8][8], matrizJogo[8][8], resposta = 'N';

    int destruiuNavio = 0, vencedor = 0, podePassarVez = 0, pontos[2] = {0, 0};

    do {
        mostrarTitulo();

        mostrarRegras();

        printf("\tJogador 1, digite seu nome: ");
        gets(nomeJogador1);

        printf("\n\tJogador 2, digite seu nome: ");
        gets(nomeJogador2);

        nomeJogador1[0] = toupper(nomeJogador1[0]);
        nomeJogador2[0] = toupper(nomeJogador2[0]);

        printf("\n\tVamos começar!!\n");

        do {
            mostrarTitulo();
            mostrarPlacar(nomeJogador1, nomeJogador2, pontos);

            if(podePassarVez) {
                trocarJogador(nomeJogador1, nomeJogador2, pontos);
            }

            inicializarMatriz(matrizJogo, 8, 8);

            inicializarMatriz(matrizGabarito, 8, 8);

            printf("\n\t%s, posicione seu navio [três casas]\n", nomeJogador1);

            printf("\n\tATENÇÃO! Garanta que o outro jogador não está vendo a sua tela.\n");

            posicionarNavio(matrizGabarito, 8, 8);

            mostrarTitulo();

            printf("\n\t%s, Sua vez de jogar, você tem direito a 5 tiros para tentar acertar o navio.\n", nomeJogador2);

            printf("\n\tA cada tiro certo você ganha mais um tiro extra.\n");

            printf("\n\tBoa sorte!\n");

            destruiuNavio = inserirJogada(matrizJogo, matrizGabarito, 8, 8);

            mostrarJogadorPontuou(nomeJogador1, nomeJogador2, pontos, destruiuNavio);
            
            vencedor = verificaVencedor(nomeJogador1, nomeJogador2, pontos);

            podePassarVez = 1;

        } while(!vencedor);

        printf("\n\tJogar novamente [S/N]: ");
        scanf("%c", &resposta);

        resposta = toupper(resposta);

    } while(resposta == 'S');

    return 0; 
}

void inicializarMatriz(char matriz[][8], int linhas, int colunas){
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            matriz[i][j] = '~';
        }
    }
}

void mostrarMatriz(char matriz[][8], int linhas, int colunas){
    printf("\n\n");

    printf("\t");
    printf("       ");

    for(int j = 0; j <= 7; j++) {
        printf("%d    ", j);
      
    }

    printf("\n\t    |----------------------------------------|\n");

    for(int i = 0; i < linhas; i++){
        printf("\n\t %d  |", i);

        for(int j = 0; j < colunas; j++){
            printf("  %c  ", matriz[i][j]);
        }

        printf("|\n");
    }

    printf("\n\t    |----------------------------------------|\n\n");
}

void posicionarNavio(char matriz[][8], int linhas, int colunas) {
    int navio[2] = {0, 0},  jogadaValida = 0, k = 0;

    for(int i = 0; i < 3; i++) {
        k = i + 1;
        jogadaValida = 0;

        printf("\n\tInforme a casa [linha (espaço) coluna] do navio.\n");

        mostrarMatriz(matriz, linhas, colunas);

        while(!jogadaValida) {
            printf("\n\t%dª casa: ", k);
            scanf("%d %d", &navio[0], &navio[1]);

            getchar();

            jogadaValida = inserirNavio(matriz, navio, linhas, colunas);
        }

        mostrarTitulo();
    }

    printf("\n\tNavio Posicionado!");
    
    mostrarMatriz(matriz, linhas, colunas);

    timeout(1500);

    limparTela();
}

int inserirJogada(char matriz[][8], char matrizGabarito[][8], int linhas, int colunas) {
    int chances = 5, cont = 1, tirosCertos = 0, tiro[2] = {0, 0};

    while(chances > 0 && tirosCertos < 3) {
        int jogadaValidaEtiroCerto[2] = {0, 0};

        printf("\n\tChances: \t%d", chances);
        printf("\n\tTiros certos: \t%d", tirosCertos);

        mostrarMatriz(matriz, 8, 8);

        while (!jogadaValidaEtiroCerto[0]) {
            printf("\n\t%dº tiro [linha, coluna]: ", cont);
            scanf("%d %d", &tiro[0], &tiro[1]);

            getchar();

            acertouTiro(matriz, matrizGabarito, tiro, jogadaValidaEtiroCerto, linhas, colunas);
        }

        if(jogadaValidaEtiroCerto[1]) {
            chances = chances < 5 ? (chances + 1) : chances;
            tirosCertos++;

            if(tirosCertos == 3) {
                mostrarTitulo();

                mostrarMatriz(matriz, 8, 8);

                printf("\n\tNavio destruído.\n");

                return 1;
            }

            printf("\n\tNa mosca! Acertou!\n");

            timeout(1500);

            cont++;

            mostrarTitulo();

            continue;
        }

        if(!jogadaValidaEtiroCerto[1]) {
            chances--;

            if(chances == 0) {
                mostrarTitulo();

                mostrarMatriz(matriz, 8, 8);

                printf("\n\tAcabaram as chances. . . \n");

                return 0;
            }

            printf("\n\tErrou!\n");

            timeout(1500);

            cont++;
        }
        
        mostrarTitulo();
    }

    return 0;
}

int inserirCasa(char matriz[][8], char jogada, int posicao[], int linhas, int colunas) {
    if(posicao[1] < 0 || posicao[1] > colunas || posicao[0] < 0 || posicao[0] > linhas) {
        printf("\n\tPosição inválida!\n");

        return 0;
    }

    if(matriz[posicao[0]][posicao[1]] != '~') {
        printf("\n\tCasa já ocupada!\n");

        return 0;
    }

    matriz[posicao[0]][posicao[1]] = jogada;

    return 1;

}

int inserirNavio(char matriz[][8], int posicao[], int linhas, int colunas){
    int k = 0, cont = 0, navio[6] = {0, 0, 0, 0, 0, 0};

    for(int i = 0; i < linhas; i++) {
        for(int j = 0; j < colunas; j++) {
            if(matriz[i][j] == '1') {
                navio[k] = i;
                navio[++k] = j;

                cont++;
                k++;
            }
        }
    }

    if(cont == 0) {
        return inserirCasa(matriz, '1', posicao, linhas, colunas);
    }

    if(cont == 1) {
        if((navio[0] != posicao[0]) && (navio[1] != posicao[1])) {
            printf("\n\tA segunda casa deve estar ao lado da primeira!");

            return 0;
        }

        if((navio[0] == posicao[0]) && ((navio[1] + 1) != posicao[1]) && 
            ((navio[1] - 1) != posicao[1]) ) {
            printf("\n\tA segunda casa deve estar ao lado da primeira!");

            return 0;
        }

        if((navio[1] == posicao[1]) && ((navio[0] + 1) != posicao[0]) &&
            ((navio[0] - 1) != posicao[0])) {
            printf("\n\tA segunda casa deve estar ao lado da primeira!");

            return 0;
        }

        return inserirCasa(matriz, '1', posicao, linhas, colunas);
    }

    if((navio[0] != posicao[0]) && (navio[1] != posicao[1])) {
        printf("\n\tA terceira casa deve ser adjascente às outras duas!");

        return 0;
    }

    if((navio[0] == posicao[0]) && ((navio[3] + 1) != posicao[1]) &&
        ((navio[1] - 1) != posicao[1]) ) {
        printf("\n\tA terceira casa deve ser adjascente às outras duas!");

        return 0;
    }

    if((navio[1] == posicao[1]) && ((navio[2] + 1) != posicao[0]) &&
        ((navio[0] - 1) != posicao[0])) {
        printf("\n\tA terceira casa deve ser adjascente às outras duas!");

        return 0;
    }

    return inserirCasa(matriz, '1', posicao, linhas, colunas);
}

void acertouTiro(char matriz[][8], char matrizGabarito[][8], int tiro[], int retorno[], int linhas, int colunas) {
    char jogada = '~';

    retorno[1] = matrizGabarito[tiro[0]][tiro[1]] == '1' ? 1 : 0;

    jogada = retorno[1] ? 'O' : 'X';

    retorno[0] = inserirCasa(matriz, jogada, tiro, linhas, colunas);
}

void mostrarTitulo() {
    limparTela();

    printf("\n\t\t|#########################################|\n");
    printf("\t\t|                                         |\n");
    printf("\t\t|              BATALHA NAVAL              |\n");
    printf("\t\t|                                         |\n");
    printf("\t\t|#########################################|\n\n");
}

void mostrarPlacar(char jogador1[], char jogador2[], int pontos[]) {
    printf("\n\tPLACAR: \n");
    printf("\t%s: \t%d\n", jogador1, pontos[0]);
    printf("\t%s: \t%d\n", jogador2, pontos[1]);
    printf("\t-------------------------------------------------------\n");
}

void mostrarRegras() {
    printf("\n\tREGRAS: \n");
    printf("\n\t* O jogador que atirar ganha um ponto se o navio for destruído por completo;\n");
    printf("\n\t* Mas se o navio não for destruído, o ponto vai para o jogador qur posicionou o navio;\n");
    printf("\n\t* O jogador que fizer três pontos primeiro ganha o jogo;\n");
    printf("\t---------------------------------------------------------------------------------------\n\n");
}

void trocarJogador(char jogador1[], char jogador2[], int pontos[]) {
    int auxPontos = 0;
    char auxNome[20];

    strcpy(auxNome, jogador1);
    strcpy(jogador1, jogador2);
    strcpy(jogador2, auxNome);

    auxPontos = pontos[0];
    pontos[0] = pontos[1];
    pontos[1] = auxPontos;
}

void mostrarJogadorPontuou(char jogador1[], char jogador2[], int pontos[],  int destruiuNavio) {
    if(!destruiuNavio) {
        printf("\n\tPonto para %s.\n", jogador1);
        
        timeout(1500);
        limparTela();
        
        pontos[0]++;

    } else {
        printf("\n\tPonto para %s \n", jogador2);
        
        timeout(1500);
        limparTela();
        
        pontos[1]++;

    }
}

int verificaVencedor(char jogador1[], char jogador2[], int pontos[]) {
    if(pontos[0] == 3) {
        mostrarTitulo();
        mostrarPlacar(jogador1, jogador2, pontos);

        printf("\n\tParabéns %s, você venceu!\n", jogador1);

        return 1;
    } 
    
    if(pontos[1] == 3) {
        mostrarTitulo();
        mostrarPlacar(jogador1, jogador2, pontos);

        printf("\n\tParabéns %s, você venceu!\n", jogador2);

        return 1;
    } 

    return 0;

}

void limparTela() {
    #ifdef _WIN64
        system("cls");
    #elif __linux
        system("clear");
    #endif
}

void timeout(int tempoMs) {
    #ifdef _WIN64
        Sleep(tempoMs);
    #elif __linux
        usleep(tempoMs * 1000);
    #endif
}
