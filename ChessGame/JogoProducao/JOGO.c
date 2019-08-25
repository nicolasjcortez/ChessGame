/***************************************************************************
 
 *  $MCI Módulo de implementação: JOG  Jogo de xadrez
 
 *
 
 *  Arquivo gerado:              JOGO.C
 
 *  Letras identificadoras:      JOG
 
 
 
 *  Autores: Bernardo Lobato Ferreira de Souza
 
 *
 
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "jogo.h"
#include "tabuleiro.h"
#include "lista.h"

#define VAZIO 'V'
#define TORRE 'T'
#define CAVALO 'C'
#define BISPO 'B'
#define DAMA 'D'
#define REI 'R'
#define PEAO 'P'
#define corBRANCA 'B'
#define corPRETA 'P'


struct tagMensagem {
    char texto[300];
};

typedef struct tagMensagem JOG_Mensagem;

static int uSys = 0;

JOG_Mensagem * msgSistema[20];

/***** Protótipos das funções encapuladas no módulo *****/

JOG_Mensagem * JOG_criaMsg(char texto[90]);

void JOG_insereMsgSistema(JOG_Mensagem * texto);

void JOG_organizaSistema(int pos);

void JOG_zeraMensagem();

void JOG_imprimeMsgSistema(int pos);

JOG_tpCondRet JOG_imprimeMensagem(int pos);

tpTabuleiro * JOG_CriaEInicializaTabuleiro(tpTabuleiro * t);

void JOG_InicializarTabuleiro(tpTabuleiro * t);

char JOG_mudarJogador(char jogador);

tppPeca  buscarRei(tpTabuleiro * t, char cor);

JOG_tpCondRet JOG_verificaCheque(tpTabuleiro * t, char cor);

JOG_tpCondRet JOG_verificaChequeMate(tpTabuleiro * t, char cor);

void JOG_copiaTabuleiro(tpTabuleiro * t_origem, tpTabuleiro * t_destino);

int  JOG_podeImpedirCheque(tpTabuleiro * t, tppPeca rei);

JOG_tpCondRet JOG_podeSairCaminhando(tpTabuleiro * t, LIS_tppLista listaAmeacadosRei, char corRei);

int JOG_ehProtegida(tpTabuleiro * t, tppPeca * peca);

int JOG_bloquearAtaque(tpTabuleiro * t, tppPeca rei) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Cria Msg
 
 *
 
 *  $ED Descrição da função
 
 *     Cria e aloca memoria para mensagem.
 
 *
 
 *  $EP Parâmetros
 
 *     texto[] - vetor de char.
 
 *
 
 *  $FV Valor retornado
 
 *     JOG_Mensagem
 
 *
 
 ***********************************************************************/

