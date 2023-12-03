/*
Nombre: Esteban Rojas Lopez
rut: 21.095.550-K
Carrera: Ingenieria en tecnoogia de la informacion
*/



#include <iostream>
#include <vector>
#include <limits>
#include "NodoArbol.h"
#include <fstream>
#include <sstream>


//declaracion de variables
const int filas = 6;
const int columnas = 7;

//declaracion de funciones
void imprimirTablero(const std::vector<std::vector<char>>& tablero);
bool cargarPartida(std::vector<std::vector<char>>& tablero);
void guardarPartida(const std::vector<std::vector<char>>& tablero);
bool verificarEmpate(const std::vector<std::vector<char>>& tablero);
void deshacerJugada(std::vector<std::vector<char>>& tablero, int columna);
int ajustarColumna(int columna);
bool verificarGanador(const std::vector<std::vector<char>>& tablero, char jugador);
int evaluarVentana(char c1, char c2, char c3, char c4);
int evaluarTablero(const std::vector<std::vector<char>>& tablero);
bool hacerJugada(std::vector<std::vector<char>>& tablero, int columna, char jugador);
void jugarIA(std::vector<std::vector<char>>& tablero, int dificultad);
int minimax(NodoArbol* nodo, int profundidad, int alpha, int beta, bool esMaximizador);
void menu();
void play();

// Función para imprimir el tablero en la consola
void imprimirTablero(const std::vector<std::vector<char>>& tablero) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            std::cout << "| " << tablero[i][j] << " ";
        }
        std::cout << "|\n";
    }
    for (int j = 0; j < columnas; ++j) {
        std::cout << "----";
    }
    std::cout << "\n";
}


// Función para cargar el estado del tablero desde un archivo CSV
bool cargarPartida(std::vector<std::vector<char>>& tablero) {
    std::ifstream archivo("partida_guardada.csv");

    if (!archivo.is_open()) {
        std::cout << "No se encontró el archivo partida_guardada.csv. Asegúrate de haber guardado una partida antes de intentar cargarla.\n";
        return false;
    }

    for (int i = 0; i < filas; ++i) {
        std::string fila;
        std::getline(archivo, fila);

        std::istringstream ss(fila);
        for (int j = 0; j < columnas; ++j) {
            std::string valor;
            std::getline(ss, valor, ','); //
            tablero[i][j] = valor[0]; 
        }
    }

    archivo.close();
    std::cout << "Partida cargada exitosamente desde partida_guardada.csv.\n";

    imprimirTablero(tablero);

     while (true) {
        int columna;
        std::cout << "Jugador 'X', ingresa la columna (1-7), o 8 para Guardar y Salir: ";
        std::cin >> columna;

        if (columna == 8) {
            // Verificar si el juego ha terminado
            if (verificarEmpate(tablero)) {
                std::cout << "Es un empate!\n";
                guardarPartida(tablero);  // Guardar solo si el juego no ha terminado
                std::cout << "Partida guardada. Hasta luego!\n";
                return true;
            }

            if (verificarGanador(tablero, 'O')) {
                std::cout << "La IA ha ganado!\n";
                guardarPartida(tablero);  // Guardar solo si el juego no ha terminado
                std::cout << "Partida guardada. Hasta luego!\n";
                return true;
            }

            guardarPartida(tablero);  // Guardar solo si el juego no ha terminado
            std::cout << "Partida guardada. Hasta luego!\n";
            return true;
        }

        if (columna < 1 || columna > 7) {
            std::cout << "Columna invalida. Ingresa nuevamente.\n";
            continue;
        }

        if (hacerJugada(tablero, columna, 'X')) {
            imprimirTablero(tablero);

            if (verificarGanador(tablero, 'X')) {
                std::cout << "Felicidades! Has ganado!\n";
                break;
            }
        } else {
            std::cout << "Columna llena. Ingresa nuevamente.\n";
        }

        // Verificar si el juego ha terminado después de la jugada del jugador
        if (verificarEmpate(tablero)) {
            std::cout << "Es un empate!\n";
            break;
        }

        if (verificarGanador(tablero, 'O')) {
            std::cout << "La IA ha ganado!\n";
            break;
        }

        // Turno de la IA
        jugarIA(tablero, 2);

        // Verificar si el juego ha terminado después del turno de la IA
        if (verificarEmpate(tablero)) {
            std::cout << "Es un empate!\n";
            break;
        }

        if (verificarGanador(tablero, 'O')) {
            std::cout << "La IA ha ganado!\n";
            break;
        }
    }

    return true;
}

