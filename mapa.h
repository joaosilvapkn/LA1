#ifndef MAPA_H
#define MAPA_H

#include "posicao.h"

typedef struct {
    char c;         //cada tela do mapa
    int parede;
    int visitado;   //para gerar mapa
    int andavel;
    int passou;
}Chao;

Chao* criachao(){
    Chao* ch = malloc(sizeof(Chao));
    int n =rand()%14;
    ch->andavel=n;                      //cria uma tela, e a tela pode ser ' '(pode se andar) ou '#'(sao as paredes)
    if(ch->andavel>0){ch->c=' ';}
    else{ch->c='#';}

    return ch;
}




void desenhachaocor(Chao t[51][121], int x, int y){

    
    if(t[x][y].c == ' '){start_color();//se o mapa for chao e ja tiver passado la imprime um bloco rosa
                    use_default_colors();
                    init_pair(10, 13,13);
                    attron(COLOR_PAIR(10));
                    mvaddch(x, y, t[x][y].c);
    	 	    refresh();
                    attroff(COLOR_PAIR(10));}
                    
    else if(t[x][y].c == 'H'){start_color();//se o mapa for a saida imprime um bloco verde mesmo ja tento passado
                    use_default_colors();
                    init_pair(7, 10,10);
                    attron(COLOR_PAIR(7));
                    mvaddch(x, y, t[x][y].c);
    		    refresh();
                    attroff(COLOR_PAIR(7));}
                    }

void desenhachao(Chao t[51][121], int x, int y){

    
    if (t[x][y].c == '#'){   //se o mapa for uma parede imprime como um bloco cinza
    
    start_color();
                    use_default_colors();
                    init_pair(6, 8,8);
                    attron(COLOR_PAIR(6));
                     mvaddch(x, y, t[x][y].c);
    refresh();
                    attroff(COLOR_PAIR(6));
                    
  }  else if(t[x][y].c == 'P'){//se o mapa for uma portal imprime como um P branco
   start_color();
                    use_default_colors();
                    init_pair(8, COLOR_WHITE,17);
                    attron(COLOR_PAIR(8));
                     mvaddch(x, y, t[x][y].c);
    refresh();
                    attroff(COLOR_PAIR(8));
                    
}else if(t[x][y].c == ' '){start_color();//se o mapa for chao imprime um bloco azul
                    use_default_colors();
                    init_pair(7, 17,17);
                    attron(COLOR_PAIR(7));
                     mvaddch(x, y, t[x][y].c);
    refresh();
                    attroff(COLOR_PAIR(7));}
 else if(t[x][y].c == 'H'){start_color();//se o mapa for a saida imprime um bloco verde
                    use_default_colors();
                    init_pair(7, 10,10);
                    attron(COLOR_PAIR(7));
                     mvaddch(x, y, t[x][y].c);
    refresh();
                    attroff(COLOR_PAIR(7));}
}

void chaovisivel(Chao mapa[51][121], int i, int j, int x, int y, int range){
    if((i - x)*(i - x) + (j - y)*(j- y) <= range*range && mapa[i][j].passou==0){
        desenhachao(mapa, i, j); //igual para o mapa
    }else if((i - x)*(i - x) + (j - y)*(j- y) <= range*range && mapa[i][j].passou==1){
    
        //desenhachao(mapa, i, j); //igual para o mapa
        use_default_colors();
                    init_pair(10,13,13);
                    attron(COLOR_PAIR(10));
                    desenhachaocor(mapa, i, j); //igual para o mapa
                    refresh();
                    attroff(COLOR_PAIR(10));
        
    }else if(mapa[i][j].passou==1){
        use_default_colors();
                    init_pair(10,13,13);
                    attron(COLOR_PAIR(10));
                    mvaddch(i, j, '*'); //se o chao nao for visivel da print ao carater *
                    refresh();
                    attroff(COLOR_PAIR(10));
    }
    else{
        start_color();
                    use_default_colors();
                    init_pair(3,COLOR_BLACK,COLOR_BLACK);
                    attron(COLOR_PAIR(3));
                    mvaddch(i, j, '*'); //se o chao nao for visivel da print ao carater *
                    refresh();
                    attroff(COLOR_PAIR(3));
        
    }
    

 }



typedef struct {
    int posicao[2][1500];
    int top;
} Stack;
void init(Stack* stack) {
    stack->top = -1;
}
void push(Stack* stack, int y,int x) {
    stack->top++;
    stack->posicao[0][stack->top] = x;
    stack->posicao[1][stack->top] = y;
}
int* pop(Stack* stack) {
    int* element = malloc(2 * sizeof(int));
    element[0] = stack->posicao[0][stack->top];
    element[1] = stack->posicao[1][stack->top];
    stack->top--;
    return element;
}
int isEmpty(Stack* stack) {
    return stack->top == -1;
}


