# Laboratorio de Matemática Discreta II, FaMAF - UNC

**Integrantes del grupo:** ***Tomás Achaval, Tomás Maraschio y Tomás Peyronel***.<br>
<br>
Leer aquí las consignas para la [**PARTE 1**](https://drive.google.com/file/d/1XVsj7g2fQtj75Bb0GrNGQFA4Xgv0AWaF/view) y [**PARTE 2**](https://drive.google.com/file/d/1LqFHJ3Cjc1CfwhlH2i1akRUhH7KTCXQ8/view) del proyecto.<br>
<br>
Opciones para correr con ```makefile```

## src/testing/main.c
Para probar el archivo main, que realiza tests relacionados a la mejora de los coloreos utilizando GulDukat y ElimGarak, se pueden correr los siguientes comandos: <br>
<br>
```make run g=grafo_raro.txt``` <-- Corre con valgrind <br>
```make release g=grafo_gigante.txt iters=500``` <-- Sin valgrind, con -O3 <br>
```make sanitize iters=25``` <-- Corre con -fsanitize=address,undefined <br>
<br>
\* Tanto ```g``` como ```iters``` son argumentos opcionales para los 3 comandos, con g default = K5.txt e iters default = 10.

## src/testing/nombre_del_test.c
Para el resto de los tests implementados, se pueden correr con: <br>
<br>
```make run_nombre_del_test g=grafo_nuevo.txt```<br>
```make release_nombre_del_test```<br>
```make sanitize_nombre_del_test g=grafo_rarisimo.txt```<br>
<br>
\* Para estos tests, únicamente ```g``` es un argumento opcional, con g default = K5.txt<br>
\*\*Notar que el ".c" es omitido.<br>
