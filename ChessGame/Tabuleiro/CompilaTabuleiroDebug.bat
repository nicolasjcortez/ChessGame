@ECHO  OFF
REM  Compila todos os exemplos simples 

pushd  .

del *.err
del TesteTabuleiroDebug.exe

del *.obj
nmake /FTesteTabuleiroDebug.make 

copy  *.err  tudo2.err

notepad tudo2.err

popd
