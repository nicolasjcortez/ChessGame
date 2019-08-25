/***************************************************************************
 *  $MCI Módulo de implementação: TAB  Tabuleiro de um jogo de xadrez
 *
 *  Arquivo gerado:              TABULEIRO.C
 *  Letras identificadoras:      TAB
 
 *  Autores: Nicolas Jordao Cortez
 *
 ***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include <stdlib.h>
//#include   <malloc.h>
#include   <assert.h>
#include   <math.h>
#include   "LISTA.h"

#ifdef _DEBUG
#include   "Generico.h"
#include   "Conta.h"
#include   "cespdin.h"
#include   "IdTiposEspacoTabuleiro.def"
#endif

#define TABULEIRO_OWN
#include "TABULEIRO.h"
#undef TABULEIRO_OWN

/***********************************************************************
 *
 *  $TC Tipo de dados: TAB Casa do tabuleiro
 *
 *
 ***********************************************************************/

typedef struct TAB_tagCasa
{
#ifdef _DEBUG
    struct TAB_tagTabuleiro* pCabeca ;
    
    /* Ponteiro para a cabeca do tabuleiro
     *
     *$ED Descrição
     *   Todos as casas do tabuleiro devem apontar para o respectivo tabueliro.
     *   Esse ponteiro corresponde a um identificador de tabuleiro para fins
     *   de verificação da integridade. */
    
    
    int i;
    /* redundancia para armazenar a linha da casa*/
    
    int j;
    /* redundancia para armazenar a coluna da casa*/
    
#endif
    
    
    
    tppPeca pecaDaCasa;
    /* peca da casa do tipo peca */
    
    LIS_tppLista listaDeAmeacados;
    /*  ponteio para a cabeca da lista de ameacados */
    
    LIS_tppLista listaDeAmeacantes;
    /*  ponteio para a cabeca da lista de ameacantes */
    
}tpCasa;

/***********************************************************************
 *
 *  $TC Tipo de dados: TAB Descritor do tabuleiro
 *
 *
 ***********************************************************************/

typedef struct TAB_tagTabuleiro {
    
    tpCasa casa[8][8];
    /* uma matriz 8x8 do tipo casa de tabuleiro */
    
} tpTabuleiro ;

/***** Protótipos das funções encapuladas no módulo *****/

static int ValidarLinhaColuna( int pos ) ;

static int ValidarCor(char cor) ;

static int ValidarTipo(char tipo) ;

static TAB_tpCondRet TAB_AtualizarListaAmeacantes( tpTabuleiro *t, int linha, int coluna);

static TAB_tpCondRet TAB_AtualizarListaAmeacados( tpTabuleiro *t, int linha, int coluna);

static TAB_tpCondRet TAB_Peao(tppPeca pecaOrigem, tppPeca pecaDestino);

static TAB_tpCondRet TAB_Bispo( tpTabuleiro *t, tppPeca pecaOrigem, tppPeca pecaDestino);

static TAB_tpCondRet TAB_Torre( tpTabuleiro *t, tppPeca pecaOrigem, tppPeca pecaDestino);

static TAB_tpCondRet TAB_Dama( tpTabuleiro *t, tppPeca pecaOrigem, tppPeca pecaDestino);

static TAB_tpCondRet TAB_Cavalo(  tppPeca pecaOrigem, tppPeca pecaDestino);

static TAB_tpCondRet TAB_Rei(  tppPeca pecaOrigem, tppPeca pecaDestino);

static TAB_tpCondRet TAB_ChecarMovimento( tpTabuleiro *t, tppPeca *pecaOrigem, tppPeca *pecaDestino );

static void TAB_DestruirValor( void * pValor );

#ifdef _DEBUG

static TAB_tpCondRet VerificarTodasCasas( tpTabuleiro * tabParm ) ;

#endif

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
 *
 *  Função: TAB  &Criar peca
 *  ****/

tppPeca* TAB_CriarPeca()
{
    tppPeca *p = NULL;
    p = ( tppPeca * ) malloc( sizeof( tppPeca )) ;
    if ( p == NULL )
    {
        return NULL ;
    }
#ifdef _DEBUG
    CED_DefinirTipoEspaco( p , TAB_TipoEspacoPeca ) ;
#endif
    return p ;
    
}  /* Fim função: TAB  &Criar Peca */


/*****  Dados encapsulados no módulo  *****/

#ifdef _DEBUG

static char EspacoLixo[ 256 ] =
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ;
/* Espaço de dados lixo usado ao testar */

#endif

/***************************************************************************
 *
 *  Função: TAB  &Criar tanuleiro
 *  ****/

tpTabuleiro* TAB_CriarTabuleiro()
{
    
    tppPeca *p = TAB_CriarPeca();
    int i, j;
    tpTabuleiro *tabuleiro = NULL;
    tabuleiro = ( tpTabuleiro * ) malloc( sizeof( tpTabuleiro )) ;
    if ( tabuleiro == NULL )
    {
        return NULL ;
    }
    
#ifdef _DEBUG
    CED_DefinirTipoEspaco( tabuleiro , TAB_TipoEspacoCabeca ) ;
#endif
    
    TAB_SetCor(p, 'V');
    TAB_SetTipo(p, 'V');
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            TAB_SetLinhaCorrente(p, i);
            TAB_SetColunaCorrente(p, j);
            TAB_InserirPeca(tabuleiro, i, j, p);
            tabuleiro->casa[i][j].listaDeAmeacantes = LIS_CriarLista( TAB_DestruirValor );
            tabuleiro->casa[i][j].listaDeAmeacados = LIS_CriarLista( TAB_DestruirValor  );
#ifdef _DEBUG
            tabuleiro->casa[i][j].i = i;
            tabuleiro->casa[i][j].j = j;
            tabuleiro->casa[i][j].pCabeca = tabuleiro;
#endif
        }
    }
    return tabuleiro ;
    
} /* Fim funncao: TAB &Criar Tabuleiro */

/***************************************************************************
 *
 *  Função: TAB  &Inserir Peca
 *  ****/

TAB_tpCondRet TAB_InserirPeca(tpTabuleiro *t, int linha , int coluna, tppPeca* peca)
{
    if(ValidarLinhaColuna(linha) == 0 || ValidarLinhaColuna(coluna) == 0)
    {
        return TAB_CondRetCoordInvalida;
    }
    else if(ValidarCor(peca->cor) == 0 || ValidarTipo(peca->tipo) == 0)
    {
        return TAB_CondRetPecaInvalida;
    }
    t->casa[linha][coluna].pecaDaCasa.linhaCorrente = linha;
    t->casa[linha][coluna].pecaDaCasa.colunaCorrente = coluna;
    t->casa[linha][coluna].pecaDaCasa.cor = peca->cor;
    t->casa[linha][coluna].pecaDaCasa.tipo = peca->tipo;
    
    return TAB_CondRetOK;
    
} /* Fim funncao: TAB &Inserir Peca  */

/***************************************************************************
 *
 *  Função: TAB  &Obter Peca
 *  ****/