// Función para guardar el estado actual del tablero en un archivo CSV
void guardarPartida(const std::vector<std::vector<char>>& tablero) {
    std::ofstream archivo("partida_guardada.csv");

    if (archivo.is_open()) {
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                archivo << tablero[i][j];
                if (j < columnas - 1) {
                    archivo << ",";   //representa las columnas con comas, esto para representar el tablero en el archivo csv
                }
            }
            archivo << "\n";
        }
        archivo.close();
        std::cout << "Partida guardada exitosamente en partida_guardada.csv.\n";
    } else {
        std::cout << "Error al guardar la partida. Asegúrate de tener permisos para escribir en el directorio actual o elige una ubicación donde puedas guardar el archivo.\n";
    }
}


bool verificarEmpate(const std::vector<std::vector<char>>& tablero) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (tablero[i][j] == ' ') {
                return false; // Todavia hay espacios vacios, no es un empate
            }
        }
    }
    return true; // No hay espacios vacios, es un empate
}

void deshacerJugada(std::vector<std::vector<char>>& tablero, int columna) {
    for (int i = 0; i < filas; ++i) {
        if (tablero[i][columna] != ' ') {
            tablero[i][columna] = ' ';
            break;
        }
    }
}

int ajustarColumna(int columna) {
    return columna - 1;
}



bool verificarGanador(const std::vector<std::vector<char>>& tablero, char jugador) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j <= columnas - 4; ++j) {
            if (tablero[i][j] == jugador && tablero[i][j + 1] == jugador &&
                tablero[i][j + 2] == jugador && tablero[i][j + 3] == jugador) {
                return true;
            }
        }
    }

    for (int i = 0; i <= filas - 4; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (tablero[i][j] == jugador && tablero[i + 1][j] == jugador &&
                tablero[i + 2][j] == jugador && tablero[i + 3][j] == jugador) {
                return true;
            }
        }
    }

    return false;
}

int evaluarVentana(char c1, char c2, char c3, char c4) {
    int utilidad = 0;

    if (c1 == 'O') {
        utilidad = 1;
    } else if (c1 == 'X') {
        utilidad = -1;
    }

    if (c2 == 'O') {
        if (utilidad == 1) {
            utilidad = 10;
        } else if (utilidad == -1) {
            return 0;
        } else {
            utilidad = 1;
        }
    } else if (c2 == 'X') {
        if (utilidad == -1) {
            utilidad = -10;
        } else if (utilidad == 1) {
            return 0;
        } else {
            utilidad = -1;
        }
    }

    if (c3 == 'O') {
        if (utilidad > 0) {
            utilidad *= 10;
        } else if (utilidad < 0) {
            return 0;
        } else {
            utilidad = 1;
        }
    } else if (c3 == 'X') {
        if (utilidad < 0) {
            utilidad *= 10;
        } else if (utilidad > 0) {
            return 0;
        } else {
            utilidad = -1;
        }
    }

    if (c4 == 'O') {
        if (utilidad > 0) {
            utilidad *= 10;
        } else if (utilidad < 0) {
            return 0;
        } else {
            utilidad = 1;
        }
    } else if (c4 == 'X') {
        if (utilidad < 0) {
            utilidad *= 10;
        } else if (utilidad > 0) {
            return 0;
        } else {
            utilidad = -1;
        }
    }

    return utilidad;
}

