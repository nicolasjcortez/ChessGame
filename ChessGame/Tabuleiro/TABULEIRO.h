#if ! defined( TABULEIRO_ )
#define TABULEIRO_
/***************************************************************************
 *
 *  $MCD Módulo de definição: TAB  Tabuleiro de um jogo de xadrez
 *
 *  Arquivo gerado:              TABULEIRO.h
 *  Letras identificadoras:      TAB
 *
 *  Autores: Nicolas Jordao Cortez
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

#if defined( TABULEIRO_OWN )
#define TABULEIRO_EXT
#else
#define TABULEIRO_EXT extern
#endif

#include "LISTA.h"

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para um Tabuleiro */

typedef struct TAB_tagTabuleiro  tpTabuleiro ;

/* Tipo referência para uma Casa */

typedef struct TAB_tagCasa  tpCasa ;

/***********************************************************************
 *
 *  $TC Tipo de dados: TAB Peca
 *
 *
 ***********************************************************************/

typedef struct TAB_tagPeca
{
    int linhaCorrente;
    int colunaCorrente;
    char cor;
    char tipo;
} tppPeca ;

/***********************************************************************
 *
 *  $TC Tipo de dados: TAB Condições de retorno
 *
 *
 *  $ED Descrição do tipo
 *     Condições de retorno das funções do Tabuleiro
 *
 ***********************************************************************/

typedef enum {
    
    TAB_CondRetOK ,
    /* Concluiu corretamente */
    
    TAB_CondRetOrigemSemPeca ,
    /* A peca da casa desejada eh vazia */
    
    TAB_CondRetMovimentoInvalido ,
    /* Nao e possivel fazer o movimento */
    
    TAB_CondRetColisao ,
    /* Houve colisao durante o movimento da peca */
    
    TAB_CondRetPecaInvalida ,
    /* Nao existe peca com o dado tipo ou cor*/
    
    TAB_CondRetCoordInvalida ,
    /* Nao existe essa corrdenada no tabuleiro*/
    
    TAB_CondRetErroEstrutura
    /* Erro na estutura tabuleiro */
    
} TAB_tpCondRet ;


/***********************************************************************
 *
 *  $TC Tipo de dados: TAB Modos de deturpar
 *
 *
 ***********************************************************************/

#ifdef _DEBUG

typedef enum {
    
    TAB_DeturpaTipoCabeca ,
    
    TAB_DeturpaPtCabecaNulo,
    /* faz voar anula a redundancia(cabeca) de uma casa aleatoria */
    
    TAB_DeturpaPtCabecaLixo,
    /*Altera ponteiro para a cabeca para lixo */
    
    TAB_DeturpaLinhaCorrente,
    /*Altera a linha corrente de uma pecaDaCasa aleatoria */
    
    TAB_DeturpaColunaCorrente,
    /*Altera a coluna corrente de uma pecaDaCasa aleatoria */
    
    TAB_DeturpaLinhaCasa,
    /*Altera a redundancia de linha(i) de uma casa aleatoria */
    
    TAB_DeturpaColunaCasa,
    /*Altera a redundancia de linha(j) de uma casa aleatoria */
    
    TAB_DeturpaListaAmeacantes,
    
    TAB_DeturpaListaAmeacados
    
} TAB_tpModosDeturpacao ;

#endif


/***********************************************************************
 *
 *  $FC Função: TAB  &Criar peca
 *
 *  $ED Descrição da função
 *     Cria uma peca para ser usada no jogo de xadrez.
 *
 *  $FV Valor retornado
 *     Se executou corretamente retorna o ponteiro para a peca.
 *     Este ponteiro será utilizado pelas funções que manipulam esta peca.
 
 *     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
 *     a função retornará NULL.
 *     Não será dada mais informação quanto ao problema ocorrido.
 *
 ***********************************************************************/

tppPeca* TAB_CriarPeca();


