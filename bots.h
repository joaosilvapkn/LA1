#ifndef BOTS_H
#define BOTS_H

#include "posicao.h"


typedef struct{
    char corpo;   //cada bot tem hp, posicao, e o carater que representa o bot
    Posicao pos;
    int hp;
} Bot;

typedef struct{
    Bot cabeca;   //os bosses sao um conjunto de 4 bots unidos, um e a cabeca, um o corpo, e os dois bracos, tambem tem hp
    Bot body;
    Bot bracoesq;
    Bot bracodir;
    int hp;
} Boss;


Bot* criabot(int a, int b){
    Bot* bo = malloc(sizeof(Bot));
    bo->pos.x=a;
    bo->pos.y=b;                     //cria um bot
    bo->corpo='X';
    bo->hp=1;
    return bo;
}

Boss* criaboss(){
    Boss* b=malloc(sizeof(Boss));
    b->cabeca.pos.x=7;
    b->cabeca.pos.y=25;
    b->cabeca.corpo='o';
    b->body.pos.x=8;
    b->body.pos.y=25;
    b->body.corpo='X';          //cira um boss, os mapas para os bosses sao 25/50, por isso vai spawnar nas cordenadas 8,25
    b->bracodir.pos.x=8;         //ou seja, no meio
    b->bracodir.pos.y=26;
    b->bracodir.corpo='>';
    b->bracoesq.pos.x=8;
    b->bracoesq.pos.y=24;
    b->bracoesq.corpo='<';
    b->hp=5;

    return b;
}

void desenhaboss(Boss *b){
if(b->cabeca.corpo!='#'){
start_color();
                    use_default_colors();
                    init_pair(1, COLOR_RED,COLOR_BLACK);
                    attron(COLOR_PAIR(1));
                    
    mvaddch(b->cabeca.pos.x, b->cabeca.pos.y, b->cabeca.corpo);           
    mvaddch(b->bracoesq.pos.x, b->bracoesq.pos.y, b->bracoesq.corpo);     //desenha o boss no mapa quando se luta contra ela
    mvaddch(b->bracodir.pos.x, b->bracodir.pos.y, b->bracodir.corpo);
    mvaddch(b->body.pos.x, b->body.pos.y, b->body.corpo);
    refresh();
                    attroff(COLOR_PAIR(1));
    
}else if(b->cabeca.corpo=='#'){
start_color();
                    use_default_colors();
                    init_pair(1, 8,8);
                    attron(COLOR_PAIR(1));
                    
    mvaddch(b->cabeca.pos.x, b->cabeca.pos.y, b->cabeca.corpo);           //desenha o boss na parede quando ele morre
    mvaddch(b->bracoesq.pos.x, b->bracoesq.pos.y, b->bracoesq.corpo);
    mvaddch(b->bracodir.pos.x, b->bracodir.pos.y, b->bracodir.corpo);
    mvaddch(b->body.pos.x, b->body.pos.y, b->body.corpo);
    refresh();
                    attroff(COLOR_PAIR(1));
}
}

void botanda(Bot* b, Jogador* jog){ 
    if(b->corpo=='X' && ((b->pos.x - jog->pos.x)*(b->pos.x - jog->pos.x) + (b->pos.y - jog->pos.y)*(b->pos.y - jog->pos.y) <= jog->range*jog->range)){
        if(jog->pos.x==b->pos.x && jog->pos.y>b->pos.y){b->pos.y++;}
        else if(jog->pos.x==b->pos.x && jog->pos.y<b->pos.y){b->pos.y--;}
        else if(jog->pos.x>b->pos.x && jog->pos.y==b->pos.y){b->pos.x++;}
        else if(jog->pos.x<=b->pos.x && jog->pos.y==b->pos.y){b->pos.x--;}
        else if(jog->pos.x>=b->pos.x && jog->pos.y>=b->pos.y){b->pos.y++;}
        else if(jog->pos.x>=b->pos.x && jog->pos.y<=b->pos.y){b->pos.x++;}
        else if(jog->pos.x<=b->pos.x && jog->pos.y<=b->pos.y){b->pos.y--;} //ESTA FUNCAO NAO E USADA, ESQUECER
        else if(jog->pos.x<=b->pos.x && jog->pos.y>=b->pos.y){b->pos.x--;}

    }
}
void botanda2(Bot* b, int n){
    if(n==1){b->pos.x++;}                   //ESTA funcao mexe a posicao dos bots, chama se a funcao e da se um numero gerado a sorte e o moviemtno dos
    else if(n==2){b->pos.x--;}          //bots fica aleatorio
    else if(n==3){b->pos.y++;}
    else if(n==4){b->pos.y--;}
}

void bossanda(Boss* b, int n){
    if(n==1 && b->cabeca.pos.x<22){b->cabeca.pos.x++; b->body.pos.x++; b->bracoesq.pos.x++; b->bracodir.pos.x++;}   //da se um numero aleatorio e o boss mexe se
    if(n==2 && b->bracoesq.pos.y>2){b->cabeca.pos.y--; b->body.pos.y--; b->bracoesq.pos.y--; b->bracodir.pos.y--;} //o corpo do boss tem que se manter junto
    if(n==3 && b->bracodir.pos.y<48){b->cabeca.pos.y++; b->body.pos.y++; b->bracoesq.pos.y++; b->bracodir.pos.y++;}
    if(n==4 && b->body.pos.x>2){b->cabeca.pos.x--; b->body.pos.x--; b->bracoesq.pos.x--; b->bracodir.pos.x--;}
    
}


void botdesfazjogada(Bot* b, int a){
    if(a==1){b->pos.x--;}   //quando o bot vai contra uma parede
    if(a==4){b->pos.y++;}
    if(a==3){b->pos.y--;}
    if(a==2){b->pos.x++;}
}



void botvisivel(Bot* b, int x, int y, int range){

    if((b->pos.x - x)*(b->pos.x - x) + (b->pos.y - y)*(b->pos.y - y) <= range*range ){
    
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED,17);
    attron(COLOR_PAIR(1));
    
        mvaddch(b->pos.x, b->pos.y, b->corpo);
        
    refresh();
    attroff(COLOR_PAIR(1));
                                                    //IMPRIme os bots que estao dentro do raio de visao do jogador, 
    } //formula dos pontos que estao dentro do circulo
    
}

void botvisivelcor(Bot* b, int x, int y, int range){  //mm da anterior mas com fundo rosa

    if((b->pos.x - x)*(b->pos.x - x) + (b->pos.y - y)*(b->pos.y - y) <= range*range ){
    
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED,13);
    attron(COLOR_PAIR(1));
    
        mvaddch(b->pos.x, b->pos.y, b->corpo);
        
    refresh();
    attroff(COLOR_PAIR(1));
                                                    //IMPRIme os bots que estao dentro do raio de visao do jogador e que se encntrem num sitio que o ogador ja tenha passado
    } 
    
}

#endif