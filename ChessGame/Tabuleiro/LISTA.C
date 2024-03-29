/*************************************************************************** 
 *  $MCI Módulo de implementação: LIS  Lista duplamente encadeada
 *
 *  Arquivo gerado:              LISTA.c
 *  Letras identificadoras:      LIS
 *
 *  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
 *  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
 *
 *  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
 *  Gestor:  LES/DI/PUC-Rio
 *  Autores: avs
 *
 *  $HA Histórico de evolução:
 *     Versão  Autor    Data     Observações
 *     4       avs   01/fev/2006 criar linguagem script simbólica
 *     3       avs   08/dez/2004 uniformização dos exemplos
 *     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
 *     1       avs   16/abr/2003 início desenvolvimento
 *
 ***************************************************************************/



#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <stdlib.h>
#include   <assert.h>
#include   <malloc.h>

#define LISTA_OWN
#include "lista.h"
#undef LISTA_OWN


#ifdef _DEBUG
   #include   "Generico.h"
   #include   "Conta.h"
   #include   "cespdin.h"
   #include   "IdTiposEspaco.def"
#endif





/***********************************************************************
 *
 *  $TC Tipo de dados: LIS Elemento da lista
 *
 ***********************************************************************/



typedef struct tagElemLista {
    
    #ifdef _DEBUG

    struct tgLista * pCabeca ;
    
    /* Ponteiro para a cabeca da lista
    *
    *$ED Descri��o
    *   Todos os elementos da lista apontar para a respectiva lista.
    *   Esse ponteiro corresponde a um identificador da lista para fins
    *   de verifica��o da integridade. */

    #endif
    
    void * pValor ;
    
    /* Ponteiro para o valor contido no elemento */
    
    
    
    struct tagElemLista * pAnt ;
    
    /* Ponteiro para o elemento predecessor */
    
    
    
    struct tagElemLista * pProx ;
    
    /* Ponteiro para o elemento sucessor */
    
    
    
} tpElemLista ;



/***********************************************************************
 *
 *  $TC Tipo de dados: LIS Descritor da cabeça de lista
 *
 *
 ***********************************************************************/



typedef struct LIS_tagLista {
    
    
    
    tpElemLista * pOrigemLista ;
    
    /* Ponteiro para a origem da lista */
    
    
    
    tpElemLista * pFimLista ;
    
    /* Ponteiro para o final da lista */
    
    
    
    tpElemLista * pElemCorr ;
    
    /* Ponteiro para o elemento corrente da lista */
    
    
    
    int numElem ;
    
    /* Número de elementos da lista */
    
    
    
    void ( * ExcluirValor ) ( void * pValor ) ;
    
    /* Ponteiro para a função de destruição do valor contido em um elemento */
    
    
    
} LIS_tpLista ;


/*****  Dados encapsulados no m�dulo  *****/

      #ifdef _DEBUG

      static char EspacoLixo[ 256 ] =
             "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ;
            /* Espa�o de dados lixo usado ao testar */

      #endif

/***** Protótipos das funções encapuladas no módulo *****/



static void LiberarElemento( LIS_tppLista   pLista ,
                            
                            tpElemLista  * pElem   ) ;



static tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                   
                                   void *       pValor  ) ;



static void LimparCabeca( LIS_tppLista pLista ) ;



#ifdef _DEBUG

   static LIS_tpCondRet VerificarElemLista( tpElemLista * pElem ) ;

#endif


/*****  Código das funções exportadas pelo módulo  *****/



/***************************************************************************
 *
 *  Função: LIS  &Criar lista
 *  ****/



LIS_tppLista LIS_CriarLista(
                            
                            void   ( * ExcluirValor ) ( void * pDado ) )

{
       LIS_tpLista * pLista = NULL ; 
       
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_CriarLista" ) ;
    #endif
    

    
    
    
    pLista = ( LIS_tpLista * ) malloc( sizeof( LIS_tpLista )) ;
    
    if ( pLista == NULL )
        
    {
        
        return NULL ;
        
    } /* if */
    
    
    
    LimparCabeca( pLista ) ;
    
    #ifdef _DEBUG
         CED_DefinirTipoEspaco( pLista , LIS_TipoEspacoCabeca ) ;
    #endif
    
    pLista->ExcluirValor = ExcluirValor ;
    
    
    
    return pLista ;
    
    
    
} /* Fim função: LIS  &Criar lista */