/***********************************************************************
 *
 *  $FC Função: TAB  &Criar Tabuleiro
 *
 *  $ED Descrição da função
 *     Cria um tabuleiro para um jogo de xadrez.
 *     Este tabuleiro eh uma matriz 8x8 do tipo casa de tabuleiro.
 *     Cada casa contem uma peca, uma lista de ameacantes e uma lista amecados.
 *     Essa funcao cria um tabuleiro que todas as casas contem pecas de
 *     tipo e cor vazio, e cria as listas vazias.
 *
 *  $FV Valor retornado
 *     Se executou corretamente retorna o ponteiro para o tabuleiro.
 *     Este ponteiro será utilizado pelas funções que manipulem este tabuleiro.
 *
 *     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
 *     a função retornará NULL.
 *     Não será dada mais informação quanto ao problema ocorrido.
 *
 ***********************************************************************/

tpTabuleiro* TAB_CriarTabuleiro();


/***********************************************************************
 *
 *  $FC Função: TAB  &Inserir peca
 *
 *  $ED Descrição da função
 *     Insere uma dada peca em uma casa de um tabuleiro.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro onde deve ser inserida a peca.
 *     linha - linha do tabuleiro para a insercao, valor de 0 a 7.
 *     coluna - coluna do tabuleiro para a insercao, valor de 0 a 7.
 *     peca - ponteiro para a peca a qual sera inserida.
 *
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetPecaInvalida
 *     TAB_CondRetCoordInvalida
 *
 ***********************************************************************/

TAB_tpCondRet TAB_InserirPeca(tpTabuleiro *t, int linha , int coluna, tppPeca* peca);


/***********************************************************************
 *
 *  $FC Função: TAB  &Obter peca
 *
 *  $ED Descrição da função
 *     Obtem a peca de uma dada casa de um tabuleiro.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro onde se quer a peca.
 *     linha - linha do tabuleirode onde se quer obter a peca,
 *     varia de 0 a 7.
 *     coluna - coluna do tabuleiro de onde se quer obter a peca,
 *     varia de 0 a 7.
 *
 *  $FV Valor retornado
 *     tppPeca - a peca que esta na dada casa, do tipo peca, nao eh um ponteiro.
 *
 ***********************************************************************/

tppPeca TAB_ObterPeca( tpTabuleiro *t, int linha, int coluna );


/***********************************************************************
 *
 *  $FC Função: TAB  &Retirar peca
 *
 *  $ED Descrição da função
 *     Retirar a peca de uma dada casa de um tabuleiro.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro onde se quer a peca.
 *     linha - linha do tabuleiro de onde se quer retirar a peca,
 *     varia de 0 a 7.
 *     coluna - coluna do tabuleiro de onde se quer retirar a peca,
 *     varia de 0 a 7.
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetCoordInvalida
 *
 ***********************************************************************/

TAB_tpCondRet TAB_RetirarPeca( tpTabuleiro *t, int linha, int coluna);


/***********************************************************************
 *
 *  $FC Função: TAB  &Mover peca
 *
 *  $ED Descrição da função
 *     Mover uma peca de uma dada casa de origem a uma
 *     de destino de um tabuleiro.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro onde se quer a peca.
 *     linhaOrigem - linha do tabuleiro aonde se localiza a peca a ser movida,
 *     varia de 0 a 7.
 *     colunaOrigem - coluna do tabuleiro aonde se localiza a peca a ser movida,
 *     varia de 0 a 7.
 *     linhaDestino - linha do tabuleiro destino da peca, varia de 0 a 7.
 *     colunaDestino - coluna do tabuleiro destino da peca, varia de 0 a 7.
 *
 *  $FV Valor retornado
 *     TAB_CondRetOK
 *     TAB_CondRetCoordInvalida
 *     TAB_CondRetMovimentoInvalido
 *     TAB_CondRetColisao
 *     TAB_CondRetOrigemSemPeca
 *
 ***********************************************************************/

TAB_tpCondRet TAB_MoverPeca( tpTabuleiro *t, int linhaOrigem, int colunaOrigem, int linhaDestino, int colunaDestino);


/***********************************************************************
 *
 *  $FC Função: TAB  &Obter Lista de Ameacantes
 *
 *  $ED Descrição da função
 *     Obtem o ponteiro para a cabeca de lista encadeada com
 *     as pecas que amecam uma dada casa.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro onde se quer a peca.
 *     linha - linha do tabuleiro de onde se quer obter a lista,
 *     varia de 0 a 7.
 *     coluna - coluna do tabuleiro de onde se quer obter a lista,
 *     varia de 0 a 7.
 *
 *  $FV Valor retornado
 *      LIS_tppLista - lista de ameacantes
 *
 ***********************************************************************/

