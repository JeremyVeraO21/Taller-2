#include <iostream>
#include "Sistema.h"

int main() {
    Sistema sistema;

    // Cargar los datos iniciales del juego (cargar vertices y conexiones)
    if (!sistema.cargarDatos()) {
        std::cerr << "Error al cargar los datos del juego." << std::endl;
        return 1;
    }

    // Configurar el juego al iniciar (posiciones iniciales de jugadores, estrella, etc.)
    sistema.iniciarVideojuego();

    // Iniciar la partida, en la que ambos jugadores se turnan para mover, tirar dados, y aplicar efectos de casillas
    sistema.jugarPartida();

    // Terminar el juego después de que la partida se complete y aplicar reglas finales
    sistema.finalizarJuego();

    return 0;
}
