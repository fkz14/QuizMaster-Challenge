#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED
#include "MENU.h"
#include "JUGADORES.h"

struct Respuesta {
    int nroRespuesta;
    int nroPreguntaAsociada;
    int correcta;
};
struct Pregunta {
    int numeroDePregunta;
    char enunciado[500];
    char opciones[4][100];
};
typedef struct Puntaje {
    int correctas;
    int incorrectas;
    int sinResponder;
    int puntajePorTiempos;
    int total;
} PuntajePtr;


typedef struct Respuesta *RespuestaPtr;
typedef struct Pregunta *PreguntaPtr;

/// ***************************  Declaracion de Funciones del Juego  ****************************

void leerPreguntasDesdeArchivo(PreguntaPtr *preguntas, int numPreguntas, const char *archivoNombre);
void leerRespuestasDesdeArchivo(RespuestaPtr *respuestas, int numRespuestas, const char* archivoNombre);
void jugarJuego(PreguntaPtr preguntas, RespuestaPtr respuestas, int numPreguntas, PuntajePtr *puntaje);
int seleccionDeJugadores();
void temporizadorDeLimpiezaPantalla(int seg);
void presentarPregunta(PreguntaPtr pregunta);
void actualizarPuntaje(RespuestaPtr respuestas, PreguntaPtr preguntas, int respuesta, PuntajePtr *puntaje, int tiempo);
void mostrarResultados(PuntajePtr *puntaje, int numPreguntas, JugadorPtr jugadorActual);
void hacerPreguntaMatematica(int *respuesta);
void encontrarJugadorGanador(JugadorPtr listaJugadores,int numJugadores);


#endif // JUEGO_H_INCLUDED
