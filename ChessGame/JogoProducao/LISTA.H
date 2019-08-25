#if ! defined( LISTA_ )

#define LISTA_

/***************************************************************************
 *
 *  $MCD Módulo de definição: LIS  Lista duplamente encadeada
 *
 *  Arquivo gerado:              LISTA.h
 *  Letras identificadoras:      LIS
 *
 *  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
 *  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
 *
 *  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
 *  Gestor:  LES/DI/PUC-Rio
 *  Autores: av
 *
 *  $HA Histórico de evolução:
 *     Versão  Autor    Data    Observações
 *     4       avs   01/fev/2006 criar linguagem script simbólica
 *     3       avs   08/dez/2004 uniformização dos exemplos
 *     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
 *     1       avs   16/abr/2003 início desenvolvimento
 *
 *  $ED Descrição do módulo
 *     Implementa listas genéricas duplamente encadeadas.
 *     Podem existir n listas em operação simultaneamente.
 *     As listas possuem uma cabeça encapsulando o seu estado.
 *
 *     Cada lista é homogênea quanto ao tipo dos dados que armazena.
 *     Cada elemento da lista referencia o valor que contém.
 *
 *     Os ponteiros para os dados são copiados para elementos da lista.
 *        Não é copiado o valor apontado por estes ponteiros.
 *
 *     O controle da destruição do valor de um elemento a ser excluído
 *        é realizado por uma função fornecida pelo usuário.
 *
 *     Cada lista referencia uma função que determina como devem ser
 *        desalocados os dados nela contidos.
 *
 *     A função de liberação dos valores contidos nos elementos deve
 *        assegurar a liberação de todos os espaços referênciados pelo
 *        valor contido em um elemento.
 *        Esta função é chamada antes de se desalocar um elemento
 *        de uma lista.
 *        Caso não seja necessário desalocar o valor referenciado pelo
 *        elemento, o ponteiro para a função de liberação poderá ser NULL .
 *        Caso o elemento da lista seja a única âncora do valor referenciado,
 *        esta função deve promover a destruição (free) desse valor e
 *        de todos os dados nele ancorados.
 *
 ***************************************************************************/



#if defined( LISTA_OWN )
    #define LISTA_EXT
#else
     #define LISTA_EXT extern
#endif



/***** Declarações exportadas pelo módulo *****/



/* Tipo referência para uma lista */



typedef struct LIS_tagLista * LIS_tppLista ;





/***********************************************************************
 *
 *  $TC Tipo de dados: LIS Condições de retorno
 *
 *
 *  $ED Descrição do tipo
 *     Condições de retorno das funções da lista
 *
 ***********************************************************************/



typedef enum {
    
    
    
    LIS_CondRetOK  = 1,
    
    /* Concluiu corretamente */
    
    
    
    LIS_CondRetListaVazia = 2,
    
    /* A lista não contém elementos */
    
    
    
    LIS_CondRetFimLista = 3,
    
    /* Foi atingido o fim de lista */
    
    
    
    LIS_CondRetNaoAchou = 4 ,
    
    /* Não encontrou o valor procurado */
    
    
    
    LIS_CondRetFaltouMemoria = 5,
    
    /* Faltou memória ao tentar criar um elemento de lista */
    
    LIS_CondRetErroEstrutura = 6
    
    /* Erro na estutura lista */
    
    
    
} LIS_tpCondRet ;


/***********************************************************************
*
*  $TC Tipo de dados: ARV Modos de deturpar
*
*
***********************************************************************/

