
#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<stdbool.h>
#include<allegro5/keyboard.h>
#include<allegro5/mouse.h>
#include<allegro5/timer.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <time.h>
#include<allegro5/allegro_image.h>
#include "LaboratorioJOGO.h"


// ATUALIZAR LABORATÓRIO
void laboratorio_atualizar(
    ALLEGRO_MOUSE_STATE* mouse,
    bool* menu,
    bool* laboratorio,
    bool* jogo, ALLEGRO_KEYBOARD_EVENT* teclado, bool *rodandoJogo)
{
    // Botão voltar
    if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 10 && mouse->x <= 150 && mouse->y >= 10 && mouse->y <= 40 && *jogo == false && *rodandoJogo == false)
    {
        *menu = true;
        *laboratorio = false;
        *jogo = false;
    }
    if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 10 && mouse->x <= 150 && mouse->y >= 10 && mouse->y <= 40 && *jogo == false && *rodandoJogo == true){
            
                *menu = false;
                *laboratorio = false;
                *jogo = true;
            
    }
}


// DESENHAR LABORATÓRIO
void laboratorio_desenhar(
    ALLEGRO_FONT* fonte,
    int largura_tela,
    int altura_tela,
    ALLEGRO_BITMAP* laboratorioIMG, Elementos e[], int *bloco1, int *bloco2, ALLEGRO_MOUSE_STATE* mouse, ALLEGRO_BITMAP* elementoLeve, ALLEGRO_BITMAP* elementoPesado, ALLEGRO_BITMAP* receita, bool *receitaV, bool *prontoPJogo, int *misturas, int *misturaFeita,ALLEGRO_BITMAP* elementoNatural, bool* reacao)
{


    if (*receitaV == false && *reacao == false) {
        al_clear_to_color(al_map_rgb(9, 9, 48));

        //receitas

        al_draw_scaled_bitmap(receita, 14, 28, 135, 135, 380, 360, 50, 50, 0);
        //al_draw_bitmap(receita, 400, 360, 0);
        if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 380 && mouse->x <= 430 && mouse->y >= 360 && mouse->y <= 410) {
            *receitaV = true;
            *reacao = false;
        }
        //625, 320, 725, 420
        if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 625 && mouse->x <= 725 && mouse->y >= 320 && mouse->y <= 420) {
            *reacao = true;
            *receitaV = false;
        }



        // Desenhos do laboratório
        al_draw_scaled_bitmap(laboratorioIMG, 0, 0, 500, 333, 10, 5, 467, 300, 0);

        al_draw_rectangle(10, 10, 150, 40, al_map_rgb(255, 255, 255), 2);

        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 0, "Voltar <--");

        al_draw_textf(fonte, al_map_rgb(255, 255, 255), largura_tela / 2 - 50, 10, 0, "LABORATORIO");

        al_draw_line(0, 50, largura_tela, 50, al_map_rgb(255, 255, 255), 2);

        //blocos
        al_draw_rectangle(500, 100, 600, 200, al_map_rgb(255, 255, 255), 2);
        if (*bloco1 != -1) {

            Elementos* elemento = &e[*bloco1];

            ALLEGRO_BITMAP* elementoIMG = al_create_sub_bitmap(laboratorioIMG, elemento->xi + 20, elemento->yi + 5, elemento->xf - elemento->xi, elemento->yf - elemento->yi);

            if (elemento->elementoID == 1) {
                al_draw_scaled_bitmap(laboratorioIMG, elemento->xi + 5, elemento->yi + 10, 23, 24, 500, 100, 100, 100, 0);
            }
            else {
                elementoIMG = al_create_sub_bitmap(laboratorioIMG, elemento->xi + 20, elemento->yi + 5, elemento->xf - elemento->xi, elemento->yf - elemento->yi);
            }
            al_draw_scaled_bitmap(elementoIMG, 0, 0, elemento->xf - elemento->xi, elemento->yf - elemento->yi, 500, 100, 100, 100, 0);

            al_destroy_bitmap(elementoIMG);
        }
        //tirar o elementos do bloco
        if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 500 && mouse->x <= 600 && mouse->y >= 100 && mouse->y <= 200) {
            *bloco1 = -1;
        }
        //soma
        al_draw_line(650, 150, 700, 150, al_map_rgb(255, 255, 255), 2);
        al_draw_line(675, 125, 675, 175, al_map_rgb(255, 255, 255), 2);

        //outro bloco
        al_draw_rectangle(750, 100, 850, 200, al_map_rgb(255, 255, 255), 2);
        if (*bloco2 != -1) {

            Elementos* elemento = &e[*bloco2];

            ALLEGRO_BITMAP* elementoIMG = al_create_sub_bitmap(laboratorioIMG, elemento->xi + 20, elemento->yi + 5, elemento->xf - elemento->xi, elemento->yf - elemento->yi);

            if (elemento->elementoID == 1) {
                al_draw_scaled_bitmap(laboratorioIMG, elemento->xi + 5, elemento->yi + 10, 23, 24, 750, 100, 100, 100, 0);
            }
            else {
                elementoIMG = al_create_sub_bitmap(laboratorioIMG, elemento->xi + 20, elemento->yi + 5, elemento->xf - elemento->xi, elemento->yf - elemento->yi);
            }
            al_draw_scaled_bitmap(elementoIMG, 0, 0, elemento->xf - elemento->xi, elemento->yf - elemento->yi, 750, 100, 100, 100, 0);

            al_destroy_bitmap(elementoIMG);
        }

        //tirar o elementos do outro bloco
        if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 750 && mouse->x <= 850 && mouse->y >= 100 && mouse->y <= 200) {
            *bloco2 = -1;
        }

        //MISTURA DE ELEMENTOS
        if (*bloco1 != -1 && *bloco2 != -1) {
            if ((e[*bloco1].elementoID == 2 && e[*bloco2].elementoID == 4) || (e[*bloco1].elementoID == 4 && e[*bloco2].elementoID == 2)) {
                al_draw_scaled_bitmap(elementoPesado, 46, 50, 159, 155, 635, 335, 100, 100, 0);
                //625, 320, 725, 420, 0);

                if (*misturaFeita == 1) {
                    (*misturaFeita)++;

                }
              

            }
        }
        if (*bloco1 != -1 && *bloco2 != -1) {
            if ((e[*bloco1].elementoID == 0 && e[*bloco2].elementoID == 1) || (e[*bloco1].elementoID == 1 && e[*bloco2].elementoID == 0)) {
                al_draw_scaled_bitmap(elementoLeve, 46, 50, 159, 155, 630, 325, 80, 80, 0);
                al_draw_textf(fonte,al_map_rgb(255,0,0), 625, 440,0,"jogo liberado!");

                if (*misturaFeita == 0) {
                    (*misturaFeita)++;
                }

                *prontoPJogo = true;


            }
        }
        if (*bloco1 != -1 && *bloco2 != -1) {
            if ((e[*bloco1].elementoID == 0 && e[*bloco2].elementoID == 3) || (e[*bloco1].elementoID == 3 && e[*bloco2].elementoID == 0)) {
                al_draw_scaled_bitmap(elementoNatural, 18, 20, 180, 165, 630, 325, 100, 100, 0);
                //625, 320, 725, 420, 0);  625, 320, 725, 420

                if (*misturaFeita == 2) {
                    (*misturaFeita)++;

                }


            }
        }
        //reação
        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 625, 220, 0, "REACAO");
        al_draw_line(600, 220, 675, 300, al_map_rgb(255, 255, 255), 2);
        al_draw_line(675, 300, 750, 220, al_map_rgb(255, 255, 255), 2);
        //bloco reação
        al_draw_rectangle(625, 320, 725, 420, al_map_rgb(255, 255, 255), 2);

        //Caracteristicas dos elementos

        if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 50 && mouse->y >= 300 && mouse->x <= 70 && mouse->y <= 325) {

            al_draw_text(fonte, al_map_rgb(255, 255, 255), 25, 330, 0, "Massa atomica: Representa a massa de um atomo");
        }

        if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 50 && mouse->y >= 380 && mouse->x <= 70 && mouse->y <= 405) {

            al_draw_multiline_text(fonte, al_map_rgb(255, 255, 255), 25, 410, 1000, 25, 0, "Numero Atomico: Indica a quantidade de \n protons no nucleo de um atomo");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 255), 50, 300, 0, "A:");


        al_draw_text(fonte, al_map_rgb(255, 255, 255), 50, 380, 0, "Z:");

        for (int i = 0; i < 5; i++) {
            if (e[i].escolhido == 1) {
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 80, 300, 0, "%d", e[i].massaAtomica);
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 80, 380, 0, "%d", e[i].numeroAtomico);
            }
        }
        //O,H,Fe,C,S
        //H + O = amarelo
        //Fe + S = branco
        //C + O = azul
    }
    if (*receitaV == true && *reacao == false) {
        al_clear_to_color(al_map_rgb(9, 9, 48));

        al_draw_filled_rectangle(280, 150, 600, 320, al_map_rgb(46, 45, 45));
        al_draw_rectangle(280, 150, 600, 320, al_map_rgb(0, 0, 0), 4);
        al_draw_filled_rectangle(580, 150, 600, 170, al_map_rgb(255, 0, 0));
        al_draw_line(585,155, 595,165,al_map_rgb(255,255,255),1);
        al_draw_line(585,165, 595,155,al_map_rgb(255, 255, 255),1);
        al_draw_scaled_bitmap(receita, 14, 28, 135, 135, 380, 360, 50, 50, 0);
        
        al_draw_textf(fonte,al_map_rgb(255,255,255), 345, 100, 0, "RECEITAS");

        if (*misturas >= 0) {
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), 290, 180, 0, "H + O = elementos leves");
        }

        if (*misturas >= 1) {
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), 290, 230, 0, "Fe + S = elementos metais");
        }

        if (*misturas >= 2) {
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), 290, 280, 0, "C + O = elementos naturais");
        }
       
        


        if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 580 && mouse->x <= 600 && mouse->y >= 150 && mouse->y <= 170) {
            *receitaV = false;
            *reacao = false;
        }
    }
    if(*receitaV == false && *reacao == true) {
   
        al_clear_to_color(al_map_rgb(9, 9, 48));
        //alt = 425 larg =785
        al_draw_filled_rectangle(75, 75, largura_tela - 75, altura_tela - 75, al_map_rgb(46, 45, 45));
        al_draw_rectangle(75, 75, largura_tela - 75, altura_tela - 75, al_map_rgb(0, 0, 0), 4);
        //280, 150, 600, 320
        al_draw_filled_rectangle(largura_tela - 95, 75, largura_tela - 75, 95, al_map_rgb(255, 0, 0));
        //580, 150, 600, 170
        al_draw_line(largura_tela - 90, 77, largura_tela - 80, 92, al_map_rgb(255, 255, 255), 1);
        al_draw_line(largura_tela - 80, 77, largura_tela - 90, 92, al_map_rgb(255, 255, 255), 1);
       //int altura_tela = 500;
        //int largura_tela = 860;

        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 375, 25, 0, "REACAO");
        if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 765 && mouse->x <= 785 && mouse->y >= 75 && mouse->y <= 95) {
            *receitaV = false;
            *reacao = false;
        }
        if (*bloco1 != -1 && *bloco2 != -1) { // Fe + S
            if ((e[*bloco1].elementoID == 2 && e[*bloco2].elementoID == 4) || (e[*bloco1].elementoID == 4 && e[*bloco2].elementoID == 2)) {
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 95, 0, "Fe + S = FeS -> Sulfeto de ferro");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 135, 0, "A combinacao de ferro e enxofre forma sulfeto de ferro.");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 170, 0, "Essa mistura representa a categoria dos metais e desbloqueia");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 195, 0, "a energia branca, capaz de enfrentar inimigos metalicos.");


                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 300, 255, 0, "Branco -> Elementos metalicos");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 290, 0, "Representa os metais, como o ferro. A cor branca foi escolhida");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 315, 0, "por lembrar o brilho prateado característico de muitos metais.");

                //alt = 425 larg =785
                al_draw_scaled_bitmap(elementoPesado, 46, 50, 159, 155, 715, 375, 100, 100, 0);
            }
        }
        if (*bloco1 != -1 && *bloco2 != -1) { // O + H
            if ((e[*bloco1].elementoID == 0 && e[*bloco2].elementoID == 1) || (e[*bloco1].elementoID == 1 && e[*bloco2].elementoID == 0)) {
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 95, 0, "H2 + O = H2O -> Agua");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 75, 135, 0, "A combinacao de hidrogenio e oxigenio forma agua e libera energia.");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 170, 0, "Essa reacao desperta uma energia amarela, utilizada pelo");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 195, 0, "personagem para enfrentar inimigos dessa categoria.");


                al_draw_textf(fonte, al_map_rgb(255, 255, 0), 300, 255, 0, "Amarelo —> Elementos leves");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 290, 0, "Representa elementos de menor massa atomica, como o");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 315, 0, "hidrogenio e o oxigenio. A cor amarela foi escolhida para");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 335, 0, "simbolizar a energia e a leveza dessa categoria.");
                 //alt = 425 larg =785
                al_draw_scaled_bitmap(elementoLeve, 46, 50, 159, 155, 715, 375, 80, 80, 0);
            }
        }
        if (*bloco1 != -1 && *bloco2 != -1) { // O + C
            if ((e[*bloco1].elementoID == 0 && e[*bloco2].elementoID == 3) || (e[*bloco1].elementoID == 3 && e[*bloco2].elementoID == 0)) {
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 95, 0, "C +  O2 = CO2 -> Dioxido de carbono.");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 135, 0, "A combinacao de carbono e oxigenio forma dioxido de carbono.");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 170, 0, "Como esses elementos estao presentes em diversos processos");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 195, 0, "naturais, essa mistura representa a categoria natural e ");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 220, 0, "desbloqueia a energia azul.");


                al_draw_textf(fonte, al_map_rgb(0, 0, 255), 300, 255, 0, "Azul -> Elementos naturais");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 290, 0, "Representa elementos presentes em diversos processos naturais ");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 315, 0, "e biologicos, como o carbono. A cor azul foi escolhida para");
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 90, 335, 0, " simbolizar a natureza e os elementos que fazem parte dela.");

                //alt = 425 larg =785
                al_draw_scaled_bitmap(elementoNatural, 18, 20, 180, 165, 700, 375, 100, 100, 0);

            }
        }
    }
}
/*Amarelo — Elementos leves

Representa elementos de menor massa atômica, como o hidrogênio e o oxigênio. A cor amarela foi escolhida para simbolizar a energia e a leveza dessa categoria.

Branco — Elementos metálicos

Representa os metais, como o ferro. A cor branca foi escolhida por lembrar o brilho prateado característico de muitos metais.

Azul — Elementos naturais

Representa elementos presentes em diversos processos naturais e biológicos, como o carbono. A cor azul foi escolhida para simbolizar a natureza e os elementos que fazem parte dela.*/
