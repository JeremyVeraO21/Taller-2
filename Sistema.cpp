#include "Sistema.h"
#include <iostream>
#include <ctime>
#include <thread>
#include <queue>
#include <unordered_map>

using namespace std;
// Constructor de la clase Sistema
Sistema::Sistema() : estrella(nullptr), jugador1Pos(nullptr), jugador2Pos(nullptr), turnoActual(1),saldoJugador1(0), saldoJugador2(0), estrellasJugador1(0), estrellasJugador2(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Inicializar aleatoriedad
}

// Destructor de la clase Sistema
Sistema::~Sistema() {}

// Carga los datos desde los archivos casillas.csv y board.txt
bool Sistema::cargarDatos() {
    if (!grafo.cargarVerticesDesdeCasillas("casillas.csv")) {
        std::cerr << "Error al cargar vértices desde casillas.csv" << std::endl;
        return false;
    }
    if (!grafo.cargarConexionesDesdeBoard("board.txt")) {
        std::cerr << "Error al cargar conexiones desde board.txt" << std::endl;
        return false;
    }
    return true;
}

// Mostrar el grafo
void Sistema::mostrarGrafo() {
    grafo.mostrarGrafo();
}

// Buscar una casilla aleatoria de un tipo específico
Nodo* Sistema::buscarCasillaTipo(const char* tipo) {
    for (int i = 0; i < grafo.getNumVertices(); i++) {
        if (strcmp(grafo.getVertice(i)->tipo, tipo) == 0) {
            return grafo.getVertice(i);
        }
    }
    return nullptr;
}

// Configuración inicial del videojuego
void Sistema::iniciarVideojuego() {
    cout << "Bienvenidos a Mario Party" << endl;
    cout << "A continuacion se posicionaran los jugadores " << '\n' <<
            "y se lanzaran los dados para ver quien comienza" << endl;
    // Posicionar la estrella en una casilla de tipo STR
    estrella = buscarCasillaTipo("STR");
    if (estrella) {
        std::cout << "Estrella posicionada en la casilla: " << estrella->id << std::endl;
    } else {
        std::cerr << "No se encontró una casilla de tipo STR para la estrella." << std::endl;
    }

    // Posicionar a los jugadores en la casilla de inicio
    jugador1Pos = buscarCasillaTipo("INICIO");
    jugador2Pos = jugador1Pos; // Ambos jugadores comienzan en la misma casilla
    if (jugador1Pos) {
        std::cout << "Jugador 1 (Mario) y Jugador 2 (Luigi) posicionados en la casilla de inicio: " << jugador1Pos->id << std::endl;
    } else {
        std::cerr << "No se encontró la casilla de inicio." << std::endl;
    }

    // Decidir el turno inicial
    decidirTurnoInicial();
}

// Decidir quién comienza lanzando un dado
void Sistema::decidirTurnoInicial() {
    int jugador1Tiro, jugador2Tiro;
    do {
        jugador1Tiro = std::rand() % 6 + 1; // Lanzar dado para jugador 1
        jugador2Tiro = std::rand() % 6 + 1; // Lanzar dado para jugador 2
        std::cout << "Jugador 1 tira: " << jugador1Tiro << ", Jugador 2 tira: " << jugador2Tiro << std::endl;
    } while (jugador1Tiro == jugador2Tiro); // Repetir si hay empate

    if (jugador1Tiro > jugador2Tiro) {
        std::cout << "Jugador 1 (Mario) comienza." << std::endl;
    } else {
        std::cout << "Jugador 2 (Luigi) comienza." << std::endl;
    }
}

void Sistema::mostrarMenuJugador(const char *jugador) {
    std::cout << "<--.........[ Es tu turno: " << jugador << " ].........-->" << std::endl;
    std::cout << "a) Lanzar dado" << std::endl;
    std::cout << "b) Buscar la estrella" << std::endl;
    std::cout << "c) Consultar saldo" << std::endl;
    std::cout << "Selecciona una opcion: ";
}

