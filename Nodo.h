#ifndef NODO_H
#define NODO_H

#include <iostream>
#include <cstring>

class Nodo {
public:
    char* id;
    char* tipo;
    Nodo** conexiones;
    int numConexiones;

    enum tipo {
        MONEDAS,
        R_A,
        TTR,
        INICIO,
        STR,
        CORTE_DE_RUTA
    } tipoEspecial;

    Nodo(const char* id, const char* tipo);
    ~Nodo();
    void agregarConexion(Nodo* vertice);
};

#endif // NODO_H
