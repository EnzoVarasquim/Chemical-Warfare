#include "reacoes.h"
#include <allegro5/allegro_primitives.h>

void inicializar_sistema_quimico(SistemaQuimico* sq) {
    sq->qtd_H = 0;
    sq->qtd_Cl = 0;
    sq->qtd_Na = 0;
    sq->research_point = 0;

    //aparência padrão
    sq->arma_ativa = TIRO_PADRAO;
    sq->formato_ativo = FORMATO_CIRCULO;
    sq->cor_r = 255;
    sq->cor_g = 255;
    sq->cor_b = 255;
    sq->tamanho_ativo = 2;
}

void checar_reacoes(SistemaQuimico* sq) {

    // ÁCIDA (HCl) - Triângulo verde
    if (sq->qtd_H >= 1 && sq->qtd_Cl >= 1) {
        sq->arma_ativa = ARMA_ACIDA;

        sq->formato_ativo = FORMATO_TRIANGULO;
        sq->cor_r = 80;
        sq->cor_g = 255;
        sq->cor_b = 80;
        sq->tamanho_ativo = 5;

        sq->qtd_H -= 1;
        sq->qtd_Cl -= 1;
    }

    // BÁSICA (NaOH) - Quadrado azul
    else if (sq->qtd_Na >= 1 && sq->qtd_H >= 1) {
        sq->arma_ativa = ARMA_BASICA;

        sq->formato_ativo = FORMATO_QUADRADO;
        sq->cor_r = 80;
        sq->cor_g = 180;
        sq->cor_b = 255;
        sq->tamanho_ativo = 7;

        sq->qtd_Na -= 1;
        sq->qtd_H -= 1;
    }

    // METÁLICA (NaCl) - Círculo prateado
    else if (sq->qtd_Na >= 2 && sq->qtd_Cl >= 1) {
        sq->arma_ativa = ARMA_METALICA;

        sq->formato_ativo = FORMATO_CIRCULO;
        sq->cor_r = 190;
        sq->cor_g = 190;
        sq->cor_b = 210;
        sq->tamanho_ativo = 3;

        sq->qtd_Na -= 2;
        sq->qtd_Cl -= 1;
    }

    // Sem reação: volta ao tiro padrão branco
    else {
        sq->arma_ativa = TIRO_PADRAO;
        sq->formato_ativo = FORMATO_CIRCULO;
        sq->cor_r = 255;
        sq->cor_g = 255;
        sq->cor_b = 255;
        sq->tamanho_ativo = 2;
    }
}

//dados de cada item da loja
static const ItemLab pool_fixo[3] = {
    {0, "Neutralizacao (Cura)", "HCl + NaOH -> NaCl + H2O", 10},
    {1, "Sintese HCl (Dano+)", "H2 + Cl2 -> 2 HCl", 15},
    {2, "Oxidacao Fe (Vel+)", "4Fe + 3O2 -> 2Fe2O3", 12}
};

void init_itens_lab(ItemLab* item) {
    if (item == NULL) return;

    //copia os dados do banco estático para o array passado por parâmetro
    for (int i = 0; i < 3; i++) {
        item[i].ID_item = pool_fixo[i].ID_item;
        item[i].nome = pool_fixo[i].nome;
        item[i].desc = pool_fixo[i].desc;
        item[i].valor = pool_fixo[i].valor;
    }
}

void desenhar_itens_lab(SistemaQuimico* sq, ItemLab* item, ALLEGRO_FONT* fonte) {
    if (item == NULL) return;

    for (int i = 0; i < 3; i++) {
        int posX = 370 + (i * 200);
        int posY = 240;
        int tamanho_caixa = 60;

        //desenha o Item (Quadrado com borda azul)
        al_draw_rectangle(posX, posY, posX + tamanho_caixa, posY + tamanho_caixa, al_map_rgb(0, 200, 255), 2);
        al_draw_filled_rectangle(posX + 2, posY + 2, posX + tamanho_caixa - 2, posY + tamanho_caixa - 2, al_map_rgb(25, 35, 45));

        //renderiza os textos de info do item
        al_draw_text(fonte, al_map_rgb(255, 255, 255), posX + (tamanho_caixa / 2), posY - 45, ALLEGRO_ALIGN_CENTER, item[i].nome);
        al_draw_text(fonte, al_map_rgb(200, 200, 200), posX + (tamanho_caixa / 2), posY - 20, ALLEGRO_ALIGN_CENTER, item[i].desc);

        //preço dos itens (muda para vermelho se o player não tiver rp suficiente)
        ALLEGRO_COLOR cor_preco = (sq->research_point >= item[i].valor) ? al_map_rgb(255, 255, 50) : al_map_rgb(255, 50, 50);
        al_draw_textf(fonte, cor_preco, posX + (tamanho_caixa / 2), posY + tamanho_caixa + 10, ALLEGRO_ALIGN_CENTER, "%d RP", item[i].valor);
    }
}

int verificar_colisao_loja(Player* player, int* item_posX, int* item_posY, int tamanho_caixa) {
    for (int i = 0; i < 3; i++) {
        //coordenadas de cada item
        int item_esquerda = item_posX[i];
        int item_direita = item_posX[i] + tamanho_caixa;
        int item_cima = item_posY[i];
        int item_baixo = item_posY[i] + tamanho_caixa;

        //coordenadas do Player
        int player_esquerda = player->x;
        int player_direita = player->x + player->size;
        int player_cima = player->y;
        int player_baixo = player->y + player->size;

        //checar colisao (coloquei em variavel pq fica mais facil de entender essa bagunça)
        if (player_direita > item_esquerda &&
            player_esquerda < item_direita &&
            player_baixo > item_cima &&
            player_cima < item_baixo) {
            return i; //retorna o índice do item colidido
        }
    }
    return -1; //retorna -1 se o player não estiver tocando em nenhum item :P
}

void computar_compra_item(SistemaQuimico* sq, Player* player, ItemLab* itens, int width, int height) {
    if (sq == NULL || player == NULL || itens == NULL || !player->vivo) return;

    //define as posições idênticas as usadas na função 'desenhar_itens_lab'
    int item_posX[3];
    int item_posY[3];
    int tamanho_caixa = 60;

    for (int i = 0; i < 3; i++) {
        item_posX[i] = 370 + (i * 200);
        item_posY[i] = 240;
    }

    //verifica se o player está colidindo com algum item agora
    int index_item = verificar_colisao_loja(player, item_posX, item_posY, tamanho_caixa);

    // Se houver colisão (index_item válido)
    if (index_item != -1) {
        ItemLab item_comprado = itens[index_item];

        //verifica se o jogador tem pontinhos suficiente
        if (sq->research_point >= item_comprado.valor) {

            //reduz o custo do item do saldo do player
            sq->research_point -= item_comprado.valor;

            //aplica o efeito no player baseado no ID do item comprado
            switch (item_comprado.ID_item) {
            case 0: // Neutralizacao (Cura)
                player->vida += 1; 
                break;

            case 1: // Sintese HCl (Dano+)
                player->dano += 1;
                break;

            case 2: // Oxidacao Fe (Vel+)
                player->vel += 1; 
                break;
            }

            //move o player para baixo  para não bugar
            player->x = width / 1.6 - 10;
            player->y = height - 110;
        }
    }
}