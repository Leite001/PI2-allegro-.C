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

typedef struct {
	float x;
	float y;
	float raio;
	float velocidade;
	bool ativo;
	int dano;
	int vida;
	int cor; // 0 = azul, 1 = branco, 2 = amarelo
	int frame_atual;
	int contadorFrame;
}inimigo;

typedef struct {
	float xi;
	float xf;
	float yi;
	float yf;
	float velocidade;
	float velocidadePulo;
	bool noChao;
	bool virou;
	bool ativo;
	bool noAr;
	bool andando;
	int dano;
	int vida;
	int frameWalk;
	int contadorFrameWalk;
	int framePulo;
	int contadorFramePulo;
}Personagem;


void recordPontos(Personagem p, int pontos, int* record) {

	if (p.ativo == false && pontos > *record) {
		*record = pontos;
	}


}

void movimentacao(Personagem* p, int largura_tela) {

	al_install_keyboard();
	ALLEGRO_KEYBOARD_STATE teclado;
	al_get_keyboard_state(&teclado);

	//variaveis
	p->velocidade = 5.0f;
	float gravidade = 0.2f;
	//

// Movimentação para a esquerda
	if (al_key_down(&teclado, ALLEGRO_KEY_A) && p->ativo) {
		p->xi -= p->velocidade;
		p->xf -= p->velocidade;
		p->virou = true;
		p->andando = true;
	}
	// Movimentação para a direita
	else if (al_key_down(&teclado, ALLEGRO_KEY_D) && p->ativo) {
		p->xi += p->velocidade;
		p->xf += p->velocidade;
		p->virou = false;
		p->andando = true;
	}
	else {
		p->andando = false;
	}

	// Animação de caminhada
	if (p->andando && p->noChao) {

		p->contadorFrameWalk++;

		if (p->contadorFrameWalk >= 3) {
			p->frameWalk++;
			p->contadorFrameWalk = 0;
		}
		if (p->frameWalk >= 12) {
			p->frameWalk = 0;

		}
	}
	else if (!p->andando) {

		p->frameWalk = 0;
		p->contadorFrameWalk = 0;

	}


	// Impede que o personagem saia da tela
	if (p->xi < 0) {
		p->xi = 0;
		p->xf = 50;
	}

	if (p->xf > largura_tela) {
		p->xf = largura_tela;
		p->xi = largura_tela - 50;
	}
	//

	// Pulo
	if (al_key_down(&teclado, ALLEGRO_KEY_W) && p->noChao == true && p->ativo == true && p->noAr == false) {
		p->velocidadePulo = -6.0f;
		p->noChao = false;
		p->noAr = true;
	}
	//
	// Animação do pulo
	if (p->noAr) {

		p->contadorFramePulo++;

		if (p->contadorFramePulo >= 5) {

			p->framePulo++;
			p->contadorFramePulo = 0;

		}

		if (p->framePulo >= 4) {
			p->framePulo = 0;
		}

	}
	else {

		p->framePulo = 0;
		p->contadorFramePulo = 0;

	}



	//gravidade
	p->velocidadePulo += gravidade;
	p->yi += p->velocidadePulo;
	p->yf += p->velocidadePulo;

	// Colisão com o chão
	if (p->yf >= 400) {
		p->yf = 400;
		p->yi = p->yf - 80;
		p->velocidadePulo = 0.0f;
		p->noChao = true;
		p->noAr = false;
	}
}

