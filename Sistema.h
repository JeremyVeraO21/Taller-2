#ifndef SISTEMA_H
#define SISTEMA_H

#include "Grafo.h"
#include <cstdlib>
#include <ctime>

class Sistema {
private:
    Grafo grafo;
    Nodo* estrella;
    Nodo* jugador1Pos;
    Nodo* jugador2Pos;
    int turnoActual;
    int saldoJugador1;
    int saldoJugador2;
    int estrellasJugador1;
    int estrellasJugador2;

public:
    Sistema();
    ~Sistema();

    bool cargarDatos();
    void iniciarVideojuego();
    void mostrarGrafo();
    Nodo* buscarCasillaTipo(const char* tipo);
    void decidirTurnoInicial();
    void mostrarMenuJugador(const char* jugador);
    void jugarPartida();
    void tirarDado();
    void buscarEstrella();
    void consultarSaldo();
    void aplicarEfectoCasilla(Nodo* casilla);
    void actualizarPosicionJugador(Nodo* nuevaPosicion);
    void cortarCamino(Nodo* casilla);
    void finalizarJuego();
    void reiniciarJuego();
    void moverEstrella();
};

#endif // SISTEMA_H