void gerar_mapa( Chao mapa[51][121], Posicao* entrada, Posicao* saida, int linhas, int colunas){
    
    for(int i=0;i<linhas;i++){//por as paredes todas a volta e o resto nao
        for(int j=0;j<colunas;j++){
            mapa[i][j].parede=0;
            mapa[i][j].visitado=0;
            mapa[i][j].c=' ';
            mapa[i][j].passou=0;
            if (i==0||i==linhas-1||j==0||j==colunas-1){
                mapa[i][j].visitado=1;
                mapa[i][j].parede=1;
                mapa[i][j].c='#';
            }
        }
    }
    if ((rand() % 2)==(1)){//cima e baixo
        entrada->y=(rand() % 2)*(linhas-1);
        entrada->x=(((rand() % ((colunas-1)/2))+1)*2)-1;
        if (entrada->y==0) saida->y=linhas-1;
        else saida->y=0;
        saida->x=(((rand() % ((colunas-1)/2))+1)*2)-1; 
    }
    else {//esquerda e direita
        entrada->y=(((rand() % ((linhas-1)/2))+1)*2)-1;
        entrada->x=(rand() % 2)*(colunas-1);
        if (entrada->x==0) saida->x=colunas-1;
        else saida->x=0;
        saida->y=(((rand() % ((linhas-1)/2))+1)*2)-1;
    }
    Stack stack;
    init(&stack);
    //cobrinha
    Posicao cobra;
    cobra.y=entrada->y;
    cobra.x=entrada->x;
    
    if (entrada->y==0)cobra.y=entrada->y+1;
    else if (entrada->y==linhas-1)cobra.y=entrada->y-1;
    if (entrada->x==0)cobra.x=entrada->x+1;
    else if (entrada->x==colunas-1)cobra.x=entrada->x-1;
    entrada->y=cobra.y;
    entrada->x=cobra.x;
    
    mapa[cobra.y][cobra.x].visitado=1;
    int z=0;
    while (isEmpty(&stack)==0||z==0){
        z++;
        while ((cobra.y>1&&mapa[cobra.y-2][cobra.x].visitado==0)||(cobra.y<linhas-2&&mapa[cobra.y+2][cobra.x].visitado==0)||(cobra.x>1&&mapa[cobra.y][cobra.x-2].visitado==0)||(cobra.x<colunas-2&&mapa[cobra.y][cobra.x+2].visitado==0)){
            while(1){
                int random=(rand() % 4);
                if (random==0){
                    if(cobra.y>1&&mapa[cobra.y-2][cobra.x].visitado==0){
                        mapa[cobra.y-2][cobra.x].visitado=1;
                        mapa[cobra.y-1][cobra.x].visitado=1;
                        push(&stack,cobra.y,cobra.x);
                        cobra.y=cobra.y-2;
                        break;
                    }
                }
                if (random==1){
                    if (cobra.y<linhas-2&&mapa[cobra.y+2][cobra.x].visitado==0){
                        mapa[cobra.y+2][cobra.x].visitado=1;
                        mapa[cobra.y+1][cobra.x].visitado=1;
                        push(&stack,cobra.y,cobra.x);
                        cobra.y=cobra.y+2;
                        break;
                    }
                }
                if (random==2){
                    if (cobra.x>1&&mapa[cobra.y][cobra.x-2].visitado==0){
                        mapa[cobra.y][cobra.x-2].visitado=1;
                        mapa[cobra.y][cobra.x-1].visitado=1;
                        push(&stack,cobra.y,cobra   .x);
                        cobra.x=cobra.x-2;
                        break;
                    }
                }
                if (random==3){
                    if (cobra.x<colunas-2&&mapa[cobra.y][cobra.x+2].visitado==0){
                        mapa[cobra.y][cobra.x+2].visitado=1;
                        mapa[cobra.y][cobra.x+1].visitado=1;
                        push(&stack,cobra.y,cobra.x);
                        cobra.x=cobra.x+2;
                        break;
                    }
                }
            }
        }
        int* anterior=pop(&stack);
        cobra.x=anterior[0];
        cobra.y=anterior[1];
        free(anterior);
    }
    for(int i=0;i<linhas;i++){
        for(int j=0;j<colunas;j++){
            if (mapa[i][j].visitado==0){
                mapa[i][j].parede=1;
                mapa[i][j].c='#';
            }
            if (i==saida->y&&j==saida->x){
                mapa[i][j].parede=0;
                mapa[i][j].c='H';
            }
        }
    }
    
    return;
}

#endif




