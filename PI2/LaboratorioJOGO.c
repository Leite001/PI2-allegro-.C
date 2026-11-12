
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
    int altura_tela)
{

    //inicializando allegro
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    //


    al_clear_to_color(al_map_rgb(255, 255, 255));

    // Desenhos do laboratório
    al_draw_rectangle(10, 10, 150, 40,
        al_map_rgb(0, 0, 0), 2);

    al_draw_textf(fonte, al_map_rgb(0, 0, 0),
        10, 10, 0, "Voltar <--");

    al_draw_textf(fonte, al_map_rgb(0, 0, 0),
        largura_tela / 2 - 50, 10, 0, "LABORATÓRIO");

    al_draw_line(0, 50, largura_tela, 50,
        al_map_rgb(0, 0, 0), 2);

    al_draw_line(80, 50, 80, altura_tela,
        al_map_rgb(0, 0, 0), 2);
}

int laboratorioJogo() {
    //inicializando allegro
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    //



    return 0;
}