int evaluarTablero(const std::vector<std::vector<char>>& tablero) {
    int utilidad = 0;

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j <= columnas - 4; ++j) {
            utilidad += evaluarVentana(tablero[i][j], tablero[i][j + 1], tablero[i][j + 2], tablero[i][j + 3]);
        }
    }

    for (int i = 0; i <= filas - 4; ++i) {
        for (int j = 0; j < columnas; ++j) {
            utilidad += evaluarVentana(tablero[i][j], tablero[i + 1][j], tablero[i + 2][j], tablero[i + 3][j]);
        }
    }

    return utilidad;
}

bool hacerJugada(std::vector<std::vector<char>>& tablero, int columna, char jugador) {
    columna = ajustarColumna(columna);

    for (int i = filas - 1; i >= 0; --i) {
        if (tablero[i][columna] == ' ') {
            tablero[i][columna] = jugador;
            return true;
        }
    }
    return false;
}

//funcion para que juegue la IA
void jugarIA(std::vector<std::vector<char>>& tablero, int dificultad) {
    if (dificultad == 1) {   //si la dificultad es facil, la IA jugara de forma aleatoria
        int columna;
        do {
            columna = std::rand() % columnas + 1;
        } while (!hacerJugada(tablero, columna, 'O'));

        std::cout << "La IA ha jugado en la columna " << columna << "\n";
    } else {
        NodoArbol* raiz = new NodoArbol(tablero);

        int mejorColumna = -1;
        int mejorValor = std::numeric_limits<int>::min();
        int profundidad = (dificultad == 2) ? 3 : 5;

        for (int col = 0; col < columnas; ++col) {
            if (tablero[0][col] == ' ') {
                hacerJugada(tablero, col + 1, 'O');
                NodoArbol* hijo = new NodoArbol(tablero);

                int minimax(NodoArbol* nodo, int profundidad, int alpha, int beta, bool esMaximizador);

                int valorMovimiento = minimax(hijo, profundidad, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);

                if (dificultad == 2) { //si la dificultad es media, la IA jugara de forma aleatoria la mitad de las veces
                
                    if (std::rand() % 2 == 0) {  
                    
                        if (valorMovimiento > mejorValor) {
                            mejorValor = valorMovimiento;
                            mejorColumna = col + 1;
                        }
                    } else {
                        
                        mejorColumna = std::rand() % columnas + 1;
                    }
                } else if (dificultad == 3) {  //si es dificil, la IA jugara de forma inteligente
                    if (valorMovimiento > mejorValor) {
                        mejorValor = valorMovimiento;
                        mejorColumna = col + 1;
                    }
                }

                deshacerJugada(tablero, col);
            }
        }

        if (mejorColumna != -1) {
            hacerJugada(tablero, mejorColumna, 'O');
            std::cout << "La IA ha jugado en la columna " << mejorColumna << "\n";
        }

        delete raiz;
    }

    imprimirTablero(tablero);
}