LIS_tppLista TAB_ObterListaAmeacantes( tpTabuleiro * t, int linha, int coluna);


/***********************************************************************
 *
 *  $FC Função: TAB  &Obter Lista de Ameacados
 *
 *  $ED Descrição da função
 *     Obtem o ponteiro para a cabeca de lista encadeada com
 *     as pecas que sao ameacadas pela peca de um dada casa.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro onde se quer a peca.
 *     linha - linha do tabuleiro de onde se quer obter a lista,
 *     varia de 0 a 7.
 *     coluna - coluna do tabuleiro de onde se quer obter a lista,
 *     varia de 0 a 7.
 *
 *  $FV Valor retornado
 *      LIS_tppLista - lista de ameacados
 *
 ***********************************************************************/

LIS_tppLista TAB_ObterListaAmeacados( tpTabuleiro * t, int linha, int coluna);


/***********************************************************************
 *
 *  $FC Função: TAB  &Imprime Tabuleiro
 *
 *  $ED Descrição da função
 *     Imprime o tabuleiro com as pecas que estao nele.
 *
 *  $EP Parâmetros
 *     t - ponteiro para o tabuleiro onde se quer a peca.
 *
 *  $FV Valor retornado
 *      TAB_CondRetOK
 *
 ***********************************************************************/

TAB_tpCondRet TAB_ImprimeTabuleiro(tpTabuleiro *t);


/***********************************************************************
 *
 *  $FC Função: TAB  &Get Linha Corrente
 *
 *  $ED Descrição da função
 *     funcao que obtem a linha corrente de uma dada peca
 *
 *  $EP Parâmetros
 *     peca - um ponteiro para uma peca
 *
 *  $FV Valor retornado
 *      int - peca->linhaCorrente
 *
 ***********************************************************************/

int TAB_GetLinhaCorrente(tppPeca* peca);


/***********************************************************************
 *
 *  $FC Função: TAB  &Get Coluna Corrente
 *
 *  $ED Descrição da função
 *     funcao que obtem a coluna corrente de uma dada peca
 *
 *  $EP Parâmetros
 *     peca - um ponteiro para uma peca
 *
 *  $FV Valor retornado
 *      int - peca->colunaCorrente
 *
 ***********************************************************************/

int TAB_GetColunaCorrente(tppPeca* peca);


/***********************************************************************
 *
 *  $FC Função: TAB  &Get Cor
 *
 *  $ED Descrição da função
 *     funcao que obtem a cor de uma dada peca
 *
 *  $EP Parâmetros
 *     peca - um ponteiro para uma peca
 *
 *  $FV Valor retornado
 *      int - peca->cor
 *
 ***********************************************************************/

char TAB_GetCor(tppPeca* peca);


/***********************************************************************
 *
 *  $FC Função: TAB  &Get Tipo
 *
 *  $ED Descrição da função
 *     funcao que obtem o tipo de uma dada peca
 *
 *  $EP Parâmetros
 *     peca - um ponteiro para uma peca
 *
 *  $FV Valor retornado
 *      int - peca->tipo
 *
 ***********************************************************************/

char TAB_GetTipo(tppPeca* peca);


/***********************************************************************
 *
 *  $FC Função: TAB  &Set Linha Corrente
 *
 *  $ED Descrição da função
 *     funcao que edita a linha corrente de uma dada peca
 *
 *  $EP Parâmetros
 *     peca - um ponteiro para uma peca
 *
 *  $FV Valor retornado
 *       TAB_CondRetOK
 *
 ***********************************************************************/

TAB_tpCondRet TAB_SetLinhaCorrente(tppPeca* peca, int linhaCorrente);


/***********************************************************************
 *
 *  $FC Função: TAB  &Set Coluna Corrente
 *
 *  $ED Descrição da função
 *     funcao que edita a coluna corrente de uma dada peca
 *
 *  $EP Parâmetros
 *     peca - um ponteiro para uma peca
 *
 *  $FV Valor retornado
 *       TAB_CondRetOK
 *
 ***********************************************************************/

TAB_tpCondRet TAB_SetColunaCorrente(tppPeca* peca, int colunaCorrente);


