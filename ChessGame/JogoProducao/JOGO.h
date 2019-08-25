#if ! defined( JOGO_ )
#define JOGO_
/***************************************************************************
 *
 *  $MCD Módulo de definição: JOG  Tabuleiro de um jogo de xadrez
 *
 *  Arquivo gerado:              JOGO.h
 *  Letras identificadoras:      JOG
 *
 *  Autores: Bernardo Lobato Ferreira de Souza
 *
 *
 *  $ED Descrição do módulo
 *     Implementa um tabuleio para o jogo de xadrez
 *     O tabuleiro eh um ponteiro para uma matriz 8x8 de casas,
 *     um novo tipo que contem uma variavel do tipo peca e duas listas.
 *
 *     O modulo armazena funcoes para a manipulacao dos das destas casas
 *     citadas para a realizacao de um jogo de tabuleiro.
 *
 ***************************************************************************/

#if defined( JOGO_OWN )
#define JOGO_EXT
#else
#define JOGO_EXT extern
#endif

#include "tabuleiro.h"

/***********************************************************************
 *
 *  $TC Tipo de dados: JOG Condições de retorno
 *
 *
 *  $ED Descrição do tipo
 *     Condições de retorno das funções do Jogo
 *
 ***********************************************************************/


typedef enum {
    
    JOG_CondRetOK ,
    /* Concluiu corretamente */
    
    JOG_CondRetCheque,
    /* Cheque eh detectado */
    
    JOG_CondRetChequeMate,
    /* Cheque-Mate eh detectado */
    
    JOG_CondRetJogoInconsistente,
    /* Nao existe peca */
    
    JOG_CondRetReiImovel,
    /* Rei nao pode se mover */
    
    JOG_CondRetJogoIniciado,
    /* Inicia a partida */
    
    JOG_CondRetFimDeJogo
    /* Jogo Finalizado */
    
} JOG_tpCondRet ;


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

void JOG_executa(tpTabuleiro * t);

#undef JOGO_EXT

/********** Fim do módulo de definição: JOG  Jogo de xadrez  **********/

#else
#endif