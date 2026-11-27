#include "reacoes.h"
#include <allegro5/allegro_primitives.h>

#define NUM_ELEMENTOS 6
#define NUM_RECEITAS 6

#define REC_HCL  0
#define REC_NAOH 1
#define REC_NACL 2
#define REC_H2O  3
#define REC_CO2  4
#define REC_SO2  5

void inicializar_sistema_quimico(SistemaQuimico* sq) {
    sq->qtd_H = 0;
    sq->qtd_Cl = 0;
    sq->qtd_Na = 0;
    sq->qtd_O = 0;
    sq->qtd_S = 0;
    sq->qtd_C = 0;
    sq->research_point = 0;

    // Arma inicial
    sq->arma_ativa = TIRO_PADRAO;

    // Aparência padrão
    sq->formato_ativo = FORMATO_CIRCULO;
    sq->cor_r = 255;
    sq->cor_g = 255;
    sq->cor_b = 255;
    sq->tamanho_ativo = 2;
    sq->arma_desbloqueada = TIRO_PADRAO;
    sq->tempo_mensagem_desbloqueio = 0;

    // Desbloqueios
    sq->acida_desbloqueada = false;
    sq->basica_desbloqueada = false;
    sq->metalica_desbloqueada = false;

    sq->mensagem_arma_bloqueada = false;
}

void atualizar_aparencia_arma(SistemaQuimico* sq) {
    switch (sq->arma_ativa) {

    case TIRO_PADRAO:
        sq->formato_ativo = FORMATO_CIRCULO;
        sq->cor_r = 255;
        sq->cor_g = 255;
        sq->cor_b = 255;
        sq->tamanho_ativo = 2;
        break;

    case ARMA_ACIDA:
        sq->formato_ativo = FORMATO_TRIANGULO;
        sq->cor_r = 80;
        sq->cor_g = 255;
        sq->cor_b = 80;
        sq->tamanho_ativo = 5;
        break;

    case ARMA_BASICA:
        sq->formato_ativo = FORMATO_QUADRADO;
        sq->cor_r = 80;
        sq->cor_g = 180;
        sq->cor_b = 255;
        sq->tamanho_ativo = 7;
        break;

    case ARMA_METALICA:
        sq->formato_ativo = FORMATO_CIRCULO;
        sq->cor_r = 190;
        sq->cor_g = 190;
        sq->cor_b = 210;
        sq->tamanho_ativo = 3;
        break;
    }
}

bool selecionar_arma(SistemaQuimico* sq, TipoArma arma) {
    bool desbloqueada = false;

    switch (arma) {

    case TIRO_PADRAO:
        desbloqueada = true;
        break;

    case ARMA_ACIDA:
        desbloqueada = sq->acida_desbloqueada;
        break;

    case ARMA_BASICA:
        desbloqueada = sq->basica_desbloqueada;
        break;

    case ARMA_METALICA:
        desbloqueada = sq->metalica_desbloqueada;
        break;
    }

    if (desbloqueada) {
        sq->arma_ativa = arma;
        sq->mensagem_arma_bloqueada = false;

        atualizar_aparencia_arma(sq);

        return true;
    }

    sq->mensagem_arma_bloqueada = true;

    return false;
}

typedef struct {
    const char* formula;
    int req[NUM_ELEMENTOS];   // ordem: H, Cl, Na, O, S, C
} Receita;

static const Receita receitas[NUM_RECEITAS] = {
    { "HCl",  { 1, 1, 0, 0, 0, 0 } },
    { "NaOH", { 1, 0, 1, 1, 0, 0 } },
    { "NaCl", { 0, 1, 1, 0, 0, 0 } },
    { "H2O",  { 2, 0, 0, 1, 0, 0 } },
    { "CO2",  { 0, 0, 0, 2, 0, 1 } },
    { "SO2",  { 0, 0, 0, 2, 1, 0 } }
};

const char* simbolo_elemento(Elemento e) {
    switch (e) {
    case ELEM_H:  return "H";
    case ELEM_CL: return "Cl";
    case ELEM_NA: return "Na";
    case ELEM_O:  return "O";
    case ELEM_S:  return "S";
    case ELEM_C:  return "C";
    default:      return "";
    }
}

static int* ptr_elemento(SistemaQuimico* sq, int e) {
    switch (e) {
    case ELEM_H:  return &sq->qtd_H;
    case ELEM_CL: return &sq->qtd_Cl;
    case ELEM_NA: return &sq->qtd_Na;
    case ELEM_O:  return &sq->qtd_O;
    case ELEM_S:  return &sq->qtd_S;
    default:      return &sq->qtd_C;
    }
}

static bool pode_fazer(SistemaQuimico* sq, int r) {
    for (int e = 0; e < NUM_ELEMENTOS; e++) {
        if (*ptr_elemento(sq, e) < receitas[r].req[e]) return false;
    }
    return true;
}

static void consumir(SistemaQuimico* sq, int r) {
    for (int e = 0; e < NUM_ELEMENTOS; e++) {
        *ptr_elemento(sq, e) -= receitas[r].req[e];
    }
}

