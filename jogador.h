#ifndef JOGADOR_H
#define JOGADOR_H

#include "posicao.h"
#include "mapa.h"



typedef struct{
    Posicao pos;
    char eu;
    int hp;
    char virado;
    char armabase;   //caracter que representa a pega de espada
    char armaponta;  //caracter que representa a ponta da espada
    Posicao armabasepos;
    Posicao armapontapos;
    char pist;
    int numbalas; //numero de balas que o jogador tem
    int range;  //range de visao
    int torcha;  //usada como vez por jogo, aumenta o range durante alguns turnos
} Jogador;

typedef struct{
    char b;
    Posicao pos;
} Bala;

Jogador* criajogador(int a, int b){
    Jogador* jog = malloc(sizeof(Jogador));
    jog->pos.x=a;
    jog->pos.y=b;
    jog->eu='@';      //cria um jogador, 
    jog->hp=5;
    jog->pist='o';
    jog->numbalas=5;
    jog->range=7;
    jog->torcha=-1;

    return jog;  
}

Bala* criabala(int a, int b){
    Bala* bal = malloc(sizeof(Bala));
    bal->pos.x=a;                          //cria uma bala, por cada jogo e posta uma no mapa para o jogador poder apanhar
    bal->pos.y=b;                          // e aumentar as balas que tem
    bal->b='B';
    return bal;
}

void balavisivel(Bala* bal, int x, int y, int range){
    if((bal->pos.x - x)*(bal->pos.x - x) + (bal->pos.y - y)*(bal->pos.y- y) <= range*range){
    
       
        start_color();
                    use_default_colors();
                    init_pair(5, COLOR_YELLOW,17);
                    attron(COLOR_PAIR(5));
                    mvaddch(bal->pos.x, bal->pos.y, bal->b); //o mesmo para a bala que o jogador apanha
                    refresh();
                    attroff(COLOR_PAIR(5));
    }
    else{
     start_color();
                    use_default_colors();
                    init_pair(3,COLOR_BLACK,COLOR_BLACK);
                    attron(COLOR_PAIR(3));
                     mvaddch(bal->pos.x, bal->pos.y,'*'); //se a bala nao for visivel dar print a '*'
                    refresh();
                    attroff(COLOR_PAIR(3));
       
    }

 }


void jogadoranda(Jogador* jog, char a){
    if(a=='2' && jog->pos.x<50){jog->pos.x++; jog->virado='b';}     //muda a posicao do jogador consoante o input do keypad
    if(a=='4' && jog->pos.y>0){jog->pos.y--; jog->virado='e';}  //nota: NO ncurses, para andar para cima, o tem que se decrementar o x
    if(a=='6' && jog->pos.y<120){jog->pos.y++; jog->virado='d';}// e para baixo, incrementar o x
    if(a=='8' && jog->pos.x>0){jog->pos.x--; jog->virado='c';}//virado representa para onde o jogador ta virado(baixo, esq, dir, cima)
}

void jogadordesfazjogada(Jogador* jog, char a){
    if(a=='2'){jog->pos.x--;}
    if(a=='6'){jog->pos.y--;}                           //quando um jogador anda para uma posicao aonde tem uma parede '#', esta funcao e chamada
    if(a=='4'){jog->pos.y++;}                            //e o jogador nao anda para tras
    if(a=='8'){jog->pos.x++;}
}


