
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
    ALLEGRO_BITMAP* laboratorioIMG, Elementos e[])
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
    
    //soma
	al_draw_line(650, 150, 700, 150, al_map_rgb(255, 255, 255), 2);
	al_draw_line(675, 125, 675, 175, al_map_rgb(255, 255, 255), 2);

    //outro bloco
    al_draw_rectangle(750, 100, 850, 200, al_map_rgb(255, 255, 255), 2);

    //reação
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 625, 220, 0, "REACAO");
	al_draw_line(600, 220, 675, 300, al_map_rgb(255, 255, 255), 2);	
    al_draw_line(675, 300, 750, 220, al_map_rgb(255, 255, 255), 2);
    //bloco reação
	al_draw_rectangle(625, 320, 725, 420, al_map_rgb(255, 255, 255), 2);

	//Caracteristicas dos elementos

    al_draw_text(fonte, al_map_rgb(255, 255, 255), 50, 300, 0, "A:");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 50, 330, 0, "Z:");

    for (int i = 0; i < 5; i++) {
        if (e[i].escolhido == 1) {
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), 80, 300, 0, "%d", e[i].massaAtomica);
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), 80, 330, 0, "%d", e[i].numeroAtomico);
        }
    }


    
    //O,H,Fe,C,S
    //H + O = amarelo
    //Fe + S = branco
    //C + O = azul
}