tppPeca TAB_ObterPeca( tpTabuleiro *t, int linha, int coluna )
{
    
    if(ValidarLinhaColuna(linha) == 0 || ValidarLinhaColuna(coluna) == 0)
    {
        printf("Coordenada de tabuleiro invalida ao tentar obter peca");
    }
    
    return t->casa[linha][coluna].pecaDaCasa;
    
} /* Fim funncao: TAB &Obter Peca  */


/***************************************************************************
 *
 *  Função: TAB  &Retirar Peca
 *  ****/

TAB_tpCondRet TAB_RetirarPeca( tpTabuleiro *t, int linha, int coluna)
{
    if(ValidarLinhaColuna(linha) == 0 || ValidarLinhaColuna(coluna) == 0)
    {
        return TAB_CondRetCoordInvalida;
    }
    t->casa[linha][coluna].pecaDaCasa.cor = 'V';
    t->casa[linha][coluna].pecaDaCasa.tipo = 'V';
    return TAB_CondRetOK;
    
} /* Fim funncao: TAB &Retirar Peca  */

/***************************************************************************
 *
 *  Função: TAB  &Mover Peca
 *  ****/

TAB_tpCondRet TAB_MoverPeca( tpTabuleiro *t, int linhaOrigem, int colunaOrigem, int linhaDestino, int colunaDestino)
{
    tppPeca pecaOrigem = TAB_ObterPeca( t, linhaOrigem, colunaOrigem);
    tppPeca pecaDestino = TAB_ObterPeca(t , linhaDestino, colunaDestino);
    
    if( (ValidarLinhaColuna(linhaOrigem) == 0) || (ValidarLinhaColuna(colunaOrigem) == 0) ||
       (ValidarLinhaColuna(linhaDestino) == 0) || (ValidarLinhaColuna(colunaDestino) == 0) )
    {
        return TAB_CondRetCoordInvalida;
    }
    
    if( TAB_ChecarMovimento(t, &pecaOrigem, &pecaDestino) == TAB_CondRetOK)
    {
        TAB_RetirarPeca(t ,linhaDestino, colunaDestino);
        TAB_RetirarPeca(t ,linhaOrigem, colunaOrigem);
        TAB_InserirPeca( t, linhaDestino, colunaDestino, &pecaOrigem );
        return TAB_CondRetOK ;
    }
    
    return TAB_ChecarMovimento(t, &pecaOrigem, &pecaDestino);
    
}  /* Fim funncao: TAB &Mover Peca  */

/***************************************************************************
 *
 *  Função: TAB  &Obter Lista de Ameacantes
 *  ****/

LIS_tppLista TAB_ObterListaAmeacantes( tpTabuleiro *t, int linha, int coluna)
{
    TAB_AtualizarListaAmeacantes( t, linha, coluna);
    return t->casa[linha][coluna].listaDeAmeacantes;
    
}  /* Fim funncao: TAB &Obter Lista de Ameacantes   */

/***************************************************************************
 *
 *  Função: TAB  &Obter Lista de Ameacados
 *  ****/

LIS_tppLista TAB_ObterListaAmeacados( tpTabuleiro *t, int linha, int coluna)
{
    TAB_AtualizarListaAmeacados( t, linha, coluna);
    return t->casa[linha][coluna].listaDeAmeacados;
    
} /* Fim funncao: TAB &Obter Lista de Ameacados  */

/***************************************************************************
 *
 *  Função: TAB  &Imprime Tabuleiro
 *  ****/

TAB_tpCondRet TAB_ImprimeTabuleiro(tpTabuleiro *t)

