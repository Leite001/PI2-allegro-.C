
#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<stdbool.h>
#include<allegro5/keyboard.h>
#include<allegro5/mouse.h>
#include<allegro5/timer.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include<stdlib.h>
#include<time.h>
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
} inimigo;

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
} Personagem;


// MOVIMENTAÇÃO

void movimentacao(Personagem* p, int largura_tela) {

    ALLEGRO_KEYBOARD_STATE teclado;
    al_get_keyboard_state(&teclado);

    p->velocidade = 5.0f;
    float gravidade = 0.2f;

    // Movimentação para esquerda
    if (al_key_down(&teclado, ALLEGRO_KEY_A) && p->ativo) {

        p->xi -= p->velocidade;
        p->xf -= p->velocidade;

        p->virou = true;
        p->andando = true;

    }

    // Movimentação para direita
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

        if (p->contadorFrameWalk >= 5) {

            p->frameWalk++;
            p->contadorFrameWalk = 0;

        }
    }
    else if (!p->andando) {

        p->frameWalk = 0;
        p->contadorFrameWalk = 0;

    }

    // Limite de frames da caminhada
    if (p->frameWalk >= 12) {
        p->frameWalk = 0;
    }

    // Limite da tela
    if (p->xi < 0) {

        p->xi = 0;
        p->xf = 50;

    }

    if (p->xf > largura_tela) {

        p->xf = largura_tela;
        p->xi = largura_tela - 50;

    }

    // Pulo
    if (al_key_down(&teclado, ALLEGRO_KEY_W) &&
        p->noChao &&
        p->ativo &&
        !p->noAr) {

        p->velocidadePulo = -6.45f;

        p->noChao = false;
        p->noAr = true;

    }

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

    // Gravidade
    p->velocidadePulo += gravidade;

    p->yi += p->velocidadePulo;
    p->yf += p->velocidadePulo;

    // Colisão com chão
    if (p->yf >= 400) {

        p->yf = 400;
        p->yi = p->yf - 80;

        p->velocidadePulo = 0.0f;

        p->noChao = true;
        p->noAr = false;

    }
}


// PROJETIL

void projetil(
    float* balaX,
    float* balaY,
    bool* balaAtiva,
    float xi,
    float xf,
    float yi,
    float yf,
    int largura_tela,
    bool* mouseAnterior,
    bool* virou,
    bool* balaVirou,
    int* corBala,
    int* corProjetil,
    bool ativoP
) {

    ALLEGRO_MOUSE_STATE mouse;
    al_get_mouse_state(&mouse);

    ALLEGRO_KEYBOARD_STATE teclado;
    al_get_keyboard_state(&teclado);

    bool mouseAtual = al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT);

    float velobala = 8.0f;

    // Escolha da cor
    if (al_key_down(&teclado, ALLEGRO_KEY_E)) {
        *corBala = 2; // amarelo
    }
    else if (al_key_down(&teclado, ALLEGRO_KEY_R)) {
        *corBala = 0; // azul
    }
    else if (al_key_down(&teclado, ALLEGRO_KEY_Q)) {
        *corBala = 1; // branco
    }

    // Disparo
    if (mouseAtual && !*mouseAnterior && !*balaAtiva && ativoP) {

        *balaVirou = *virou;

        *corProjetil = *corBala;

        if (!*balaVirou) {
            *balaX = xf;
        }
        else {
            *balaX = xi;
        }

        *balaY = yi + (yf - yi) / 2;

        *balaAtiva = true;

    }

    *mouseAnterior = mouseAtual;

    // Movimento da bala
    if (*balaAtiva && ativoP) {

        if (!*balaVirou) {
            *balaX += velobala;
        }
        else {
            *balaX -= velobala;
        }

        // Cor da bala
        ALLEGRO_COLOR cor;

        if (*corProjetil == 0) {
            cor = al_map_rgb(0, 0, 255);
        }
        else if (*corProjetil == 1) {
            cor = al_map_rgb(255, 255, 255);
        }
        else {
            cor = al_map_rgb(255, 255, 0);
        }

        // Desenho da bala
        if (!*balaVirou) {

            al_draw_filled_rectangle(
                *balaX,
                *balaY - 5,
                *balaX + 20,
                *balaY + 5,
                cor
            );

        }
        else {

            al_draw_filled_rectangle(
                *balaX - 20,
                *balaY - 5,
                *balaX,
                *balaY + 5,
                cor
            );

        }

    }

    // Desativa quando sair da tela
    if (*balaX > largura_tela || *balaX < 0) {
        *balaAtiva = false;
    }

}


