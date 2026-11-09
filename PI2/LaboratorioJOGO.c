
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
