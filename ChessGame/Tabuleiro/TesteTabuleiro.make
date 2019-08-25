##################################################
###
### Diretivas de MAKE para o construto: TesteTabuleiro
### Gerado a partir de: TesteTabuleiro.comp
###
### ----- Arquivo gerado, NÃO EDITE!!! -----
###
##################################################

### Nomes globais

NOME            = TesteTabuleiro


### Nomes de paths

Pobj                 = .
Perr                 = .
PDEFAULT             = .
Pc                   = .

### Nomes de diretórios para geração

Fobj                 = .
Ferr                 = .
FDEFAULT             = .
Fc                   = .

### Macros da plataforma


O       = /D_CRT_SECURE_NO_DEPRECATE
OD      = /Zi /Od /D_CRT_SECURE_NO_DEPRECATE
L       =
LD      = /DEBUG /DEBUGTYPE:CV

!IFDEF PRD
O       = /Ox /D_CRT_SECURE_NO_DEPRECATE
!ENDIF

OPT     = /c /J /W4 /nologo
INCLUDE = $(INCLUDE);$(PDEFAULT)


### Regras de geração

all : limpa \
   $(Fobj)\TESTTAB.obj   $(Fobj)\TABULEIRO.obj   $(Fobj)\LISTA.obj \
   Construto

### Limpar arquivos

limpa :
   if exist $(Ferr)\$(NOME).err  del $(Ferr)\$(NOME).err


### Dependências de módulos objeto a compilar

$(Fobj)\TESTTAB.obj :  {$(Pc)}\TESTTAB.c \
    {$(PDEFAULT)}Generico.h           {$(PDEFAULT)}LerParm.h            {$(PDEFAULT)}Lista.h              \
    {$(PDEFAULT)}TST_Espc.h           {$(PDEFAULT)}Tabuleiro.h         
   cl $(O) $(OPT) /Fo$(Fobj)\ $(Fc)\$(@B).c >> $(Ferr)\$(NOME).err

$(Fobj)\TABULEIRO.obj :  {$(Pc)}\TABULEIRO.c \
    {$(PDEFAULT)}LISTA.H              {$(PDEFAULT)}TABULEIRO.h         
   cl $(O) $(OPT) /Fo$(Fobj)\ $(Fc)\$(@B).c >> $(Ferr)\$(NOME).err

$(Fobj)\LISTA.obj :  {$(Pc)}\LISTA.c \
    {$(PDEFAULT)}Conta.h              {$(PDEFAULT)}Generico.h           {$(PDEFAULT)}IdTiposEspaco.def    \
    {$(PDEFAULT)}TST_Espc.h           {$(PDEFAULT)}TiposEspacosLista.def {$(PDEFAULT)}cespdin.h            \
    {$(PDEFAULT)}lista.h             
   cl $(O) $(OPT) /Fo$(Fobj)\ $(Fc)\$(@B).c >> $(Ferr)\$(NOME).err


### Terminação

Construto : \
   $(Fobj)\TESTTAB.obj   $(Fobj)\TABULEIRO.obj   $(Fobj)\LISTA.obj
    cd $(Fobj)
    LINK $(L) @$(NOME).build >> $(Ferr)\$(NOME).err

##################################################
###
### Fim de diretivas MAKE para o construto: TesteTabuleiro
###
##################################################