void  projetil(float* balaX, float* balaY, bool* balaAtiva, float xi, float xf, float yi, float yf, int largura_tela, bool* mouseAnterior, bool* virou, bool* balaVirou, int* corBala, int* corProjetil, bool ativoP, int misturaFeita) {

	ALLEGRO_MOUSE_STATE mouse;
	al_get_mouse_state(&mouse);

	ALLEGRO_KEYBOARD_STATE teclado;
	al_get_keyboard_state(&teclado);

	bool mouseAtual = al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT);

	float velobala = 8.0f;

	//mudar a cor da bala com as teclas E, R e Q
	if (misturaFeita >= 0) {
		if (al_key_down(&teclado, ALLEGRO_KEY_E)) {

			*corBala = 2; // amarelo
		}
	}
	if (misturaFeita >= 2) {
		if (al_key_down(&teclado, ALLEGRO_KEY_Q)) {
			
			*corBala = 1; // branco	
		}
	}
	if(misturaFeita >= 3){
		if (al_key_down(&teclado, ALLEGRO_KEY_R)) {
			
			*corBala = 0; // azul
		}
	}
	

	// Detecta um novo clique
	if (mouseAtual && !*mouseAnterior && !*balaAtiva && ativoP == true) {
		*balaVirou = *virou;
		*corProjetil = *corBala;

		if (*balaVirou == false) {
			*balaX = xf;
		}
		else {
			*balaX = xi;
		}
		*balaY = yi + (yf - yi) / 2;
		*balaAtiva = true;
	}

	*mouseAnterior = mouseAtual;

	if (*balaAtiva && *balaVirou == false && ativoP == true) {
		*balaX += velobala;
	}
	else
	{
		if (*balaAtiva && *balaVirou == true && ativoP == true) {
			*balaX -= velobala;
		}
	}
	// Cor da bala
	ALLEGRO_COLOR cor;

	if (*corProjetil == 0) {
		cor = al_map_rgb(0, 0, 255);
		al_draw_rectangle(280, 400, 380, 460, al_map_rgb(0, 0, 255), 4);
		
	}
	else if (*corProjetil == 1) {
		cor = al_map_rgb(255, 255, 255);
		al_draw_rectangle(400, 400, 500, 460, al_map_rgb(255, 255, 255), 4);
	}
	else {
		cor = al_map_rgb(255, 255, 0);
		al_draw_rectangle(160, 400, 260, 460, al_map_rgb(255, 255, 0), 4);
	}

	//desenhando a bala de acordo com a cor

	if (*balaAtiva == true && ativoP == true) {
		if (!*balaVirou) {

			al_draw_filled_rectangle(*balaX, *balaY - 5, *balaX + 20, *balaY + 5, cor);

		}
		else {
			al_draw_filled_rectangle(*balaX - 20, *balaY - 5, *balaX, *balaY + 5, cor);

		}
	}
	// Desativa a bala quando sair da tela
	if (*balaX > largura_tela) {
		*balaAtiva = false;
	}
	else {
		if (*balaX < 0) {
			*balaAtiva = false;
		}
	}
}

void colisaoBala(float* balaX, float* balaY, bool* balaAtiva, bool* balaVirou, int* corProjetil, inimigo* inimigo, int* pontos) {

	if (*balaAtiva && inimigo->ativo) {

		float balaEsquerda;
		float balaDireita;

		// Define os limites horizontais da bala
		if (*balaVirou == false) {
			balaEsquerda = *balaX;
			balaDireita = *balaX + 20;
		}
		else {
			balaEsquerda = *balaX - 20;
			balaDireita = *balaX;
		}

		float balaTopo = *balaY - 5;
		float balaBaixo = *balaY + 5;

		// Limites do inimigo
		//limite por Y
		float inimigoEsquerdaY = inimigo->y - inimigo->raio;
		float inimigoDireitaY = inimigo->y + inimigo->raio;
		//limite por X
		float inimigoEsquerdaX = inimigo->x - inimigo->raio;
		float inimigoDireitaX = inimigo->x + inimigo->raio;

		// Verifica colisão
		if (balaDireita >= inimigoEsquerdaX && balaEsquerda <= inimigoDireitaX && balaBaixo >= inimigoEsquerdaY && balaTopo <= inimigoDireitaY) {

			if (*corProjetil == inimigo->cor) {
				inimigo->vida -= 3;
				*balaAtiva = false;
				*pontos += 50;
			}
			else {
				inimigo->vida -= 1; // Dano normal pontos normais
				*balaAtiva = false;
				*pontos += 10;
			}
			*balaAtiva = false;
		}
	}

	// Verifica se o inimigo morreu
	if (inimigo->vida <= 0) {
		inimigo->ativo = false;
	}
}

