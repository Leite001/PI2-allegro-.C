#ifndef LABORATORIOJOGO_H
#define LABORATORIOJOGO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

void laboratorio_atualizar(ALLEGRO_MOUSE_STATE* mouse, bool* menu, bool* laboratorio, bool* jogo);

void laboratorio_desenhar( ALLEGRO_FONT* fonte, int largura_tela, int altura_tela);

#endif