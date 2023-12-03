# Taller2
Desarrollo taller 2/Estuctura de datos
Nombre: Esteban Rojas Lopez
Rut: 21.095.550-K
Carrera: ITI

Comandos para la ejecución:
-> g++ -o main main.cpp
-> ./main


Implementación:
el tablero esta implementado con una matriz 6x7 para representar las jugadas. 
Las posibles jugadas estan implementadas en un arbol, el cual guarda los estados actuales del juego.
Con minimax y poda alfa beta se busca la mejor jugada posible (depende de la dificultad elegida)
Al estar en medio de una partida, el usuario tiene la opcion de guardar y terminar la partida.
Si guardo anteriormente, al retomar el programa puede cargar el estado anterior, almacenado en un archivo csv.


Menu:
el menu consta de 3 opciones:
Juego Nuevo: pregunta por la dificultad y da inicio al juego. Pregunta por la columna a jugar (1 - 7) y opcion 8 para guardar (termina el programa)
cargar partida: si se ha guardado anteriormente, se puede cargar el csv y retomar la jugada.
salir: termina el programa



  