void Sistema::jugarPartida() {
    int rondas = 0;
    while (rondas < 20) { // Termina el juego después de 20 rondas
        const char *jugador = (turnoActual == 1) ? "Mario" : "Luigi";

        mostrarMenuJugador(jugador);

        char opcion;
        std::cin >> opcion;

        switch (opcion) {
            case 'a': // Tirar dado y mover al jugador
                tirarDado();
                break;
            case 'b': // Buscar estrella
                buscarEstrella();
                break;
            case 'c': // Consultar saldo
                consultarSaldo();
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente." << std::endl;
                continue;
        }

        // Cambiar turno después de cada selección válida
        turnoActual = (turnoActual == 1) ? 2 : 1;

        // Contar la ronda después de que ambos jugadores hayan jugado
        if (turnoActual == 1) {
            rondas++;
            std::cout << "Fin de la ronda " << rondas << std::endl;
        }
    }

    std::cout << "El juego ha terminado después de 20 rondas." << std::endl;
}

    void Sistema::tirarDado() {
        int dado = (std::rand() % 6) + 1; // Dado con valores entre 1 y 6
        std::cout << "Presiona Enter para lanzar el dado:\n";
        std::cin.ignore(); // Esperar a que el usuario presione Enter
        std::cout << "Ha salido el valor de: " << dado << "." << std::endl;

        Nodo* posicionActual = (turnoActual == 1) ? jugador1Pos : jugador2Pos;

        for (int i = 0; i < dado; i++) {
            if (posicionActual->numConexiones == 0) {
                std::cout << "No hay más conexiones desde aquí. Turno finalizado." << std::endl;
                break;
            }

            // Ofrecer opción de cortar camino si estamos en una casilla CORTE_DE_RUTA y tiene múltiples conexiones
            if (strcmp(posicionActual->tipo, "CORTE_DE_RUTA") == 0 && posicionActual->numConexiones > 1) {
                std::cout << "Has llegado a una casilla de corte de ruta antes de una bifurcación." << std::endl;
                std::cout << "¿Deseas cortar un camino? (7 monedas)" << std::endl;
                std::cout << "a) Cortar camino (7 monedas)\n";
                std::cout << "b) No hacer nada.\n";
                char opcion;
                std::cin >> opcion;

                if (opcion == 'a' && ((turnoActual == 1 && saldoJugador1 >= 7) || (turnoActual == 2 && saldoJugador2 >= 7))) {
                    if (turnoActual == 1) {
                        saldoJugador1 -= 7;
                    } else {
                        saldoJugador2 -= 7;
                    }
                    cortarCamino(posicionActual);
                }
            }

            // Mover al jugador en una bifurcación
            if (posicionActual->numConexiones > 1) {
                std::cout << "Has llegado a una bifurcación. Elige un camino:" << std::endl;
                for (int j = 0; j < posicionActual->numConexiones; j++) {
                    std::cout << j + 1 << ". " << posicionActual->conexiones[j]->id << std::endl;
                }
                int opcion;
                std::cin >> opcion;
                while (opcion < 1 || opcion > posicionActual->numConexiones) {
                    std::cout << "Opcion invalida. Elige un camino entre 1 y " << posicionActual->numConexiones << ": ";
                    std::cin >> opcion;

                }
                posicionActual = posicionActual->conexiones[opcion - 1];
            } else {
                // Si solo hay una conexión, mover automáticamente
                posicionActual = posicionActual->conexiones[0];
            }

            std::cout << "Te has movido a la casilla " << posicionActual->id << std::endl;

            // Aplicar el efecto de la casilla
            aplicarEfectoCasilla(posicionActual);
        }

        // Actualizar la posición del jugador después del movimiento
        actualizarPosicionJugador(posicionActual);
    }