void espadamexe(Jogador* jog, Chao mapa[51][121]){
            if(jog->virado=='c'){
                
                if(mapa[jog->pos.x-1][jog->pos.y].c!='#'){
                    jog->armabasepos.x=jog->pos.x-1;      //se tiver virado para cima, a espada fica na posicao acima do jogador
                    jog->armabasepos.y=jog->pos.y;        //a base da espada na posicao x-1 e a ponta na posicao x-2 comparado ao jogador
                    jog->armabase='-';
                
                    if(mapa[jog->pos.x-2][jog->pos.y].c!='#'){
                            jog->armapontapos.x=jog->pos.x-2;  //NOTA: no ncurses para andar para cima na tela, que se decrementar a posicao x
                            jog->armapontapos.y=jog->pos.y;
                            jog->armaponta='|';
                    }

                    else{
                        jog->armaponta=' ';
                        jog->armapontapos.x=52;
                        jog->armapontapos.y=0;
                        }
            
                }
                else{
                    jog->armabase=' ';
                    jog->armabasepos.x=52;
                    jog->armabasepos.y=0;
                    jog->armaponta=' ';
                    jog->armapontapos.x=52;
                    jog->armapontapos.y=0;
                }
            }

            if(jog->virado=='b'){
                if(mapa[jog->pos.x+1][jog->pos.y].c!='#'){
                    jog->armabasepos.x=jog->pos.x+1;      //se tiver virado para cima, a espada fica na posicao acima do jogador
                    jog->armabasepos.y=jog->pos.y;        //a base da espada na posicao x-1 e a ponta na posicao x-2 comparado ao jogador
                    jog->armabase='-';
                
                    if(mapa[jog->pos.x+2][jog->pos.y].c!='#'){
                        jog->armapontapos.x=jog->pos.x+2;  //NOTA: no ncurses para andar para cima na tela, que se decrementar a posicao x
                        jog->armapontapos.y=jog->pos.y;
                        jog->armaponta='|';
                    }

                    else{
                        jog->armaponta=' ';
                        jog->armapontapos.x=52;
                        jog->armapontapos.y=0;
                    }
            
                }

                else{
                    jog->armabase=' ';
                    jog->armabasepos.x=52;
                    jog->armabasepos.y=0;
                    jog->armaponta=' ';
                    jog->armapontapos.x=52;
                    jog->armapontapos.y=0;
                    }
                }

            if(jog->virado=='e'){
                if(mapa[jog->pos.x][jog->pos.y-1].c!='#'){
                    jog->armabasepos.x=jog->pos.x;      //se tiver virado para cima, a espada fica na posicao acima do jogador
                    jog->armabasepos.y=jog->pos.y-1;        //a base da espada na posicao x-1 e a ponta na posicao x-2 comparado ao jogador
                    jog->armabase='|';
                    
                    if(mapa[jog->pos.x][jog->pos.y-2].c!='#'){
                            jog->armapontapos.x=jog->pos.x;  //NOTA: no ncurses para andar para cima na tela, que se decrementar a posicao x
                            jog->armapontapos.y=jog->pos.y-2;
                            jog->armaponta='\\';
                    }
                        
                    else{
                        jog->armaponta=' ';
                        jog->armapontapos.x=52;
                        jog->armapontapos.y=0;
                    }
                }

                else{
                    jog->armabase=' ';
                    jog->armabasepos.x=52;
                    jog->armabasepos.y=0;
                    jog->armaponta=' ';
                    jog->armapontapos.x=52;
                    jog->armapontapos.y=0;
                }
            }

            if(jog->virado=='d'){
                if(mapa[jog->pos.x][jog->pos.y+1].c!='#'){
                    jog->armabasepos.x=jog->pos.x;      //se tiver virado para cima, a espada fica na posicao acima do jogador
                    jog->armabasepos.y=jog->pos.y+1;        //a base da espada na posicao x-1 e a ponta na posicao x-2 comparado ao jogador
                    jog->armabase='|';
                    
                    if(mapa[jog->pos.x][jog->pos.y+2].c!='#'){
                            jog->armapontapos.x=jog->pos.x;  //NOTA: no ncurses para andar para cima na tela, que se decrementar a posicao x
                            jog->armapontapos.y=jog->pos.y+2;
                            jog->armaponta='/';
                    }

                    else{
                        jog->armaponta=' ';
                        jog->armapontapos.x=52;
                        jog->armapontapos.y=0;
                    }
                }

                else{

                    jog->armabase=' ';
                    jog->armabasepos.x=52;
                    jog->armabasepos.y=0;
                    jog->armaponta=' ';
                    jog->armapontapos.x=52;
                    jog->armapontapos.y=0;
                }
            }
}

#endif
