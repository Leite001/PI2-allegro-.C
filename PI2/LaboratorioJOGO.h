#ifndef LABORATORIOJOGO_H
#define LaboratorioJOGO_h

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

typedef	struct {
	char nome[50];
	char simbolo[5];
	int elementoID;
	int numeroAtomico;
	int massaAtomica;
	int escolhido;
	float xi;
	float xf;
	float yi;
	float yf;
}Elementos;

void laboratorio_atualizar(ALLEGRO_MOUSE_STATE* mouse, bool* menu, bool* laboratorio, bool* jogo, ALLEGRO_KEYBOARD_EVENT* teclado, bool *rodandoJogo);

void laboratorio_desenhar( ALLEGRO_FONT* fonte, int largura_tela, int altura_tela, ALLEGRO_BITMAP* laboratorioIMG, Elementos e[], int *bloco1, int *bloco2, ALLEGRO_MOUSE_STATE* mouse, ALLEGRO_BITMAP* elementoLeve, ALLEGRO_BITMAP* elementoPesado, ALLEGRO_BITMAP* receita, bool *receitaV, bool *prontoPJogo, int *misturas, int *misturaFeita, ALLEGRO_BITMAP* elementoNatural, bool *reacao);



#endif