void Sistema::buscarEstrella() {
// Determinar la posición actual del jugador
    Nodo* posicionActual = (turnoActual == 1) ? jugador1Pos : jugador2Pos;

    if (!posicionActual) {
        std::cerr << "Error: La posición del jugador no está definida." << std::endl;
        return;
    }

    // Verificar si la estrella está en la misma casilla que el jugador
    if (posicionActual == estrella) {
        std::cout << "¡Ya estás en la casilla con la estrella!" << std::endl;
        return;
    }

    // Obtener el número total de vértices
    int numVertices = grafo.getNumVertices();

    // Crear arreglos dinámicos para almacenar la distancia y el estado de visitado de cada nodo
    int* distancia = new int[numVertices];
    bool* visitado = new bool[numVertices];

    // Inicializar los arreglos
    for (int i = 0; i < numVertices; i++) {
        distancia[i] = -1;  // -1 indica que la distancia no se ha calculado
        visitado[i] = false; // Ningún nodo ha sido visitado inicialmente
    }

    // Cola para el BFS
    std::queue<Nodo*> cola;

    // Obtener el índice de la posición actual
    int inicioIdx = grafo.obtenerIndice(posicionActual);
    distancia[inicioIdx] = 0;
    visitado[inicioIdx] = true;
    cola.push(posicionActual);

    // Realizar la búsqueda en anchura (BFS)
    while (!cola.empty()) {
        Nodo* actual = cola.front();
        cola.pop();

        // Obtener el índice del nodo actual
        int actualIdx = grafo.obtenerIndice(actual);

        // Revisar cada conexión del nodo actual
        for (int i = 0; i < actual->numConexiones; i++) {
            Nodo* vecino = actual->conexiones[i];
            int vecinoIdx = grafo.obtenerIndice(vecino);

            // Si el vecino no ha sido visitado
            if (!visitado[vecinoIdx]) {
                visitado[vecinoIdx] = true;
                distancia[vecinoIdx] = distancia[actualIdx] + 1;

                // Si encontramos la estrella, mostramos la distancia y salimos
                if (vecino == estrella) {
                    std::cout << "¡Debes avanzar " << distancia[vecinoIdx] << " casillas para llegar a la estrella!" << std::endl;

                    // Liberar memoria y retornar
                    delete[] distancia;
                    delete[] visitado;
                    return;
                }

                // Agregar el vecino a la cola para continuar la búsqueda
                cola.push(vecino);
            }
        }
    }

    // Si la estrella no es accesible
    std::cout << "La estrella no es accesible desde tu posición actual." << std::endl;

    // Liberar memoria
    delete[] distancia;
    delete[] visitado;
}

void Sistema::consultarSaldo() {
    int saldoActual = (turnoActual == 1) ? saldoJugador1 : saldoJugador2;
    const char* jugador = (turnoActual == 1) ? "Mario" : "Luigi";
    std::cout << jugador << ", tu saldo actual es de: " << saldoActual << " monedas." << std::endl;
}

void Sistema::aplicarEfectoCasilla(Nodo *casilla) {
    int* saldo = (turnoActual == 1) ? &saldoJugador1 : &saldoJugador2;
    int* estrellas = (turnoActual == 1) ? &estrellasJugador1 : &estrellasJugador2;

    if (strcmp(casilla->tipo, "MONEDAS") == 0) {
        std::cout << "Has caído en una casilla de monedas.\n+3 Monedas" << std::endl;
        *saldo += 3;
    } else if (strcmp(casilla->tipo, "R_A") == 0) {
        std::cout << "Has caído en una casilla de mala suerte :(\n-2 Monedas" << std::endl;
        *saldo = (*saldo >= 2) ? (*saldo - 2) : 0;
    } else if (strcmp(casilla->tipo, "TTR") == 0) {
        if (casilla->numConexiones > 0) {
            std::cout << "Has caído en una casilla de teletransporte." << std::endl;
            Nodo* destino = casilla->conexiones[0]; // Teletransportar al primer destino definido
            std::cout << "Te has movido a la casilla: " << destino->id << std::endl;
            actualizarPosicionJugador(destino); // Cambia la posición del jugador al destino
        }
    } else if (strcmp(casilla->tipo, "STR") == 0) {
        if (casilla == estrella) { // La estrella está en la casilla actual
            std::cout << "¡Has encontrado una estrella! ¿Deseas comprarla?" << std::endl;
            std::cout << "1. Sí (Precio 10 monedas)" << std::endl;
            std::cout << "2. No" << std::endl;
            int opcionCompra;
            std::cin >> opcionCompra;
            if (opcionCompra == 1 && *saldo >= 10) {
                std::cout << "Has comprado la estrella." << std::endl;
                *saldo -= 10;
                (*estrellas)++;
                moverEstrella(); // Llamada para mover la estrella a una nueva casilla STR
            } else {
                std::cout << "No compraste la estrella." << std::endl;
            }
        } else {
            std::cout << "Esta casilla es de tipo estrella, pero la estrella no está aquí ahora." << std::endl;
        }
    }
}


// Método para actualizar la posición del jugador
void Sistema::actualizarPosicionJugador(Nodo* nuevaPosicion) {
    if (turnoActual == 1) {
        jugador1Pos = nuevaPosicion;
    } else {
        jugador2Pos = nuevaPosicion;
    }
}

