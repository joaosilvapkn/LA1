#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include "mapa.h"
#include "jogador.h"
#include "bots.h"




typedef struct{  //no fim de derrotar cada boss aparece um coracao nesta forma <3
    Posicao pos1;
    char c1;  //<
    Posicao pos2;
    char c2; //3
    
} Coracao;






Coracao* criacoracao(){
    Coracao* cor = malloc(sizeof(Coracao));
    cor->c1='<';
    cor->c2='3';               //cria um coracao(usado quando se derrota um boss)
    cor->pos1.x=12;
    cor->pos1.y=25;
    cor->pos2.x=12;
    cor->pos2.y=26;
    return cor;
}



int main(){
    srand(time(NULL));
    Chao mapa[51][121];   //desenha o mapa
    Posicao entrada;
    Posicao saida;
    gerar_mapa(mapa,&entrada,&saida,51,121);
    
    Jogador* jog=criajogador(entrada.y,entrada.x);  //jogador nasce 

    initscr();
    noecho();
    curs_set(0);
    mvaddch(jog->pos.x, jog->pos.y, jog->eu);

    int numbots=rand()%50;    //um numero random de bots de 0 a 50
    //numbots=0; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    Bot* bots[numbots];   

    for(int i=0; i<numbots; i++){
        Bot* b = criabot(rand()%51, rand()%121);
        if(mapa[b->pos.x][b->pos.y].c!='#' && (b->pos.x!=jog->pos.x || b->pos.y!=jog->pos.y) ){
            bots[i]=b;
        }
        else{          //enche um arrai de bots com bots, nao poe o bot no array se a poosicao inicial dele for uma parede
            i--;
        }
    }
    //5 portais para bosses no jogo
    for(int a=0; a<5; a++){
        int n1=rand()%51;               //cria 5 portais com numeros a sorte, nao cria se a pos do portal for uma parede
        int n2=rand()%121;
        if(mapa[n1][n2].c!='#' && mapa[n1][n2].c!='P'){ //o portal e representado por P NO MAPA
            mapa[n1][n2].c='P';
        }
        else{
            a--;
        }

    }


    int np=5; //num de portais a ser decrementado quando se entra num

  


    int nbots=numbots; //num de bots que e decrementado quando se mata um



    int n1=rand()%51;
    int n2=rand()%121;

    while(1){

        n1 = rand()%51;
        n2 = rand()%121;
        if(mapa[n1][n2].c!='#'){
            break;
        }
    }

    Bala* bal = criabala(n1, n2); //cria um bala numa posicao do mapa que nao seja parede
    
    keypad(stdscr, TRUE); //ativa o keypad




    int acabou=0;

    //jog->range=150;


    while(jog->hp>0 /*&& ((nbots>0)||(np>0))*/){  //jogase enquanto o hp do jogador for > 0 ou (enquanto nao houver bots e portais por entrar)
                                                 //jogador morre ---------------------------jogador completa o jogo
        //imprime texto com informacoes
        //por esta parte mais bonita e com cores depois


        if((np<1) && (jog->pos.x==saida.y) && (jog->pos.y==saida.x)){
            acabou=1;
            break;
        }
        char str[20];
        sprintf(str, "%d", jog->hp);

        mvprintw(25,125, "HP: %s",str); 

        if(nbots>=10){
            char str2[20]=" ";
            sprintf(str2, "%d", nbots);
            mvprintw(26,125, "NUMERO DE BOTS: %s", str2);
        }

        else if(nbots<10){
            char str2[20]=" ";
            sprintf(str2, "%d", nbots);
            mvprintw(26,125, "NUMERO DE BOTS: %s  ", str2);
          
        }

        char str6[20];
        sprintf(str6, "%d", np);
        mvprintw(27,125, "PORTAIS POR VISITAR: %s", str6);

        if(jog->numbalas>=10){
            char str4[20];
            sprintf(str4, "%d", jog->numbalas);
            mvprintw(28, 125, "NUMERO DE BALAS: %s", str4);
        }
        else{
            char str4[20];
            sprintf(str4, "%d", jog->numbalas);
            mvprintw(28, 125, "NUMERO DE BALAS : %s ", str4);
        }

        if(jog->torcha==-1){
            mvprintw(29, 125, "TORCHA DISPONIVEL");
        }

        else if(jog->torcha<-1){
            mvprintw(29, 125, "TORCHA EM USO    ");
        }
        else if(jog->torcha>-1){
            mvprintw(29, 125, "A PREPARAR TORCHA");
        }

        mvprintw(30, 125, "s -> PARA USAR ESPADA");
        mvprintw(31, 125, "a -> PARA USAR PISTOLA");





       //se a torcha e maior que zero e porque foi ativada, por cada turno e decrementada  e quando chegar a zero o range volta ao normal(7)

        if(jog->torcha<-25){
            jog->range=7;
            jog->torcha=100;
        }

        else if(jog->torcha<-1){
            jog->torcha=jog->torcha-1;
        }
        
        else if(jog->torcha>-1){
            jog->torcha=jog->torcha-1;
        }

               
        char c = getch(); //busca de input
        if (c=='Q'){c='q';}
        if (c=='T'){c='t';}  //caso o input seja em maiuscula, transofrma em minuscula
        if (c=='S'){c='s';}
        if (c=='A'){c='a';}
        if (c=='L'){c='l';}
        if (c=='q'){  //q sai do jogo
            break;
        }

        if(c=='u'){nbots=0;}
        if(c=='i'){np=0;}
        if(c=='g'){
            jog->pos.x=saida.y;
            jog->pos.y=saida.x;
        }

        else if(c=='t' && jog->torcha==-1){  //se for t e se a torcha tiver por ativar(t=-1) aumenta o range para 11 e a torcha passa para 25
            jog->range=11;                     //quando a torcha chegar a 0 o range volta a ser 7
            //jog->torcha=25;
            jog->torcha=-2;

        }
        else if (c=='l'){
        //por=0;
        np=0;
        nbots=0;
        }

        else if(c!='q' && c!='a' && c!='s' && c!='t' && c!='l'){  
            
            
            
            mapa[jog->pos.x][jog->pos.y].passou=1;
            jogadoranda(jog, c);                              //chama a funcao que mexe o jogador
            if(mapa[jog->pos.x][jog->pos.y].c=='#'){
                jogadordesfazjogada(jog, c); //se for contra uma parede, desfaz a jogada
            }
            for(int i=0; i<numbots; i++){
                if(jog->pos.x==bots[i]->pos.x && jog->pos.y==bots[i]->pos.y && bots[i]->corpo=='X'){ //o carater x do bot significa que ele ta vivo
                    jogadordesfazjogada(jog, c); //se o jogador esbarrar contra um bot anda duas casas para tras
                    jogadordesfazjogada(jog, c); // e perde 1 de hp
                    jog->hp--;
                }
            }


            for(int i=0; i<51; i++){
                for(int j=0; j<121; j++){
                    chaovisivel(mapa, i, j,jog->pos.x, jog->pos.y, jog->range);
                } //chama a funcao que desenha o mapa se ele for visivel
            }

            balavisivel(bal, jog->pos.x, jog->pos.y, jog->range);  //se for visivel, desenha a bala

            start_color();
            use_default_colors();
            init_pair(4, 10,13);
            attron(COLOR_PAIR(4));
            mvaddch(jog->pos.x, jog->pos.y, jog->eu); //desenha o jogador
            refresh();
            attroff(COLOR_PAIR(4));
            
            


            if(jog->pos.x==bal->pos.x && jog->pos.y==bal->pos.y && bal->b=='B'){
                jog->numbalas+=5;
                bal->b=' ';  //se o jogador pisar a bala por apanhar('B') recebe mais balas e a bala passa a ter o carater 'b'

            }

            for(int i=0; i<numbots; i++){
                if((c=='2' || c=='4' || c=='6' || c=='8') && bots[i]->corpo=='X'){  //MEXE os bots se o jogador tambem se mexer
                    int n = rand();                                                   //e so se o bot tiver vivo 'X'
                    n+=bots[i]->pos.x+bots[i]->pos.y;
                    int n2=n%5;
                    botanda2(bots[i], n2);
                    if(mapa[bots[i]->pos.x][bots[i]->pos.y].c=='#'){ //SE O BOT FOR CONTRA UMA PAREDE DESFAZ A JOGADA
                        botdesfazjogada(bots[i], n2);
                    }
                    if(jog->pos.x == bots[i]->pos.x && jog->pos.y==bots[i]->pos.y){
                        botdesfazjogada(bots[i], n2);           //SE O bot for contra o jogador o bot anda 2 vezes para tras e o jogador perde hp
                        botdesfazjogada(bots[i], n2);
                        jog->hp--;
                    }
                    

                   

                }
            }



            for(int i=0; i<numbots; i++){
            
                if(mapa[bots[i]->pos.x][bots[i]->pos.y].passou==0){
                    botvisivel(bots[i], jog->pos.x, jog->pos.y, jog->range); //desenha os bots dentro do range
                }

                else if(mapa[bots[i]->pos.x][bots[i]->pos.y].passou==1){  //nao esta a funcionar direito, verificar melhor
                        botvisivelcor(bots[i], jog->pos.x, jog->pos.y, jog->range); //desenha os bots dentro do range com o fundo a rosa
                }
            }
          

  
        }

        else if(c=='s'){   //se o input for s o jogador usa a espada
  
            espadamexe(jog, mapa);

            if(jog->armabasepos.x != 52){
            if(mapa[jog->armabasepos.x][jog->armabasepos.y].passou == 0){
                start_color();
                use_default_colors();
                init_pair(2, COLOR_WHITE,17);
                attron(COLOR_PAIR(2));
                mvaddch(jog->armabasepos.x, jog->armabasepos.y, jog->armabase); //DESENHA A BASE E A PONTA DE ESPADA
                //mvaddch(jog->armapontapos.x, jog->armapontapos.y, jog->armaponta);
                refresh();
                attroff(COLOR_PAIR(2));
            }else if(mapa[jog->armabasepos.x][jog->armabasepos.y].passou == 1){
                start_color();
                use_default_colors();
                init_pair(12, COLOR_BLACK,13);
                attron(COLOR_PAIR(12));
                mvaddch(jog->armabasepos.x, jog->armabasepos.y, jog->armabase); //DESENHA A BASE E A PONTA DE ESPADA
                //mvaddch(jog->armapontapos.x, jog->armapontapos.y, jog->armaponta);
                refresh();
                attroff(COLOR_PAIR(12));
            }}

            if(jog->armapontapos.x != 52){
                if(mapa[jog->armapontapos.x][jog->armapontapos.y].passou == 0){
                start_color();
                use_default_colors();
                init_pair(2, COLOR_WHITE,17);
                attron(COLOR_PAIR(2));
                mvaddch(jog->armapontapos.x, jog->armapontapos.y, jog->armaponta); //DESENHA A BASE E A PONTA DE ESPADA
                //mvaddch(jog->armapontapos.x, jog->armapontapos.y, jog->armaponta);
                refresh();
                attroff(COLOR_PAIR(2));
            }else if(mapa[jog->armapontapos.x][jog->armapontapos.y].passou == 1){
                start_color();
                use_default_colors();
                init_pair(12, COLOR_BLACK,13);
                attron(COLOR_PAIR(12));
                mvaddch(jog->armapontapos.x, jog->armapontapos.y, jog->armaponta); //DESENHA A BASE E A PONTA DE ESPADA
                //mvaddch(jog->armapontapos.x, jog->armapontapos.y, jog->armaponta);
                refresh();
                attroff(COLOR_PAIR(12));
            }}


            for(int i=0 ; i<numbots; i++){
                if((jog->armapontapos.x==bots[i]->pos.x && jog->armapontapos.y==bots[i]->pos.y && bots[i]->corpo=='X')||(jog->armabasepos.x==bots[i]->pos.x && jog->armabasepos.y==bots[i]->pos.y && bots[i]->corpo=='X')){
                    bots[i]->hp=0;
                    bots[i]->corpo='0';
                    nbots--;               //SE A ponta ou a base da espada acertarem num bot, o bot morre, o carater fica '0' (e perde hp)
                } 
            }
            
        }

        else if(c=='a' && jog->numbalas>0){  //se o jogador carregar no a e ainda tiver balas!


            if(jog->virado=='c'){
                for(int i=1; i<5; i++){          //se tiver virado para cima, manda 4 'o' para ccima que representam a bala
                    mvaddch(jog->pos.x-i, jog->pos.y, 'o'); //desenha as balas
                    for(int j=0; j<numbots; j++){
                        if(bots[j]->pos.x==jog->pos.x-i && bots[j]->pos.y==jog->pos.y && bots[i]->hp>0){
                            bots[j]->hp--;         //se alguma da bala acertar em algum dos bots, entao o bot morre
                            bots[j]->corpo='0'; 
                            nbots--;
                           
                        }
                    }
                }
            }
        

            if(jog->virado=='b'){
                for(int i=1; i<5; i++){
                    mvaddch(jog->pos.x+i, jog->pos.y, 'o');
                    for(int j=0; j<numbots; j++){
                        if(bots[j]->pos.x==jog->pos.x+i && bots[j]->pos.y==jog->pos.y && bots[i]->hp>0){
                            bots[j]->hp--;
                            bots[j]->corpo='0';
                            nbots--;
                        
                        }
                    }
                    
                }
        

            }                           //ANALOGO AO PRIMEIRO EXEMPLO

            if(jog->virado=='e'){
                for(int i=1; i<5; i++){
                    mvaddch(jog->pos.x, jog->pos.y-i, 'o');
                    for(int j=0; j<numbots; j++){
                        if(bots[j]->pos.x==jog->pos.x && bots[j]->pos.y==jog->pos.y-i && bots[i]->hp>0){
                            bots[j]->hp--;
                            bots[j]->corpo='0';
                            nbots--;
                        }
                    }
                    
                }

            }

            if(jog->virado=='d'){
                for(int i=1; i<5; i++){
                    mvaddch(jog->pos.x, jog->pos.y+i, 'o');
                    for(int j=0; j<numbots; j++){
                        if(bots[j]->pos.x==jog->pos.x && bots[j]->pos.y==jog->pos.y+i && bots[i]->hp>0){
                            bots[j]->hp--;
                            bots[j]->corpo='0';
                            nbots--;
                   
                        }
                    }
                    
                }

            }
            jog->numbalas--; //decrementa o numero de balas do jogador
        }

        if(mapa[jog->pos.x][jog->pos.y].c=='P'){   //se o jogador tiver na mesma posicao de um portal
            clear();
          
            int x= jog->pos.x;  //guarda as posicoes do jogador
            int y=jog->pos.y;
            Chao* portal[25][50];   //cria um novo mapa mais pequeno, 25 por 50
            for(int i=0; i<25; i++){
                for(int j=0; j<50; j++){
                    portal[i][j]=criachao();
                    portal[i][j]->c=' ';
                    if(i==0){portal[i][j]->c='#';}
                    if(i==24){portal[i][j]->c='#';} //mapa bom as bordas a '#' e sem paredes no meio
                    if(j==0){portal[i][j]->c='#';}
                    if(j==49){portal[i][j]->c='#';}
                    
                }
            }
            jog->pos.x=12;
            jog->pos.y=25;   //o jogador tem novas cordenadas
            char p;

            Boss* b = criaboss();               //cria o boss, ver funcao acima
            Coracao* cor = criacoracao();

            while(jog->hp>0 && cor->c1=='<'){  //enquanto o jogador tiver hp e enquanto o jogador nao apanhar o coracao
                                               //innformacoes sobre o jogo, por mais bonito deppois
                char stra[20];
                sprintf(stra,"%d",jog->hp);
                mvprintw(12, 75,"JOGADOR HP: %s", stra);

                if(b->hp>=10){
                    char strb[20];
                    sprintf(strb, "%d", b->hp);
                    mvprintw(13,75,"BOSS HP: %s", strb);
                }
                else{
                    char strb[20];
                    sprintf(strb, "%d", b->hp);
                    mvprintw(13,75,"BOSS HP: %s ", strb);
                }


                p=getch(); //busca input



                for(int i=0; i<25; i++){
                    for(int j=0; j<50; j++){
                        if(portal[i][j]->c == '#'){   //se o mapa for uma parede imprime como um bloco cinza
    
                            start_color();
                            use_default_colors();
                            init_pair(6, 8,8);
                            attron(COLOR_PAIR(6));
                            mvaddch(i,j,portal[i][j]->c);  //imprime o mapa
                            refresh();
                            attroff(COLOR_PAIR(6));
                    
                        }
                        
                        else if(portal[i][j]->c == ' '){
                            start_color();//se o mapa for chao imprime um bloco azul
                            use_default_colors();
                            init_pair(7, COLOR_BLACK,COLOR_BLACK);
                            attron(COLOR_PAIR(7));
                            mvaddch(i,j,portal[i][j]->c);  //imprime o mapa
                            refresh();
                            attroff(COLOR_PAIR(7));
                        }
                        
                    }
                }
              

                if(p=='q'){break;}

                else if(p!='q' && p!='s'){
                    jogadoranda(jog, p);
                    if(portal[jog->pos.x][jog->pos.y]->c=='#'){ //funcao que mexe o jogador
                        jogadordesfazjogada(jog, p);
                    }
                }

                else if(p=='s'){
                    
                    if(jog->virado=='c'){
                        if(portal[jog->pos.x-1][jog->pos.y]->c!='#'){
                            jog->armabasepos.x=jog->pos.x-1;      //se tiver virado para cima, a espada fica na posicao acima do jogador
                            jog->armabasepos.y=jog->pos.y;        //a base da espada na posicao x-1 e a ponta na posicao x-2 comparado ao jogador
                            jog->armabase='-';
                
                            if(portal[jog->pos.x-2][jog->pos.y]->c!='#'){
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
                        if(portal[jog->pos.x+1][jog->pos.y]->c!='#'){
                            jog->armabasepos.x=jog->pos.x+1;      //se tiver virado para cima, a espada fica na posicao acima do jogador
                            jog->armabasepos.y=jog->pos.y;        //a base da espada na posicao x-1 e a ponta na posicao x-2 comparado ao jogador
                            jog->armabase='-';
                                
                            if(portal[jog->pos.x+2][jog->pos.y]->c!='#'){
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
                        if(portal[jog->pos.x][jog->pos.y-1]->c!='#'){
                            jog->armabasepos.x=jog->pos.x;      //se tiver virado para cima, a espada fica na posicao acima do jogador
                            jog->armabasepos.y=jog->pos.y-1;        //a base da espada na posicao x-1 e a ponta na posicao x-2 comparado ao jogador
                            jog->armabase='|';
                        
                            if(portal[jog->pos.x][jog->pos.y-2]->c!='#'){
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

                        else {
                            jog->armabase=' ';
                            jog->armabasepos.x=52;
                            jog->armabasepos.y=0;
                            jog->armaponta=' ';
                            jog->armapontapos.x=52;
                            jog->armapontapos.y=0;
                        }
                    }


                    if(jog->virado=='d'){
                        if(portal[jog->pos.x][jog->pos.y+1]->c!='#'){
                            jog->armabasepos.x=jog->pos.x;      //se tiver virado para cima, a espada fica na posicao acima do jogador
                            jog->armabasepos.y=jog->pos.y+1;        //a base da espada na posicao x-1 e a ponta na posicao x-2 comparado ao jogador
                            jog->armabase='|';
                    
                            if(portal[jog->pos.x][jog->pos.y+2]->c!='#'){
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
               

                    if(jog->armabasepos.x != 52){
                        start_color();
                        use_default_colors();
                        init_pair(2, COLOR_WHITE,COLOR_BLACK);
                        attron(COLOR_PAIR(2));
                        mvaddch(jog->armabasepos.x, jog->armabasepos.y, jog->armabase); //DESENHA A BASE E A PONTA DE ESPADA
                        //mvaddch(jog->armapontapos.x, jog->armapontapos.y, jog->armaponta);
                        refresh();
                        attroff(COLOR_PAIR(2));}
                        
                    if(jog->armapontapos.x != 52){
                        start_color();
                        use_default_colors();
                        init_pair(2, COLOR_WHITE,COLOR_BLACK);
                        attron(COLOR_PAIR(2));
                        //mvaddch(jog->armabasepos.x, jog->armabasepos.y, jog->armabase); //DESENHA A BASE E A PONTA DE ESPADA
                        mvaddch(jog->armapontapos.x, jog->armapontapos.y, jog->armaponta);
                        refresh();
                        attroff(COLOR_PAIR(2));
                    }
                  
                
                    if(((jog->armapontapos.x==b->cabeca.pos.x) && (jog->armapontapos.y==b->cabeca.pos.y)) || ((jog->armabasepos.x==b->cabeca.pos.x) && (jog->armabasepos.y==b->cabeca.pos.y)&& b->cabeca.corpo=='o')){
                        b->hp--;
                        b->cabeca.pos.x=7;
                        b->cabeca.pos.y=25;
                                                        //se a espada do acertar na cabeca do boss, o boss volta a posicao inicial dele(meio do mapa)
                        b->body.pos.x=8;
                        b->body.pos.y=25;
                            
                        b->bracodir.pos.x=8;
                        b->bracodir.pos.y=26;
                            
                        b->bracoesq.pos.x=8;
                        b->bracoesq.pos.y=24;
    
                            
                    }

                }

            
                int n=rand()%5;

                if((p=='2' || p=='4' || p=='6' || p=='8') && b->cabeca.corpo=='o'){
                    bossanda(b, n);           //se o boss tiver vivo e se o jogador se mexer, o boss tambem se mexe
                }

                if(b->cabeca.corpo=='o'){
                    desenhaboss(b); //desenha o boss, ver funcao acima
                }
                
                start_color();
                use_default_colors();
                init_pair(4, 10,COLOR_BLACK);
                attron(COLOR_PAIR(4));
                mvaddch(jog->pos.x, jog->pos.y, jog->eu); //desenha o jogador
                refresh();
                attroff(COLOR_PAIR(4));
                    
                if((((jog->pos.x==b->cabeca.pos.x)  && (jog->pos.y==b->cabeca.pos.y))||((jog->pos.x==b->bracodir.pos.x) && (jog->pos.y==b->bracodir.pos.y))|| ((jog->pos.x==b->bracoesq.pos.x)  && (jog->pos.y==b->bracoesq.pos.y)) || ((jog->pos.x==b->body.pos.x) && (jog->pos.y==b->body.pos.y))) && b->cabeca.corpo=='o'){
                    jog->pos.x=1; jog->pos.y=1; jog->hp--;
                    b->cabeca.pos.x=7;
                    b->cabeca.pos.y=25;
                        
                    b->body.pos.x=8;
                    b->body.pos.y=25;
                                             //se o boss acertar no jogador, o jogador perde hp e o jogador e o boss voltam as pos iniciais
                    b->bracodir.pos.x=8;
                    b->bracodir.pos.y=26;
                        
                    b->bracoesq.pos.x=8;
                    b->bracoesq.pos.y=24;
                }

                if(b->hp<1){     //QUANOO O BOSS MORRER
                    b->cabeca.corpo='w';
                    
                    start_color();
                    use_default_colors();
                    init_pair(2, 13,COLOR_BLACK);
                    attron(COLOR_PAIR(2));
                    
                    mvaddch(cor->pos1.x, cor->pos1.y, cor->c1); //SE O BOSS MORRER IMPRIME O CORACAO
                    mvaddch(cor->pos2.x, cor->pos2.y, cor->c2);   //cores implementadas
                    
                    refresh();
                    attroff(COLOR_PAIR(2));
                   
                    
                    b->body.pos.x=0;
                    b->body.pos.y=0;
                    
                    b->bracodir.pos.x=0;
                    b->bracodir.pos.y=0;
                    				//METE O BOSS NUM CANTO PARA NAO ATRAPALHAR NO ESPACO LIVRE
                    b->bracoesq.pos.x=0;
                    b->bracoesq.pos.y=0;
                    
                    b->cabeca.pos.x=0;
                    b->cabeca.pos.y=0;
                    
                    b->cabeca.corpo='#';
                    b->body.corpo='#';			//METE O BOSS COM A IMAGEM DA PAREDE
                    b->bracodir.corpo='#';
                    b->bracoesq.corpo='#';
                    
                    if((jog->pos.x==cor->pos1.x && jog->pos.y== cor->pos1.y) || (jog->pos.x==cor->pos2.x && jog->pos.y== cor->pos2.y)){
                        jog->hp+=2;
                        break; //SE O JOGADOR APANHA O CORACAO ENTAO GANHA 2 DE HP E SAI DO PORTAL E VOLTA AO MAPA(BREAK)
                    }
                }

               







                
            

            }
            
            mapa[x][y].c=' ';  //quando o jogador apanha o coracao o portal desaparece, fica com ' ' em vez de 'P'
            jog->pos.x=x;
            jog->pos.y=y; //o jogador volta as posicao aonde entrou no portal que foram guardads em cima
            np--;             //num de portais decrementa
            
            
            
        }

        
    }

    if(acabou==1){
        char* texto = "JOGO COMPLETADO COM SUCESSO";
        int fimx = (COLS - strlen(texto)) / 2; // Center the text horizontally
        int fimy = LINES / 2;                  // Center the text vertically

        clear();
        mvprintw(fimy, fimx, texto); // Print the text at the specified position
        refresh();            // Update the screen

        usleep(5000000); // Wait for 5 seconds
    }
    else{
        char* texto = "DERROTADO";
        int fimx = (COLS - strlen(texto)) / 2; 
        int fimy = LINES / 2;                  
        clear();
        mvprintw(fimy, fimx, texto); 
        refresh();            
        usleep(5000000); 
    }


    endwin();

}
