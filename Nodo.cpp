#include "Nodo.h"
#include <cstdlib>

Nodo::Nodo(const char* id, const char* tipo): numConexiones(0), conexiones(nullptr) {
    this->id = new char[strlen(id) + 1];
    strcpy(this->id, id);

    this->tipo = new char[strlen(tipo) + 1];
    strcpy(this->tipo, tipo);

    if (strcmp(tipo, "MONEDAS") == 0) {
        tipoEspecial = MONEDAS;
    } else if (strcmp(tipo, "R_A") == 0) {
        tipoEspecial = R_A;
    } else if (strcmp(tipo, "TTR") == 0) {
        tipoEspecial = TTR;
    } else if (strcmp(tipo, "INICIO") == 0) {
        tipoEspecial = INICIO;
    } else if (strcmp(tipo, "STR") == 0) {
        tipoEspecial = STR;
    } else if (strcmp(tipo, "CORTE_DE_RUTA") == 0) {
        tipoEspecial = CORTE_DE_RUTA;
    }
}

Nodo::~Nodo() {
    delete[] id;
    delete[] tipo;
    free(conexiones);
}


void Nodo::agregarConexion(Nodo *vertice) {
    numConexiones++;
    conexiones = (Nodo**) realloc(conexiones, numConexiones * sizeof(Nodo*));
    conexiones[numConexiones - 1] = vertice;

}
