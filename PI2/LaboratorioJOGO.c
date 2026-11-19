
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
    bool* jogo)
{
    // Botão voltar
    if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) &&
        mouse->x >= 10 && mouse->x <= 150 &&
        mouse->y >= 10 && mouse->y <= 40)
    {
        *menu = true;
        *laboratorio = false;
        *jogo = false;
    }

}


// DESENHAR LABORATÓRIO
void laboratorio_desenhar(
    ALLEGRO_FONT* fonte,
    int largura_tela,
    int altura_tela,
    ALLEGRO_BITMAP* laboratorioIMG, Elementos e[], int *bloco1, int *bloco2, ALLEGRO_MOUSE_STATE* mouse, ALLEGRO_BITMAP* elementoLeve, ALLEGRO_BITMAP* elementoPesado)
{

    al_clear_to_color(al_map_rgb(9, 9, 48));



    // Desenhos do laboratório
    al_draw_scaled_bitmap(laboratorioIMG, 0, 0, 500, 333, 10, 5, 467, 300, 0);

    al_draw_rectangle(10, 10, 150, 40,
        al_map_rgb(255, 255, 255), 2);

    al_draw_textf(fonte, al_map_rgb(255, 255, 255),
        10, 10, 0, "Voltar <--");

    al_draw_textf(fonte, al_map_rgb(255, 255, 255),
        largura_tela / 2 - 50, 10, 0, "LABORATORIO");

    al_draw_line(0, 50, largura_tela, 50,
        al_map_rgb(255, 255, 255), 2);
    //blocos
    al_draw_rectangle(500, 100, 600, 200, al_map_rgb(255, 255, 255), 2);
    if (*bloco1 != -1) {

        Elementos* elemento = &e[*bloco1];

        ALLEGRO_BITMAP* elementoIMG = al_create_sub_bitmap(laboratorioIMG, elemento->xi + 20, elemento->yi + 5, elemento->xf - elemento->xi, elemento->yf - elemento->yi);

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

        al_draw_scaled_bitmap(elementoIMG, 0, 0, elemento->xf - elemento->xi, elemento->yf - elemento->yi, 750, 100, 100, 100, 0);

        al_destroy_bitmap(elementoIMG);
    }

    //tirar o elementos do outro bloco
    if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 750 && mouse->x <= 850 && mouse->y >= 100 && mouse->y <= 200) {
        *bloco2 = -1;
    }


    //MISTURA DE ELEMENTOS
    for (int i = 0; i < 5; i++) {

        for (int j = 0; j < 5; j++) {
            if (e[i].elementoID == 2 && e[j].elementoID == 4) {
                al_draw_scaled_bitmap(elementoPesado, 46, 50, 159, 155, 625, 320, 725, 420, 0);


                    //al_draw_scaled_bitmap(laboratorioIMG, 0, 0, 500, 333, 10, 5, 467, 300, 0);

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

    if (al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse->x >= 50 && mouse->y >= 380 && mouse->x <= 70 && mouse->y <=405) {

        al_draw_multiline_text(fonte, al_map_rgb(255, 255, 255), 25, 410, 1000, 25,  0, "Numero Atomico: Indica a quantidade de \n protons no nucleo de um atomo");
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

