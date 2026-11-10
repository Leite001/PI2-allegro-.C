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
	int frame;
	int contadorFrame;
}Personagem;

void movimentacao(Personagem* p, int largura_tela) {

	al_install_keyboard();
	ALLEGRO_KEYBOARD_STATE teclado;
	al_get_keyboard_state(&teclado);

	//variaveis
	p->velocidade = 5.0f;
	float gravidade = 0.2f;
	//

//idle - parado
	if (p->andando == false) {
		p->frame = 0;
		p->contadorFrame = 0;

	}


	// Movimentação para a esquerda
	if (al_key_down(&teclado, ALLEGRO_KEY_LEFT) && p->ativo == true) {
		p->xi -= p->velocidade;
		p->xf -= p->velocidade;
		p->virou = true;
		p->andando = true;

		p->contadorFrame++;

		if (p->contadorFrame >= 5) {
			p->frame++;
			p->contadorFrame = 0;
		}
	}

	// Movimentação para a direita
	else if (al_key_down(&teclado, ALLEGRO_KEY_RIGHT) && p->ativo == true) {
		p->xi += p->velocidade;
		p->xf += p->velocidade;
		p->virou = false;
		p->andando = true;

		p->contadorFrame++;

		if (p->contadorFrame >= 5) {
			p->frame++;
			p->contadorFrame = 0;
		}
	}
	else {
		p->andando = false;
	}

	// Limita os frames
	if (p->frame >= 12) {
		p->frame = 0;
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

	// Pulo
	if (al_key_down(&teclado, ALLEGRO_KEY_UP) && p->noChao == true && p->ativo == true && p->noAr == false) {
		p->velocidadePulo = -6.0f;
		p->noChao = false;
	}

	p->velocidadePulo += gravidade;
	//gravidade
	p->yi += p->velocidadePulo;
	p->yf += p->velocidadePulo;

	// Colisão com o chão
	if (p->yf >= 400) {
		p->yf = 400;
		p->yi = p->yf - 80;
		p->velocidadePulo = 0.0f;
		p->noChao = true;
	}
}

void  projetil(float* balaX, float* balaY, bool* balaAtiva, float xi, float xf, float yi, float yf, int largura_tela, bool* mouseAnterior, bool* virou, bool* balaVirou, bool ativoP) {

	al_install_mouse();
	ALLEGRO_MOUSE_STATE mouse;
	al_get_mouse_state(&mouse);

	bool mouseAtual = al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT);

	float velobala = 8.0f;

	// Detecta um novo clique
	if (mouseAtual && !*mouseAnterior && !*balaAtiva && ativoP == true) {
		*balaVirou = *virou;

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

		al_draw_filled_rectangle(
			*balaX, *balaY - 5,
			*balaX + 20, *balaY + 5,
			al_map_rgb(0, 255, 0)
		);//projetil desenhado direita
	}
	else
	{
		if (*balaAtiva && *balaVirou == true && ativoP == true) {
			*balaX -= velobala;
			al_draw_filled_rectangle(
				*balaX, *balaY - 5,
				*balaX - 20, *balaY + 5,
				al_map_rgb(0, 255, 0)
			);//projetil desenhado esquerda
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

void colisaoBala(float* balaX, float* balaY, bool* balaAtiva, bool* balaVirou, inimigo* inimigo) {

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
		float inimigoEsquerda = inimigo->x - inimigo->raio;
		float inimigoDireita = inimigo->x + inimigo->raio;

		// Verifica colisão
		if (balaDireita >= inimigoEsquerda && balaEsquerda <= inimigoDireita) {
			inimigo->vida -= 1;
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
			inimigo->x = p->xi - inimigo->raio;
		else
			inimigo->x = p->xf + inimigo->raio;
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
}

void sistemaWave(int* waves, bool* aguardandoWave, double* tempoProximaWave, inimigo inimigo[], int largura_tela) {
	int inimigosVivos = 0;

	int quantidadeInimigos = 3 + (*waves - 1) / 5;

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

			if (i % 3 == 0) inimigo[i].velocidade = 1.5f;
			if (i % 3 == 1) inimigo[i].velocidade = 1.0f;
			if (i % 3 == 2) inimigo[i].velocidade = 2.5f;

			inimigo[i].y = 370;

			if (rand() % 2 == 0) {
				inimigo[i].x = -inimigo[i].raio;
			}
			else {
				inimigo[i].x = largura_tela + inimigo[i].raio;
			}
		}

		*aguardandoWave = false;
	}
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
	ALLEGRO_BITMAP* menuImg = al_load_bitmap("Imagens/MenuImagem.png");
	ALLEGRO_BITMAP* personagemImg = al_load_bitmap("Imagens/skeleton-walk.png");
	//dimensões do personagem
	float largura_total = al_get_bitmap_width(personagemImg);
	float altura_total = al_get_bitmap_height(personagemImg);
	float largura_frame = largura_total / 12; // 12 frames na horizontal	

	Personagem principal = { 320.0f, 450.0f, 300.0f, 500.0f, 0.03f, 0.0f, true, false, true, false, false, 1, 30, 0, 0};
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
	bool rodando = true;
	ALLEGRO_EVENT_QUEUE* filaEventos = al_create_event_queue();
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	//50 e 80
	//

	//registro de eventos
	al_register_event_source(filaEventos, al_get_display_event_source(display));
	//al_register_event_source(filaEventos, al_get_keyboard_event_source());
	//al_register_event_source(filaEventos, al_get_mouse_event_source());
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
		{-30, 370,30,1.5f,true, 5, 3}, // azul
		{-40, 370,30,1.0f, true, 5, 3}, // branco
		{-50, 370,30,2.5f, true, 5, 3}  // amarelo
	};
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

//jogo rodando
	while (rodando) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(filaEventos, &evento);

		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			rodando = false;
		}

		if (evento.type == ALLEGRO_EVENT_TIMER) {
			al_get_keyboard_state(&teclado);
			al_get_mouse_state(&mouse);

			if (menu == true && jogo == false && laboratorio == false) // menu rodando
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_scaled_bitmap(menuImg, 0, 0, al_get_bitmap_width(menuImg), al_get_bitmap_height(menuImg), 0, 0, largura_tela, altura_tela, 0);
				al_draw_rectangle(340, 259, 520, 299, al_map_rgb(255, 0, 0), 5);
				al_draw_rectangle(340, 365, 520, 420, al_map_rgb(255, 0, 0), 5);
				if (al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse.x >= 340 && mouse.x <= 520 && mouse.y >= 259 && mouse.y <= 299) {
					principal.vida = 30;
					principal.ativo = true;
					principal.xi = 320.0f;
					principal.xf = 450.0f;
					principal.yi = 300.0f;
					principal.yf = 500.0f;
					principal.noChao = true;
					principal.noAr = false;
					principal.virou = false;
					principal.andando = false;
					principal.frame = 0;
					principal.contadorFrame = 0;

					waves = 0;
					aguardandoWave = false;
					tempoProximaWave = 0;

					balaAtiva = false;
					balaVirou = false;

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
				}
				else if (al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT) && mouse.x >= 340 && mouse.x <= 520 && mouse.y >= 365 && mouse.y <= 420) {
					laboratorio = true;
					jogo = false;
					menu = false;

				}
			}
			else if (menu == false && jogo == true && laboratorio == false)// jogo rodando
			{

				//chamando a função
				movimentacao(&principal, largura_tela);
				// Calcula o centro horizontal do jogador
				float jogadorCentroX = (principal.xi + principal.xf) / 2;
				int quantidadeInimigos = 3 + (waves - 1) / 5;

				if (quantidadeInimigos > 20) quantidadeInimigos = 20;

				//chamada para inimigos
				for (int i = 0; i < quantidadeInimigos; i++) {
					perseguir_jogador(&inimigo[i], jogadorCentroX, principal.ativo);
					colisaoPrincipal(&principal, &inimigo[i]);
					colisaoBala(&balaX, &balaY, &balaAtiva, &balaVirou, &inimigo[i]);
				}

				//função waves
				sistemaWave(&waves, &aguardandoWave, &tempoProximaWave, inimigo, largura_tela);



				//desenhos apos al_clear_to_color
				al_clear_to_color(al_map_rgb(194, 191, 29));

				if (fonte == NULL) {
					printf("Erro ao carregar fonte\n");
					return -1;
				}
				if (principal.vida < 0) {
					principal.vida = 0;
				}

				// texto da wave
				al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 40, 0, "Wave: %d", waves);
				// texto da vida do personagem
				al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 0, "Vida: %d", principal.vida);

				/*
				if (personagemImg == NULL) {
					printf("Erro ao carregar sprite!\n");
				}
				else {
					printf("Sprite carregada: %d x %d\n",
						al_get_bitmap_width(personagemImg),
						al_get_bitmap_height(personagemImg));
				}
				*/
				
				if (principal.vida > 0) {
					principal.ativo = true;

					int total_colunas = 12;
					int largura_frame = 2170 / total_colunas; // ~180 pixels
					int altura_frame = 500;

					// Usamos al_draw_scaled_bitmap para pegar APENAS O PRIMEIRO FRAME 
					// e redimensioná-lo para caber exatamente no tamanho do seu personagem (xi, yi até xf, yf)
					al_draw_scaled_bitmap(
						personagemImg,
						principal.frame* largura_frame, 0,
						largura_frame, altura_frame,
						principal.xi, principal.yi,
						principal.xf - principal.xi,
						principal.yf - principal.yi,
						principal.virou ? ALLEGRO_FLIP_HORIZONTAL : 0
					);
				}
				else {
					principal.ativo = false;
				}

				//função projetil float *balaX, float *balaY, bool *balaAtiva, float xf, float yi, float yf, int largura_tela, bool *mouseAnterior, bool *mouseAtual
				projetil(&balaX, &balaY, &balaAtiva, principal.xi, principal.xf, principal.yi, principal.yf, largura_tela, &mouseAnterior, &principal.virou, &balaVirou, principal.ativo);

				//inimigos
				for (int i = 0; i < quantidadeInimigos; i++) {
					if (inimigo[i].vida > 0 && inimigo[i].ativo == true) {
						if (i % 3 == 0) {
							al_draw_filled_circle(inimigo[i].x, inimigo[i].y, inimigo[i].raio, al_map_rgb(0, 0, 255));
						}
						else if (i % 3 == 1) {
							al_draw_filled_circle(inimigo[i].x, inimigo[i].y, inimigo[i].raio, al_map_rgb(255, 255, 255));
						}
						else {
							al_draw_filled_circle(inimigo[i].x, inimigo[i].y, inimigo[i].raio, al_map_rgb(255, 255, 0));
						}
					}
				}

				al_draw_filled_rectangle(0, 400, largura_tela, altura_tela, al_map_rgb(0, 0, 0));//chao

				if (principal.vida <= 0) {
					al_draw_textf(fonte, al_map_rgb(255, 0, 0), largura_tela / 2 - 50, altura_tela / 2 - 20, 0, "GAME OVER");
					al_draw_textf(fonte, al_map_rgb(255, 0, 0), largura_tela - 300, 10, 0, "Pressione ESC para sair");

					if (al_key_down(&teclado, ALLEGRO_KEY_ESCAPE)) {
						menu = true;
						laboratorio = false;
						jogo = false;

					}
				}

			}

			else if (laboratorio == true && menu == false && jogo == false)//laboratorio rodando
			{
				ALLEGRO_MOUSE_STATE mouse;
				al_get_mouse_state(&mouse);

				laboratorio_atualizar(&mouse, &menu, &laboratorio, &jogo);

				laboratorio_desenhar(fonte, largura_tela, altura_tela);
}

		}
		al_flip_display();
	}
	al_destroy_bitmap(personagemImg);
	al_destroy_event_queue(filaEventos);
	al_destroy_timer(timer);
	al_destroy_bitmap(menuImg);
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_destroy_display(display);

	return 0;
}