#ifdef _DEBUG

   typedef enum {

         DeturpaFreeCorrente        =  1 ,
               /* Elemina(free) corrente sem atualizar arestas */

         DeturpaEliminaCorrente          =  2 ,
               /* Elemina(sem free) corrente sem atualizar arestas */

         DeturpaPtAntNulo      =  3 ,
               /* atribui NULL ao ponteiro irm�o � esquerda */

         DeturpaPtProxNulo          =  4 ,
               /* atribui NULL ao ponteiro irm�o � direita */

         DeturpaPtAntLixo      =  5 ,
               /* atribui ponteiro para lixo ao ponteiro irm�o � esquerda */

         DeturpaPtProxLixo            =  6 ,
               /* atribui ponteiro para lixo ao ponteiro irm�o � direita */

         DeturpaCorrenteNulo      =  7 ,
               /* atribui NULL ao ponteiro corrente */

         DeturpaCorrenteLixo         =  8 ,
               /* atribui ponteiro para lixo ao ponteiro corrente */

         DeturpaOrigemNula         =  9 ,
               /* atribui NULL ao ponteiro origem */

         DeturpaOrigemLixo         = 10 ,
               /* atribui ponteiro para lixo ao ponteiro origem */

         DeturpaPtValorNulo      = 11 ,
               /* atribui NULL ao ponteiro valor do n� corrente */

         DeturpaPtValorLixo         = 12 ,
               /* atribui ponteiro para lixo ao ponteiro valor do n� corrente */

         DeturpaFimNulo        = 13 ,
               /* atribui NULL ao ponteiro fim*/

         DeturpaFimLixo         = 14 ,
               /*  atribui ponteiro para lixo ao ponteiro fim */

         DeturpaTipoCabeca             = 15 ,
               /* Modifica o tipo da cabe�a */

         DeturpaPtCabecaNulo     = 16 ,
               /* atribui NULL ao ponteiro cabeca */

         DeturpaPtCabecaLixo         = 17,
               /*  atribui ponteiro para lixo ao ponteiro cabeca */
               
         DeturpaTipoElem         = 18
               /* Modifica o tipo do elemento corrente*/

   } LIS_tpModosDeturpacao ;

#endif


/***********************************************************************
 *
 *  $FC Função: LIS  &Criar lista
 *
 *  $ED Descrição da função
 *     Cria uma lista genérica duplamente encadeada.
 *     Os possíveis tipos são desconhecidos a priori.
 *     A tipagem é implicita.
 *     Não existe identificador de tipo associado à lista.
 *
 *  $EP Parâmetros
 *     ExcluirValor  - ponteiro para a função que processa a
 *                     exclusão do valor referenciado pelo elemento
 *                     a ser excluído.
 *                     Ver descrição do módulo.
 *
 *  $FV Valor retornado
 *     Se executou corretamente retorna o ponteiro para a lista.
 *     Este ponteiro será utilizado pelas funções que manipulem esta lista.
 *
 *     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
 *     a função retornará NULL.
 *     Não será dada mais informação quanto ao problema ocorrido.
 *
 ***********************************************************************/



LIS_tppLista LIS_CriarLista(
                            
                            void   ( * ExcluirValor ) ( void * pDado ) ) ;





/***********************************************************************
 *
 *  $FC Função: LIS  &Destruir lista
 *
 *  $ED Descrição da função
 *     Destrói a lista fornecida.
 *     O parâmetro ponteiro para a lista não é modificado.
 *     Se ocorrer algum erro durante a destruição, a lista resultará
 *     estruturalmente incorreta.
 *     OBS. não existe previsão para possíveis falhas de execução.
 *
 *  $FV Valor retornado
 *     LIS_CondRetOK    - destruiu sem problemas
 * 
 ***********************************************************************/



void LIS_DestruirLista( LIS_tppLista pLista ) ;





/***********************************************************************
 *
 *  $FC Função: LIS  &Esvaziar lista
 *
 *  $ED Descrição da função
 *     Elimina todos os elementos, sem contudo eliminar a lista
 *
 *  $EP Parâmetros
 *     pLista - ponteiro para a lista a ser esvaziada
 *
 ***********************************************************************/



void LIS_EsvaziarLista( LIS_tppLista pLista ) ;




/***********************************************************************
 *
 *  $FC Função: LIS  &Inserir elemento antes
 *
 *  $ED Descrição da função
 *     Insere novo elemento antes do elemento corrente.
 *     Caso a lista esteja vazia, insere o primeiro elemento da lista.
 *
 *  $EP Parâmetros
 *     pLista - ponteiro para a lista onde deve ser inserido o elemento
 *     pValor - ponteiro para o valor do novo elemento
 *              Pode ser NULL
 *
 *  $FV Valor retornado
 *     LIS_CondRetOK
 *     LIS_CondRetFaltouMemoria
 *
 ***********************************************************************/



LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                       
                                       void * pValor        ) ;





/**********************************************************************
 *
 *  $FC Função: LIS  &Inserir elemento após
 *
 *  $ED Descrição da função
 *     Insere novo elemento apás o elemento corrente.
 *     Caso a lista esteja vazia, insere o primeiro elemento da lista.
 *
 *  $EP Parâmetros
 *     Parâmetros
 *        pLista - ponteiro para a lista onde deve ser inserido o elemento
 *        pValor - ponteiro para o valor do novo elemento
 *                 Pode ser NULL
 *
 *
 *  $FV Valor retornado
 *     Valor retornado
 *        LIS_CondRetOK
 *        LIS_CondRetFaltouMemoria
 *
 ***********************************************************************/



LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                      
                                      void * pValor        )

;




/***********************************************************************
 *
 *  $FC Função: LIS  &Excluir elemento
 *
 *  $ED Descrição da função
 *     Exclui o elemento corrente da lista dada.
 *     Se existir o elemento aa esquerda do corrente será o novo corrente
 *     Se não existir e existir o elemento à direita, este se tornará corrente.
 *     Se este também não existir a lista tornou-se vazia.
 *
 *  $EP Parâmetros
 *     pLista    - ponteiro para a lista na qual deve excluir.
 *
 *  $FV Valor retornado
 *     LIS_CondRetOK
 *     LIS_CondRetListaVazi
 *
 ***********************************************************************/



LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista ) ;





/***********************************************************************
 *
 *  $FC Função: LIS  &Obter referência para o valor contido no elemento
 *
 *  $ED Descrição da função
 *     Obtem a referência para o valor contido no elemento corrente da lista
 *
 *  $EP Parâmetros
 *     pLista - ponteiro para a lista de onde se quer o valor
 *
 *  $FV Valor retornado
 *     não NULL - se o elemento corrente existe
 *     NULL     - se a lista estiver vazia
 *                Pode retornar NULL se o valor inserido no elemento for NULL.
 * 
 ***********************************************************************/



void * LIS_ObterValor( LIS_tppLista pLista ) ;




/***********************************************************************
 *
 *  $FC Função: LIS  &Ir para o elemento inicial
 *
 *  $ED Descrição da função
 *     Torna corrente o primeiro elemento da lista
 *     Faz nada se a lista está vazia.
 *
 *  $EP Parâmetros
 *     pLista - ponteiro para a lista a manipular
 *
 ***********************************************************************/



void LIS_IrInicioLista( LIS_tppLista pLista ) ;





/***********************************************************************
 *
 *  $FC Função: LIS  &Ir para o elemento final
 *
 *  $ED Descrição da função
 *     Torna corrente o elemento final da lista.
 *     Faz nada se a lista está vazia.
 *
 *  $EP Parâmetros
 *     pLista - ponteiro para a lista a manipular
 * 
 ***********************************************************************/



void LIS_IrFinalLista( LIS_tppLista pLista ) ;





/***********************************************************************
 *
 *  $FC Função: LIS  &Avançar elemento
 *
 *  $ED Descrição da função
 *     Avança o elemento corrente numElem elementos na lista
 *     Se numElem for positivo avança em direção ao final
 *     Se numElem for negativo avança em direção ao início
 *     numElem pode ser maior do que o númro de elementos existentes na
 *               direção desejada
 *     Se numElem for zero somente verifica se a lista está vazia
 *
 *  $EP Parâmetros
 *     pLista  - ponteiro para a lista a ser manipulada
 *     numElem - o número de elementos a andar
 *
 *  $FV Valor retornado
 *     CondRetOK         - se numElem elementos tiverem sido andados
 *     CondRetFimLista   - se encontrou o fim da lista antes de andar numElem
 *                         elementos
 *     CondRetListaVazia - se a lista está vazia
 *
 ***********************************************************************/



LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                          
                                          int numElem ) ;