/***************************************************************************
 *
 *  Função: LIS  &Destruir lista
 *  ****/



void LIS_DestruirLista( LIS_tppLista pLista )

{
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_DestruirLista" ) ;
    #endif
    
#ifdef _DEBUG
    
    assert( pLista != NULL ) ;
    
#endif
    
    
    
    LIS_EsvaziarLista( pLista ) ;
    
    
    
    free( pLista ) ;
    
    
    
} /* Fim função: LIS  &Destruir lista */



/***************************************************************************
 *
 *  Função: LIS  &Esvaziar lista
 *  ****/



void LIS_EsvaziarLista( LIS_tppLista pLista )

{
    
    
    
    tpElemLista * pElem ;
    
    tpElemLista * pProx ;
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_EsvaziarLista" ) ;
    #endif
    
#ifdef _DEBUG
    
    assert( pLista != NULL ) ;
    
#endif
    
    
    
    pElem = pLista->pOrigemLista ;
    
    while ( pElem != NULL )
        
    {
        
        pProx = pElem->pProx ;
        
        LiberarElemento( pLista , pElem ) ;
        
        pElem = pProx ;
        
    } /* while */
    
    
    
    LimparCabeca( pLista ) ;
    
    
    
} /* Fim função: LIS  &Esvaziar lista */



/***************************************************************************
 *
 *  Função: LIS  &Inserir elemento antes
 *  ****/



LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                       
                                       void * pValor        )

{
    
    
    
    tpElemLista * pElem ;
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_InserirElementoAntes" ) ;
    #endif
    
#ifdef _DEBUG
    
    assert( pLista != NULL ) ;
    
#endif
    
    
    
    /* Criar elemento a inerir antes */
    
    
    
    pElem = CriarElemento( pLista , pValor ) ;
    
    if ( pElem == NULL )
        
    {
        
        return LIS_CondRetFaltouMemoria ;
        
    } /* if */
    
    
    
    /* Encadear o elemento antes do elemento corrente */
    
    
    
    if ( pLista->pElemCorr == NULL )
        
    {
        
        pLista->pOrigemLista = pElem ;
        
        pLista->pFimLista = pElem ;
        
    } else
        
    {
        
        if ( pLista->pElemCorr->pAnt != NULL )
            
        {
            
            pElem->pAnt  = pLista->pElemCorr->pAnt ;
            
            pLista->pElemCorr->pAnt->pProx = pElem ;
            
        } else
            
        {
            
            pLista->pOrigemLista = pElem ;
            
        } /* if */
        
        
        
        pElem->pProx = pLista->pElemCorr ;
        
        pLista->pElemCorr->pAnt = pElem ;
        
    } /* if */
    
    
    
    pLista->pElemCorr = pElem ;
    
    
    
    return LIS_CondRetOK ;
    
    
    
} /* Fim função: LIS  &Inserir elemento antes */



/***************************************************************************
 *
 *  Função: LIS  &Inserir elemento após
 *  ****/



LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                      
                                      void * pValor        )



{
    tpElemLista * pElem ;
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_InserirElementoApos" ) ;
    #endif
    
    
    
    
    
#ifdef _DEBUG
    
    assert( pLista != NULL ) ;
    
#endif
    
    
    
    /* Criar elemento a inerir após */
    
    
    
    pElem = CriarElemento( pLista , pValor ) ;
    
    if ( pElem == NULL )
        
    {
        
        return LIS_CondRetFaltouMemoria ;
        
    } /* if */
    
    
    
    /* Encadear o elemento após o elemento */
    
    
    
    if ( pLista->pElemCorr == NULL )
        
    {
        
        pLista->pOrigemLista = pElem ;
        
        pLista->pFimLista = pElem ;
        
    } else
        
    {
        
        if ( pLista->pElemCorr->pProx != NULL )
            
        {
            
            pElem->pProx  = pLista->pElemCorr->pProx ;
            
            pLista->pElemCorr->pProx->pAnt = pElem ;
            
        } else
            
        {
            
            pLista->pFimLista = pElem ;
            
        } /* if */
        
        
        
        pElem->pAnt = pLista->pElemCorr ;
        
        pLista->pElemCorr->pProx = pElem ;
        
        
        
    } /* if */
    
    
    
    pLista->pElemCorr = pElem ;
    
    
    
    return LIS_CondRetOK ;
    
    
    
} /* Fim função: LIS  &Inserir elemento após */