/***********************************************************************
 *
 *  $FC Função: TAB  &Set Cor
 *
 *  $ED Descrição da função
 *     funcao que edita a cor de uma dada peca
 *
 *  $EP Parâmetros
 *     peca - um ponteiro para uma peca
 *
 *  $FV Valor retornado
 *       TAB_CondRetOK
 *
 ***********************************************************************/

TAB_tpCondRet TAB_SetCor(tppPeca* peca, char cor);


/***********************************************************************
 *
 *  $FC Função: TAB  &Set Tipo
 *
 *  $ED Descrição da função
 *     funcao que edita o tipo de uma dada peca
 *
 *  $EP Parâmetros
 *     peca - um ponteiro para uma peca
 *
 *  $FV Valor retornado
 *       TAB_CondRetOK
 *
 ***********************************************************************/

TAB_tpCondRet TAB_SetTipo(tppPeca* peca, char tipo);


/***********************************************************************
 *
 *  $FC Função: TAB  &Destruir Tabuleiro
 *
 *  $ED Descrição da função
 *     funcao que libera todas as listas  de todas casas do tabuleiro
 *     e libera o ponteiro pro tabuleiro
 *
 *  $EP Parâmetros
 *     t - um ponteiro para um tabuleiro
 *
 *
 ***********************************************************************/

void TAB_DestruirTabuleiro( tpTabuleiro* tabuleiro );


/***********************************************************************
 *
 *  $FC Função: TAB  &Verificar um tabuleiro
 *
 *  $ED Descrição da função
 *     Função da interface de teste.
 *     Verifica completamente um determinado tabueliro.
 *     Também marca todos os espaços por ele ocupados.
 *
 ***********************************************************************/

#ifdef _DEBUG

TAB_tpCondRet TAB_VerificarTabuleiro( tpTabuleiro* tabParam ) ;

#endif



/***********************************************************************
 *
 *  $FC Função: TAB  &Verificar uma cabeça de tabuleiro
 *
 *  $ED Descrição da função
 *     Função da interface de teste.
 *     Verifica a integridade de um a cabeca de tabueliro.
 *
 *  $EP Parâmetros
 *     $P pCabecaParm - ponteiro para um espaço que deveria ser uma cabeça
 *                      de tabueliro.
 *
 *  $FV Valor retornado
 *     Condição de retorno de teste
 *
 ***********************************************************************/

#ifdef _DEBUG

TAB_tpCondRet TAB_VerificarCabeca( tpTabuleiro* pCabecaParm ) ;

#endif



/***********************************************************************
 *
 *  $FC Função: TAB  &Verificar uma casa
 *
 *  $ED Descrição da função
 *     Função da interface de teste.
 *     Verifica a integridade de uma casa do tabuleiro.
 *
 *  $EP Parâmetros
 *     $P Casa - ponteiro para um espaço que deveria ser um elemento de lista.
 *
 *  $FV Valor retornado
 *     Condição de retorno de teste
 *
 ***********************************************************************/

#ifdef _DEBUG

TAB_tpCondRet TAB_VerificarCasa( tpCasa casaParm ) ;

#endif



/***********************************************************************
 *
 *  $FC Função: LIS  &Deturpar tabuleiro
 *
 *  $ED Descrição da função
 *     Função da interface de teste.
 *     Corrompe elementos específicos da estrutura do tabuleiro.
 *     Essa função destina-se a preparar os cenários de teste dos
 *     casos de teste utilizados ao testar os verificadores estruturais
 *     do tabuleiro.
 *     Esta função não tem proteção contra erros de uso, consequentemente
 *     poderá levar o programa a ser cancelado pelo sistema operacional.
 *
 *  $EP Parâmetros
 *     $P pTabuleiroParm  - tabueleiro a ser deturpada
 *     $P ModoDeturpar - identifica como deve ser feita a deturpação
 *                       TAB_tpModosDeturpacao identifica os modos de
 *                       deturpação conhecidos
 *
 ***********************************************************************/

#ifdef _DEBUG

void TAB_Deturpar( void * tabParm ,
                  TAB_tpModosDeturpacao ModoDeturpar ) ;

#endif


#undef TABULEIRO_EXT

/********** Fim do módulo de definição: TAB  Tabuleiro de xadrez  **********/

#else
#endif