JOG_Mensagem * JOG_criaMsg(char texto[300])
{
    JOG_Mensagem * novaMsg = NULL;
    novaMsg = (JOG_Mensagem *) malloc(sizeof(JOG_Mensagem));
    if (novaMsg == NULL) {
        printf("Erro ao alocar.");
        return  NULL;
    }
    strcpy(novaMsg->texto, texto);
    return novaMsg;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Insere Msg Sistema
 
 *
 
 *  $ED Descrição da função
 
 *     Insere uma mensagem na heap.
 
 *
 
 *  $EP Parâmetros
 
 *     texto - um ponteiro para mensagem.
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/

void JOG_insereMsgSistema(JOG_Mensagem * texto)
{
    msgSistema[uSys] = texto;
    JOG_organizaSistema(uSys);
    uSys++;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Organiza Sistema
 
 *
 
 *  $ED Descrição da função
 
 *     Organiza a heap de mensagens.
 
 *
 
 *  $EP Parâmetros
 
 *     pos - um inteiro passando a posicao inicial.
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/

void JOG_organizaSistema(int pos)
{
    while (pos > 0) {
        int pai = (pos -1) / 2;
        if (strcmp(msgSistema[pos]->texto, msgSistema[pai]->texto) < 0) {
            JOG_Mensagem * aux;
            aux = msgSistema[pai];
            msgSistema[pai] = msgSistema[pos];
            msgSistema[pos] = aux;
            pos = pai;
        }
        else {
            break;
        }
    }
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Zera Mensagem
 
 *
 
 *  $ED Descrição da função
 
 *     Zera a heap de mensagem.
 
 *
 
 *  $EP Parâmetros
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/

void JOG_zeraMensagem()
{
    uSys = 0;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Imprime Msg Sistema
 
 *
 
 *  $ED Descrição da função
 
 *     Imprime na tela as mensagens recursivamente.
 
 *
 
 *  $EP Parâmetros
 
 *     pos - um int que define a posicao inicial.
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/

void JOG_imprimeMsgSistema(int pos) {
    if (pos >= uSys || pos > 20) {
        return;
    }
    printf("%*c %s\n", 2, '-', msgSistema[pos]->texto);
    JOG_imprimeMsgSistema((pos * 2) + 1);
    JOG_imprimeMsgSistema((pos * 2) + 2);
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Imprime Mensagem
 
 *
 
 *  $ED Descrição da função
 
 *     Imprime o painel de mensagens.
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *
 
 *  $FV Valor retornado
 
 *     JOG_CondRet
 
 *
 
 ***********************************************************************/


JOG_tpCondRet JOG_imprimeMensagem(int pos) {
    if (msgSistema[0]->texto == NULL) {
        return JOG_CondRetOK;
    }
    if(!uSys){
        return JOG_CondRetOK;
    }
    /* Imprime as linhas superiores */
    for (int i = 0; i < 64; i++) {
        printf("%c", '*');
    }
    printf("\n");
    for (int i = 0; i < 64; i++) {
        printf("%c", '*');
    }
    printf("\n");
    printf("\n");
    printf("              *** Mensagem *** \n");
    
    if (uSys != 0 ) {
        printf("Juiz: \n");
        JOG_imprimeMsgSistema(pos);
        printf("\n");
    }
    /* Imprime as linhas inferiores */
    for (int i = 0; i < 64; i++) {
        printf("%c", '*');
    }
    printf("\n");
    for (int i = 0; i < 64; i++) {
        printf("%c", '*');
    }
    printf("\n");
    JOG_zeraMensagem();
    printf("\n");
    
    return JOG_CondRetOK;
}



/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Cria E Inicializa Tabuleiro
 
 *
 
 *  $ED Descrição da função
 
 *     Verifica se  tabuleiro já foi inicializado e criado.
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *
 
 *  $FV Valor retornado
 
 *     tpTabuleiro
 
 *
 
 ***********************************************************************/


tpTabuleiro * JOG_CriaEInicializaTabuleiro(tpTabuleiro * t)
{
    if (t == NULL)
    {
        t = TAB_CriarTabuleiro();
        JOG_InicializarTabuleiro(t);
        //JOG_InicializarTabuleiroEmCheque(t);
        JOG_insereMsgSistema(JOG_criaMsg("Jogo Iniciado"));
        return t;
    }
    return t;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Inicializar Tabuleiro
 
 *
 
 *  $ED Descrição da função
 
 *     Arruma as pecas no Tabuleiro para que o jogo seja iniciado.
 
 *
 
 *  $EP Parâmetros
 
 *     t - Um ponteiro para tabuleiro.
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/


void JOG_InicializarTabuleiro(tpTabuleiro * t)
{
    /* Instanciar um ponteiro para Peca */
    tppPeca * p = TAB_CriarPeca();
    
    /* Inicializar Pecas Pretas */
    TAB_SetTipo(p, TORRE);
    TAB_SetCor(p, corPRETA);
    TAB_InserirPeca(t, 0, 0, p);
    
    TAB_SetTipo(p, CAVALO);
    TAB_SetCor(p, corPRETA);
    TAB_InserirPeca(t, 0, 1, p);
    
    TAB_SetTipo(p, BISPO);
    TAB_SetCor(p, corPRETA);
    TAB_InserirPeca(t, 0, 2, p);
    
    TAB_SetTipo(p, DAMA);
    TAB_SetCor(p, corPRETA);
    TAB_InserirPeca(t, 0, 3, p);
    
    TAB_SetTipo(p, REI);
    TAB_SetCor(p, corPRETA);
    TAB_InserirPeca(t, 0, 4, p);
    
    TAB_SetTipo(p, BISPO);
    TAB_SetCor(p, corPRETA);
    TAB_InserirPeca(t, 0, 5, p);
    
    TAB_SetTipo(p, CAVALO);
    TAB_SetCor(p, corPRETA);
    TAB_InserirPeca(t, 0, 6, p);
    
    TAB_SetTipo(p, TORRE);
    TAB_SetCor(p, corPRETA);
    TAB_InserirPeca(t, 0, 7, p);
    
    for (int i = 0; i < 8; i++)
    {
        TAB_SetTipo(p, PEAO);
        TAB_SetCor(p, corPRETA);
        TAB_InserirPeca(t, 1, i, p);
    }
    
    /*inicializar Pecas Brancas */
    TAB_SetTipo(p, TORRE);
    TAB_SetCor(p, corBRANCA);
    TAB_InserirPeca(t, 7, 0, p);
    
    TAB_SetTipo(p, CAVALO);
    TAB_SetCor(p, corBRANCA);
    TAB_InserirPeca(t, 7, 1, p);
    
    TAB_SetTipo(p, BISPO);
    TAB_SetCor(p, corBRANCA);
    TAB_InserirPeca(t, 7, 2, p);
    
    TAB_SetTipo(p, DAMA);
    TAB_SetCor(p, corBRANCA);;
    TAB_InserirPeca(t, 7, 3, p);
    
    TAB_SetTipo(p, REI);
    TAB_SetCor(p, corBRANCA);
    TAB_InserirPeca(t, 7, 4, p);
    
    TAB_SetTipo(p, BISPO);
    TAB_SetCor(p, corBRANCA);
    TAB_InserirPeca(t, 7, 5, p);
    
    TAB_SetTipo(p, CAVALO);
    TAB_SetCor(p, corBRANCA);
    TAB_InserirPeca(t, 7, 6, p);
    
    TAB_SetTipo(p, TORRE);
    TAB_SetCor(p, corBRANCA);
    TAB_InserirPeca(t, 7, 7, p);
    
    for (int i = 0; i < 8; i++)
    {
        TAB_SetTipo(p, PEAO);
        TAB_SetCor(p, corBRANCA);
        TAB_InserirPeca(t, 6, i, p);
    }
    
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Mudar Jogador
 
 *
 
 *  $ED Descrição da função
 
 *     Alterna o jogador da rodada
 
 *
 
 *  $EP Parâmetros
 
 *     jogador - um char que representa o jogador.
 
 *
 
 *  $FV Valor retornado
 
 *     char - corBRANCA caso seja jogador das pecas brancas.
 
 *     char - corPRETA caso seja jogador das pecas pretas.
 
 *
 
 ***********************************************************************/


char JOG_mudarJogador(char jogador)
{
    if (jogador == corBRANCA)
    {
        return corPRETA;
    }
    return corBRANCA;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Buscar Rei
 
 *
 
 *  $ED Descrição da função
 
 *     buscar a peca do rei no tabuleiro
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *     cor - um char que representa a cor.
 
 *
 
 *  $FV Valor retornado
 
 *     tppPeca
 
 *
 
 ***********************************************************************/

tppPeca  buscarRei(tpTabuleiro * t, char cor)
{
    int achou = 0;
    tppPeca pecaProcurada;
    pecaProcurada.tipo = VAZIO;
    pecaProcurada.cor = VAZIO;
    
    /* Percorrer o tabuleiro procurando pelo REI */
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (cor == TAB_ObterPeca(t, i, j).cor && REI == TAB_ObterPeca(t, i, j).tipo) {
                pecaProcurada = TAB_ObterPeca(t, i, j);
                achou = 1;
                break;
            }
        }
        if (achou) {
            break;
        }
    }
    
    return pecaProcurada;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Verifica Cheque
 
 *
 
 *  $ED Descrição da função
 
 *     Verifica a existe cheque.
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *     cor - um char que representa a cor.
 
 *
 
 *  $FV Valor retornado
 
 *     JOG_CondRetOK
 
 *     JOG_CondRetCheque
 
 *     JOG_CondeRetJogoInconsistente
 
 *
 
 ***********************************************************************/


JOG_tpCondRet JOG_verificaCheque(tpTabuleiro * t, char cor)
{
    
    tppPeca rei = buscarRei(t, cor);
    if (rei.tipo == VAZIO) {
        return JOG_CondRetJogoInconsistente;
    }
    LIS_tppLista lAmeacantesRei = TAB_ObterListaAmeacantes(t, rei.linhaCorrente, rei.colunaCorrente);
    if (LIS_GetNumElem(lAmeacantesRei) != 0) {
        /* Cheque */
        return JOG_CondRetCheque;
    }
    return JOG_CondRetOK;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Verifica Cheque
 
 *
 
 *  $ED Descrição da função
 
 *     Verifica a existe cheque-mate.
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *     cor - um char que representa a cor.
 
 *
 
 *  $FV Valor retornado
 
 *     JOG_CondRetOK
 
 *     JOG_CondRetChequeMate
 
 *     JOG_CondRetJogoInconsistente
 
 *
 
 ***********************************************************************/

JOG_tpCondRet JOG_verificaChequeMate(tpTabuleiro * t, char cor)
{
    tppPeca rei = buscarRei(t, cor);
    if (rei.tipo == VAZIO) {
        return JOG_CondRetJogoInconsistente;
    }
    LIS_tppLista listaAmeacadosRei = TAB_ObterListaAmeacados(t, rei.linhaCorrente, rei.colunaCorrente);
    if (JOG_verificaCheque(t, cor) == JOG_CondRetCheque) {
        if (LIS_GetNumElem(listaAmeacadosRei) != 0) {
            if (JOG_podeSairCaminhando(t, listaAmeacadosRei, rei.cor) == JOG_CondRetOK) {
                return JOG_CondRetOK;
            }
            //destruir quem deu cheque
            if (JOG_podeImpedirCheque(t, rei)) {
                return JOG_CondRetOK;
            }
            if (JOG_bloquearAtaque(t,rei)) {
                return JOG_CondRetOK;
            }
            return JOG_CondRetChequeMate;
        } else {
            /* Cavalo da cheque */
            //verificar se o cavalo pode ser destruido
            if (!JOG_podeImpedirCheque(t, rei)) {
                return JOG_CondRetChequeMate;
            }

        }
    }
    return JOG_CondRetOK;
}



/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Copia Tabuleiro
 
 *
 
 *  $ED Descrição da função
 
 *     copia o estado do tabuleiro anterior.
 
 *
 
 *  $EP Parâmetros
 
 *     t_origem - um ponteiro para tabuleiro inicial.
 
 *     t_destino - um ponteiro para tabuleiro da copia.
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/

void JOG_copiaTabuleiro(tpTabuleiro * t_origem, tpTabuleiro * t_destino) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            //pegar peca t_origem[i][j]
            tppPeca p = TAB_ObterPeca(t_origem, i, j);
            //copiar todos atributos da peca t_origem[i][j] para t_destino[i][j]
            TAB_InserirPeca(t_destino, i, j, &p);
            //pegar lista de ameacantes t_origem[i][j] - A
            LIS_tppLista listaAmeacantesOrigem = TAB_ObterListaAmeacantes(t_origem, i, j);
            //esvaziar a lista de ameacantes t_destino[i][j] - B
            LIS_tppLista listaAmeacantesDestino = TAB_ObterListaAmeacantes(t_destino, i, j);
            LIS_EsvaziarLista(listaAmeacantesDestino);
            //percorrer lista de amecantes A inserindo elementos em B
            int s = LIS_GetNumElem(listaAmeacantesOrigem);
            LIS_IrInicioLista(listaAmeacantesOrigem);
            for (int k = 1; k <= s; k++) {
                tppPeca * pecaListaOrigem = (tppPeca *) LIS_ObterValor(listaAmeacantesOrigem);
                tppPeca * novaPeca = TAB_CriarPeca();
                novaPeca->tipo = pecaListaOrigem->tipo;
                novaPeca->cor = pecaListaOrigem->cor;
                novaPeca->linhaCorrente = pecaListaOrigem->linhaCorrente;
                novaPeca->colunaCorrente = pecaListaOrigem->colunaCorrente;
                LIS_InserirElementoApos(listaAmeacantesDestino, novaPeca);\
                LIS_AvancarElementoCorrente(listaAmeacantesOrigem, 1);
            }
            //pegar lista de ameacados t_origem[i][j] - A
            LIS_tppLista listaAmeacadosOrigem = TAB_ObterListaAmeacados(t_origem, i, j);
            //esvaziar a lista de ameacados t_destino[i][j] - B
            LIS_tppLista listaAmeacadosDestino = TAB_ObterListaAmeacados(t_destino, i, j);
            LIS_EsvaziarLista(listaAmeacadosDestino);
            //percorrer lista de amecados A inserindo elementos em B
            s = LIS_GetNumElem(listaAmeacadosOrigem);
            LIS_IrInicioLista(listaAmeacadosOrigem);
            for (int k = 1; k <= s; k++) {
                tppPeca * pecaListaOrigem = (tppPeca *) LIS_ObterValor(listaAmeacadosOrigem);
                tppPeca * novaPeca = TAB_CriarPeca();
                novaPeca->tipo = pecaListaOrigem->tipo;
                novaPeca->cor = pecaListaOrigem->cor;
                novaPeca->linhaCorrente = pecaListaOrigem->linhaCorrente;
                novaPeca->colunaCorrente = pecaListaOrigem->colunaCorrente;
                LIS_InserirElementoApos(listaAmeacadosDestino, novaPeca);
                LIS_AvancarElementoCorrente(listaAmeacadosOrigem, 1);
            }

            
        }
    }
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Eh Protegida
 
 *
 
 *  $ED Descrição da função
 
 *     descobrir se a ameacante do REI eh protegida por outra peca.
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *     peca - um ponteiro para peca.
 
 *
 
 *  $FV Valor retornado
 
 *     int
 
 *
 
 ***********************************************************************/

int JOG_ehProtegida(tpTabuleiro * t, tppPeca * peca)
{
    int linha = peca->linhaCorrente;
    int coluna = peca->colunaCorrente;
    
    for (int i = 1; i < 8; i++) {
        /* Linha superior esquerda */
        if (linha - i >= 0 && coluna - i >= 0) {
            tppPeca p = TAB_ObterPeca(t, linha - i, coluna - i);
            if (p.cor == peca->cor && (p.tipo == BISPO || p.tipo == DAMA)) {
                return 1;
            }
            if (p.cor != VAZIO) {
                break;
            }
        } else {
            break;
        }
    }
    for (int i = 1; i < 8; i++) {
        /* Linnha superior */
        if (linha - i >= 0) {
            tppPeca p = TAB_ObterPeca(t, linha - i, coluna);
            if (p.cor == peca->cor && (p.tipo == TORRE || p.tipo == DAMA)) {
                return 1;
            }
            if (p.cor != VAZIO) {
                break;
            }
        } else {
            break;
        }
        
    }
    for (int i = 1; i < 8; i++) {
        
        /* Linha superior Direita */
        if (linha - i >= 0 && coluna + i < 8) {
            tppPeca p = TAB_ObterPeca(t, linha - i, coluna + i);
            if (p.cor == peca->cor && (p.tipo == BISPO || p.tipo == DAMA)) {
                return 1;
            }if (p.cor != VAZIO) {
                break;
            }
        } else {
            break;
        }
    }
    for (int i = 1; i < 8; i++) {
        /* Linha inferior esquerda */
        if (linha + i < 8 && coluna - i >= 0) {
            tppPeca p = TAB_ObterPeca(t, linha + i, coluna - i);
            if (p.cor == peca->cor && (p.tipo == BISPO || p.tipo == DAMA)) {
                return 1;
            }if (p.cor != VAZIO) {
                break;
            }
        } else {
            break;
        }
    }
    for (int i = 1; i < 8; i++) {
        /* Linha inferior */
        if (linha + i < 8) {
            tppPeca p = TAB_ObterPeca(t, linha + i, coluna);
            if (p.cor == peca->cor && (p.tipo == TORRE || p.tipo == DAMA)) {
                return 1;
            }if (p.cor != VAZIO) {
                break;
            }
        } else {
            break;
        }
    }
    for (int i = 1; i < 8; i++) {
        /* Linha inferior direita */
        if (linha + i < 8 && coluna + i < 8) {
            tppPeca p = TAB_ObterPeca(t, linha + i, coluna + i);
            if (p.cor == peca->cor && (p.tipo == BISPO || p.tipo == DAMA)) {
                return 1;
            }if (p.cor != VAZIO) {
                break;
            }
        } else {
            break;
        }
    }
    for (int i = 1; i < 8; i++) {
        /* coluna esquerda */
        if (coluna - i >= 0) {
            tppPeca p = TAB_ObterPeca(t, linha, coluna - i);
            if (p.cor == peca->cor && (p.tipo == TORRE || p.tipo == DAMA)) {
                return 1;
            }if (p.cor != VAZIO) {
                break;
            }
        } else {
            break;
        }
    }
    for (int i = 1; i < 8; i++) {
        
        /* coluna direita */
        if (coluna + i < 8 ){
            tppPeca p = TAB_ObterPeca(t, linha, coluna + i);
            if (p.cor == peca->cor && (p.tipo == TORRE || p.tipo == DAMA)) {
                return 1;
            }if (p.cor != VAZIO) {
                break;
            }
        } else {
            break;
        }
    }/* Fim do for*/
    if (linha - 1 >= 0) {
        /* Cavalo superior esquerda abaixo */
        if (coluna - 2 >= 0) {
            tppPeca pp = TAB_ObterPeca(t, linha - 1, coluna - 2);
            if (pp.cor == peca->cor && pp.tipo == CAVALO) {
                return 1;
            }
        }
        /* Cavalo superior direita abaixo */
        if (coluna + 2 >= 0) {
            tppPeca pp = TAB_ObterPeca(t, linha - 1, coluna + 2);
            if (pp.cor == peca->cor && pp.tipo == CAVALO) {
                return 1;
            }
        }
    }
    if (linha + 1 < 8) {
        /* Peao inferior esquerda */
        if (coluna - 1 >= 0) {
            tppPeca pp = TAB_ObterPeca(t, linha + 1, coluna - 1);
            if (pp.cor == peca->cor && pp.tipo == PEAO) {
                return 1;
            }
        }
        /* Cavalo inferior esquerda acima */
        if (coluna - 2 >= 0) {
            tppPeca pp = TAB_ObterPeca(t, linha + 1, coluna - 2);
            if (pp.cor == peca->cor && pp.tipo == CAVALO) {
                return 1;
            }
        }
        /* Peao inferior direita */
        if (coluna + 1 < 8) {
            tppPeca pp = TAB_ObterPeca(t, linha + 1, coluna + 1);
            if (pp.cor == peca->cor && pp.tipo == PEAO) {
                return 1;
            }
        }
        /* Cavalo inferior direita acima */
        if (coluna + 2 >= 0) {
            tppPeca pp = TAB_ObterPeca(t, linha + 1, coluna + 2);
            if (pp.cor == peca->cor && pp.tipo == CAVALO) {
                return 1;
            }
        }
    }
    if (linha - 2 >= 0) {
        /* Cavalo superior esquerda acima */
        if (coluna - 1 >= 0) {
            tppPeca pp = TAB_ObterPeca(t, linha - 2, coluna - 1);
            if (pp.cor == peca->cor && pp.tipo == CAVALO) {
                return 1;
            }
        }
        /* Cavalo superior direita acima */
        if (coluna + 1 < 8) {
            tppPeca pp = TAB_ObterPeca(t, linha - 2, coluna + 1);
            if (pp.cor == peca->cor && pp.tipo == CAVALO) {
                return 1;
            }
        }
    }
    if (linha + 2 < 8) {
        /* Cavalo inferior esquerda abaixo */
        if (coluna - 1 >= 0) {
            tppPeca pp = TAB_ObterPeca(t, linha + 2, coluna - 1);
            if (pp.cor == peca->cor && pp.tipo == CAVALO) {
                return 1;
            }
        }
        /* Cavalo inferior direita abaixo */
        if (coluna + 1 < 8) {
            tppPeca pp = TAB_ObterPeca(t, linha + 2, coluna + 1);
            if (pp.cor == peca->cor && pp.tipo == CAVALO) {
                return 1;
            }
        }
    }
    if(linha + (peca->cor == corBRANCA ? 1: -1) < 8 && linha + (peca->cor == corBRANCA ? 1: -1) >= 0) {
        /* Peao esquerda */
        if (coluna - 1 >= 0) {
            tppPeca pp = TAB_ObterPeca(t, linha + (peca->cor == corBRANCA ? 1: -1), coluna - 1);
            if (pp.cor == peca->cor && pp.tipo == PEAO) {
                return 1;
            }
        }
        /* Peao direita */
        if (coluna + 1 < 8) {
            tppPeca pp = TAB_ObterPeca(t, linha + (peca->cor == corBRANCA ? 1: -1), coluna + 1);
            if (pp.cor == peca->cor && pp.tipo == PEAO) {
                return 1;
            }
        }
    }
    /* se é protegido pelo Rei */
    for (int i = linha - 1; i <= linha + 1; i++) {
        for (int j = coluna - 1; j <= coluna + 1; j++) {
            if (i < 8 && i >= 0 && j < 8 && j >= 0) {
                tppPeca pp = TAB_ObterPeca(t, i, j);
                if (pp.cor == peca->cor && pp.tipo == REI) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Bloqueia Ataque
 
 *
 
 *  $ED Descrição da função
 
 *     verifica se alguma peca amiga pode bloquear o cheque.
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *     rei - uma peca.
 
 *
 
 *  $FV Valor retornado
 
 *     int
 
 *
 
 ***********************************************************************/

int JOG_bloquearAtaque(tpTabuleiro * t, tppPeca rei) {
    LIS_tppLista listaAmeacantesRei = TAB_ObterListaAmeacantes(t, rei.linhaCorrente, rei.colunaCorrente);
    LIS_IrInicioLista(listaAmeacantesRei);
    tppPeca * p = (tppPeca *) LIS_ObterValor(listaAmeacantesRei);
    if (p == NULL) {
        return JOG_CondRetJogoInconsistente;
    }
    
    if(p->tipo==PEAO || p->tipo==CAVALO){
        return 0;
    }
    
    int inc_i = 1, inc_j=1;
    if(rei.colunaCorrente > p->colunaCorrente) {
        inc_j=-1;
    }
    if(rei.linhaCorrente>p->linhaCorrente) {
        inc_i=-1;
    }
    if(rei.colunaCorrente == p->colunaCorrente) {
        inc_j=0;
    }
    if(rei.linhaCorrente==p->linhaCorrente) {
        inc_i=0;
    }
    for (int i = rei.linhaCorrente+inc_i, j = rei.colunaCorrente+inc_j; (i != p->linhaCorrente && j!= p->colunaCorrente) || (i == p->linhaCorrente && j!= p->colunaCorrente) || (i != p->linhaCorrente && j== p->colunaCorrente); i+=inc_i, j+=inc_j) {
        LIS_tppLista listaAmeacantes = TAB_ObterListaAmeacantes(t, i,j);
        //saber se tem algum da cor do rei
        
        LIS_IrInicioLista(listaAmeacantes);
        int s = LIS_GetNumElem(listaAmeacantes);
        /* Verifica a lista de ameacados do REI */
        for (int i=0; i<s; i++) {
            tppPeca * peca = (tppPeca *) LIS_ObterValor(listaAmeacantes);
            if (peca->cor==rei.cor) {
                return 1;
            }
            LIS_AvancarElementoCorrente(listaAmeacantes, 1);
        }
    }
    return 0;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Pode Sair Caminhando
 
 *
 
 *  $ED Descrição da função
 
 *     verifica se o REI em cheque pode sair do cheque caminhando.
 
 *
 
 *  $EP Parâmetros
 
 *     t_origem - um ponteiro para tabuleiro inicial.
 
 *     t_destino - um ponteiro para tabuleiro da copia.
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/

/* Verifica se o Rei pode sair caminhando apos o cheque */
JOG_tpCondRet JOG_podeSairCaminhando(tpTabuleiro * t, LIS_tppLista listaAmeacadosRei, char corRei) {
    LIS_IrInicioLista(listaAmeacadosRei);
    int s = LIS_GetNumElem(listaAmeacadosRei);
    /* Verifica a lista de ameacados do REI */
    for (int i=0; i<s; i++) {
        tppPeca * peca = (tppPeca *) LIS_ObterValor(listaAmeacadosRei);
        /* Se for VAZIO verifica se esta ameacada pelo oponente */
        if (peca->tipo == VAZIO) {
            LIS_tppLista lista =  TAB_ObterListaAmeacantes(t, peca->linhaCorrente, peca->colunaCorrente);
            
            int ehPossivelIr = 1;
            
            LIS_IrInicioLista(lista);
            int s1 = LIS_GetNumElem(lista);
            for (int i=0; i<s1; i++) {
                tppPeca * p = (tppPeca *) LIS_ObterValor(lista);
                
                if (p->cor == corRei) { //se for amigo do rei
                    LIS_AvancarElementoCorrente(lista, 1);
                }else{ /* A casa vazia tem um ameacante que nao eh amigo do rei */
                    ehPossivelIr = 0;
                    break;
                }
                
            }
            LIS_IrInicioLista(lista);
            
            if(ehPossivelIr){
                return JOG_CondRetOK;
            }
        } else { /* Se tiver uma peca do oponente, verifica se eh protegida */
            if(!JOG_ehProtegida(t,peca)){
                return JOG_CondRetOK;
            }
        }
        LIS_AvancarElementoCorrente(listaAmeacadosRei, 1);
    }
    LIS_IrInicioLista(listaAmeacadosRei);
    
    return JOG_CondRetReiImovel;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Pode Impedir Cheque
 
 *
 
 *  $ED Descrição da função
 
 *     Verifica se a peca ameacdora do REI pode ser destruida.
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *     rei - uma peca do REI.
 
 *
 
 *  $FV Valor retornado
 
 *     int
 
 *
 
 ***********************************************************************/

int  JOG_podeImpedirCheque(tpTabuleiro * t, tppPeca rei) {
    LIS_tppLista listaAmeacantesRei = TAB_ObterListaAmeacantes(t, rei.linhaCorrente, rei.colunaCorrente);
    LIS_IrInicioLista(listaAmeacantesRei);
    tppPeca * p = (tppPeca *) LIS_ObterValor(listaAmeacantesRei);
    if (p == NULL) {
        return JOG_CondRetJogoInconsistente;
    }
    LIS_tppLista listaAmecantesPeca = TAB_ObterListaAmeacantes(t, p->linhaCorrente, p->colunaCorrente);
    LIS_IrInicioLista(listaAmecantesPeca);
    int s = LIS_GetNumElem(listaAmecantesPeca);
    tpTabuleiro * t_simulacao = TAB_CriarTabuleiro();
    JOG_copiaTabuleiro(t, t_simulacao);
    for (int i = 1; i <= s; i++) {
        tppPeca * peca = (tppPeca *)LIS_ObterValor(listaAmecantesPeca);
        if (TAB_MoverPeca(t_simulacao,peca->linhaCorrente , peca->colunaCorrente, p->linhaCorrente, p->colunaCorrente) == TAB_CondRetOK) {
            if (JOG_verificaCheque(t_simulacao, rei.cor) == JOG_CondRetCheque) {
                JOG_copiaTabuleiro(t, t_simulacao);
            } else {
                return 1;
            }
        }
    }

    return 0;
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Imprimir Lista
 
 *
 
 *  $ED Descrição da função
 
 *     imprime a lista de ameacados e ameacantes da casa destino.
 
 *
 
 *  $EP Parâmetros
 
 *     t - um ponteiro para tabuleiro.
 
 *     linhaDestino - um int representando linha.
 
 *     colunaDestino - um int representando coluna.
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/


void JOG_imprimeLista(tpTabuleiro * t, int linhaDestino, int colunaDestino)
{
    char printAmeacantes[300];
    char printAmeacados[300];
    sprintf(printAmeacantes, "Lista de Ameacantes [%c%d]: ", colunaDestino + 'A', linhaDestino + 1);
    sprintf(printAmeacados, "Lista de Ameacados [%c%d]: ", colunaDestino + 'A', linhaDestino + 1);
    LIS_tppLista listaAmeacantes = TAB_ObterListaAmeacantes(t, linhaDestino, colunaDestino);
    LIS_tppLista listaAmeacados = TAB_ObterListaAmeacados(t, linhaDestino, colunaDestino);
    int nAmecantes = LIS_GetNumElem(listaAmeacantes);
    int nAmeacados = LIS_GetNumElem(listaAmeacados);
    LIS_IrInicioLista(listaAmeacantes);
    LIS_IrInicioLista(listaAmeacados);
    for (int i = 0; i < nAmecantes; i++) {
        char str[11];
        tppPeca * p = (tppPeca *) LIS_ObterValor(listaAmeacantes);
        sprintf(str,"[%c%c - %c%d] ", p->tipo, p->cor, p->colunaCorrente + 'A', p->linhaCorrente + 1);
        //sprintf(str, "[VV - 00] ");
        //sprintf(printAmeacantes, "%s%c", printAmeacantes, str[i - 1]);
        strcpy(printAmeacantes + i * 10 + 27, str);
        LIS_AvancarElementoCorrente(listaAmeacantes, 1);
    }
    for (int i = 0; i < nAmeacados; i++) {
        char str[11];
        tppPeca * p = (tppPeca *) LIS_ObterValor(listaAmeacados);
        sprintf(str, "[%c%c - %c%d] ", p->tipo, p->cor, p->colunaCorrente + 'A', p->linhaCorrente + 1);
        //sprintf(str, "[VV - 00] ");
        //sprintf(printAmeacados, "%s%c", printAmeacados, str[i - 1]);
        strcpy(printAmeacados + i * 10 + 25, str);
        LIS_AvancarElementoCorrente(listaAmeacados, 1);
    }
    if (nAmecantes == 0) {
        sprintf(printAmeacantes, "%s%s", printAmeacantes, "VAZIO");
    }
    if (nAmeacados == 0) {
        sprintf(printAmeacados, "%s%s", printAmeacados, "VAZIO");
    }
    if (nAmeacados != 0 || nAmecantes != 0) {
        JOG_insereMsgSistema(JOG_criaMsg(printAmeacantes));
        JOG_insereMsgSistema(JOG_criaMsg(printAmeacados));
    }
}

/***********************************************************************
 
 *
 
 *  $FC Função: JOG  &Executa
 
 *
 
 *  $ED Descrição da função
 
 *     Executa a logica do jogo.
 
 *
 
 *  $EP Parâmetros
 
 *     t - Um ponteiro para tabuleiro.
 
 *
 
 *  $FV Valor retornado
 
 *     void
 
 *
 
 ***********************************************************************/


void JOG_executa(tpTabuleiro * t)
{
    tpTabuleiro * t_backup = TAB_CriarTabuleiro();
    t = JOG_CriaEInicializaTabuleiro(t);
    static char * clear = "cls"; /*[OSX / LINUX]: "clear" | [MSDOS / WINDOWS]: "cls"*/
    static char jogador = corBRANCA;
    int JOG_CondRetOK = 1;
    char colunaOrigem;
    int linhaOrigem;
    char colunaDestino;
    int linhaDestino;
    static int i = 1;
    char print[280];
    while (JOG_CondRetOK)
    {
        printf(" \n\n*****************************<<[ JOGADA %d ]>>*****************************\n\n", i);
        
        JOG_imprimeMensagem(0);
        TAB_ImprimeTabuleiro(t);
        /* INICIO TESTE*/
        //LIS_debugLista(t);
        /* FIM TESTE */
        printf("\nVez do jogador [%c]\n", jogador);
        printf("\nEntre com os valores das posicoes de Origem e Destino: (Ex: A1B2)\n");
        
        scanf(" %c", &colunaOrigem);
        scanf(" %d", &linhaOrigem);
        scanf(" %c", &colunaDestino);
        scanf(" %d", &linhaDestino);
        
        tppPeca  p = TAB_ObterPeca(t, linhaOrigem - 1, toupper(colunaOrigem) - 'A');
        
        if (jogador != TAB_GetCor(&p))
        {
            system(clear);
            sprintf(print, "Movimento Invalido");
            JOG_insereMsgSistema(JOG_criaMsg(print));
            sprintf(print, "O movimento eh do jogador [%c]", jogador);
            JOG_insereMsgSistema(JOG_criaMsg(print));
            JOG_executa(t);
        }
        
        JOG_copiaTabuleiro(t, t_backup);
        TAB_tpCondRet mudarOK;
        mudarOK = TAB_MoverPeca(t, linhaOrigem - 1, toupper(colunaOrigem) - 'A', linhaDestino - 1, toupper(colunaDestino) - 'A');
        if (mudarOK == TAB_CondRetOK) {
            /* Verificar cheque descoberto */
            if (JOG_verificaCheque(t, jogador) == JOG_CondRetCheque) {
                JOG_copiaTabuleiro(t_backup, t);
                system(clear);
                JOG_insereMsgSistema(JOG_criaMsg("Em Cheque"));
                JOG_insereMsgSistema(JOG_criaMsg("Movimento Invalido"));
                continue;
            }
            /*Verificar Cheque Mate*/
            jogador = JOG_mudarJogador(jogador);
            if (JOG_verificaChequeMate(t, jogador) == JOG_CondRetChequeMate) {
                JOG_CondRetOK = 0;
                //system(clear);
                jogador = JOG_mudarJogador(jogador);
                JOG_insereMsgSistema(JOG_criaMsg("Cheque-Mate"));
                sprintf(print, "Jogo Terminou, o Jogador %c venceu", jogador);
                JOG_insereMsgSistema(JOG_criaMsg(print));
                JOG_imprimeMsgSistema(0);
                break;
                
            }
            JOG_imprimeLista(t, linhaDestino - 1, toupper(colunaDestino) - 'A');
            i++;
            system(clear);
            
        } else {
            sprintf(print, "Movimento Invalido");
            JOG_insereMsgSistema(JOG_criaMsg(print));
        }
        system(clear);
    }/*end whiile */
    return;
}