/***************************************************************************
 *
 *  Função: LIS  &Excluir elemento
 *  ****/



LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )

{
    
    
    
    tpElemLista * pElem ;
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_ExcluirElemento" ) ;
    #endif
    
#ifdef _DEBUG
    
    assert( pLista  != NULL ) ;
    
#endif
    
    
    
    if ( pLista->pElemCorr == NULL )
        
    {
        
        return LIS_CondRetListaVazia ;
        
    } /* if */
    
    
    
    pElem = pLista->pElemCorr ;
    
    
    
    /* Desencadeia à esquerda */
    
    
    
    if ( pElem->pAnt != NULL )
        
    {
        
        pElem->pAnt->pProx   = pElem->pProx ;
        
        pLista->pElemCorr    = pElem->pAnt ;
        
    } else {
        
        pLista->pElemCorr    = pElem->pProx ;
        
        pLista->pOrigemLista = pLista->pElemCorr ;
        
    } /* if */
    
    
    
    /* Desencadeia à direita */
    
    
    
    if ( pElem->pProx != NULL )
        
    {
        
        pElem->pProx->pAnt = pElem->pAnt ;
        
    } else
        
    {
        
        pLista->pFimLista = pElem->pAnt ;
        
    } /* if */
    
    
    
    LiberarElemento( pLista , pElem ) ;
    
    
    
    return LIS_CondRetOK ;
    
    
    
} /* Fim função: LIS  &Excluir elemento */



/***************************************************************************
 *
 *  Função: LIS  &Obter referência para o valor contido no elemento
 *  ****/



void * LIS_ObterValor( LIS_tppLista pLista )

{
    
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_ObterValor" ) ;
    #endif
    
#ifdef _DEBUG
    
    assert( pLista != NULL ) ;
    
#endif
    
    
    
    if ( pLista->pElemCorr == NULL )
        
    {
        
        return NULL ;
        
    } /* if */
    
    
    
    return pLista->pElemCorr->pValor ;
    
    
    
} /* Fim função: LIS  &Obter referência para o valor contido no elemento */



/***************************************************************************
 
 *
 
 *  Função: LIS  &Ir para o elemento inicial
 
 *  ****/



void LIS_IrInicioLista( LIS_tppLista pLista )

{
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_IrInicioLista" ) ;
    #endif
    
#ifdef _DEBUG
    
    assert( pLista != NULL ) ;
    
#endif
    
    
    
    pLista->pElemCorr = pLista->pOrigemLista ;
    
    
    
} /* Fim função: LIS  &Ir para o elemento inicial */



/***************************************************************************
 *
 *  Função: LIS  &Ir para o elemento final
 *  ****/



void LIS_IrFinalLista( LIS_tppLista pLista )

{
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_IrFinalLista" ) ;
    #endif
    
#ifdef _DEBUG
    
    assert( pLista != NULL ) ;
    
#endif
    
    
    
    pLista->pElemCorr = pLista->pFimLista ;
    
    
    
} /* Fim função: LIS  &Ir para o elemento final */



/***************************************************************************
 *
 *  Função: LIS  &Avançar elemento
 *  ****/



LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                          
                                          int numElem )

