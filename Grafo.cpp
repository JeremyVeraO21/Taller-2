#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

// Constructor de la clase Grafo
Grafo::Grafo() : vertices(nullptr), numVertices(0) {}

// Destructor de la clase Grafo
Grafo::~Grafo() {
    for (int i = 0; i < numVertices; i++) {
        delete vertices[i];
    }
    free(vertices);
}

// Método para cargar vértices desde casillas.csv
bool Grafo::cargarVerticesDesdeCasillas(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return false;
    }

    char line[256];
    while (file.getline(line, sizeof(line))) {
        // Extraer el ID y el tipo
        char* id = strtok(line, "#");
        char* tipo = strtok(nullptr, "#");

        if (!id || !tipo) {
            std::cerr << "Formato incorrecto en " << filename << " en línea: " << line << std::endl;
            continue;
        }

        // Crear un nuevo vértice
        Nodo* nodo = new Nodo(id, tipo);

        // Expander el arreglo dinámico de vértices
        numVertices++;
        vertices = (Nodo**)realloc(vertices, numVertices * sizeof(Nodo*));
        if (!vertices) {
            std::cerr << "Error de asignación de memoria para vertices" << std::endl;
            exit(1);
        }

        // Almacenar el nuevo nodo en el arreglo
        vertices[numVertices - 1] = nodo;
    }

    file.close();
    return true;
}
// Método para cargar conexiones desde board.txt
bool Grafo::cargarConexionesDesdeBoard(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return false;
    }

    char line[256];
    while (file.getline(line, sizeof(line))) {
        // Procesar la línea para extraer el ID del vértice origen y sus conexiones
        char* origenID = strtok(line, "#");
        char* conexionesStr = strtok(nullptr, "{}");

        if (!origenID || !conexionesStr) {
            std::cerr << "Formato incorrecto en " << filename << " en línea: " << line << std::endl;
            continue;
        }

        // Buscar el vértice de origen en el arreglo
        Nodo* origen = nullptr;
        for (int i = 0; i < numVertices; i++) {
            if (strcmp(vertices[i]->id, origenID) == 0) {
                origen = vertices[i];
                break;
            }
        }

        if (!origen) {
            std::cerr << "Vértice de origen no encontrado: " << origenID << std::endl;
            continue;
        }

        // Procesar cada conexión y agregarla al vértice de origen
        char* conexionID = strtok(conexionesStr, ",");
        while (conexionID) {
            // Remover espacios en blanco al inicio y final de cada ID de conexión
            while (*conexionID == ' ') conexionID++;
            char* connEnd = conexionID + strlen(conexionID) - 1;
            while (connEnd > conexionID && (*connEnd == ' ' || *connEnd == '\n' || *connEnd == '\r')) connEnd--;
            *(connEnd + 1) = '\0';

            // Buscar el vértice de destino en el arreglo
            Nodo* destino = nullptr;
            for (int i = 0; i < numVertices; i++) {
                if (strcmp(vertices[i]->id, conexionID) == 0) {
                    destino = vertices[i];
                    break;
                }
            }

            if (destino) {
                origen->agregarConexion(destino);  // Agregar la conexión al vértice origen
            } else {
                std::cerr << "Vértice de destino no encontrado: " << conexionID << std::endl;
            }

            conexionID = strtok(nullptr, ",");
        }
    }

    file.close();
    return true;
}

// Método para mostrar el grafo
void Grafo::mostrarGrafo() const {
    std::cout << "Conexiones en el grafo:" << std::endl;
    for (int i = 0; i < numVertices; i++) {
        std::cout << "Vértice ID: " << vertices[i]->id << ", Tipo: " << vertices[i]->tipo << std::endl;
        for (int j = 0; j < vertices[i]->numConexiones; j++) {
            std::cout << "  -> " << vertices[i]->conexiones[j]->id << std::endl;
        }
    }
}

int Grafo::getNumVertices() const {
    return numVertices;
}
Nodo* Grafo::getVertice(int index) const {
    if (index < 0 || index >= numVertices) {
        return nullptr;  // Índice fuera de rango
    }
    return vertices[index];
}

int Grafo::obtenerIndice(Nodo *nodo) const {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == nodo) {
            return i;
        }
    }
    return -1; // Retorna -1 si no se encuentra el nodo
}