//funcion para el algoritmo minimax
int minimax(NodoArbol* nodo, int profundidad, int alpha, int beta, bool esMaximizador) {
    if (verificarGanador(nodo->estado, 'X')) {
        return -1;
    } else if (verificarGanador(nodo->estado, 'O')) {
        return 1;
    } else if (profundidad == 0) {
        return evaluarTablero(nodo->estado);
    }

    if (esMaximizador) {
        int mejorValor = std::numeric_limits<int>::min(); //Se inicializa el mejor valor con el valor minimo 
        for (int col = 0; col < columnas; ++col) {
            if (nodo->estado[0][col] == ' ') {
                std::vector<std::vector<char>> nuevoEstado = nodo->estado;
                hacerJugada(nuevoEstado, col, 'O');
                NodoArbol* hijo = new NodoArbol(nuevoEstado);
                nodo->hijos.push_back(hijo);

                //llama recursivamente al algoritmo minimax    
                int valorMovimiento = minimax(hijo, profundidad - 1, alpha, beta, false);
                mejorValor = std::max(mejorValor, valorMovimiento);
                alpha = std::max(alpha, mejorValor);
                //implementacion de poda alfa-beta  
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return mejorValor;
    } else {
        int mejorValor = std::numeric_limits<int>::max();
        for (int col = 0; col < columnas; ++col) {
            if (nodo->estado[0][col] == ' ') {
                std::vector<std::vector<char>> nuevoEstado = nodo->estado;
                hacerJugada(nuevoEstado, col, 'X');
                NodoArbol* hijo = new NodoArbol(nuevoEstado);
                nodo->hijos.push_back(hijo);

                int valorMovimiento = minimax(hijo, profundidad - 1, alpha, beta, true);
                mejorValor = std::min(mejorValor, valorMovimiento);
                beta = std::min(beta, mejorValor);

                if (beta <= alpha) {
                    break;
                }
            }
        }
        return mejorValor;
    }
}

/*metodo para imprimir el menu*/
void menu() {
    std::cout << "Bienvenido al juego Conecta 4!\n";
    std::cout << "1. Juego Nuevo\n";
    std::cout << "2. Cargar Partida\n";
    std::cout << "3. Salir\n";
    std::cout << "Selecciona una opcion: ";
}


//Inicio del programa
void play() {
    std::vector<std::vector<char>> tableroCargardo(filas, std::vector<char>(columnas, ' '));
    bool juegoNuevo = true;  // Indica si es un juego nuevo o una partida cargada

    while (true) {
        if (juegoNuevo) {
            menu();

            int opcion;
            std::cin >> opcion;

            switch (opcion) {
                case 1: {
                    std::vector<std::vector<char>> tablero(filas, std::vector<char>(columnas, ' '));
                    int dificultad;
                    std::cout << "Selecciona la dificultad:\n";
                    std::cout << "1. Facil\n";
                    std::cout << "2. Medio\n";
                    std::cout << "3. Dificil\n";
                    std::cout << "Dificultad: ";
                    std::cin >> dificultad;   //imprimir el tablero al iniciar el juego
                    imprimirTablero(tablero);
                    while (dificultad < 1 || dificultad > 3) {
                        std::cout << "Dificultad invalida. Ingresa nuevamente.\n";
                        std::cout << "Dificultad: ";
                        std::cin >> dificultad;
                    }

                    while (true) {
                        int columna;
                        std::cout << "Jugador 'X', ingresa la columna (1-7), o 8 para Guardar y Salir: ";
                        std::cin >> columna;

                        if (columna == 8) {
                            guardarPartida(tablero);  // Agrega la llamada para guardar la partida
                            std::cout << "Partida guardada. Hasta luego!\n";
                            return;
                        }

                        if (columna < 1 || columna > 7) {
                            std::cout << "Columna invalida. Ingresa nuevamente.\n";
                            continue;
                        }

                        if (hacerJugada(tablero, columna, 'X')) {
                            imprimirTablero(tablero);

                            if (verificarGanador(tablero, 'X')) {
                                std::cout << "Felicidades! Has ganado!\n";
                                break;
                            }
                        } else {
                            std::cout << "Columna llena. Ingresa nuevamente.\n";
                        }

                        //turno de la ia
                        jugarIA(tablero, dificultad);

                        if (verificarGanador(tablero, 'O')) {
                            std::cout << "La IA ha ganado!\n";
                            break;
                        }

                        if (verificarEmpate(tablero)) {
                            std::cout << "Es un empate!\n";
                            break;
                        }
                    }
                    break;
                }
                case 2:
                    if (cargarPartida(tableroCargardo)) {
                        imprimirTablero(tableroCargardo);
                      
                    } else {
                        continue; // Volver al menú si no se pudo cargar la partida
                    }
                    break;
                case 3:
                    std::cout << "Gracias por jugar. Hasta luego!\n";
                    return;
                default:
                    std::cout << "Opcion invalida. Ingresa nuevamente.\n";
            }
        } 
    }
}

int main() {
    play();
    
    return 0;
}