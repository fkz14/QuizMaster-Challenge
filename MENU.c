#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Para kbhit y getch
#include <time.h>  // Para time / para aleatorio
#include <windows.h>  // Para Sleep / para tomar los tiempo
#include "MENU.h"
#include "JUEGO.h"
#include "JUGADORES.h"


/// ***************************** INPLEMENTACION DE FUNCIONES DE MENU ****************************

void opcionesDeMenu() {
    int opcion;
    mostrarMenu();
    int tiempoRestante = 120;

    while (!kbhit() && tiempoRestante > 0) {
        // Espera hasta que se presione una tecla o transcurra el tiempo deseado
        Sleep(1000);
        tiempoRestante--;
    }

    if (kbhit()) {
        opcion = getch() - '0'; // Convierte el carácter ingresado a un entero
        switch (opcion) {
            case 1:
                comenzarJuego();
                break;
            case 2:
                ranking();
                break;
            case 3:
                salir();
                break;
            default:
                printf("Opcion no valida. Por favor, selecciona una opcion valida.\n");
                break;
        }
    } else {
        ranking();
    }
}
void mostrarMenu() {
    printf("\n        Selecciona una opcion: \n\n");
    printf("        1. Comenzar Juego\n");
    printf("        2. Ver Ranking de Jugadores por Puntos\n");
    printf("        3. Salir del Juego\n");
}
void comenzarJuego() {    // main para juego

    mostrarReglas();

    int numPreguntas = 100;
    PreguntaPtr preguntas;
    RespuestaPtr respuestas;
    PuntajePtr puntaje;

    leerPreguntasDesdeArchivo(&preguntas, numPreguntas, "preguntas.txt");
    leerRespuestasDesdeArchivo(&respuestas, numPreguntas, "respuestas.txt");
    jugarJuego(preguntas, respuestas, numPreguntas, &puntaje);

    free(preguntas);
    free(respuestas);

}
void ranking() {          // main para base de datos

    JugadorPtr listaJugadores = leerJugadoresDesdeArchivo("jugadores.txt");
    mostrarRankingJugadores(listaJugadores);

    int tiempoRestante = 120; // Establece el tiempo en segundos

    printf("\n\n     1. Eliminar un jugador del ranking\n");
    printf("     2. Volver al Menu Principal\n");
    printf("\n     Selecciona una opcion: ");

    while (1) {
        if (kbhit()) {
            int tecla = getch();
            if (tecla == '1') {
                // Opción 1: Eliminar jugador del ranking
                mostrarRankingJugadores(listaJugadores);
                eliminarJugadorPorAlias(&listaJugadores);
                mostrarRankingJugadores(listaJugadores); // Mostrar la lista actualizada
                printf("\n\n     1. Eliminar un jugador del ranking\n");
                printf("     2. Volver al Menu Principal\n");
                printf("\n     Selecciona una opcion: ");
                tiempoRestante = 120;

            } else if (tecla == '2') {
                // Opción 2: Volver al menú principal
                break;
            }
        }
        if (tiempoRestante > 0) {
            Sleep(1000);
            tiempoRestante--;
        } else {
            // Si no se presiona ninguna tecla y pasa el tiempo, volver al menú principal automáticamente
            break;
        }
    }


    guardarJugadoresEnArchivo(listaJugadores, "jugadores.txt");
    liberarListaJugadores(&listaJugadores);
}
void mostrarBienvenida() {
    system("cls"); // Limpia la pantalla
    printf("\n\n\n");
    printf("          /////////////////////////////////////////////////////////////////////////////////////////////////////\n");
    printf("          //                                                                                                 //\n");
    printf("          //  `7MM'''Mq.                                                       mm                            //\n");
    printf("          //    MM   `MM.                                                      MM                            //\n");
    printf("          //    MM   ,M9 `7Mb,od8  .gP'Ya   .P'Ybm   `7MM  `7MM  `7MMpMMMb.  mmMMmm   ,6'Yb.       pd*''b.   //\n");
    printf("          //    MMmmdM9    MM' '' ,M'   Yb :MI  I8     MM    MM    MM    MM    MM    8)   MM      (O)   j8   //\n");
    printf("          //    MM         MM     8M''''''  WmmmP'     MM    MM    MM    MM    MM     ,pm9MM  MMM     ,;j9   //\n");
    printf("          //    MM         MM     YM.    , 8M          MM    MM    MM    MM    MM    8M   MM       ,-='      //\n");
    printf("          //  .JMML.     .JMML.    `Mbmmd'  YMMMMMb    `Mbod'YML..JMML  JMML.  `Mbmo `Moo9^Yo.    Ammmmmmm   //\n");
    printf("          //                               6'     dP                                                         //\n");
    printf("          //                               Ybmmmd'                                                           //\n");
    printf("          //                                                                                                 //\n");
    printf("          /////////////////////////////////////////////////////////////////////////////////////////////////////\n");

    printf("\n\n\n                                    Bienvenido al juego de preguntas y respuestas!                            \n\n\n");

}
void mostrarReglas() {
    system("cls");
    printf("\n\n");
    printf("          /////////////////////////////////////////////////////////////////////////////////////////////////////\n");
    printf("          //                                                                                                 //\n");
    printf("          //                                                                                                 //\n");
    printf("          //    Reglas del juego:                                                                            //\n");
    printf("          //                                                                                                 //\n");
    printf("          //                                                                                                 //\n");
    printf("          //  * Cada turno consiste en una pregunta de opcion multiple que aparecera en pantalla durante     //\n");
    printf("          //    15 segundos.                                                                                 //\n");
    printf("          //  * Los jugadores tienen 15 segundos para seleccionar la respuesta que consideren correcta de    //\n");
    printf("          //    las cuatro opciones disponibles.                                                             //\n");
    printf("          //  * Si un jugador selecciona la respuesta correcta, obtiene 10 puntos.                           //\n");
    printf("          //  * Se otorgaran puntos adicionales, en base al tiempo de su respuesta.                          //\n");
    printf("          //  * Si un jugador selecciona una respuesta incorrecta o no selecciona ninguna respuesta antes    //\n");
    printf("          //    de que se acabe el tiempo, no recibe puntos.                                                 //\n");
    printf("          //  * El juego continua con una nueva pregunta despues de cada turno.                              //\n");
    printf("          //  * Despues de tres respuestas incorrectas o de no responder nada el juego se termina.           //\n");
    printf("          //  * Luego de terminar la partida el juego mostrara las estadisticas de los jugadores             //\n");
    printf("          //  * El jugador con la mayor cantidad de puntos al final del juego es declarado ganador.          //\n");
    printf("          //                                                                                                 //\n");
    printf("          //                                                                                                 //\n");
    printf("          //                                   Buena suerte en las respuestas!                               //\n");
    printf("          //                                                                                                 //\n");
    printf("          //                                                                                                 //\n");
    printf("          /////////////////////////////////////////////////////////////////////////////////////////////////////\n");

    printf("\n\n  Presiona una tecla para continuar...");

    temporizadorDeLimpiezaPantalla(120);
}
void salir() {
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n                                           Gracias por jugar. Hasta luego!\n\n\n\n\n\n");
    printf("\n\n\n\n      Creditos:\n\n");
    printf("      Alumno: Almiron Alejo.\n");
    printf("      Materia: Programacion de computadoras.\n");
    printf("      Profesores: Nicolas Perez - Jorge Golfieri.\n");
    printf("      Anio: 2023.\n");
    printf("      Institucion: Universidad de Lanus.\n");
    temporizadorDeLimpiezaPantalla(30);
    exit(0);
}
