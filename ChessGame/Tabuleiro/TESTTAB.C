/***************************************************************************
*  $MCI Módulo de implementação: TTAB Teste tabuleiro de xadrez
*
*  Arquivo gerado:              TESTTAB.c
*
*  Autores: Nicolas Jordao Cortez
*
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include    "Lista.h"
#include    "Tabuleiro.h"


    
static const char CRIAR_PECA_CMD                [ ] = "=criarpeca";
static const char GET_LINHA_CORRENTE_PECA_CMD   [ ] = "=getlinha";
static const char GET_COLUNA_CORRENTE_PECA_CMD  [ ] = "=getcoluna";
static const char GET_COR_PECA_CMD              [ ] = "=getcor";
static const char GET_TIPO_PECA_CMD             [ ] = "=gettipo";
static const char SET_LINHA_CORRENTE_PECA_CMD   [ ] = "=setlinha";
static const char SET_COLUNA_CORRENTE_PECA_CMD  [ ] = "=setcoluna";
static const char SET_COR_PECA_CMD              [ ] = "=setcor";
static const char SET_TIPO_PECA_CMD             [ ] = "=settipo";
static const char CRIAR_TABULEIRO_CMD           [ ] = "=criartabuleiro";
static const char INSERIR_PECA_CMD              [ ] = "=inserirpeca";
static const char MOVER_PECA_CMD                [ ] = "=moverpeca";
static const char OBTER_PECA_CMD                [ ] = "=obterpeca";
static const char RETIRAR_PECA_CMD              [ ] = "=retirarpeca";
static const char OBTER_LISTA_AMEACANTES_CMD    [ ] = "=obterameacantes";
static const char OBTER_LISTA_AMEACADOS_CMD     [ ] = "=obterameacados";
static const char DESTRUIR_TABULEIRO_CMD        [ ] = "=destruirtabuleiro";

/* os comandos a seguir somente operam em modo _DEBUG */

const char VER_CABECA_CMD[ ] = "=verificarcabeca" ;
const char VER_TABULEIRO_CMD[ ] = "=verificartabuleiro" ;
const char VER_MEMORIA_CMD[ ] = "=verificarmemoria" ;
const char DETURPAR_CMD[ ]   = "=deturpar" ;

#define TRUE  1
#define FALSE 0

#define VAZIO     0
#define NAO_VAZIO 1



/***** Protótipos das funções encapuladas no módulo *****/

   static void DestruirValor( void * pValor ) ;
   static int ValidarLinhaColuna( int pos );
   static int ValidarCor(char cor);
   static int ValidarTipo(char tipo);
   static tppPeca*  peca = NULL;
   static tppPeca  pecaObtida;
   static tpTabuleiro* tabuleiro = NULL;
   static LIS_tppLista lista = NULL;
   