// COLISAO BALA

void colisaoBala(
    float* balaX,
    float* balaY,
    bool* balaAtiva,
    bool* balaVirou,
    int* corProjetil,
    inimigo* inimigo
) {

    if (*balaAtiva && inimigo->ativo) {

        float balaEsquerda;
        float balaDireita;

        // Limites horizontais
        if (!*balaVirou) {

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

        float inimigoTopo = inimigo->y - inimigo->raio;
        float inimigoBaixo = inimigo->y + inimigo->raio;

        // Colisão horizontal e vertical
        if (balaDireita >= inimigoEsquerda &&
            balaEsquerda <= inimigoDireita &&
            balaBaixo >= inimigoTopo &&
            balaTopo <= inimigoBaixo) {

            // Só causa dano se as cores forem iguais
            if (*corProjetil == inimigo->cor) {
                inimigo->vida -= 1;
            }

            // A bala desaparece após atingir
            *balaAtiva = false;

        }

    }

    // Morte do inimigo
    if (inimigo->vida <= 0) {
        inimigo->ativo = false;
    }

}


// COLISAO PERSONAGEM

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

    // Distância
    float dx = inimigo->x - pontoX;
    float dy = inimigo->y - pontoY;

    float distancia = dx * dx + dy * dy;

    // Colisão
    if (distancia < inimigo->raio * inimigo->raio &&
        p->yf > inimigo->y - inimigo->raio) {

        // Pisando no inimigo
        if (p->yf < inimigo->y) {
            return;
        }

        // Dano
        p->vida -= inimigo->dano;

        // Empurra inimigo
        if (inimigo->x < (p->xi + p->xf) / 2) {
            inimigo->x = p->xi - inimigo->raio;
        }
        else {
            inimigo->x = p->xf + inimigo->raio;
        }

    }

}


// PERSEGUIR JOGADOR

void perseguir_jogador(inimigo* inimigo, float jogadorX, bool jogadorAtivo) {

    if (!inimigo->ativo || !jogadorAtivo) {
        return;
    }

    if (inimigo->x < jogadorX) {
        inimigo->x += inimigo->velocidade;
    }
    else if (inimigo->x > jogadorX) {
        inimigo->x -= inimigo->velocidade;
    }

}


// SISTEMA WAVE

