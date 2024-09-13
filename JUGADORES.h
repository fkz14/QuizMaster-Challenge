#ifndef JUGADORES_H_INCLUDED
#define JUGADORES_H_INCLUDED
#include "MENU.h"
#include "JUEGO.h"

struct Jugador {
    char alias[20];
    int dni;
    int puntajeTotal;
    char fecha[11]; // Campo para almacenar la fecha en formato "dd/mm/yyyy"
    struct Jugador* siguiente;
};

typedef struct Jugador *JugadorPtr;


///************************ Funciones de la Base de datos "Jugador"

JugadorPtr leerJugadoresDesdeArchivo(const char* nombreArchivo);
void agregarJugadorPorTeclado(JugadorPtr* listaJugadores);
void ordenarJugadoresPorPuntosMenorAMayor(JugadorPtr* listaJugadores);
void ordenarJugadoresPorPuntos(JugadorPtr* listaJugadores);
void mostrarRankingJugadores(JugadorPtr listaJugadores);
void guardarJugadoresEnArchivo(JugadorPtr listaJugadores, const char* nombreArchivo);
void eliminarJugadorPorAlias(JugadorPtr* listaJugadores);
JugadorPtr copiarListaJugadores(JugadorPtr listaOriginal);
void obtenerFechaChar(char fecha[]);
void liberarListaJugadores(JugadorPtr* listaJugadores);



#endif // JUGADORES_H_INCLUDED
