#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window * window;
SDL_Renderer * render;
SDL_Event event;

int matriz[16][16];

struct anel {
    int linha;
    int coluna;
    struct anel * anterior;
};

typedef struct anel Anel;

Anel * cabeca;

int sorteiaNumero() {
    int lin, col;
    lin = rand() % 16;
    col = rand() % 16;
    matriz[lin][col] = 1;
}

int iniciarVideo() {
    if (SDL_Init( SDL_INIT_EVERYTHING ) >= 0 ) {
        window = SDL_CreateWindow("S N A K E",
                                  100, 100, WIDTH, HEIGHT,
                                  SDL_WINDOW_SHOWN);
        if (window != NULL) {
            render = SDL_CreateRenderer(window, -1, 0);
            if (render != NULL) {
                return 0;
            }
        }
    }
    return -1;
}

int inicializar() {
    int lin, col;
    for (lin = 0; lin < 16; lin++) {
        for (col = 0; col < 16; col++) {
            matriz[lin][col] = 0;
        }
    }
    cabeca = (Anel*) malloc( sizeof( Anel ) );
    cabeca->coluna = 10;
    cabeca->linha = 3;
    cabeca->anterior = NULL;
    sorteiaNumero();
}

int printCelula(int linha, int coluna) {
    Anel * cauda = cabeca->anterior;
    if (    cabeca->linha == linha &&
            cabeca->coluna == coluna) {
            return '\xDB';
    } else if ( cauda != NULL &&
                cauda->linha == linha &&
                cauda->coluna == coluna) {
            return '\xB1';
    } else {
            return matriz[linha][coluna];
    }
}

int mostraMatriz() {
    system("CLS");
    int lin, col;
    for (lin = 0; lin < 16; lin++) {
        for (col = 0; col < 16; col++) {
            int i = printCelula(lin, col);
            printf("%c|", i);
        }
        printf("\n--------------------------------\n");
    }
}

int adicionaAnel(int linha, int coluna) {
    Anel * cauda = (Anel*) malloc( sizeof ( Anel ) );
    cauda->linha = linha;
    cauda->coluna = coluna;
    cauda->anterior = NULL;
    cabeca->anterior = cauda;
}

int calculaRegras() {
    if (cabeca->coluna > 15) {
        cabeca->coluna = 0;
    } else if (cabeca->coluna < 0) {
        cabeca->coluna = 15;
    } else if (cabeca->linha > 15) {
        cabeca->linha = 0;
    } else if (cabeca->linha < 0) {
        cabeca->linha = 15;
    }

    if (matriz[cabeca->linha][cabeca->coluna] != 0) {
        adicionaAnel(cabeca->linha, cabeca->coluna);
    }
}

int moveCobra(int deltaLinha, int deltaColuna) {
     Anel * cauda = cabeca->anterior;
     if (cauda != NULL) {
        cauda->linha = cabeca->linha;
        cauda->coluna = cabeca->coluna;
     }
     cabeca->linha += deltaLinha;
     cabeca->coluna += deltaColuna;
}

int capturaEventos() {
    int tipo = SDL_PollEvent( &event );
    if ( event.type == SDL_QUIT ){
        exit(0);
    } else if (event.type == SDL_KEYDOWN) {
        printf(" Tecla Apertada : %d \n", event.key.keysym.scancode );
        if ( event.key.keysym.scancode == 79) {
            moveCobra(0, 1);
        } else if (event.key.keysym.scancode == 80) {
            moveCobra(0, -1);
        } else if (event.key.keysym.scancode == 82) {
            moveCobra(-1, 0);
        } else if (event.key.keysym.scancode == 81) {
            moveCobra(1, 0);
        }
    }
}

int mostrarVideo() {
    int lin, col;
    int w = WIDTH / 16;
    int h = HEIGHT / 16;
    for( lin = 0; lin < 16; lin++) {
        for( col = 0; col < 16; col++) {
            int x = col * w;
            int y = lin * h;
            SDL_Rect r = {x, y, w, h};
            SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
            SDL_RenderFillRect(render, &r );
            int i = printCelula(lin, col);
            if (i == '\xDB') {
                SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
                SDL_RenderFillRect(render, &r );
            } else if (i == '\xB1') {
                SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
                SDL_RenderFillRect(render, &r );
            } else if (i == 1) {
                SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
                SDL_RenderFillRect(render, &r );
            }
            SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
            SDL_RenderDrawRect( render, &r );
        }
    }
}


int main(int argc, char ** argv) {
    inicializar();
    if ( iniciarVideo() == 0) {
        while (1) {
            calculaRegras();
            mostrarVideo();
            SDL_RenderPresent(render);
            SDL_Delay(20);
            // mostraMatriz();
            capturaEventos();
        }
    } else {
        printf("Não foi possível iniciar a área gráfica....\n");
    }
    return 0;
}