// 1ª vez desbloqueia a arma (pop-up); depois vira reforço de +3 RP
static void sintetizar_arma(SistemaQuimico* sq, int r, bool* desbloqueada, TipoArma arma) {
    consumir(sq, r);

    if (!*desbloqueada) {
        *desbloqueada = true;
        sq->arma_desbloqueada = arma;
        sq->tempo_mensagem_desbloqueio = al_get_time();
    }
    else {
        sq->research_point += 3;
    }
}

// Verifica as combinações químicas (uma reação por chamada)
void checar_reacoes(SistemaQuimico* sq, Player* player) {

    if (pode_fazer(sq, REC_HCL)) {
        sintetizar_arma(sq, REC_HCL, &sq->acida_desbloqueada, ARMA_ACIDA);
    }
    else if (pode_fazer(sq, REC_NAOH)) {
        sintetizar_arma(sq, REC_NAOH, &sq->basica_desbloqueada, ARMA_BASICA);
    }
    else if (pode_fazer(sq, REC_NACL)) {
        sintetizar_arma(sq, REC_NACL, &sq->metalica_desbloqueada, ARMA_METALICA);
    }
    else if (pode_fazer(sq, REC_H2O)) {
        consumir(sq, REC_H2O);
        player->vida += 1;
    }
    else if (pode_fazer(sq, REC_CO2)) {
        consumir(sq, REC_CO2);
        player->dano += 1;
    }
    else if (pode_fazer(sq, REC_SO2)) {
        consumir(sq, REC_SO2);
        player->vel += 1;
    }
}
void desenhar_progresso_receitas(SistemaQuimico* sq, ALLEGRO_FONT* fonte, int x, int y) {
    ALLEGRO_COLOR verde = al_map_rgb(80, 255, 80);
    ALLEGRO_COLOR cinza = al_map_rgb(200, 200, 200);

    for (int r = 0; r < NUM_RECEITAS; r++) {
        int pos_y = y + r * 16;
        int coluna = 0;
        bool completa = true;

        // 1) descobre se a receita já está completa
        for (int e = 0; e < NUM_ELEMENTOS; e++) {
            if (*ptr_elemento(sq, e) < receitas[r].req[e]) {
                completa = false;
            }
        }

        ALLEGRO_COLOR cor = cinza;
        if (completa == true) {
            cor = verde;
        }

        // 2) escreve a fórmula
        al_draw_textf(fonte, cor, x, pos_y, ALLEGRO_ALIGN_LEFT, "%s:", receitas[r].formula);

        // 3) escreve cada elemento da receita, um por coluna
        for (int e = 0; e < NUM_ELEMENTOS; e++) {
            int precisa = receitas[r].req[e];
            if (precisa == 0) {
                continue;
            }

            int tem = *ptr_elemento(sq, e);
            if (tem > precisa) {
                tem = precisa;
            }

            int pos_x = x + 42 + coluna * 56;

            if (coluna > 0) {
                al_draw_text(fonte, cor, pos_x - 9, pos_y, ALLEGRO_ALIGN_LEFT, "|");
            }

            al_draw_textf(fonte, cor, pos_x, pos_y, ALLEGRO_ALIGN_LEFT, "%s %d/%d", simbolo_elemento((Elemento)e), tem, precisa);
            coluna++;
        }
    }
}

//dados de cada item da loja
static const ItemLab pool_fixo[3] = {
    {0, "Neutralizacao (Cura)", "HCl + NaOH -> NaCl + H2O", 10, 0},
    {1, "Sintese HCl (Dano+)", "H2 + Cl2 -> 2 HCl", 15, 0},
    {2, "Oxidacao Fe (Vel+)", "4Fe + 3O2 -> 2Fe2O3", 12, 0}
};

void init_itens_lab(ItemLab* item) {
    if (item == NULL) return;

    //copia os dados do banco estático para o array passado por parâmetro
    for (int i = 0; i < 3; i++) {
        item[i].ID_item = pool_fixo[i].ID_item;
        item[i].nome = pool_fixo[i].nome;
        item[i].desc = pool_fixo[i].desc;
        item[i].valor = pool_fixo[i].valor;
        item[i].contador_compra = pool_fixo[i].contador_compra;
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
        al_draw_text(fonte, al_map_rgb(255, 255, 255), posX + (tamanho_caixa / 2.0f), posY - 45, ALLEGRO_ALIGN_CENTER, item[i].nome);
        al_draw_text(fonte, al_map_rgb(200, 200, 200), posX + (tamanho_caixa / 2.0f), posY - 20, ALLEGRO_ALIGN_CENTER, item[i].desc);

        //preço dos itens (muda para vermelho se o player não tiver rp suficiente)
        ALLEGRO_COLOR cor_preco = (sq->research_point >= item[i].valor) ? al_map_rgb(255, 255, 50) : al_map_rgb(255, 50, 50);
        al_draw_textf(fonte, cor_preco, posX + (tamanho_caixa / 2.0f), posY + tamanho_caixa + 10, ALLEGRO_ALIGN_CENTER, "%d RP", item[i].valor);
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
        //verifica se o jogador tem pontinhos suficiente
        if (sq->research_point >= itens[index_item].valor) {

            //reduz o custo do item do saldo do player
            sq->research_point -= itens[index_item].valor;

            itens[index_item].contador_compra++;

            //aplica o efeito no player baseado no ID do item comprado
            switch (itens[index_item].ID_item) {
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
            player->x = width / 1.6f - 10;
            player->y = height - 110;
        }
    }
}