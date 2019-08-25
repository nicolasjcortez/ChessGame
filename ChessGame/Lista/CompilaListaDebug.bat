@ECHO  OFF
REM  Compila todos os exemplos simples 

pushd  .

del *.err
del TesteListaDebug.exe

del *.obj
nmake /Ftestelistadebug.make 

copy  *.err  tudo2.err

notepad tudo2.err

popd