void colisaoPrincipal(Personagem* p, inimigo* inimigo) {

	if (!inimigo->ativo) {
		return;
	}
	// Ponto mais próximo do círculo dentro do retângulo
	float pontoX = inimigo->x;
	float pontoY = inimigo->y;

	if (pontoX < p->xi) {
		pontoX = p->xi;
	}
	else if (pontoX > p->xf) {
		pontoX = p->xf;
	}
	if (pontoY < p->yi) {
		pontoY = p->yi;
	}
	else if (pontoY > p->yf) {
		pontoY = p->yf;
	}

	// Distância entre o círculo e o ponto
	float dx = inimigo->x - pontoX;
	float dy = inimigo->y - pontoY;

	float distancia = dx * dx + dy * dy;

	// Colisão
	if (distancia < inimigo->raio * inimigo->raio &&
		p->yf > inimigo->y - inimigo->raio) {

		// Verifica se o personagem está pisando no inimigo
		if (p->yf < inimigo->y) {
			return;
		}
		//dano no personagem principal
		p->vida -= inimigo->dano;

		// Empurra o inimigo para fora do personagem
		if (inimigo->x < (p->xi + p->xf) / 2)
			inimigo->x = p->xi - inimigo->raio + 50;
		else
			inimigo->x = p->xf + inimigo->raio + 50;
	}
}

void perseguir_jogador(inimigo* inimigo, float jogadorX, bool jogadorAtivo) {

	if (!inimigo->ativo)
		return;

	
	// Para a direita
	if (inimigo->x < jogadorX && jogadorAtivo == true) {
		inimigo->x += inimigo->velocidade;

	}
	// Para a esquerda
	else if (inimigo->x > jogadorX) {
		inimigo->x -= inimigo->velocidade;
	}

	// Animação
	inimigo->contadorFrame++;

	if (inimigo->contadorFrame >= 5) {
		inimigo->frame_atual++;

		if (inimigo->frame_atual >= 4) {
			inimigo->frame_atual = 0;
		}

		inimigo->contadorFrame = 0;
	}
}

void sistemaWave(int* waves, bool* aguardandoWave, double* tempoProximaWave, inimigo inimigo[], int largura_tela) {
	int inimigosVivos = 0;

	int quantidadeInimigos = 3 + (*waves - 1) / 5;
	int inimigosEsquerda = 0;
	int inimigosDireita = 0;

	if (quantidadeInimigos > 20) {
		quantidadeInimigos = 20;
	}

	for (int i = 0; i < quantidadeInimigos; i++) {
		if (inimigo[i].ativo == true && inimigo[i].vida > 0) {
			inimigosVivos++;
		}
	}

	if (inimigosVivos == 0 && *aguardandoWave == false) {
		*aguardandoWave = true;
		*tempoProximaWave = al_get_time() + 5;
	}

	if (*aguardandoWave == true && al_get_time() >= *tempoProximaWave) {
		(*waves)++;

		quantidadeInimigos = 3 + (*waves - 1) / 5;

		if (quantidadeInimigos > 20) {
			quantidadeInimigos = 20;
		}

		for (int i = 0; i < quantidadeInimigos; i++) {
			inimigo[i].vida = 3;
			inimigo[i].ativo = true;
			inimigo[i].raio = 30;
			inimigo[i].dano = 5;

			inimigo[i].cor = i % 3;

			if (i % 3 == 0) inimigo[i].velocidade = 1.5f;
			if (i % 3 == 1) inimigo[i].velocidade = 1.0f;
			if (i % 3 == 2) inimigo[i].velocidade = 2.5f;

			inimigo[i].y = 370;

			if (rand() % 2 == 0) {
				inimigo[i].x = -inimigo[i].raio -inimigosEsquerda * 65;
				inimigosEsquerda++;
			}
			else {
				inimigo[i].x = largura_tela + inimigo[i].raio + inimigosDireita * 65;
				inimigosDireita++;
			}
		}

		*aguardandoWave = false;
	}
}