{
    
    printf("\n");
    
    printf("       A        B        C        D        E        F        G        H\n");
    
    printf("  *************************************************************************\n");
    
    printf("  **********        **********        **********        **********        *\n");
    
    printf("1 *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   * 1\n",  t->casa[0][0].pecaDaCasa.tipo,t->casa[0][0].pecaDaCasa.cor,t->casa[0][1].pecaDaCasa.tipo,t->casa[0][1].pecaDaCasa.cor,t->casa[0][2].pecaDaCasa.tipo,t->casa[0][2].pecaDaCasa.cor,t->casa[0][3].pecaDaCasa.tipo,t->casa[0][3].pecaDaCasa.cor,t->casa[0][4].pecaDaCasa.tipo,t->casa[0][4].pecaDaCasa.cor,t->casa[0][5].pecaDaCasa.tipo,t->casa[0][5].pecaDaCasa.cor,t->casa[0][6].pecaDaCasa.tipo,t->casa[0][6].pecaDaCasa.cor,t->casa[0][7].pecaDaCasa.tipo,t->casa[0][7].pecaDaCasa.cor );
    
    printf("  **********        **********        **********        **********        *\n");
    
    printf("  *************************************************************************\n");
    
    printf("  *        **********        **********        **********        **********\n");
    
    printf("2 *   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c *** 2\n",  t->casa[1][0].pecaDaCasa.tipo,t->casa[1][0].pecaDaCasa.cor,t->casa[1][1].pecaDaCasa.tipo,t->casa[1][1].pecaDaCasa.cor,t->casa[1][2].pecaDaCasa.tipo,t->casa[1][2].pecaDaCasa.cor,t->casa[1][3].pecaDaCasa.tipo,t->casa[1][3].pecaDaCasa.cor,t->casa[1][4].pecaDaCasa.tipo,t->casa[1][4].pecaDaCasa.cor,t->casa[1][5].pecaDaCasa.tipo,t->casa[1][5].pecaDaCasa.cor,t->casa[1][6].pecaDaCasa.tipo,t->casa[1][6].pecaDaCasa.cor,t->casa[1][7].pecaDaCasa.tipo,t->casa[1][7].pecaDaCasa.cor );
    
    printf("  *        **********        **********        **********        **********\n");
    
    printf("  *************************************************************************\n");
    
    printf("  **********        **********        **********        **********        *\n");
    
    printf("3 *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   * 3\n",  t->casa[2][0].pecaDaCasa.tipo,t->casa[2][0].pecaDaCasa.cor,t->casa[2][1].pecaDaCasa.tipo,t->casa[2][1].pecaDaCasa.cor,t->casa[2][2].pecaDaCasa.tipo,t->casa[2][2].pecaDaCasa.cor,t->casa[2][3].pecaDaCasa.tipo,t->casa[2][3].pecaDaCasa.cor,t->casa[2][4].pecaDaCasa.tipo,t->casa[2][4].pecaDaCasa.cor,t->casa[2][5].pecaDaCasa.tipo,t->casa[2][5].pecaDaCasa.cor,t->casa[2][6].pecaDaCasa.tipo,t->casa[2][6].pecaDaCasa.cor,t->casa[2][7].pecaDaCasa.tipo,t->casa[2][7].pecaDaCasa.cor );
    
    printf("  **********        **********        **********        **********        *\n");
    
    printf("  *************************************************************************\n");
    
    printf("  *        **********        **********        **********        **********\n");
    
    printf("4 *   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c *** 4\n",  t->casa[3][0].pecaDaCasa.tipo,t->casa[3][0].pecaDaCasa.cor,t->casa[3][1].pecaDaCasa.tipo,t->casa[3][1].pecaDaCasa.cor,t->casa[3][2].pecaDaCasa.tipo,t->casa[3][2].pecaDaCasa.cor,t->casa[3][3].pecaDaCasa.tipo,t->casa[3][3].pecaDaCasa.cor,t->casa[3][4].pecaDaCasa.tipo,t->casa[3][4].pecaDaCasa.cor,t->casa[3][5].pecaDaCasa.tipo,t->casa[3][5].pecaDaCasa.cor,t->casa[3][6].pecaDaCasa.tipo,t->casa[3][6].pecaDaCasa.cor,t->casa[3][7].pecaDaCasa.tipo,t->casa[3][7].pecaDaCasa.cor );
    
    printf("  *        **********        **********        **********        **********\n");
    
    printf("  *************************************************************************\n");
    
    printf("  **********        **********        **********        **********        *\n");
    
    printf("5 *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   * 5\n",  t->casa[4][0].pecaDaCasa.tipo,t->casa[4][0].pecaDaCasa.cor,t->casa[4][1].pecaDaCasa.tipo,t->casa[4][1].pecaDaCasa.cor,t->casa[4][2].pecaDaCasa.tipo,t->casa[4][2].pecaDaCasa.cor,t->casa[4][3].pecaDaCasa.tipo,t->casa[4][3].pecaDaCasa.cor,t->casa[4][4].pecaDaCasa.tipo,t->casa[4][4].pecaDaCasa.cor,t->casa[4][5].pecaDaCasa.tipo,t->casa[4][5].pecaDaCasa.cor,t->casa[4][6].pecaDaCasa.tipo,t->casa[4][6].pecaDaCasa.cor,t->casa[4][7].pecaDaCasa.tipo,t->casa[4][7].pecaDaCasa.cor );
    
    printf("  **********        **********        **********        **********        *\n");
    
    printf("  *************************************************************************\n");
    
    printf("  *        **********        **********        **********        **********\n");
    
    printf("6 *   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c *** 6\n",  t->casa[5][0].pecaDaCasa.tipo,t->casa[5][0].pecaDaCasa.cor,t->casa[5][1].pecaDaCasa.tipo,t->casa[5][1].pecaDaCasa.cor,t->casa[5][2].pecaDaCasa.tipo,t->casa[5][2].pecaDaCasa.cor,t->casa[5][3].pecaDaCasa.tipo,t->casa[5][3].pecaDaCasa.cor,t->casa[5][4].pecaDaCasa.tipo,t->casa[5][4].pecaDaCasa.cor,t->casa[5][5].pecaDaCasa.tipo,t->casa[5][5].pecaDaCasa.cor,t->casa[5][6].pecaDaCasa.tipo,t->casa[5][6].pecaDaCasa.cor,t->casa[5][7].pecaDaCasa.tipo,t->casa[5][7].pecaDaCasa.cor );
    
    printf("  *        **********        **********        **********        **********\n");
    
    printf("  *************************************************************************\n");
    
    printf("  **********        **********        **********        **********        *\n");
    
    printf("7 *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   * 7\n",  t->casa[6][0].pecaDaCasa.tipo,t->casa[6][0].pecaDaCasa.cor,t->casa[6][1].pecaDaCasa.tipo,t->casa[6][1].pecaDaCasa.cor,t->casa[6][2].pecaDaCasa.tipo,t->casa[6][2].pecaDaCasa.cor,t->casa[6][3].pecaDaCasa.tipo,t->casa[6][3].pecaDaCasa.cor,t->casa[6][4].pecaDaCasa.tipo,t->casa[6][4].pecaDaCasa.cor,t->casa[6][5].pecaDaCasa.tipo,t->casa[6][5].pecaDaCasa.cor,t->casa[6][6].pecaDaCasa.tipo,t->casa[6][6].pecaDaCasa.cor,t->casa[6][7].pecaDaCasa.tipo,t->casa[6][7].pecaDaCasa.cor );
    
    printf("  **********        **********        **********        **********        *\n");
    
    printf("  *************************************************************************\n");
    
    printf("  *        **********        **********        **********        **********\n");
    
    printf("8 *   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c ***   %c%c   *** %c%c *** 8\n",  t->casa[7][0].pecaDaCasa.tipo,t->casa[7][0].pecaDaCasa.cor,t->casa[7][1].pecaDaCasa.tipo,t->casa[7][1].pecaDaCasa.cor,t->casa[7][2].pecaDaCasa.tipo,t->casa[7][2].pecaDaCasa.cor,t->casa[7][3].pecaDaCasa.tipo,t->casa[7][3].pecaDaCasa.cor,t->casa[7][4].pecaDaCasa.tipo,t->casa[7][4].pecaDaCasa.cor,t->casa[7][5].pecaDaCasa.tipo,t->casa[7][5].pecaDaCasa.cor,t->casa[7][6].pecaDaCasa.tipo,t->casa[7][6].pecaDaCasa.cor,t->casa[7][7].pecaDaCasa.tipo,t->casa[7][7].pecaDaCasa.cor );
    
    printf("  *        **********        **********        **********        **********\n");
    
    printf("  *************************************************************************\n");
    
    printf("       A        B        C        D        E        F        G        H\n");
    
    return TAB_CondRetOK ;
    
    
    
}/* Fim funncao: TAB &Imprime Tabuleiro*/
/***************************************************************************
 *
 *  Função: TAB  &Get Linha Corrente
 *  ****/
int TAB_GetLinhaCorrente(tppPeca* peca)
{
    return peca->linhaCorrente;
    
} /*  Fim funncao: TAB &Get Linha Corrente*/

/***************************************************************************
 *
 *  Função: TAB  &Get Coluna Corrente
 *  ****/
int TAB_GetColunaCorrente(tppPeca* peca)
{
    return peca->colunaCorrente;
    
} /*  Fim funncao: TAB &Get Coluna Corrente*/

/***************************************************************************
 *
 *  Função: TAB  &Get Cor
 *  ****/

char TAB_GetCor(tppPeca* peca)
{
    return peca->cor;
    
}  /*  Fim funncao: TAB &Get Cor*/

/***************************************************************************
 *
 *  Função: TAB  &Get Tipo
 *  ****/
char TAB_GetTipo(tppPeca* peca)
{
    return peca->tipo;
    
}  /*  Fim funncao: TAB &Get Tipo*/

/***************************************************************************
 *
 *  Função: TAB  &Set Linha Corrente
 *  ****/
TAB_tpCondRet TAB_SetLinhaCorrente(tppPeca* peca, int linhaCorrente)
{
    peca->linhaCorrente = linhaCorrente;
    return TAB_CondRetOK;
    
} /*  Fim funncao: TAB &Set Linha Corrente*/

/***************************************************************************
 *
 *  Função: TAB  &Set Coluna Corrente
 *  ****/
TAB_tpCondRet TAB_SetColunaCorrente(tppPeca* peca, int colunaCorrente)
{
    peca->colunaCorrente = colunaCorrente;
    return TAB_CondRetOK;
    
}  /*  Fim funncao: TAB &Set Coluna Corrente*/

/***************************************************************************
 *
 *  Função: TAB  &Set Cor
 *  ****/
TAB_tpCondRet TAB_SetCor(tppPeca* peca, char cor)
{
    peca->cor = cor;
    return TAB_CondRetOK;
    
} /*  Fim funncao: TAB &Set Cor*/