void Sistema::cortarCamino(Nodo *casilla) {if (casilla->numConexiones > 1) {
        std::cout << "Has caído en una casilla de corte de ruta." << std::endl;
        if ((turnoActual == 1 ? saldoJugador1 : saldoJugador2) >= 7) {
            std::cout << "¿Deseas cortar un camino? (7 monedas)" << std::endl;
            std::cout << "1. Sí\n2. No" << std::endl;
            int opcion;
            std::cin >> opcion;
            if (opcion == 1) {
                int* saldo = (turnoActual == 1) ? &saldoJugador1 : &saldoJugador2;
                *saldo -= 7;

                // Elegir el camino a cortar
                int opcionCortar = -1;
                do {
                    std::cout << "Elige qué camino deseas cortar:" << std::endl;
                    for (int i = 0; i < casilla->numConexiones; i++) {
                        std::cout << i + 1 << ". " << casilla->conexiones[i]->id << std::endl;
                    }
                    std::cin >> opcionCortar;

                    if (std::cin.fail() || opcionCortar < 1 || opcionCortar > casilla->numConexiones) {
                        std::cin.clear(); // Limpiar el estado de error
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descartar entrada inválida
                        std::cout << "Opción no válida. Por favor, selecciona un número entre 1 y " << casilla->numConexiones << "." << std::endl;
                        opcionCortar = -1; // Reiniciar para forzar la validación de nuevo
                    }
                } while (opcionCortar < 1 || opcionCortar > casilla->numConexiones);

                // Eliminar la conexión seleccionada
                for (int i = opcionCortar - 1; i < casilla->numConexiones - 1; i++) {
                    casilla->conexiones[i] = casilla->conexiones[i + 1];
                }
                casilla->numConexiones--;
                casilla->conexiones = (Nodo**) realloc(casilla->conexiones, casilla->numConexiones * sizeof(Nodo*));
                std::cout << "¡Camino cortado!" << std::endl;
            }
        } else {
            std::cout << "No tienes suficientes monedas para cortar el camino." << std::endl;
        }
    } else {
        std::cout << "No hay suficientes caminos para cortar." << std::endl;
    }
}


void Sistema::finalizarJuego() {
    std::cout << "\nFin del juego. Resultados finales:" << std::endl;
    std::cout << "Jugador 1 (Mario): " << estrellasJugador1 << " estrellas, " << saldoJugador1 << " monedas." << std::endl;
    std::cout << "Jugador 2 (Luigi): " << estrellasJugador2 << " estrellas, " << saldoJugador2 << " monedas." << std::endl;

    // Determinar el ganador basado en las reglas
    if (estrellasJugador1 > estrellasJugador2) {
        std::cout << "¡Jugador 1 (Mario) gana por tener más estrellas!" << std::endl;
    } else if (estrellasJugador2 > estrellasJugador1) {
        std::cout << "¡Jugador 2 (Luigi) gana por tener más estrellas!" << std::endl;
    } else { // Empate en estrellas
        if (saldoJugador1 > saldoJugador2) {
            std::cout << "¡Jugador 1 (Mario) gana por tener más monedas!" << std::endl;
        } else if (saldoJugador2 > saldoJugador1) {
            std::cout << "¡Jugador 2 (Luigi) gana por tener más monedas!" << std::endl;
        } else {
            std::cout << "El juego ha terminado en empate." << std::endl;
        }
    }

    // Preguntar si el usuario quiere jugar nuevamente
    char opcion;
    std::cout << "\n¿Quieres jugar nuevamente? (S/N): ";
    std::cin >> opcion;

    if (opcion == 'S' || opcion == 's') {
        reiniciarJuego();
    } else {
        std::cout << "Apagando consola..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void Sistema::reiniciarJuego() {
    saldoJugador1 = 0;
    saldoJugador2 = 0;
    estrellasJugador1 = 0;
    estrellasJugador2 = 0;
    turnoActual = 1;

    // Reiniciar posiciones, cargas y grafo si es necesario
    cargarDatos();
    iniciarVideojuego();
    jugarPartida();
}

void Sistema::moverEstrella() {
    Nodo* nuevaEstrella = nullptr;

    // Buscar una nueva casilla de tipo STR que sea diferente a la actual
    do {
        nuevaEstrella = buscarCasillaTipo("STR"); // Encuentra una casilla aleatoria de tipo STR
    } while (nuevaEstrella == estrella);

    // Mover la estrella a la nueva casilla encontrada
    estrella = nuevaEstrella;
    if (estrella) {
        std::cout << "La estrella ha sido movida a la casilla: " << estrella->id << std::endl;
    } else {
        std::cerr << "Error al mover la estrella: No se encontró una nueva casilla de tipo STR." << std::endl;
    }
}