/***********************************************************************
 *
 *  $FC Função: LIS  &Procurar elemento contendo valor
 *
 *  $ED Descrição da função
 *     Procura o elemento que referencia o valor dado.
 *     A função compara ponteiro e não conteúdo apontado.
 *
 *  $EP Parâmetros
 *     pLista  - ponteiro para a lista onde procura
 *     pValor  - ponteiro para o valor procurado
 *               Pode ser NULL
 *
 *  $FV Valor retornado
 *     LIS_CondRetOK  - se encontrou.
 *                      O elemento corrente é o primeiro elemento do
 *                      elemento corrente inclusive para o fim da lista
 *                      e que contém o ponteiro procurado
 *
 *     LIS_CondRetNaoEncontrou - se o ponteiro não foi encontrado
 *                      O elemento corrente continua o mesmo
 *     LIS_CondRetListaVazia   - se a lista estiver vazia
 *
 ***********************************************************************/



LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista ,
                                
                                void * pValor        ) ;





/***********************************************************************
 *
 *  $FC Função: LIS  &Get Numero De Elementos
 *
 *  $ED Descrição da função
 *     Obtem o numero de elementos da lista
 *
 *  $EP Parâmetros
 *     pLista  - ponteiro para a lista de onde se quer o dado
 *
 *  $FV Valor retornado
 *     int - numeros de elementos contidos na lista
 *
 ***********************************************************************/



int LIS_GetNumElem(LIS_tppLista pLista);


/***********************************************************************
*
*  $FC Fun��o: LIS  &Verificar uma Lista
*
*  $ED Descri��o da fun��o
*     Fun��o da interface de teste.
*     Verifica completamente uma determinada lista.
*     Tamb�m marca todos os espa�os por ela ocupados.
*
***********************************************************************/

#ifdef _DEBUG

   LIS_tpCondRet LIS_VerificarLista( void * pLista ) ;

#endif



/***********************************************************************
*
*  $FC Fun��o: LIS  &Verificar um cabe�a da lista
*
*  $ED Descri��o da fun��o
*     Fun��o da interface de teste.
*     Verifica a integridade de um a cabeca da lista.
*
*  $EP Par�metros
*     $P pCabecaParm - ponteiro para um espa�o que deveria ser uma cabe�a
*                      de lista.
*
*  $FV Valor retornado
*     Condi��o de retorno de teste
*
***********************************************************************/

#ifdef _DEBUG

   LIS_tpCondRet LIS_VerificarCabeca( void * pCabecaParm ) ;

#endif



/***********************************************************************
*
*  $FC Fun��o: LIS  &Verificar um elemento
*
*  $ED Descri��o da fun��o
*     Fun��o da interface de teste.
*     Verifica a integridade de um elemento de lista.
*
*  $EP Par�metros
*     $P pElem - ponteiro para um espa�o que deveria ser um elemento de lista.
*
*  $FV Valor retornado
*     Condi��o de retorno de teste
*
***********************************************************************/

#ifdef _DEBUG

   LIS_tpCondRet LIS_VerificarElemLista( void * pElem ) ;

#endif



/***********************************************************************
*
*  $FC Fun��o: LIS  &Deturpar lista
*
*  $ED Descri��o da fun��o
*     Fun��o da interface de teste.
*     Corrompe elementos espec�ficos da estrutura da lista.
*     Essa fun��o destina-se a preparar os cen�rios de teste dos
*     casos de teste utilizados ao testar os verificadores estruturais
*     da lista.
*     Esta fun��o n�o tem prote��o contra erros de uso, consequentemente
*     poder� levar o programa a ser cancelado pelo sistema operacional.
*
*  $EP Par�metros
*     $P pListaParm  - lista a ser deturpada
*     $P ModoDeturpar - identifica como deve ser feita a deturpa��o
*                       LIS_tpModosDeturpacao identifica os modos de
*                       deturpa��o conhecidos
*
***********************************************************************/

#ifdef _DEBUG

   void LIS_Deturpar( void * pListaParam ,
                      LIS_tpModosDeturpacao ModoDeturpar ) ;

#endif


#undef LISTA_EXT



/********** Fim do módulo de definição: LIS  Lista duplamente encadeada **********/

#else

#endif