void sistemaWave(
    int* waves,
    bool* aguardandoWave,
    double* tempoProximaWave,
    inimigo inimigo[],
    int largura_tela
) {

    int inimigosVivos = 0;

    int quantidadeInimigos = 3 + (*waves - 1) / 5;

    if (quantidadeInimigos > 20) {
        quantidadeInimigos = 20;
    }

    // Conta inimigos vivos
    for (int i = 0; i < quantidadeInimigos; i++) {

        if (inimigo[i].ativo && inimigo[i].vida > 0) {
            inimigosVivos++;
        }

    }

    // Inicia espera
    if (inimigosVivos == 0 && !*aguardandoWave) {

        *aguardandoWave = true;
        *tempoProximaWave = al_get_time() + 5;

    }

    // Próxima wave
    if (*aguardandoWave && al_get_time() >= *tempoProximaWave) {

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

            // Cor
            inimigo[i].cor = i % 3;

            // Velocidade
            if (i % 3 == 0) {
                inimigo[i].velocidade = 1.5f;
            }
            else if (i % 3 == 1) {
                inimigo[i].velocidade = 1.0f;
            }
            else {
                inimigo[i].velocidade = 2.5f;
            }

            inimigo[i].y = 370;

            // Spawn aleatório
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


// MAIN

int main() {

    // Inicialização
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();

    // Display
    int altura_tela = 500;
    int largura_tela = 860;

    ALLEGRO_DISPLAY* display = al_create_display(largura_tela, altura_tela);

    if (display == NULL) {
        return -1;
    }

    al_set_window_position(display, 200, 200);

    // Imagens
    ALLEGRO_BITMAP* menuImg = al_load_bitmap("Imagens/MenuImagem.png");
    ALLEGRO_BITMAP* personagemImg = al_load_bitmap("Imagens/skeleton-walk.png");
    ALLEGRO_BITMAP* personagemJump = al_load_bitmap("Imagens/jump-skeleton.png");

    if (menuImg == NULL || personagemImg == NULL || personagemJump == NULL) {
        printf("Erro ao carregar imagens!\n");
        return -1;
    }

    // Fonte
    ALLEGRO_FONT* fonte = al_load_ttf_font(
        "C:\\Windows\\Fonts\\arial.ttf",
        24,
        0
    );

    if (fonte == NULL) {
        printf("Erro ao carregar fonte!\n");
        return -1;
    }

    // Personagem: 50x80
    Personagem principal = {
        320.0f, 370.0f,
        320.0f, 400.0f,
        5.0f, 0.0f,
        true, false, true, false, false,
        1, 30,
        0, 0, 0, 0
    };

    // Projétil
    float balaX = 0.0f;
    float balaY = 0.0f;

    bool balaAtiva = false;
    bool balaVirou = false;
    bool mouseAnterior = false;

    int corBala = 2;       // cor selecionada
    int corProjetil = 2;   // cor da bala disparada

    // Estados
    bool menu = true;
    bool laboratorio = false;
    bool jogo = false;
    bool rodando = true;

    // Eventos
    ALLEGRO_EVENT_QUEUE* filaEventos = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    al_register_event_source(
        filaEventos,
        al_get_display_event_source(display)
    );

    al_register_event_source(
        filaEventos,
        al_get_timer_event_source(timer)
    );

    al_start_timer(timer);

    // Waves
    int waves = 0;
    bool aguardandoWave = false;
    double tempoProximaWave = 0;

    // Inimigos
    inimigo inimigo[20] = { 0 };

    for (int i = 0; i < 3; i++) {

        inimigo[i].raio = 30;
        inimigo[i].velocidade = (i == 0) ? 1.5f : (i == 1) ? 1.0f : 2.5f;
        inimigo[i].ativo = true;
        inimigo[i].dano = 5;
        inimigo[i].vida = 3;
        inimigo[i].cor = i;
        inimigo[i].y = 370;

    }

    srand(time(NULL));

    for (int i = 0; i < 3; i++) {

        if (rand() % 2 == 0) {
            inimigo[i].x = -inimigo[i].raio;
        }
        else {
            inimigo[i].x = largura_tela + inimigo[i].raio;
        }

    }


    // JOGO
    while (rodando) {

        ALLEGRO_EVENT evento;
        al_wait_for_event(filaEventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }

        if (evento.type == ALLEGRO_EVENT_TIMER) {

            ALLEGRO_KEYBOARD_STATE teclado;
            ALLEGRO_MOUSE_STATE mouse;

            al_get_keyboard_state(&teclado);
            al_get_mouse_state(&mouse);


            // MENU
            if (menu && !jogo && !laboratorio) {

                al_clear_to_color(al_map_rgb(0, 0, 0));

                al_draw_scaled_bitmap(
                    menuImg,
                    0, 0,
                    al_get_bitmap_width(menuImg),
                    al_get_bitmap_height(menuImg),
                    0, 0,
                    largura_tela,
                    altura_tela,
                    0
                );

                al_draw_rectangle(
                    340, 259, 520, 299,
                    al_map_rgb(255, 0, 0), 5
                );

                al_draw_rectangle(
                    340, 365, 520, 420,
                    al_map_rgb(255, 0, 0), 5
                );

                // Iniciar jogo
                if (al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT) &&
                    mouse.x >= 340 && mouse.x <= 520 &&
                    mouse.y >= 259 && mouse.y <= 299) {

                    principal.vida = 30;
                    principal.ativo = true;

                    principal.xi = 320;
                    principal.xf = 370;
                    principal.yi = 320;
                    principal.yf = 400;

                    principal.noChao = true;
                    principal.noAr = false;
                    principal.virou = false;
                    principal.andando = false;

                    principal.velocidadePulo = 0;

                    principal.frameWalk = 0;
                    principal.contadorFrameWalk = 0;

                    principal.framePulo = 0;
                    principal.contadorFramePulo = 0;

                    waves = 0;
                    aguardandoWave = false;
                    tempoProximaWave = 0;

                    balaAtiva = false;
                    balaVirou = false;
                    mouseAnterior = false;

                    corBala = 2;
                    corProjetil = 2;

                    for (int i = 0; i < 20; i++) {
                        inimigo[i].vida = 0;
                        inimigo[i].ativo = false;
                    }

                    for (int i = 0; i < 3; i++) {

                        inimigo[i].vida = 3;
                        inimigo[i].ativo = true;
                        inimigo[i].cor = i;
                        inimigo[i].y = 370;

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

                // Laboratório
                else if (al_mouse_button_down(&mouse, ALLEGRO_MOUSE_BUTTON_LEFT) &&
                    mouse.x >= 340 && mouse.x <= 520 &&
                    mouse.y >= 365 && mouse.y <= 420) {

                    laboratorio = true;
                    jogo = false;
                    menu = false;

                }

            }


            // JOGO RODANDO
            else if (!menu && jogo && !laboratorio) {

                // Movimentação
                movimentacao(&principal, largura_tela);

                // Centro do jogador
                float jogadorCentroX = (principal.xi + principal.xf) / 2;

                int quantidadeInimigos = 3 + (waves - 1) / 5;

                if (quantidadeInimigos > 20) {
                    quantidadeInimigos = 20;
                }

                // Perseguição e colisão personagem
                for (int i = 0; i < quantidadeInimigos; i++) {

                    perseguir_jogador(
                        &inimigo[i],
                        jogadorCentroX,
                        principal.ativo
                    );

                    colisaoPrincipal(
                        &principal,
                        &inimigo[i]
                    );

                }

                // Limita vida
                if (principal.vida < 0) {
                    principal.vida = 0;
                }

                // Desenho
                al_clear_to_color(al_map_rgb(194, 191, 29));

                // Vida e wave
                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    10, 10, 0,
                    "Vida: %d",
                    principal.vida
                );

                al_draw_textf(
                    fonte,
                    al_map_rgb(255, 255, 255),
                    10, 40, 0,
                    "Wave: %d",
                    waves
                );

                // Personagem
                if (principal.vida > 0) {

                    principal.ativo = true;

                    if (principal.noAr) {

                        int totalFramesPulo = 4;

                        int larguraFramePulo =
                            al_get_bitmap_width(personagemJump) / totalFramesPulo;

                        int alturaFramePulo =
                            al_get_bitmap_height(personagemJump);

                        al_draw_scaled_bitmap(
                            personagemJump,
                            principal.framePulo * larguraFramePulo,
                            0,
                            larguraFramePulo,
                            alturaFramePulo,
                            principal.xi,
                            principal.yi,
                            principal.xf - principal.xi,
                            principal.yf - principal.yi,
                            principal.virou ? ALLEGRO_FLIP_HORIZONTAL : 0
                        );

                    }
                    else {

                        int larguraFrameWalk =
                            al_get_bitmap_width(personagemImg) / 12;

                        int alturaFrameWalk =
                            al_get_bitmap_height(personagemImg);

                        al_draw_scaled_bitmap(
                            personagemImg,
                            principal.frameWalk * larguraFrameWalk,
                            0,
                            larguraFrameWalk,
                            alturaFrameWalk,
                            principal.xi,
                            principal.yi,
                            principal.xf - principal.xi,
                            principal.yf - principal.yi,
                            principal.virou ? ALLEGRO_FLIP_HORIZONTAL : 0
                        );

                    }

                }
                else {
                    principal.ativo = false;
                    balaAtiva = false;
                }


                // Projétil
                projetil(
                    &balaX,
                    &balaY,
                    &balaAtiva,
                    principal.xi,
                    principal.xf,
                    principal.yi,
                    principal.yf,
                    largura_tela,
                    &mouseAnterior,
                    &principal.virou,
                    &balaVirou,
                    &corBala,
                    &corProjetil,
                    principal.ativo
                );


                // Colisão da bala
                for (int i = 0; i < quantidadeInimigos; i++) {

                    colisaoBala(
                        &balaX,
                        &balaY,
                        &balaAtiva,
                        &balaVirou,
                        &corProjetil,
                        &inimigo[i]
                    );

                }


                // Inimigos
                for (int i = 0; i < quantidadeInimigos; i++) {

                    if (inimigo[i].vida > 0 && inimigo[i].ativo) {

                        if (inimigo[i].cor == 0) {

                            al_draw_filled_circle(
                                inimigo[i].x,
                                inimigo[i].y,
                                inimigo[i].raio,
                                al_map_rgb(0, 0, 255)
                            );

                        }
                        else if (inimigo[i].cor == 1) {

                            al_draw_filled_circle(
                                inimigo[i].x,
                                inimigo[i].y,
                                inimigo[i].raio,
                                al_map_rgb(255, 255, 255)
                            );

                        }
                        else {

                            al_draw_filled_circle(
                                inimigo[i].x,
                                inimigo[i].y,
                                inimigo[i].raio,
                                al_map_rgb(255, 255, 0)
                            );

                        }

                    }

                }


                // Chão
                al_draw_filled_rectangle(
                    0, 400,
                    largura_tela, altura_tela,
                    al_map_rgb(0, 0, 0)
                );


                // Waves
                sistemaWave(
                    &waves,
                    &aguardandoWave,
                    &tempoProximaWave,
                    inimigo,
                    largura_tela
                );


                // Game Over
                if (principal.vida <= 0) {

                    al_draw_textf(
                        fonte,
                        al_map_rgb(255, 0, 0),
                        largura_tela / 2 - 50,
                        altura_tela / 2 - 20,
                        0,
                        "GAME OVER"
                    );

                    al_draw_textf(
                        fonte,
                        al_map_rgb(255, 0, 0),
                        largura_tela - 300,
                        10,
                        0,
                        "Pressione ESC para sair"
                    );

                    if (al_key_down(&teclado, ALLEGRO_KEY_ESCAPE)) {

                        menu = true;
                        laboratorio = false;
                        jogo = false;

                    }

                }

            }


            // LABORATÓRIO
            else if (laboratorio && !menu && !jogo) {

                laboratorio_atualizar(
                    &mouse,
                    &menu,
                    &laboratorio,
                    &jogo
                );

                laboratorio_desenhar(
                    fonte,
                    largura_tela,
                    altura_tela
                );

            }

        }

        al_flip_display();

    }


    // Finalização
    al_destroy_bitmap(personagemImg);
    al_destroy_bitmap(personagemJump);
    al_destroy_bitmap(menuImg);

    al_destroy_font(fonte);

    al_destroy_event_queue(filaEventos);
    al_destroy_timer(timer);

    al_destroy_display(display);

    al_uninstall_keyboard();
    al_uninstall_mouse();

    return 0;

}