/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: &Testar lista
*
*  $ED Descrição da função
*     Podem ser criados apenas uma peca e um tabulerio. 
*
*     Comandos disponíveis:
*
*            =criarpeca
*            =getlinha     <tipo>     <CondRet>
*            =getcoluna    <coluna>   <CondRet>
*            =getcor       <cor>      <CondRet>
*            =gettipo      <tipo>     <CondRet>
*            =setlinha     <tipo>     
*            =setcoluna    <coluna>  
*            =setcor       <cor>      
*            =settipo      <tipo>    
*            =criartabuleiro
*            =inserirpeca  <linha>  <coluna>  <CondRet>
*            =moverpeca  <linOrig>  <colOrig>  <linDest> <colDest> <CondRet>
*            =obterpeca    <linha> <coluna> <tipoRet> <corRet>
*            =retirarpeca  <linha> <coluna>  <CondRet>
*            =obterameacantes  <linha> <coluna> <condRetPonteiro>
*            =obterameacados   <linha> <coluna> <condRetPonteiro>
*            =destruirtabuleiro
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {
         // tppPeca*  peca;

           int linhaCorrente    = -1 ,
               colunaCorrente   = -1 ,
               linha            = -1 ,
               coluna           = -1 ,
               linhaOrigem      = -1 ,
               colunaOrigem     = -1 ,
               linhaDestino     = -1 ,
               colunaDestino    = -1 ,
               numLidos         = -1 ,
               CondRetEsp       = -1 ,
               Ret              = -1 ;
               
          char cor = 'X',
              tipo = 'X',
              corObtida = 'X',
              tipoObtido = 'X';   
                   
          #ifdef _DEBUG
              int  IntEsperado   = -1 ;
          #endif
      /* Testar CriarPeca */
    if ( strcmp( ComandoTeste , CRIAR_PECA_CMD ) == 0 )
    {


            peca = TAB_CriarPeca( ) ;
            
            //Deve retornar TST_CondRetOk
            return TST_CompararPonteiroNulo( 1 , peca ,
               "Erro em ponteiro de nova peca."  ) ;
     }
     
     else if (strcmp( ComandoTeste , GET_LINHA_CORRENTE_PECA_CMD ) == 0)
     {
            numLidos = LER_LerParametros( "i" , &linhaCorrente ) ;

            if ( ( numLidos !=  1)
              || ( ! ValidarLinhaColuna( linhaCorrente) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( linhaCorrente ,
                      TAB_GetLinhaCorrente(peca) ,
                      "Condicao de retorno errada ao obter linha corrente" ) ;

     }  
     
     else if (strcmp( ComandoTeste , SET_LINHA_CORRENTE_PECA_CMD ) == 0)
     {
            numLidos = LER_LerParametros( "ii" , &linhaCorrente, &CondRetEsp ) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarLinhaColuna( linhaCorrente) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( CondRetEsp ,
                      TAB_SetLinhaCorrente(peca, linhaCorrente) ,
                      "Condicao de retorno errada ao editar linha corrente" ) ;
     }
     else if (strcmp( ComandoTeste , GET_COLUNA_CORRENTE_PECA_CMD ) == 0)
     {
            numLidos = LER_LerParametros( "i" , &colunaCorrente ) ;

            if ( ( numLidos !=  1)
              || ( ! ValidarLinhaColuna( colunaCorrente) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( colunaCorrente ,
                      TAB_GetColunaCorrente(peca) ,
                      "Condicao de retorno errada ao obter coluna corrente" ) ;

     }  
     
     else if (strcmp( ComandoTeste , SET_COLUNA_CORRENTE_PECA_CMD ) == 0)
     {
            numLidos = LER_LerParametros( "ii" , &colunaCorrente, &CondRetEsp ) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarLinhaColuna( colunaCorrente) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( CondRetEsp ,
                      TAB_SetColunaCorrente(peca, colunaCorrente) ,
                      "Condicao de retorno errada ao editar coluna corrente" ) ;
     }
     else if (strcmp( ComandoTeste , GET_COR_PECA_CMD ) == 0)
     {    
          
          
            numLidos = LER_LerParametros( "c" , &cor) ;
            
            if ( ( ! ValidarCor( cor ) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( cor ,
                      TAB_GetCor(peca) ,
                      "Condicao de retorno errada ao obter cor" ) ;

     }  
     
     else if (strcmp( ComandoTeste , SET_COR_PECA_CMD ) == 0)
     {
          printf("\n aqui \n");
        
        numLidos = LER_LerParametros( "ci" , &cor, &CondRetEsp ) ;
        
            if ( ( numLidos != 2 )
              || ( ! ValidarCor( cor ) ) )
            {
                 printf("\n%d", numLidos);
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( CondRetEsp ,
                      TAB_SetCor(peca, cor ) ,
        
                      "Condicao de retorno errada ao editar cor" ) ;
     }         
     else if (strcmp( ComandoTeste , GET_TIPO_PECA_CMD ) == 0)
     {
            numLidos = LER_LerParametros( "c" , &tipo ) ;

            if ( ( numLidos !=  1)
              || ( ! ValidarTipo( tipo ) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( tipo ,
                      TAB_GetTipo(peca) ,
                      "Condicao de retorno errada ao obter tipo" ) ;

     }  
     
     else if (strcmp( ComandoTeste , SET_TIPO_PECA_CMD ) == 0)
     {
            numLidos = LER_LerParametros( "ci" , &tipo, &CondRetEsp ) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarTipo( tipo ) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( CondRetEsp ,
                      TAB_SetTipo(peca, tipo ) ,      
                      "Condicao de retorno errada ao editar tipo" ) ;
     }   
     if ( strcmp( ComandoTeste , CRIAR_TABULEIRO_CMD ) == 0 )
    {


            tabuleiro = TAB_CriarTabuleiro( ) ;
            
            //Deve retornar TST_CondRetOk
            return TST_CompararPonteiroNulo( 1 , tabuleiro ,
               "Erro em ponteiro de novo tabuleiro."  ) ;
     }
     if ( strcmp( ComandoTeste , INSERIR_PECA_CMD ) == 0 )
    {
            numLidos = LER_LerParametros( "iii" , &linha, &coluna, &CondRetEsp ) ;

            if ( (peca==NULL) || (tabuleiro == NULL) || ( numLidos != 3 ) ||
                 ( !ValidarLinhaColuna( linha ) ) || ( !ValidarLinhaColuna( linha ) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( CondRetEsp ,
                      TAB_InserirPeca(tabuleiro, linha, coluna, peca ) ,      
                      "Condicao de retorno errada ao inserir peca" ) ;
     }
     if ( strcmp( ComandoTeste , OBTER_PECA_CMD ) == 0 )
    {
            numLidos = LER_LerParametros( "iicc" , &linha, &coluna, &tipo, &cor ) ;

            if (  (tabuleiro == NULL) || ( numLidos != 4 ) ||
                 ( !ValidarTipo( tipo ) ) || ( !ValidarCor( cor ) )  ||
                 ( !ValidarLinhaColuna( linha ) ) || ( !ValidarLinhaColuna( linha ) ) )
            {
               return TST_CondRetParm ;
            } /* if */
            
            pecaObtida = TAB_ObterPeca(tabuleiro, linha, coluna);
            tipoObtido = TAB_GetTipo(&pecaObtida);
            corObtida = TAB_GetCor(&pecaObtida);
            if ( (tipoObtido != tipo) || (corObtida != cor) )
            {
               return TST_CondRetErro ;
            }
            return TST_CondRetOK ;
     }
     if ( strcmp( ComandoTeste , MOVER_PECA_CMD ) == 0 )
    {
            numLidos = LER_LerParametros( "iiiii" , &linhaOrigem, &colunaOrigem
                                          ,&linhaDestino, &colunaDestino, &CondRetEsp ) ;

            if ( (tabuleiro == NULL) || ( numLidos != 5 ) ||
                 ( !ValidarLinhaColuna( linhaOrigem ) ) || ( !ValidarLinhaColuna( linhaOrigem ) ) ||
                 ( !ValidarLinhaColuna( linhaDestino ) ) || ( !ValidarLinhaColuna( linhaDestino ) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( CondRetEsp ,
                      TAB_MoverPeca(tabuleiro, linhaOrigem, colunaOrigem, linhaDestino, colunaDestino ) ,      
                      "Condicao de retorno errada ao mover peca" ) ;
     }
     if ( strcmp( ComandoTeste , RETIRAR_PECA_CMD ) == 0 )
    {
            numLidos = LER_LerParametros( "iii" , &linha, &coluna, &CondRetEsp ) ;

            if ( (tabuleiro == NULL) || ( numLidos != 3 ) ||
                 ( !ValidarLinhaColuna( linha ) ) || ( !ValidarLinhaColuna( linha ) ) )
            {
               return TST_CondRetParm ;
            } /* if */


            return TST_CompararInt( CondRetEsp ,
                      TAB_RetirarPeca(tabuleiro, linha, coluna ) ,      
                      "Condicao de retorno errada ao retirar peca" ) ;
     }
     if ( strcmp( ComandoTeste , OBTER_LISTA_AMEACANTES_CMD ) == 0 )
     {
            numLidos = LER_LerParametros( "iii" , &linha, &coluna, &CondRetEsp ) ;
            if ( (tabuleiro == NULL) || ( numLidos != 3 ) ||
                 ( !ValidarLinhaColuna( linha ) ) || ( !ValidarLinhaColuna( linha ) ) )
            {
               return TST_CondRetParm ;
            } /* if */

            lista = TAB_ObterListaAmeacantes(tabuleiro,linha,coluna);
            if(lista == NULL)
            {
                     Ret = 0;
            }
            else
            {
                     Ret = 1;
            }
            return TST_CompararInt( CondRetEsp ,Ret,      
                      "Lista de ameacantes nao esta como o esperado" ) ;
     }
     if ( strcmp( ComandoTeste , OBTER_LISTA_AMEACADOS_CMD ) == 0 )
     {
            numLidos = LER_LerParametros( "iii" , &linha, &coluna, &CondRetEsp ) ;
            if ( (tabuleiro == NULL) || ( numLidos != 3 ) ||
                 ( !ValidarLinhaColuna( linha ) ) || ( !ValidarLinhaColuna( linha ) ) )
            {
               return TST_CondRetParm ;
            } /* if */

            lista = TAB_ObterListaAmeacados(tabuleiro,linha,coluna);
            if(lista == NULL)
            {
                     Ret = 0;
            }
            else
            {
                     Ret = 1;
            }
            return TST_CompararInt( CondRetEsp ,Ret,      
                      "Lista de ameacados nao esta como o esperado" ) ;
     }
     if ( strcmp( ComandoTeste , DESTRUIR_TABULEIRO_CMD ) == 0 )
     {
     
            if ( (tabuleiro == NULL) )
            {
               return TST_CondRetParm ;
            } /* if */

            TAB_DestruirTabuleiro(tabuleiro);
            tabuleiro = NULL;
            
            return TST_CondRetOK;
      }
      #ifdef _DEBUG

         else if ( strcmp( ComandoTeste , VER_CABECA_CMD ) == 0 )
         {

            
            if ( (tabuleiro == NULL) )
            {
               return TST_CondRetParm ;
            } /* if */

            return TAB_VerificarCabeca( tabuleiro ) ;

         } /* fim ativa: Testar verificador de cabeça */
      #endif
      
      /* Testar verificador de tabulerio */
      #ifdef _DEBUG

         else if ( strcmp( ComandoTeste , VER_TABULEIRO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,&CondRetEsp ) ;
            if (  (tabuleiro == NULL) && numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */
            
            return TST_CompararInt( CondRetEsp ,
                             TAB_VerificarTabuleiro( tabuleiro ) ,
                             "Retorno incorreto ao verificar tabuleiro." ) ;

         } /* fim ativa: Testar verificador de árvore */
      #endif
      
      /* Deturpar tabueliro */
      #ifdef _DEBUG

         else if ( strcmp( ComandoTeste , DETURPAR_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" , &IntEsperado ) ;

            if ( ( numLidos != 1 ) && tabuleiro == NULL )
            {
               return TST_CondRetParm ;
            } /* if */

            TAB_Deturpar( tabuleiro, IntEsperado ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Deturpar uma lista */
      #endif
      
      
      return TST_CondRetNaoConhec ;

   } /* Fim função: &Testar tabuleiro */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: TLIS -Destruir valor
*
***********************************************************************/

   void DestruirValor( void * pValor )
   {

      free( pValor ) ;

   } /* Fim função: TLIS -Destruir valor */



//valida o indice da linha ou coluna
int ValidarLinhaColuna( int pos )
{

      if ( ( pos <  0 )
        || ( pos >= 8 ))
      {
         return FALSE ;
      } /* if */
         
     return TRUE ;

}

int ValidarCor(char cor)
{
    if ( (cor == 'B') || (cor == 'P') || (cor == 'V'))
      {
         return TRUE ;
      } /* if */
         
     return FALSE ;
}

int ValidarTipo(char tipo)
{
    if ( (tipo == 'T') || (tipo == 'C') || (tipo == 'V') || 
          (tipo == 'D') || (tipo == 'R') || (tipo == 'P') || (tipo == 'B'))
      {
         return TRUE ;
      } /* if */
         
     return FALSE ;
}


/********** Fim do módulo de implementação: Teste Tabuleiro **********/