{
    
    
    
    int i ;
    
    
    
    tpElemLista * pElem ;
    
    #ifdef _DEBUG
         CNT_CONTAR( "LIS_AvancarElementoCorrente" ) ;
    #endif
    
#ifdef _DEBUG
    
    assert( pLista != NULL ) ;
    
#endif
    
    
    
    /* Tratar lista vazia */
    
    
    
    if ( pLista->pElemCorr == NULL )
        
    {
        
        
        
        return LIS_CondRetListaVazia ;
        
        
        
    } /* fim ativa: Tratar lista vazia */
    
    
    
    /* Tratar avançar para frente */
    
    
    
    if ( numElem > 0 )
        
    {
        
        
        
        pElem = pLista->pElemCorr ;
        
        for( i = numElem ; i > 0 ; i-- )
            
        {
            
            if ( pElem == NULL )
                
            {
                
                break ;
                
            } /* if */
            
            pElem    = pElem->pProx ;
            
        } /* for */
        
        
        
        if ( pElem != NULL )
            
        {
            
            pLista->pElemCorr = pElem ;
            
            return LIS_CondRetOK ;
            
        } /* if */
        
        
        
        pLista->pElemCorr = pLista->pFimLista ;
        
        return LIS_CondRetFimLista ;
        
        
        
    } /* fim ativa: Tratar avançar para frente */
    
    
    
    /* Tratar avançar para trás */
    
    
    
    else if ( numElem < 0 )
        
    {
        
        
        
        pElem = pLista->pElemCorr ;
        
        for( i = numElem ; i < 0 ; i++ )
            
        {
            
            if ( pElem == NULL )
                
            {
                
                break ;
                
            } /* if */
            
            pElem    = pElem->pAnt ;
            
        } /* for */
        
        
        
        if ( pElem != NULL )
            
        {
            
            pLista->pElemCorr = pElem ;
            
            return LIS_CondRetOK ;
            
        } /* if */
        
        
        
        pLista->pElemCorr = pLista->pOrigemLista ;
        
        return LIS_CondRetFimLista ;
        
        
        
    } /* fim ativa: Tratar avançar para trás */
    
    
    
    /* Tratar não avançar */
    
    
    
    return LIS_CondRetOK ;
    
    
    
} /* Fim função: LIS  &Avançar elemento */



/***************************************************************************
 *
 *  Função: LIS  &Procurar elemento contendo valor
 *  ****/



LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista ,
                                
                                void * pValor        )

{ 
    
    tpElemLista * pElem ;
    
 #ifdef _DEBUG
         CNT_CONTAR( "LIS_ProcurarValor" ) ;
    #endif   
    
#ifdef _DEBUG
    
    assert( pLista  != NULL ) ;
    
#endif
    
    
    
    if ( pLista->pElemCorr == NULL )
        
    {
        
        return LIS_CondRetListaVazia ;
        
    } /* if */
    
    
    
    for ( pElem  = pLista->pElemCorr ;
         
         pElem != NULL ;
         
         pElem  = pElem->pProx )
        
    {
        
        if ( pElem->pValor == pValor )
            
        {
            
            pLista->pElemCorr = pElem ;
            
            return LIS_CondRetOK ;
            
        } /* if */
        
    } /* for */
    
    
    
    return LIS_CondRetNaoAchou ;
    
    
    
} /* Fim função: LIS  &Procurar elemento contendo valor */





/***************************************************************************
 *
 *  Função: LIS  &Get Numeros de elementos
 *  ****/



int LIS_GetNumElem(LIS_tppLista pLista)

{
    
    return pLista->numElem;
    
    
    
} /* Fim função: LIS  &Get Numero de elementos */

#ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: LIS  &Verificar uma lista
*  ****/

   LIS_tpCondRet LIS_VerificarLista( void * pListaParm ) 
   {

      LIS_tpLista * pLista = NULL ;

      if ( LIS_VerificarCabeca( pListaParm ) != LIS_CondRetOK )
      {

         return LIS_CondRetErroEstrutura ;
      } /* if */

      CED_MarcarEspacoAtivo( pListaParm ) ;

      pLista = ( LIS_tpLista * ) ( pListaParm ) ;


      return VerificarElemLista( pLista->pElemCorr ) ;

   } /* Fim fun��o: LIS  &Verificar uma lista */

#endif

#ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: LIS  &Verificar a cabe�a da lista
*  ****/

   LIS_tpCondRet LIS_VerificarCabeca( void * pCabecaParm )
   {

      LIS_tpLista * pLista = NULL ;

      /* Verifica o tipo do espa�o */

         if ( pCabecaParm == NULL )
         {
            TST_NotificarFalha( "Tentou verificar cabe�a inexistente." ) ;
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador1" ) ;
            #endif
            return LIS_CondRetErroEstrutura ;
         } /* if */

    /*     if ( ! CED_VerificarEspaco( pCabecaParm , NULL ))
         {
            TST_NotificarFalha( "Controle do espa�o acusou erro." ) ;
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador2" ) ;
            #endif
            return LIS_CondRetErroEstrutura ;
         } /* if */

         if ( TST_CompararInt( LIS_TipoEspacoCabeca ,
              CED_ObterTipoEspaco( pCabecaParm ) ,
              "Tipo do espa�o de dados n�o � cabe�a de lista." ) != TST_CondRetOK )
         {
             #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador3" ) ;
            #endif
            return LIS_CondRetErroEstrutura ;
         } /* if */

         pLista = ( LIS_tpLista * )( pCabecaParm ) ;

      /* Verifica o elemento origem */

         if ( pLista->pOrigemLista != NULL )
         {
            if ( TST_CompararPonteiro( pCabecaParm , pLista->pOrigemLista->pCabeca ,
                 "Elemento origem n�o aponta para cabe�a." ) != TST_CondRetOK )
            {
               #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador4" ) ;
               #endif
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador4.5" ) ;
             #endif
         } else {
            if ( TST_CompararPonteiro( NULL , pLista->pElemCorr ,
                 "Lista vazia tem elemento corrente n�o NULL." ) != TST_CondRetOK )
            {
                 #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador5" ) ;
             #endif
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador5.5" ) ;
             #endif
         } /* if */

         /* Verifica o elemento fim */

         if ( pLista->pFimLista != NULL )
         {
            if ( TST_CompararPonteiro( pCabecaParm , pLista->pFimLista->pCabeca ,
                 "Elemento fim n�o aponta para cabe�a." ) != TST_CondRetOK )
            {
               #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador6" ) ;
               #endif
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador6.5" ) ;
             #endif
         } else {
            if ( TST_CompararPonteiro( NULL , pLista->pElemCorr ,
                 "Lista vazia tem elemento corrente n�o NULL." ) != TST_CondRetOK )
            {
               #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador7" ) ;
             #endif  
               return LIS_CondRetErroEstrutura ;
            } /* if */
             #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador7.5" ) ;
             #endif
         } /* if */

      /* Verifica corrente */

         if ( pLista->pElemCorr != NULL )
         {
            if ( TST_CompararPonteiro( pCabecaParm , pLista->pElemCorr->pCabeca ,
                 "Elemento corrente n�o aponta para cabe�a." ) != TST_CondRetOK )
            {
               #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador8" ) ;
             #endif  
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador8.5" ) ;
             #endif
         } else {
            if ( TST_CompararPonteiro( NULL , pLista->pOrigemLista ,
                 "Lista nao vazia tem elemento corrente NULL." ) != TST_CondRetOK )
            {
               #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador9" ) ;
             #endif  
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador9.5" ) ;
             #endif
         } /* if */

         #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador10" ) ;
         #endif
      return LIS_CondRetOK ;

   } /* Fim fun��o: LIS  &Verificar cabeca*/

#endif

#ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: ARV  &Verificar um elemento de estrutura
*  ****/

   LIS_tpCondRet LIS_VerificarElem( void * pElemParm )
   {

      tpElemLista * pElem     = NULL ;
      LIS_tpLista   * pLista = NULL ;

      /* Verificar se � elemento estrutural */

         if ( pElemParm == NULL )
         {
            TST_NotificarFalha( "Tentou verificar elemento inexistente." ) ;
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador11" ) ;
            #endif
            return LIS_CondRetErroEstrutura ;

         } /* if */

         if ( ! CED_VerificarEspaco( pElemParm , NULL ))
         {
            TST_NotificarFalha( "Controle do espa�o acusou erro." ) ;
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador12" ) ;
         #endif
            return LIS_CondRetErroEstrutura ;
         } /* if */

         if ( TST_CompararInt( LIS_TipoEspacoElemento ,
              CED_ObterTipoEspaco( pElemParm ) ,
              "Tipo do espa�o de dados n�o � elemento de lista." ) != TST_CondRetOK )
         {
              #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador13" ) ;
              #endif
            return LIS_CondRetErroEstrutura ;
         } /* if */
         
         pElem     = ( tpElemLista * )( pElemParm ) ;
         pLista = (LIS_tpLista *)pElem->pCabeca ;
         
         /*Verificar pValor */
         
         if ( pElem->pValor == NULL )
         {
            TST_NotificarFalha( "Tentou verificar elemento com valor nulo." ) ;
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador14" ) ;
            #endif
            return LIS_CondRetErroEstrutura ;

         } /* if */

      /* Verificar cabe�a */

         if ( pLista->pOrigemLista != NULL  )
         {
            if ( TST_CompararPonteiro( pLista , pLista->pOrigemLista->pCabeca ,
                 "Elemento n�o pertence � lista." ) != TST_CondRetOK )
            {
                 #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador15" ) ;
                #endif
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador15.5" ) ;
            #endif
         } else
         {
             #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador16" ) ;
             #endif  
            return TST_NotificarFalha( "Elemento pertence a lista vazia." ) ;
         } /* if */

      /* Verificar do elemento proximo */

         if ( pElem->pProx != NULL )
         {
            if ( TST_CompararPonteiro( pElem , pElem->pProx->pAnt ,
                 "Anterior de proximo n�o � este elemento." ) != TST_CondRetOK )
            {
                #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador17" ) ;
                #endif 
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador17.5" ) ;
            #endif
         } else
         {
            if ( TST_CompararPonteiro( pElem , pLista->pFimLista ,
                 "Elemento n�o tem proximo e nao eh fim da lista." ) != TST_CondRetOK )
            {
               #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador18" ) ;
               #endif  
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador18.5" ) ;
            #endif  
         } /* if */

      /* Verificar do elemteno anterior */

         if ( pElem->pAnt != NULL )
         {
            if ( TST_CompararPonteiro( pElem , pElem->pAnt->pProx ,
                 "Proximo de anterior n�o � este elemento." ) != TST_CondRetOK )
            {
               #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador19" ) ;
               #endif    
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador19.5" ) ;
            #endif  
         } else
         {
            if ( TST_CompararPonteiro( pElem , pLista->pOrigemLista ,
                 "Elemento n�o tem anterior e nao eh origem da lista." ) != TST_CondRetOK )
            {
                #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador20" ) ;
               #endif   
               return LIS_CondRetErroEstrutura ;
            } /* if */
            #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador20.5" ) ;
            #endif  
         } /* if */

      #ifdef _DEBUG
                   CNT_CONTAR( "LIS_Verificador21" ) ;
      #endif     
      return LIS_CondRetOK ;

   } /* Fim fun��o: LIS  &Verificar um elemento da lista */

#endif

#ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: LIS  &Deturpar lista
*  ****/

   void LIS_Deturpar( void * pListaParm ,
                      LIS_tpModosDeturpacao ModoDeturpar )
   {

      LIS_tpLista * pLista = NULL ;
      tpElemLista *pElemAux = NULL;

      if ( pListaParm == NULL )
      {
         return ;
      } /* if */

      pLista = ( LIS_tpLista * )( pListaParm ) ;

      switch ( ModoDeturpar )
      {

      /* Modifica o tipo da cabe�a */

         case DeturpaTipoCabeca :
         {

            CED_DefinirTipoEspaco( pLista , CED_ID_TIPO_VALOR_NULO ) ;

            break ;

         } /* fim ativa: Modifica o tipo da cabe�a */

      /* Anula ponteiro origem */

         case DeturpaOrigemNula :
         {

            pLista->pOrigemLista = NULL ;

            break ;

         } /* fim ativa: Anula ponteiro origem */

      /* Anula ponteiro fim*/

         case DeturpaFimNulo :
         {

            pLista->pFimLista = NULL ;

            break ;

         } /* fim ativa: Anula ponteiro corrente */
         
      /* Anula ponteiro corrente */

         case DeturpaCorrenteNulo :
         {

            pLista->pElemCorr = NULL ;

            break ;

         } /* fim ativa: Anula ponteiro corrente */

      /* Faz origem apontar para lixo */

         case DeturpaOrigemLixo :
         {

            pLista->pOrigemLista = ( tpElemLista * )( EspacoLixo ) ;

            break ;

         } /* fim ativa: Faz Origem apontar para lixo */

      /* Faz fim apontar para lixo */

         case DeturpaFimLixo :
         {

            pLista->pFimLista = ( tpElemLista * )( EspacoLixo ) ;

            break ;

         } /* fim ativa: Faz fim apontar para lixo */

      /* Faz corrente apontar para lixo */

         case DeturpaCorrenteLixo :
         {

            pLista->pElemCorr = ( tpElemLista * )( EspacoLixo ) ;

            break ;

         } /* fim ativa: Faz corrente apontar para lixo */


      /* Deturpa n�o */

         default :

         if ( pLista->pElemCorr != NULL )
         {

            switch ( ModoDeturpar ) {

            /* Modifica tipo elemento corrente */

               case DeturpaTipoElem :
               {

                  CED_DefinirTipoEspaco( pLista->pElemCorr , CED_ID_TIPO_VALOR_NULO ) ;

                  break ;

               } /* fim ativa: Modifica tipo elemetnto corrente */

            /* Anula ponteiro cabe�a */

               case DeturpaPtCabecaNulo :
               {

                  pLista->pElemCorr->pCabeca = NULL ;

                  break ;

               } /* fim ativa: Anula ponteiro cabe�a */

            /* Anula ponteiro proximo */

               case DeturpaPtProxNulo :
               {

                  pLista->pElemCorr->pProx = NULL ;

                  break ;

               } /* fim ativa: Anula ponteiro prox */

            /* Anula ponteiro anterior */

               case DeturpaPtAntNulo :
               {

                  pLista->pElemCorr->pAnt = NULL ;

                  break ;

               } /* fim ativa: Anula ponteiro anterior */

            /* Anula ponteiro valor */

               case DeturpaPtValorNulo :
               {

                  pLista->pElemCorr->pValor = NULL ;

                  break ;

               } /* fim ativa: Anula ponteiro valor */

            /* Faz ponteiro cabe�a apontar para lixo */

               case DeturpaPtCabecaLixo :
               {

                  pLista->pElemCorr->pCabeca = ( LIS_tpLista* )( EspacoLixo ) ;

                  break ;

               } /* fim ativa: Faz ponteiro cabe�a apontar para lixo */

            /* Faz ponteiro proximo apontar para lixo */

               case DeturpaPtProxLixo :
               {

                  pLista->pElemCorr->pProx = ( tpElemLista * )( EspacoLixo ) ;

                  break ;

               } /* fim ativa: Faz ponteiro proximo apontar para lixo */

            /* Faz ponteiro anterior apontar para lixo */

               case DeturpaPtAntLixo :
               {

                  pLista->pElemCorr->pAnt = ( tpElemLista * )( EspacoLixo ) ;

                  break ;

               } /* fim ativa: Faz ponteiro anterior apontar para lixo */

            /* Faz ponteiro valor apontar para lixo */

               case DeturpaPtValorLixo :
               {

                  pLista->pElemCorr->pValor = ( tpElemLista * )( EspacoLixo ) ;

                  break ;

               } /* fim ativa: Faz ponteiro valor apontar para lixo */
               
               /* Elemina(free) corrente sem atualizar arestas */

               case DeturpaFreeCorrente :
               {
                  if(pLista->pElemCorr->pProx != NULL)
                  {
                                              
                        pElemAux = pLista->pElemCorr->pProx ;                      
                        free(pLista->pElemCorr) ;
                         pLista->pElemCorr = CriarElemento(pLista, pElemAux->pValor);
                        /*pLista->pElemCorr = pElemAux;*/    
                  }
                  else if(pLista->pElemCorr->pAnt != NULL)
                  {
                        pElemAux = pLista->pElemCorr->pAnt ;                      
                        free(pLista->pElemCorr) ;
                        pLista->pElemCorr = CriarElemento(pLista, pElemAux->pValor);
                        /*pLista->pElemCorr = pElemAux;*/
                  }
                  else
                  {
                        free(pLista->pElemCorr) ;   
                  }
                  
                  break ;

               } /* fim ativa: Elemina(free) corrente sem atualizar arestas */
               
               /* Elemina(sem free) corrente sem atualizar arestas */

               case DeturpaEliminaCorrente :
               {

                  if(pLista->pElemCorr->pProx != NULL)
                  {
                                              
                        pElemAux = pLista->pElemCorr->pProx ;                      
                        memcpy( (( char * )( pLista->pElemCorr )) - 10 , "????" , 4 ) ;
                        pLista->pElemCorr = pElemAux;    
                  }
                  else if(pLista->pElemCorr->pAnt != NULL)
                  {
                        pElemAux = pLista->pElemCorr->pAnt ;                      
                        memcpy( (( char * )( pLista->pElemCorr )) - 10 , "????" , 4 ) ;
                        pLista->pElemCorr = pElemAux;
                  }
                  else
                  {
                        memcpy( (( char * )( pLista->pElemCorr )) - 10 , "????" , 4 ) ;  
                  }
                  

                  break ;

               } /* fim ativa: Elemina(sem free) corrente sem atualizar arestas */


            } /* fim seleciona: Deturpa elemento */

            break ;

         } /* fim ativa: Deturpa elemento */

      } /* fim seleciona: Cabeca de LIS  &Deturpar lista */

   } /* Fim fun��o: LIS  &Deturpar lista */

#endif



/*****  Código das funções encapsuladas no módulo  *****/





/***********************************************************************
 *
 *  $FC Função: LIS  -Liberar elemento da lista
 *
 *  $ED Descrição da função
 *     Elimina os espaços apontados pelo valor do elemento e o
 *     próprio elemento.
 *
 ***********************************************************************/

void LiberarElemento( LIS_tppLista   pLista ,
                         tpElemLista  * pElem   )
   {

      if ( ( pLista->ExcluirValor != NULL )
        && ( pElem->pValor != NULL        ))
      {
         pLista->ExcluirValor( pElem->pValor ) ;
      } /* if */

      free( pElem ) ;

      pLista->numElem-- ;
      
      #ifdef _DEBUG
         CNT_CONTAR( "LiberarElemento" ) ;
      #endif

   } /* Fim fun��o: LIS  -Liberar elemento da lista */




/***********************************************************************
 *
 *  $FC Função: LIS  -Criar o elemento
 *
 ***********************************************************************/



tpElemLista * CriarElemento( LIS_tpLista * pLista ,
                            
                            void *       pValor  )

{
    
    
    
    tpElemLista * pElem ;
    
    #ifdef _DEBUG
         CNT_CONTAR( "CriarElemento" ) ;
    #endif
    
    pElem = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
    
    if ( pElem == NULL )
        
    {
        
        return NULL ;
        
    } /* if */
    
    #ifdef _DEBUG
         CED_DefinirTipoEspaco( pElem , LIS_TipoEspacoElemento ) ;
         pElem->pCabeca = pLista ;
      #else
         pLista = pLista ;
      #endif
    
    pElem->pValor = pValor ;
    
    pElem->pAnt   = NULL  ;
    
    pElem->pProx  = NULL  ;
    
    
    
    pLista->numElem ++ ;
    
    
    
    return pElem ;
    
    
    
} /* Fim função: LIS  -Criar o elemento */





/***********************************************************************
 *
 *  $FC Função: LIS  -Limpar a cabeça da lista
 *
 ***********************************************************************/



void LimparCabeca( LIS_tppLista pLista )

{
    
    #ifdef _DEBUG
         CNT_CONTAR( "LimparCabeca" ) ;
    #endif
    
    pLista->pOrigemLista = NULL ;
    
    pLista->pFimLista = NULL ;
    
    pLista->pElemCorr = NULL ;
    
    pLista->numElem   = 0 ;
    
    
    
} /* Fim função: LIS  -Limpar a cabeça da lista */



#ifdef _DEBUG


/***********************************************************************
*
*  $FC Fun��o: LIS  -Explorar verificando os elementos da lista
*
*  $ED Descri��o da fun��o
*     Percorre recursivamente a lista verificando os elementos � medida que forem
*     visitados. Caso seja encontrado alguma falha, a verifica��o ser�
*     suspensa. Portanto, no caso de falha, � poss�vel que nem todos
*     os elementos da lista sejam visitados.
*
***********************************************************************/

   LIS_tpCondRet VerificarElemLista( tpElemLista * pElem )
   {

      LIS_tpCondRet CondErro = LIS_CondRetOK ;

      if ( pElem == NULL )
      {
         return LIS_CondRetOK ;
      } /* if */

      CED_MarcarEspacoAtivo( pElem ) ;

      CondErro = LIS_VerificarElem( pElem ) ;

      if ( CondErro == LIS_CondRetOK )
      {
         CondErro = LIS_VerificarElem( pElem->pAnt ) ;
      } /* if */

      if ( CondErro == LIS_CondRetOK )
      {
         CondErro = LIS_VerificarElem( pElem->pProx ) ;
      } /* if */

      return CondErro ;

   } /* Fim fun��o: LIS  -Explorar verificando os elementos da lista */

#endif



/********** Fim do módulo de implementação: LIS  Lista duplamente encadeada **********/