//funcão laboratorioJogo
void selectionElemento(Elementos elementos[], ALLEGRO_MOUSE_STATE* mouse, int *bloco1, int *bloco2, bool *mouseAnterior) {

	bool clicando = al_mouse_button_down(mouse, ALLEGRO_MOUSE_BUTTON_LEFT);

	if (clicando && !*mouseAnterior) {

		for (int i = 0; i < 5; i++) {

			if (mouse->x >= elementos[i].xi && mouse->x <= elementos[i].xf && mouse->y >= elementos[i].yi && mouse->y <= elementos[i].yf) {
				
				for (int j = 0; j < 5; j++) {
					elementos[j].escolhido = 0;
				}

				elementos[i].escolhido = 1;


				if (*bloco1 == -1) {
					*bloco1 = i;
				}
				else if (*bloco2 == -1) {
					*bloco2 = i;
				}
				else {
					*bloco2 = i;
				}



				break;
			}
		}
	}
	*mouseAnterior = clicando;
}

int main() {
	//inicializando allegro
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	//

	//display dimensões (tela)
	int altura_tela = 500;
	int largura_tela = 860;
	ALLEGRO_DISPLAY* display = al_create_display(largura_tela, altura_tela);
	al_set_window_position(display, 200, 200);

	//

	//variaveis	
	ALLEGRO_BITMAP* menuImg = al_load_bitmap("Imagens/MenuImagm.png");
	ALLEGRO_BITMAP* personagemImg = al_load_bitmap("Imagens/skeleton-walk.png");
	ALLEGRO_BITMAP* personagemJump = al_load_bitmap("Imagens/jump-skeleton.png");
	ALLEGRO_BITMAP* chaoImg = al_load_bitmap("Imagens/chao.png");
	ALLEGRO_BITMAP* laboratorioIMG = al_load_bitmap("Imagens/tabela_periodica.png");
	ALLEGRO_BITMAP* elementoLeve = al_load_bitmap("Imagens/leaf(2).png");
	ALLEGRO_BITMAP* elementoPesado = al_load_bitmap("Imagens/metal.png");
	ALLEGRO_BITMAP* elementoNatural = al_load_bitmap("Imagens/elementoNatural.png");
	ALLEGRO_BITMAP* receita = al_load_bitmap("Imagens/book.png");
	ALLEGRO_BITMAP* inimigoLeve = al_load_bitmap("Imagens/inimigoLeve.png");
	ALLEGRO_BITMAP* inimigoMetal = al_load_bitmap("Imagens/inimigoMetal.png");
	ALLEGRO_BITMAP* inimigoNatural = al_load_bitmap("Imagens/inimigoNatural.png");

	if (menuImg == NULL || personagemImg == NULL || personagemJump == NULL || chaoImg == NULL || laboratorioIMG == NULL || elementoLeve == NULL || elementoPesado == NULL || receita == NULL) {
		printf("Erro ao carregar imagens!\n");
		return -1;
	}

	Personagem principal = { 400.0f, 450.0f, 320.0f, 400.0f, 0.03f, 0.0f, true, false, true, false,false, 1 , 30, 0, 0, 0, 0 };
	float balaX = 0.0f;
	float balaY = 0.0f;
	bool balaAtiva = false;
	bool balaVirou = false;
	ALLEGRO_KEYBOARD_STATE teclado;
	ALLEGRO_MOUSE_STATE mouse;
	bool mouseAnterior = false;
	ALLEGRO_FONT* fonte = al_load_ttf_font("C:\\Windows\\Fonts\\arial.ttf", 24, 0);
	bool menu = true;
	bool laboratorio = false;
	bool jogo = false;
	bool prontoPJogo = false;
	bool rodando = true;
	bool receitaV = false;
	bool mostrarReceita = false;
	bool rodandoJogo = false;
	bool reacao = false;
	int tempoMensagem = 0;
	int misturas = 0;
	int misturaFeita = 0;
	int corBala = 2;       // cor selecionada
	int corProjetil = 2;   // cor da bala disparada
	ALLEGRO_EVENT_QUEUE* filaEventos = al_create_event_queue();
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	int pontos = 0;
	int record = 0;
	int bloco1 = -1;
	int bloco2 = -1;
	
	//50 e 80
	//

	//registro de eventos
	al_register_event_source(filaEventos, al_get_display_event_source(display));
	al_register_event_source(filaEventos, al_get_keyboard_event_source());
	al_register_event_source(filaEventos, al_get_mouse_event_source());
	al_register_event_source(filaEventos, al_get_timer_event_source(timer));
	al_start_timer(timer);

	//
	// variaveis Waves
	int waves = 0;
	bool aguardandoWave = false;
	double tempoProximaWave = 0;

	//
	//struct inimigo
	inimigo inimigo[20] = {
		{ -30, 370,30,1.5f,true, 5, 3, 0, 29,4 }, // azul natural
		{ -40, 370,30,1.0f, true, 5, 3, 1, 60,4 }, // branco metal
		{ -50, 370,30,2.5f, true, 5, 3, 2, 21,4 }  // amarelo leve
	};

	int framesLeveX[4] = { 4, 34, 64, 93 };
	int framesLeveY[4] = { 6, 12, 17, 15 };

	int framesMetalX[4] = { 0, 73, 137, 202 };
	int framesMetalY[4] = { 0, 4, 0, 4 };

	int framesNaturalX[4] = { 1, 31, 60, 90 };
	int framesNaturalY[4] = { 5, 17, 17, 17 };



	//randomizar local de spawn dos inimigos
	srand(time(NULL));
	for (int i = 0; i < 3; i++) {
		if (rand() % 2 == 0) {
			inimigo[i].x = -inimigo[i].raio;
		}
		else {
			inimigo[i].x = largura_tela + inimigo[i].raio;
		}
	}
	//

	//struct elementos char nome[50]; char simbolo[5]; int numeroAtomico; int massaAtomica; int escolhido;
	//O,H,Fe,C,S
	Elementos elementos[5] = {
		{ "Oxigenio", "O", 0, 8, 16, 0, 380, 400, 87, 107},
		{ "Hidrogenio", "H", 1, 1, 1, 0, 30, 60, 55, 85},
		{ "Ferro", "Fe", 2, 26, 55, 0, 185, 205, 135, 155},
		{ "Carbono", "C", 3, 6, 12, 0, 335, 355, 87, 107},
		{ "Enxofre", "S", 4, 16, 32, 0, 380, 400, 109, 128}
	};


//jogo rodando
	while (rodando) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(filaEventos, &evento);

		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			rodando = false;
		}

		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (evento.keyboard.keycode == ALLEGRO_KEY_TAB && jogo == true && rodandoJogo == true) {
				laboratorio = true;
				jogo = false;
				menu = false;
			}
		}

		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_get_keyboard_state(&teclado);
			al_get_mouse_state(&mouse);

			

			if (menu == true && jogo == false && laboratorio == false) // menu rodando
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_scaled_bitmap(menuImg, 0, 0, al_get_bitmap_width(menuImg), al_get_bitmap_height(menuImg), 0, 0, largura_tela, altura_tela, 0);
				//al_draw_rectangle(340, 259, 520, 299, al_map_rgb(255, 0, 0), 5);
				//al_draw_rectangle(340, 365, 520, 420, al_map_rgb(255, 0, 0), 5);

				if (prontoPJogo == false) {
					al_draw_textf(fonte, al_map_rgb(255, 0, 0), 340, 265, 0, "-----------------------");
				}

				if (prontoPJogo == true  && al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse.x >= 340 && mouse.x <= 520 && mouse.y >= 259 && mouse.y <= 299) {
					//reiniciando o personagem
					principal.vida = 30;
					principal.ativo = true;
					principal.xi = 400;
					principal.xf = 450;
					principal.yi = 320;
					principal.yf = 400;
					principal.noChao = true;
					principal.noAr = false;
					principal.virou = false;
					principal.velocidadePulo = 0;
					principal.frameWalk = 0;
					principal.contadorFrameWalk = 0;
					principal.framePulo = 0;
					principal.contadorFramePulo = 0;

					//
					//reiniciando waves
					waves = 0;
					aguardandoWave = false;
					tempoProximaWave = 0;
					//
					balaAtiva = false;
					balaVirou = false;
					corBala = 2;
					corProjetil = 2;

					//reiniciando pontos
					pontos = 0;
					bool mostrarReceita = false;
					int tempoMensagem = 0;

					//reiniciando elementos
					for (int i = 0; i < 5; i++) {
						elementos[i].escolhido = 0;
					}
					bloco1 = -1;
					bloco2 = -1;
					//
					for (int i = 0; i < 20; i++) {
						inimigo[i].vida = 0;
						inimigo[i].ativo = false;
					}


					for (int i = 0; i < 3; i++) {
						if (rand() % 2 == 0) {
							inimigo[i].x = -inimigo[i].raio;
						}
						else {
							inimigo[i].x = largura_tela + inimigo[i].raio;
						}
					}



					menu = false;
					laboratorio = false;
					jogo = true;
					rodandoJogo = true;
				}

				if (al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse.x >= 340 && mouse.x <= 520 && mouse.y >= 365 && mouse.y <= 420) {
					laboratorio = true;
					jogo = false;
					menu = false;

				}
			}
			else if (menu == false && jogo == true && laboratorio == false && prontoPJogo == true && rodandoJogo == true)// jogo rodando
			{
				//codigo mensagem
				if (pontos >= 1000 && misturaFeita == 1 && misturas == 0)
				{
					misturas++;
					mostrarReceita = true;
					tempoMensagem = 180;
				}
				if (pontos >= 2000 && misturaFeita == 2 && misturas == 1) {
					misturas++;
					mostrarReceita = true;
					tempoMensagem = 180;
				}

				if (mostrarReceita == true)
				{
					tempoMensagem--;

					if (tempoMensagem <= 0)
					{
						mostrarReceita = false;
					}
				}

				//chamando a função
				movimentacao(&principal, largura_tela);
				recordPontos(principal, pontos, &record);

				// Calcula o centro horizontal do jogador
				float jogadorCentroX = (principal.xi + principal.xf) / 2;
				int quantidadeInimigos = 3 + (waves - 1) / 5;

				if (quantidadeInimigos > 20) quantidadeInimigos = 20;

	
				for (int i = 0; i < quantidadeInimigos; i++) {
					perseguir_jogador(&inimigo[i], jogadorCentroX, principal.ativo);
					colisaoPrincipal(&principal, &inimigo[i]);
				}

				//função waves
				sistemaWave(&waves, &aguardandoWave, &tempoProximaWave, inimigo, largura_tela);

				if (fonte == NULL) {
					printf("Erro ao carregar fonte\n");
					return -1;
				}
				if (principal.vida < 0) {
					principal.vida = 0;
				}
				



				//desenhos apos al_clear_to_color
				al_clear_to_color(al_map_rgb(194, 191, 29));
			

				


				//fundo do jogo
				al_draw_scaled_bitmap(chaoImg, 0, 0, al_get_bitmap_width(chaoImg), al_get_bitmap_height(chaoImg), 0, 0, largura_tela, altura_tela, 0);
				//

				// texto da wave
				al_draw_textf(fonte, al_map_rgb(255, 0, 0), largura_tela / 2 - 50, 20, 0, "Wave: %d", waves);
				// texto da vida do personagem
				al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 0, "Vida: %d", principal.vida);
				al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 40, 0, "Pontos: %d", pontos);
				al_draw_textf(fonte, al_map_rgb(255, 255, 255), largura_tela/2 - 50, altura_tela - 30, 0, "Record: %d", record);
				al_draw_textf(fonte, al_map_rgb(255, 0, 0), 10, 80, 0, "Tab -> laboratorio");
				

				//mensagem da receita
				if (mostrarReceita == true)
				{
					al_draw_text(fonte, al_map_rgb(0, 255, 0), largura_tela - 130, 120, 0, "+1 Receita!!");
				}


				if (principal.vida > 0) {
					principal.ativo = true;

					if (principal.noAr == true) {
						int totalFramesPulo = 4;

						int larguraFramePulo = al_get_bitmap_width(personagemJump) / totalFramesPulo;

						int alturaFramePulo = al_get_bitmap_height(personagemJump);

						al_draw_scaled_bitmap(personagemJump, principal.framePulo * larguraFramePulo, 0, larguraFramePulo, alturaFramePulo, principal.xi, principal.yi, principal.xf - principal.xi, principal.yf - principal.yi, principal.virou ? ALLEGRO_FLIP_HORIZONTAL : 0
						);

					}
					else {
						int larguraFrameWalk = al_get_bitmap_width(personagemImg) / 12;

						int alturaFrameWalk = al_get_bitmap_height(personagemImg);

						al_draw_scaled_bitmap(personagemImg, principal.frameWalk * larguraFrameWalk, 0, larguraFrameWalk, alturaFrameWalk, principal.xi, principal.yi, principal.xf - principal.xi, principal.yf - principal.yi, principal.virou ? ALLEGRO_FLIP_HORIZONTAL : 0);
					}

				}
				else {
					principal.ativo = false;
				}

				//função projetil float *balaX, float *balaY, bool *balaAtiva, float xf, float yi, float yf, int largura_tela, bool *mouseAnterior, bool *mouseAtual
				projetil(&balaX, &balaY, &balaAtiva, principal.xi, principal.xf, principal.yi, principal.yf, largura_tela, &mouseAnterior, &principal.virou, &balaVirou, &corBala, &corProjetil, principal.ativo, misturaFeita);

				for (int i = 0; i < quantidadeInimigos; i++) {
					colisaoBala(&balaX, &balaY, &balaAtiva, &balaVirou, &corProjetil, &inimigo[i], &pontos);
				}



				//inimigos
				for (int i = 0; i < quantidadeInimigos; i++) {

					int flip = 0;

					if (inimigo[i].x > principal.xf) {
						flip = ALLEGRO_FLIP_HORIZONTAL;
					}

					if (inimigo[i].vida > 0 && inimigo[i].ativo == true) {
						if (inimigo[i].cor == 0) {
							al_draw_bitmap_region(inimigoNatural, framesNaturalX[inimigo[i].frame_atual], framesNaturalY[inimigo[i].frame_atual], 30, 51, inimigo[i].x, inimigo[i].y - 35,flip);
							al_draw_filled_circle(inimigo[i].x + 10, inimigo[i].y - 55, inimigo[i].raio - 20, al_map_rgb(0, 0, 255));//azul
						}
						else if (inimigo[i].cor == 1) {
							al_draw_bitmap_region(inimigoMetal, framesMetalX[inimigo[i].frame_atual], framesMetalY[inimigo[i].frame_atual], 65, 51, inimigo[i].x, inimigo[i].y - 35, flip);
							al_draw_filled_circle(inimigo[i].x + 20, inimigo[i].y - 55, inimigo[i].raio - 20, al_map_rgb(255, 255, 255));//branco
						}
						else {
							al_draw_bitmap_region(inimigoLeve, framesLeveX[inimigo[i].frame_atual], framesLeveY[inimigo[i].frame_atual], 30, 51, inimigo[i].x, inimigo[i].y - 35, flip);
							al_draw_filled_circle(inimigo[i].x + 10, inimigo[i].y - 55, inimigo[i].raio - 20, al_map_rgb(255, 255, 0));//amarelo
						}
					}
				}
				if (principal.vida <= 0) {
					al_draw_textf(fonte, al_map_rgb(255, 0, 0), largura_tela / 2 - 50, altura_tela / 2 - 20, 0, "GAME OVER");
					al_draw_textf(fonte, al_map_rgb(255, 0, 0), largura_tela - 300, 10, 0, "Pressione ESC para sair");

					if (al_key_down(&teclado, ALLEGRO_KEY_ESCAPE)) {
						menu = true;
						laboratorio = false;
						jogo = false;

					}
				}
				//escolha de elemento
				if (misturaFeita >= 1) {//160, 400, 260, 460
					al_draw_scaled_bitmap(elementoLeve, 46, 50, 159, 155, 180, 410, 50, 50, 0);

				}
				if (misturaFeita >= 2) { //400, 400, 500, 460
					al_draw_scaled_bitmap(elementoPesado, 46, 50, 159, 155, 420, 410, 50, 50, 0);
				}
				if (misturaFeita >= 3) {//280, 400, 380, 460

					al_draw_scaled_bitmap(elementoNatural, 18, 20, 180, 165, 300, 410, 50, 50, 0);

				}


				//texto cor da bala com as teclas E, R e Q
				//leve - amarelo - E 160, 400, 260, 460
				//metais - branco - Q 400, 400, 500, 460
				//Naturais - azul - R 280, 400, 380, 460
				al_draw_textf(fonte, al_map_rgb(255, 255, 0), 230, 405, 0, "E");
				
				if (misturaFeita >= 2) {
					al_draw_textf(fonte, al_map_rgb(255, 255, 255), 470, 405, 0, "Q");
				}
				if (misturaFeita >= 3) {
					al_draw_textf(fonte, al_map_rgb(0, 0, 255), 350, 405, 0, "R");
				}


			}

			else if (laboratorio == true && menu == false && jogo == false)//laboratorio rodando
			{
				ALLEGRO_MOUSE_STATE mouse;
				al_get_mouse_state(&mouse);

				laboratorio_atualizar(&mouse, &menu, &laboratorio, &jogo, &teclado, &rodandoJogo);
				
				selectionElemento(elementos, &mouse, &bloco1, &bloco2, &mouseAnterior);

				

				laboratorio_desenhar(fonte, largura_tela, altura_tela, laboratorioIMG, elementos, &bloco1, &bloco2, &mouse, elementoLeve, elementoPesado, receita, &receitaV, &prontoPJogo, &misturas, &misturaFeita, elementoNatural, &reacao);



	
			}

		}
		al_flip_display();
	}

	al_destroy_bitmap(elementoLeve);
	al_destroy_bitmap(elementoNatural);
	al_destroy_bitmap(inimigoLeve);
	al_destroy_bitmap(inimigoMetal);
	al_destroy_bitmap(inimigoNatural);
	al_destroy_bitmap(receita);
	al_destroy_bitmap(elementoPesado);
	al_destroy_bitmap(laboratorioIMG);
	al_destroy_bitmap(chaoImg);
	al_destroy_bitmap(personagemImg);
	al_destroy_event_queue(filaEventos);
	al_destroy_timer(timer);
	al_destroy_bitmap(menuImg);
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_destroy_display(display);

	return 0;
}