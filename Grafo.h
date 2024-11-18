#ifndef GRAFO_H
#define GRAFO_H

#include "Nodo.h"

class Grafo {
private:
    Nodo** vertices;
    int numVertices;

public:
    Grafo();
    ~Grafo();

    bool cargarVerticesDesdeCasillas(const char* filename);
    bool cargarConexionesDesdeBoard(const char* filename);
    void mostrarGrafo() const;
    int getNumVertices() const;
    Nodo* getVertice(int index) const;
    int obtenerIndice(Nodo* nodo) const;
};

#endif // GRAFO_H
