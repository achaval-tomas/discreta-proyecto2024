# Laboratorio de Matemática Discreta II, FaMAF - UNC

**Integrantes del grupo:** ***Tomás Achaval, Tomás Maraschio y Tomás Peyronel***.<br>
<br>
Leer aquí las consignas para la [**PARTE 1**](https://drive.google.com/file/d/1XVsj7g2fQtj75Bb0GrNGQFA4Xgv0AWaF/view) y [**PARTE 2**](https://drive.google.com/file/d/1LqFHJ3Cjc1CfwhlH2i1akRUhH7KTCXQ8/view) del proyecto.<br>
<br>
Opciones para correr con ```makefile```

## Main de la parte 2
Para correr el main de la parte 2 del proyecto, se debe ejecutar ```make main g=grafo.txt```. En windows, ```make win g=grafo.txt```.

## src/testing/main.c
Para probar el otro main, que realiza tests relacionados a la mejora de los coloreos utilizando GulDukat y ElimGarak, se pueden correr los siguientes comandos: <br>
<br>
```make run g=grafo_raro.txt``` <-- Corre con valgrind <br>
```make release g=grafo_gigante.txt iters=500``` <-- Sin valgrind, con -O3 <br>
```make sanitize iters=25``` <-- Corre con -fsanitize=address,undefined <br>
<br>
\* Tanto ```g``` como ```iters``` son argumentos opcionales para los 3 comandos, con g default = K5.txt (que no es un K5) e iters default = 10.

## src/testing/nombre_del_test.c
Para correr los tests con distintas flags y niveles de optimizacion de gcc, se puede utilizar: <br>
<br>
```make run_nombre_del_test g=grafo_nuevo.txt```<br>
```make release_nombre_del_test g=grafinho.txt```<br>
```make sanitize_nombre_del_test g=grafo_rarisimo.txt```<br>
<br>
\* Para estos tests, únicamente ```g``` es un argumento opcional, con g default = K5.txt<br>
\*\*Notar que el ".c" es omitido.<br>
