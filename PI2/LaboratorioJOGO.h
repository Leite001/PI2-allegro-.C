#ifndef LABORATORIOJOGO_H
#define LaboratorioJOGO_h

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

typedef	struct {
	char nome[50];
	char simbolo[5];
	int numeroAtomico;
	int massaAtomica;
	int escolhido;
}Elementos;

void laboratorio_atualizar(ALLEGRO_MOUSE_STATE* mouse, bool* menu, bool* laboratorio, bool* jogo);

void laboratorio_desenhar( ALLEGRO_FONT* fonte, int largura_tela, int altura_tela, ALLEGRO_BITMAP* laboratorioIMG, Elementos e[]);



#endif