/***************************************************************************
 *
 *  Função: TAB  &Set Tipo
 *  ****/
TAB_tpCondRet TAB_SetTipo(tppPeca* peca, char tipo)
{
    peca->tipo = tipo;
    return TAB_CondRetOK;
    
} /*  Fim funncao: TAB &Set Tipo*/

/***************************************************************************
 *
 *  Função: TAB  &Destruir Tabuleiro
 *  ****/
void TAB_DestruirTabuleiro( tpTabuleiro* tabuleiro )
{
    int i,j;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            LIS_DestruirLista( tabuleiro->casa[i][j].listaDeAmeacantes ) ;
            LIS_DestruirLista( tabuleiro->casa[i][j].listaDeAmeacados ) ;
        }
    }
    free(tabuleiro);
    
} /*  Fim funncao: TAB &Destruir Tabuleiro*/


#ifdef _DEBUG

/***************************************************************************
 *
 *  Função: TAB  &Verificar um tabuleiro
 *  ****/

TAB_tpCondRet TAB_VerificarTabuleiro( tpTabuleiro* tabParm )
{
    
    tpTabuleiro * tab = NULL ;
    
    if ( TAB_VerificarCabeca( tabParm ) != TAB_CondRetOK )
    {
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador1" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
    CED_MarcarEspacoAtivo( tabParm ) ;
    
    tab =  tabParm ;
#ifdef _DEBUG
    CNT_CONTAR( "TAB_Verificador2" ) ;
#endif
    return VerificarTodasCasas( tab ) ;
    
} /* Fim função: TAB  &Verificar um tabuleiro */

#endif

#ifdef _DEBUG

/***************************************************************************
 *
 *  Função: TAB  &Verificar a cabeça de tabuleiro
 *  ****/

TAB_tpCondRet TAB_VerificarCabeca( tpTabuleiro* pCabecaParm )
{
    int i,j;
    
    /* Verifica o tipo do espaço */
    
    if ( pCabecaParm == NULL )
    {
        TST_NotificarFalha( "Tentou verificar cabeça inexistente." ) ;
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador3" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
    /*  if ( ! CED_VerificarEspaco( pCabecaParm , NULL ))
     {
     TST_NotificarFalha( "Controle do espaço acusou erro." ) ;
     #ifdef _DEBUG
     CNT_CONTAR( "TAB_Verificador4" ) ;
     #endif
     return TAB_CondRetErroEstrutura ;
     } /* if */
    
    if ( TST_CompararInt( TAB_TipoEspacoCabeca ,
                         CED_ObterTipoEspaco( pCabecaParm ) ,
                         "Tipo do espaço de dados não é cabeça de tabuleiro." ) != TST_CondRetOK )
    {
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador5" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
    for(i=0; i<8; i++)
    {
        for(j=0; j<8; j++)
        {
            if ( TST_CompararPonteiro( pCabecaParm->casa[i][j].pCabeca , pCabecaParm,
                                      "Alguma casa aponta pro tabuleiro(pCabeca) errado." ) != TST_CondRetOK )
            {
#ifdef _DEBUG
                CNT_CONTAR( "TAB_Verificador6" ) ;
#endif
                return TAB_CondRetErroEstrutura;
            } /* if */
#ifdef _DEBUG
            CNT_CONTAR( "TAB_Verificador7" ) ;
#endif
        }
    }
    
#ifdef _DEBUG
    CNT_CONTAR( "TAB_Verificador8" ) ;
#endif
    return TAB_CondRetOK ;
    
}/* Fim função: TAB  &Verificar a cabeca de tabuleiro */

#endif

#ifdef _DEBUG
/***************************************************************************
 *
 *  Função: TAB  &Verificar uma casa de estrutura
 *  ****/

TAB_tpCondRet TAB_VerificarCasa( tpCasa casaParm )
{
    
    tpTabuleiro* tab;
    int linha;
    int coluna;
    
    
    /* Verificar se é casa estrutural */
    
    
    if ( TST_CompararInt( LIS_CondRetOK , LIS_VerificarLista( casaParm.listaDeAmeacados ) ,
                         "Lista de ameacados com erro." ) != TST_CondRetOK )
    {
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador9" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
    if ( TST_CompararInt( LIS_CondRetOK , LIS_VerificarLista( casaParm.listaDeAmeacantes ) ,
                         "Lista de ameacantes com erro." ) != TST_CondRetOK )
    {
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador10" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
    tab = casaParm.pCabeca;
    linha = casaParm.i;
    coluna = casaParm.j;
    
    /* Verificar redundancias */
    
    if ( TST_CompararInt( tab->casa[linha][coluna].i , linha  ,
                         "Redundancia cabeca-casa (linha) incorreta." ) != TST_CondRetOK )
    {
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador11" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
    if ( TST_CompararInt( tab->casa[linha][coluna].j , coluna  ,
                         "Redundancia cabeca-casa (coluna) incorreta." ) != TST_CondRetOK )
    {
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador12" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
    if ( TST_CompararInt( linha , casaParm.pecaDaCasa.linhaCorrente  ,
                         "Redundancia casa-peca (Linha) incorreta." ) != TST_CondRetOK )
    {
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador13" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
    if ( TST_CompararInt( coluna , casaParm.pecaDaCasa.colunaCorrente  ,
                         "Redundancia casa-peca (coluna) incorreta." ) != TST_CondRetOK )
    {
#ifdef _DEBUG
        CNT_CONTAR( "TAB_Verificador14" ) ;
#endif
        return TAB_CondRetErroEstrutura ;
    } /* if */
    
#ifdef _DEBUG
    CNT_CONTAR( "TAB_Verificador15" ) ;
#endif
    return TAB_CondRetOK ;
    
} /* Fim função: LIS  &Verificar um elemento da lista */

#endif

#ifdef _DEBUG
/***************************************************************************
 *
 *  Função: TAB  &Deturpar tabuleiro
 *  ****/

void TAB_Deturpar( tpTabuleiro* tabParm ,
                  TAB_tpModosDeturpacao ModoDeturpar )
{
    
    if ( tabParm == NULL )
    {
        return ;
    } /* if */
    
    
    switch ( ModoDeturpar )
    {
            
            /* Modifica o tipo da cabeça */
            
        case TAB_DeturpaTipoCabeca :
        {
            
            CED_DefinirTipoEspaco( tabParm , CED_ID_TIPO_VALOR_NULO ) ;
            
            break ;
            
        } /* fim ativa: Modifica o tipo da cabeça */
            
        case TAB_DeturpaPtCabecaNulo :
        {
            
            tabParm->casa[0][0].pCabeca = NULL ;
            
            break ;
            
        } /* fim ativa: Anula ponteiro cabeça de uma casa aleatoria */
            
            
            
        case TAB_DeturpaPtCabecaLixo :
        {
            
            tabParm->casa[5][3].pCabeca = ( tpTabuleiro* )( EspacoLixo ) ;
            
            break ;
            
        } /* fim ativa: Faz ponteiro cabeça apontar para lixo */
            
            
        case TAB_DeturpaLinhaCorrente :
        {
            
            tabParm->casa[6][7].pecaDaCasa.linhaCorrente = 0 ;
            
            break ;
            
        } /* fim ativa: Altera a linha corrente de uma pecaDaCasa aleatoria */
            
        case TAB_DeturpaColunaCorrente :
        {
            
            tabParm->casa[6][7].pecaDaCasa.colunaCorrente = 0 ;
            
            break ;
            
        } /* fim ativa: Altera a coluna corrente de uma pecaDaCasa aleatoria */
            
        case TAB_DeturpaLinhaCasa :
        {
            
            tabParm->casa[0][3].i = 8 ;
            
            break ;
            
        } /* fim ativa: Altera a redundancia de linha(i) de uma casa aleatoria */
            
        case TAB_DeturpaColunaCasa :
        {
            
            tabParm->casa[0][7].j = -1 ;
            
            break ;
            
        } /* fim ativa: Altera a redundancia de coluna(j) de uma casa aleatoria */
            
        case TAB_DeturpaListaAmeacantes :
        {
            
            LIS_Deturpar( tabParm->casa[0][0].listaDeAmeacantes,  16 ) ;
            
            break ;
            
        } /* fim ativa: Deturpa Lista de ameacantes */
            
        case TAB_DeturpaListaAmeacados :
        {
            
            LIS_Deturpar( tabParm->casa[0][0].listaDeAmeacantes,  15 ) ;
            
            break ;
            
        } /* fim ativa: Deturpa lista de ameacados */
            
    }
} /* Fim função: TAB  &Deturpar tabuleiro */

#endif

/*****  Código das funções encapsuladas no módulo  *****/

/***********************************************************************
 *
 *  $FC Função: TAB  &Validar Linha ou Coluna
 *
 *  $ED Descrição da função
 *     Valida se uma linha ou coluna eh uma coordenada do tabuleiro,
 *     ou seja, esta entre 0 e 7.
 *
 *  $EP Parâmetros
 *     pos - inteiro com o numero da linha ou coluna   .
 *
 *  $FV Valor retornado
 *     int - 1 caso seja valido.
 *     int - 0 caso nao seja valido.
 *
 ***********************************************************************/

int ValidarLinhaColuna( int pos )
{
    
    if ( ( pos <  0 )
        || ( pos >= 8 ))
    {
        return 0 ;
    } /* if */
    
    return 1 ;
    
} /*  Fim funncao: TAB &Validar Linha Coluna*/


/***********************************************************************
 *
 *  $FC Função: TAB  &Validar Cor
 *
 *  $ED Descrição da função
 *     Valida se uma cor eh coerente,tem que ser :
 *     B, P ou V.
 *
 *  $EP Parâmetros
 *     cor - um char que representa a cor.
 *
 *  $FV Valor retornado
 *     int - 1 caso seja valido.
 *     int - 0 caso nao seja valido.
 *
 ***********************************************************************/

int ValidarCor(char cor)
{
    if ( (cor == 'B') || (cor == 'P') || (cor == 'V'))
    {
        return 1 ;
    } /* if */
    
    return 0 ;
    
} /*  Fim funncao: TAB &Validar Cor */


/***********************************************************************
 *
 *  $FC Função: TAB  &Validar Tipo
 *
 *  $ED Descrição da função
 *     Valida se um tipo eh coerente,tem que ser :
 *     P, T, C, B, D, R ou V.
 *
 *  $EP Parâmetros
 *     cor - um char que representa a cor
 *
 *  $FV Valor retornado
 *     int - 1 caso seja valido
 *     int - 0 caso nao seja valido
 *
 ***********************************************************************/

int ValidarTipo(char tipo)
{
    if ( (tipo == 'T') || (tipo == 'C') || (tipo == 'V') ||
        (tipo == 'D') || (tipo == 'R') || (tipo == 'P') || (tipo == 'B'))
    {
        return 1 ;
    } /* if */
    
    return 0 ;
    
} /*  Fim funncao: TAB &Validar Tipo*/


/***********************************************************************
 *
 *  $FC Função: TAB  &Atualiza lista de ameacantes
 *
 *  $ED Descrição da função
 *     Antes de obter uma lista de ameacantes essa funcao eh chamada
 *     para atualizar os dados da lista.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro.
 *     linha - linha do tabuleiro que contem a lista, valor de 0 a 7.
 *     coluna - coluna do tabuleiro que contem a lista, valor de 0 a 7.
 *
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *
 ***********************************************************************/

TAB_tpCondRet TAB_AtualizarListaAmeacantes( tpTabuleiro *t, int linha, int coluna)
{
    int i , j;
    tppPeca pecaEmQuestao;
    
    //checa se a lista ja foi criada
    if(t->casa[linha][coluna].listaDeAmeacantes == 0 )
    {
        LIS_tppLista lista =  LIS_CriarLista( TAB_DestruirValor );
        t->casa[linha][coluna].listaDeAmeacantes = lista;
        
    }
    else
    {
        LIS_EsvaziarLista( t->casa[linha][coluna].listaDeAmeacantes );
    }
    
    //pega a peca da casa em questao
    pecaEmQuestao = TAB_ObterPeca( t, linha, coluna);
    
    
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            //compara com todo o tabuleiro se a peca ameacadora pode se movimentar(comer) para a casa em questao
            tppPeca pecaAmeacadora = TAB_ObterPeca(t , i, j);
            if( TAB_ChecarMovimento(t, &pecaAmeacadora, &pecaEmQuestao) == TAB_CondRetOK )
            {
                //insere la lista de ameacantes a peca que satisfez
                LIS_InserirElementoApos(t->casa[linha][coluna].listaDeAmeacantes, &t->casa[i][j].pecaDaCasa);
            }
        }
    }
    return TAB_CondRetOK ;
    
} /*  Fim funncao: TAB &Atualiza Lista de ameacantes*/


/***********************************************************************
 *
 *  $FC Função: TAB  &Atualiza lista de ameacados
 *
 *  $ED Descrição da função
 *     Antes de obter uma lista de ameacados essa funcao eh chamada
 *     para atualizar os dados da lista.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro.
 *     linha - linha do tabuleiro que contem a lista, valor de 0 a 7.
 *     coluna - coluna do tabuleiro que contem a lista, valor de 0 a 7.
 *
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *
 ***********************************************************************/

TAB_tpCondRet TAB_AtualizarListaAmeacados( tpTabuleiro *t, int linha, int coluna)
{
    int i , j;
    tppPeca pecaEmQuestao;
    //checa se a lista ja foi criada
    if(t->casa[linha][coluna].listaDeAmeacados == 0 )
    {
        LIS_tppLista lista =  LIS_CriarLista(TAB_DestruirValor);
        t->casa[linha][coluna].listaDeAmeacados = lista;
        
    }
    else
    {
        LIS_EsvaziarLista( t->casa[linha][coluna].listaDeAmeacados );
    }
    
    //pega a peca da casa em questao
    pecaEmQuestao = TAB_ObterPeca( t, linha, coluna);
    
    
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            //compara com todo o tabuleiro se a peca ameacadora pode se movimentar(comer) para a casa em questao
            tppPeca pecaAmeacada = TAB_ObterPeca(t , i, j);
            if( TAB_ChecarMovimento(t, &pecaEmQuestao, &pecaAmeacada) == TAB_CondRetOK /*&& pecaAmeacada.cor !='V'*/)
            {
                //insere la lista de ameacantes a peca que satisfez
                LIS_InserirElementoApos(t->casa[linha][coluna].listaDeAmeacados, &t->casa[i][j].pecaDaCasa);
            }
        }
    }
    return TAB_CondRetOK ;
    
} /*  Fim funncao: TAB &Atualiza Lista de ameacados*/


/***********************************************************************
 *
 *  $FC Função: TAB  &Peao
 *
 *  $ED Descrição da função
 *     Verifica se o movimento de um peao eh valido
 *
 *  $EP Parâmetros
 *     pecaOrigem - peca peao a ser movida
 *     pecaDestino - peca que esta na casa destino do movimento
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetMovimentoInvalido
 *
 ***********************************************************************/

TAB_tpCondRet TAB_Peao(tppPeca pecaOrigem, tppPeca pecaDestino)
{
    int variacaoMovimento;
    
    //verifica a cor do peao
    if(pecaOrigem.cor == 'P')
    {
        variacaoMovimento = 1;
    }
    else if(pecaOrigem.cor == 'B')
    {
        variacaoMovimento = -1;
    }
    else
    {
        return TAB_CondRetOrigemSemPeca;
    }
    //movimento sem comer(tem que ser em linha reta e nao ter ninguem na frente)
    if( (pecaDestino.colunaCorrente == pecaOrigem.colunaCorrente) &&
       ( pecaDestino.linhaCorrente  == (pecaOrigem.linhaCorrente + variacaoMovimento) ) &&
       (pecaDestino.cor == 'V') && (pecaDestino.tipo == 'V') )
    {
        return TAB_CondRetOK;
    }
    
    //movimento comendo (diagonal para frente sendo o destino um oponente)
    else if ( ( (pecaDestino.colunaCorrente == (pecaOrigem.colunaCorrente + 1)) ||
               (pecaDestino.colunaCorrente == (pecaOrigem.colunaCorrente - 1)) ) &&
             pecaDestino.linhaCorrente == ((pecaOrigem.linhaCorrente)+ variacaoMovimento) &&
             pecaDestino.cor != 'V' && pecaDestino.tipo != 'V' &&
             pecaDestino.cor != pecaOrigem.cor )
    {
        return TAB_CondRetOK;
    }
    else
    {
        return TAB_CondRetMovimentoInvalido;
    }
    
} /*  Fim funncao: TAB &Peao */

/***********************************************************************
 *
 *  $FC Função: TAB  &Bispo
 *
 *  $ED Descrição da função
 *     Verifica se o movimento de um bispo eh valido
 *
 *  $EP Parâmetros
 *     pecaOrigem - peca bispo a ser movida
 *     pecaDestino - peca que esta na casa destino do movimento
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetMovimentoInvalido
 *     TAB_CondRetColisao
 *
 ***********************************************************************/

TAB_tpCondRet TAB_Bispo( tpTabuleiro *t, tppPeca pecaOrigem, tppPeca pecaDestino)
{
    //verificar movimentacao diagonal e se o destino eh de cor diferente(vazio ou adversario)
    if( ( fabs(pecaOrigem.colunaCorrente - pecaDestino.colunaCorrente) ==
         fabs(pecaOrigem.linhaCorrente - pecaDestino.linhaCorrente) ) &&
       (pecaOrigem.cor != pecaDestino.cor) )
    {
        //verificar colisoes
        
        //anda para cima e esquerda no tabuleiro
        if(pecaOrigem.linhaCorrente > pecaDestino.linhaCorrente &&
           pecaOrigem.colunaCorrente > pecaDestino.colunaCorrente)
        {
            tppPeca pecaNoCaminho;
            TAB_SetLinhaCorrente(&pecaNoCaminho, pecaOrigem.linhaCorrente-1);
            TAB_SetColunaCorrente(&pecaNoCaminho, pecaOrigem.colunaCorrente-1);
            
            //verifica todas as casas ate chegar no destino checando se ha peca
            while(pecaNoCaminho.linhaCorrente > pecaDestino.linhaCorrente)
            {
                pecaNoCaminho = TAB_ObterPeca(t, pecaNoCaminho.linhaCorrente,pecaNoCaminho.colunaCorrente);
                if(pecaNoCaminho.cor != 'V' && pecaNoCaminho.tipo != 'V')
                {
                    return TAB_CondRetColisao;
                }
                
                TAB_SetLinhaCorrente(&pecaNoCaminho, pecaNoCaminho.linhaCorrente-1);
                TAB_SetColunaCorrente(&pecaNoCaminho, pecaNoCaminho.colunaCorrente-1);
            }
        }
        
        //anda para cima e direita no tabuleiro
        if(pecaOrigem.linhaCorrente > pecaDestino.linhaCorrente &&
           pecaOrigem.colunaCorrente < pecaDestino.colunaCorrente)
        {
            tppPeca pecaNoCaminho;
            TAB_SetLinhaCorrente(&pecaNoCaminho, pecaOrigem.linhaCorrente-1);
            TAB_SetColunaCorrente(&pecaNoCaminho, pecaOrigem.colunaCorrente+1);
            
            //verifica todas as casas ate chegar no destino checando se ha peca
            while(pecaNoCaminho.linhaCorrente > pecaDestino.linhaCorrente)
            {
                pecaNoCaminho = TAB_ObterPeca(t, pecaNoCaminho.linhaCorrente,pecaNoCaminho.colunaCorrente);
                if(pecaNoCaminho.cor != 'V' && pecaNoCaminho.tipo != 'V')
                {
                    return TAB_CondRetColisao;
                }
                
                //percore a diagonal
                TAB_SetLinhaCorrente(&pecaNoCaminho, pecaNoCaminho.linhaCorrente-1);
                TAB_SetColunaCorrente(&pecaNoCaminho, pecaNoCaminho.colunaCorrente+1);
                
            }
        }
        
        //anda para baixo e esquerda
        if(pecaOrigem.linhaCorrente < pecaDestino.linhaCorrente &&
           pecaOrigem.colunaCorrente > pecaDestino.colunaCorrente)
        {
            tppPeca pecaNoCaminho;
            TAB_SetLinhaCorrente(&pecaNoCaminho, pecaOrigem.linhaCorrente+1);
            TAB_SetColunaCorrente(&pecaNoCaminho, pecaOrigem.colunaCorrente-1);
            
            //verifica todas as casas ate chegar no destino checando se ha peca
            while(pecaNoCaminho.linhaCorrente < pecaDestino.linhaCorrente)
            {
                pecaNoCaminho = TAB_ObterPeca(t, pecaNoCaminho.linhaCorrente,pecaNoCaminho.colunaCorrente);
                if(pecaNoCaminho.cor != 'V' && pecaNoCaminho.tipo != 'V')
                {
                    return TAB_CondRetColisao;
                }
                
                //percore a diagonal
                TAB_SetLinhaCorrente(&pecaNoCaminho, pecaNoCaminho.linhaCorrente+1);
                TAB_SetColunaCorrente(&pecaNoCaminho, pecaNoCaminho.colunaCorrente-1);
                
            }
        }
        
        //anda para baixo e direita
        if(pecaOrigem.linhaCorrente < pecaDestino.linhaCorrente &&
           pecaOrigem.colunaCorrente < pecaDestino.colunaCorrente)
        {
            tppPeca pecaNoCaminho;
            TAB_SetLinhaCorrente(&pecaNoCaminho, pecaOrigem.linhaCorrente+1);
            TAB_SetColunaCorrente(&pecaNoCaminho, pecaOrigem.colunaCorrente+1);
            
            //verifica todas as casas ate chegar no destino checando se ha peca
            while(pecaNoCaminho.linhaCorrente < pecaDestino.linhaCorrente)
            {
                pecaNoCaminho = TAB_ObterPeca(t, pecaNoCaminho.linhaCorrente,pecaNoCaminho.colunaCorrente);
                if(pecaNoCaminho.cor != 'V' && pecaNoCaminho.tipo != 'V')
                {
                    return TAB_CondRetColisao;
                }
                
                //percore a diagonal
                TAB_SetLinhaCorrente(&pecaNoCaminho, pecaNoCaminho.linhaCorrente+1);
                TAB_SetColunaCorrente(&pecaNoCaminho, pecaNoCaminho.colunaCorrente+1);
                
            }
        }
        return TAB_CondRetOK;
    }
    return TAB_CondRetMovimentoInvalido;
    
} /*  Fim funncao: TAB &Bispo */


/***********************************************************************
 *
 *  $FC Função: TAB  &Torre
 *
 *  $ED Descrição da função
 *     Verifica se o movimento de uma torre eh valido
 *
 *  $EP Parâmetros
 *     pecaOrigem - peca torre a ser movida
 *     pecaDestino - peca que esta na casa destino do movimento
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetMovimentoInvalido
 *     TAB_CondRetColisao
 *
 ***********************************************************************/

TAB_tpCondRet TAB_Torre( tpTabuleiro *t, tppPeca pecaOrigem, tppPeca pecaDestino)
{
    //verificar movimentacao reta e se o destino eh de cor diferente(vazio ou adversario)
    if( ( (pecaOrigem.colunaCorrente == pecaDestino.colunaCorrente) ||
         (pecaOrigem.linhaCorrente == pecaDestino.linhaCorrente) ) &&
       (pecaOrigem.cor != pecaDestino.cor) )
    {
        //verificar colisoes
        
        //anda para cima no tabuleiro
        if(pecaOrigem.linhaCorrente > pecaDestino.linhaCorrente &&
           pecaOrigem.colunaCorrente == pecaDestino.colunaCorrente)
        {
            tppPeca pecaNoCaminho;
            TAB_SetLinhaCorrente(&pecaNoCaminho, pecaOrigem.linhaCorrente-1);
            TAB_SetColunaCorrente(&pecaNoCaminho, pecaOrigem.colunaCorrente);
            
            //verifica todas as casas ate chegar no destino checando se ha peca
            while(pecaNoCaminho.linhaCorrente > pecaDestino.linhaCorrente)
            {
                pecaNoCaminho = TAB_ObterPeca(t, pecaNoCaminho.linhaCorrente,pecaNoCaminho.colunaCorrente);
                if(pecaNoCaminho.cor != 'V' && pecaNoCaminho.tipo != 'V')
                {
                    return TAB_CondRetColisao;
                }
                
                TAB_SetLinhaCorrente(&pecaNoCaminho, pecaNoCaminho.linhaCorrente-1);
                TAB_SetColunaCorrente(&pecaNoCaminho, pecaNoCaminho.colunaCorrente);
            }
        }
        
        //anda para baixo no tabuleiro
        if(pecaOrigem.linhaCorrente < pecaDestino.linhaCorrente &&
           pecaOrigem.colunaCorrente == pecaDestino.colunaCorrente)
        {
            tppPeca pecaNoCaminho;
            TAB_SetLinhaCorrente(&pecaNoCaminho, pecaOrigem.linhaCorrente+1);
            TAB_SetColunaCorrente(&pecaNoCaminho, pecaOrigem.colunaCorrente);
            
            //verifica todas as casas ate chegar no destino checando se ha peca
            while(pecaNoCaminho.linhaCorrente < pecaDestino.linhaCorrente)
            {
                pecaNoCaminho = TAB_ObterPeca(t, pecaNoCaminho.linhaCorrente,pecaNoCaminho.colunaCorrente);
                if(pecaNoCaminho.cor != 'V' && pecaNoCaminho.tipo != 'V')
                {
                    return TAB_CondRetColisao;
                }
                
                //percore a diagonal
                TAB_SetLinhaCorrente(&pecaNoCaminho, pecaNoCaminho.linhaCorrente+1);
                TAB_SetColunaCorrente(&pecaNoCaminho, pecaNoCaminho.colunaCorrente);
                
            }
        }
        
        //anda para  esquerda
        if(pecaOrigem.linhaCorrente == pecaDestino.linhaCorrente &&
           pecaOrigem.colunaCorrente > pecaDestino.colunaCorrente)
        {
            tppPeca pecaNoCaminho;
            TAB_SetLinhaCorrente(&pecaNoCaminho, pecaOrigem.linhaCorrente);
            TAB_SetColunaCorrente(&pecaNoCaminho, pecaOrigem.colunaCorrente-1);
            
            //verifica todas as casas ate chegar no destino checando se ha peca
            while(pecaNoCaminho.colunaCorrente > pecaDestino.colunaCorrente)
            {
                pecaNoCaminho = TAB_ObterPeca(t, pecaNoCaminho.linhaCorrente,pecaNoCaminho.colunaCorrente);
                if(pecaNoCaminho.cor != 'V' && pecaNoCaminho.tipo != 'V')
                {
                    return TAB_CondRetColisao;
                }
                
                //percore a diagonal
                TAB_SetLinhaCorrente(&pecaNoCaminho, pecaNoCaminho.linhaCorrente);
                TAB_SetColunaCorrente(&pecaNoCaminho, pecaNoCaminho.colunaCorrente-1);
                
            }
        }
        
        //anda para  direita
        if(pecaOrigem.linhaCorrente == pecaDestino.linhaCorrente &&
           pecaOrigem.colunaCorrente < pecaDestino.colunaCorrente)
        {
            tppPeca pecaNoCaminho;
            TAB_SetLinhaCorrente(&pecaNoCaminho, pecaOrigem.linhaCorrente/*+1*/);
            TAB_SetColunaCorrente(&pecaNoCaminho, pecaOrigem.colunaCorrente+1);
            
            //verifica todas as casas ate chegar no destino checando se ha peca
            while(pecaNoCaminho.colunaCorrente < pecaDestino.colunaCorrente)
            {
                pecaNoCaminho = TAB_ObterPeca(t, pecaNoCaminho.linhaCorrente,pecaNoCaminho.colunaCorrente);
                if(pecaNoCaminho.cor != 'V' && pecaNoCaminho.tipo != 'V')
                {
                    return TAB_CondRetColisao;
                }
                
                //percore a diagonal
                TAB_SetLinhaCorrente(&pecaNoCaminho, pecaNoCaminho.linhaCorrente);
                TAB_SetColunaCorrente(&pecaNoCaminho, pecaNoCaminho.colunaCorrente+1);
                
            }
        }
        return TAB_CondRetOK;
    }
    return TAB_CondRetMovimentoInvalido;
    
} /*  Fim funncao: TAB &Torre */


/***********************************************************************
 *
 *  $FC Função: TAB  &Dama
 *
 *  $ED Descrição da função
 *     Verifica se o movimento de uma dama eh valido
 *
 *  $EP Parâmetros
 *     pecaOrigem - peca dama a ser movida
 *     pecaDestino - peca que esta na casa destino do movimento
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetMovimentoInvalido
 *     TAB_CondRetColisao
 *
 ***********************************************************************/

TAB_tpCondRet TAB_Dama( tpTabuleiro *t, tppPeca pecaOrigem, tppPeca pecaDestino)
{
    if( (TAB_Bispo(t, pecaOrigem, pecaDestino) == TAB_CondRetOK) ||
       (TAB_Torre(t, pecaOrigem, pecaDestino) == TAB_CondRetOK) )
    {
        return TAB_CondRetOK;
    }
    return TAB_CondRetMovimentoInvalido;
    
} /*  Fim funncao: TAB &Dama */


/***********************************************************************
 *
 *  $FC Função: TAB  &Cavalo
 *
 *  $ED Descrição da função
 *     Verifica se o movimento de um cavalo eh valido
 *
 *  $EP Parâmetros
 *     pecaOrigem - peca cavalo a ser movida
 *     pecaDestino - peca que esta na casa destino do movimento
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetMovimentoInvalido
 *
 ***********************************************************************/

TAB_tpCondRet TAB_Cavalo(  tppPeca pecaOrigem, tppPeca pecaDestino)
{
    if( ( ( fabs(pecaOrigem.linhaCorrente - pecaDestino.linhaCorrente) == 2  &&
           fabs(pecaOrigem.colunaCorrente - pecaDestino.colunaCorrente) == 1 )||
         ( fabs(pecaOrigem.linhaCorrente - pecaDestino.linhaCorrente) == 1   &&
          fabs(pecaOrigem.colunaCorrente - pecaDestino.colunaCorrente) == 2 )  ) &&
       (pecaOrigem.cor != pecaDestino.cor) )
    {
        return TAB_CondRetOK;
    }
    return TAB_CondRetMovimentoInvalido;
    
} /*  Fim funncao: TAB &Cavalo */


/***********************************************************************
 *
 *  $FC Função: TAB  &Rei
 *
 *  $ED Descrição da função
 *     Verifica se o movimento de um rei eh valido
 *
 *  $EP Parâmetros
 *     pecaOrigem - peca rei a ser movida
 *     pecaDestino - peca que esta na casa destino do movimento
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetMovimentoInvalido
 *
 ***********************************************************************/

TAB_tpCondRet TAB_Rei(  tppPeca pecaOrigem, tppPeca pecaDestino)
{
    
    //movimento (tem que andar apenas 1 e nao ter ninguem na frente ou ser o adversario)
    if( (fabs(pecaDestino.colunaCorrente - pecaOrigem.colunaCorrente) <= 1) &&
       (fabs(pecaDestino.linhaCorrente  - pecaOrigem.linhaCorrente) <= 1) &&
       ( ( (pecaDestino.cor == 'V') && (pecaDestino.tipo == 'V') ) ||
        pecaDestino.cor != pecaOrigem.cor ) )
    {
        return TAB_CondRetOK;
    }
    return TAB_CondRetMovimentoInvalido;
    
} /*  Fim funncao: TAB &Rei */


/***********************************************************************
 *
 *  $FC Função: TAB  &Checar movimento
 *
 *  $ED Descrição da função
 *     Verifica o movimento de uma peca qualquer
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro aonde se quer realizar o movimento
 *     pecaOrigem - ponteiro peca a ser movida
 *     pecaDestino - ponteiro para peca que esta na casa destino do movimento
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetMovimentoInvalido
 *     TAB_CondRetColisao
 *     TAB_OrigemSemPeca
 *
 ***********************************************************************/

TAB_tpCondRet TAB_ChecarMovimento(tpTabuleiro *t,tppPeca *pecaOrigem, tppPeca *pecaDestino )
{
    
    
    if( pecaOrigem->tipo == 'P' )
    {
        return TAB_Peao( *pecaOrigem, *pecaDestino);
    }
    if( pecaOrigem->tipo == 'B' )
    {
        return TAB_Bispo(t, *pecaOrigem, *pecaDestino);
    }
    if( pecaOrigem->tipo == 'T' )
    {
        return TAB_Torre(t, *pecaOrigem, *pecaDestino);
    }
    if( pecaOrigem->tipo == 'D' )
    {
        return TAB_Dama(t, *pecaOrigem, *pecaDestino);
    }
    if( pecaOrigem->tipo == 'C' )
    {
        return TAB_Cavalo( *pecaOrigem, *pecaDestino);
    }
    if( pecaOrigem->tipo == 'R' )
    {
        return TAB_Rei( *pecaOrigem, *pecaDestino);
    }
    
    return TAB_CondRetOrigemSemPeca;
    
} /*  Fim funncao: TAB &Checar Movimento */


/***********************************************************************
 *
 *  $FC Função: TAB  &Destruir Valor
 *
 *  $ED Descrição da função
 *     libera um ponteiro do tipo void.
 *
 *  $EP Parâmetros
 *     pValor - ponteiro para o valor.
 *
 ***********************************************************************/

void TAB_DestruirValor( void * pValor )
{
    
    if (pValor != NULL) {
        pValor = NULL;
        free(pValor);
    }
    free( pValor ) ;
    
} /*  Fim funncao: TAB &Destruir Valor */

#ifdef _DEBUG
/***********************************************************************
 *
 *  $FC Função: TAB  -Explorar verificando todas as casa do tabuleiro
 *
 *  $ED Descrição da função
 *     Percorre o tabuleiro verificando as casas à medida que forem
 *     visitados. Caso seja encontrado alguma falha, a verificação será
 *     suspensa. Portanto, no caso de falha, é possível que nem todas
 *     as casas do tabuleiro sejam visitados.
 *
 ***********************************************************************/

TAB_tpCondRet VerificarTodasCasas( tpTabuleiro *tabParm )
{
    
    TAB_tpCondRet CondErro = TAB_CondRetOK ;
    int i,j;
    /*if ( pElem == NULL )
     {
     return TAB_CondRetOK ;
     } /* if */
    
    CED_MarcarEspacoAtivo( tabParm ) ;
    
    for(i=0; i<8 ;i++)
    {
        for(j=0; j<8; j++)
        {
            CondErro = TAB_VerificarCasa( tabParm->casa[i][j] ) ;
            if ( CondErro != TAB_CondRetOK )
            {
                return CondErro ;
            } /* if */
        }
    }
    
    
    return CondErro ;
}
#endif
/********** Fim do módulo de implementação: TAB tabuleiro de xadrez **********/