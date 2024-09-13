#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Para kbhit y getch
#include <time.h>  // Para time / para aleatorio
#include <windows.h>  // Para Sleep / para tomar los tiempo
#include "MENU.h"
#include "JUEGO.h"

/// ***************************** INPLEMENTACION DE FUNCIONES DEl JUEGO PRINCIPAL ****************************

void jugarJuego(PreguntaPtr preguntas, RespuestaPtr respuestas, int numPreguntas, PuntajePtr *puntaje) {            ///*************** Algoritmo del juego (principal).
    JugadorPtr listaJugadores = leerJugadoresDesdeArchivo("jugadores.txt");
    int numJugadores;
    printf("\n    --------------------------------------------------------------------------------------------------");
    printf("\n    *   Configuracion de partida                                                                     *");
    printf("\n    --------------------------------------------------------------------------------------------------\n\n");
    printf("\n\n      Ingrese la cantidad de jugadores (de 1 a 4): ");
    while (1) {
        if (kbhit()) {
            char tecla = getch();

            if (tecla >= '1' && tecla <= '4') {
                numJugadores = tecla - '0';  // Convierte el carácter en un número
                break;  // Sal del bucle si se seleccionó un número válido
            }
        }
    }
    fflush(stdin);

    if (numJugadores < 1 || numJugadores > 4) {
        printf("Numero de jugadores no valido. Debe ser de 1 a 4.\n");
        return;
    }


    // Llama a agregarJugadorPorTeclado para agregar a los jugadores
    for (int i = 0; i < numJugadores; i++) {
        system("cls");
        printf("\n    -------------------------------------------------------------------------------------------------- ");
        printf("\n    *   Jugador # %d                                                                                 *", i+1);
        printf("\n    -------------------------------------------------------------------------------------------------- \n\n");
        agregarJugadorPorTeclado(&listaJugadores);
        ordenarJugadoresPorPuntosMenorAMayor(&listaJugadores);
    }
        JugadorPtr jugadorActual = listaJugadores;
    for (int jugador = 1; jugador <= numJugadores; jugador++) {
        //JugadorPtr jugadorActual = listaJugadores;
        fflush(stdin);
        printf("\n\n\n\n\n\n\n                         Jugador %d (%s), presione Enter para comenzar su turno...", jugador, jugadorActual->alias);
        getchar();

        fflush(stdin);
        puntaje->correctas = 0;
        puntaje->incorrectas = 0;
        puntaje->sinResponder = 0;
        puntaje->puntajePorTiempos = 0;
        puntaje->total = 0;

        srand(time(NULL));
        int preguntasDisponibles[numPreguntas];
        for (int i = 0; i < numPreguntas; i++) {
            preguntasDisponibles[i] = i;  // Inicialmente, todas las preguntas están disponibles.
        }
            int intentos =0;
        for (int i = 0; i < numPreguntas; i++) {
            int min = 0;   // Valor mínimo
            int max = numPreguntas - i - 1; // Valor máximo (disminuye en cada iteración)

            // Genera un número aleatorio entre min y max
            int indexAleatorio = rand() % (max - min + 1) + min;
            int numeroAleatorio = preguntasDisponibles[indexAleatorio];

            system("cls");
            printf("\n    -------------------------------------------------------------------------------------------------- ");
            printf("\n    *   Pregunta # %d para Jugador %d Alias: (%s)                                                    *", i + 1, jugador,jugadorActual->alias);
            printf("\n    -------------------------------------------------------------------------------------------------- \n\n");
            presentarPregunta(&preguntas[numeroAleatorio]);

            int respuesta = -1;
            int tiempoRestante =  15;
            int tiempoRespuesta;
            printf("    --------------------------------------------------------------------------------------------------");
            printf("\n    *     Tienes %d intentos restante.                                                               *",3-intentos);
            printf("\n    -------------------------------------------------------------------------------------------------- \n\n");
            while (tiempoRestante > 0) {
                Sleep(1000);
                tiempoRestante--;

                if (kbhit()) {
                    respuesta = _getch() - '0';
                    if (respuesta >= 1 && respuesta <= 4) {
                        break;
                    }
                }


                printf("\r                                   Tiempo restante: %d segundos.", tiempoRestante);
                fflush(stdout);
            }
            tiempoRespuesta = tiempoRestante + 1;
            printf("\n");
            actualizarPuntaje(&respuestas[numeroAleatorio], &preguntas[numeroAleatorio], respuesta, puntaje, tiempoRespuesta);

            temporizadorDeLimpiezaPantalla(3);

            //int intentos;
            intentos = puntaje->incorrectas + puntaje->sinResponder;
            if (intentos == 3) {
                i = i + numPreguntas;
            }
            // Elimina la pregunta utilizada de las disponibles
            for (int j = indexAleatorio; j < max; j++) {
                preguntasDisponibles[j] = preguntasDisponibles[j + 1];
            }

            preguntasDisponibles[max] = -1; // Marca la pregunta como utilizada

        }
        jugadorActual->puntajeTotal = puntaje->total;
        mostrarResultados(puntaje, numPreguntas, jugadorActual);
        jugadorActual = jugadorActual->siguiente;
    }
    encontrarJugadorGanador(listaJugadores,numJugadores);
    ordenarJugadoresPorPuntos(&listaJugadores);
    guardarJugadoresEnArchivo(listaJugadores, "jugadores.txt");
    liberarListaJugadores(&listaJugadores);

}
void leerPreguntasDesdeArchivo(PreguntaPtr *preguntas, int numPreguntas, const char *archivoNombre) {
    FILE *archivo = fopen(archivoNombre, "r");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de preguntas.\n");
        return;
    }

    *preguntas = malloc(numPreguntas * sizeof(struct Pregunta));  // Asignar memoria

    if (*preguntas == NULL) {
        printf("Error al asignar memoria para preguntas.\n");
        fclose(archivo);
        return;
    }

    for (int i = 0; i < numPreguntas; i++) {
        // Leer la línea completa
        char linea[500];
        if (fgets(linea, sizeof(linea), archivo) == NULL) {
            printf("Error al leer la pregunta %d.\n", i + 1);
            fclose(archivo);
            free(*preguntas);  // Liberar memoria en caso de error
            return;
        }

        // Utilizar sscanf para extraer el número de la línea
        int numeroPregunta;
        if (sscanf(linea, "Pregunta %d", &numeroPregunta) != 1) {
            printf("Error al leer el número de la pregunta %d.\n", i + 1);
            fclose(archivo);
            free(*preguntas);  // Liberar memoria en caso de error
            return;
        }

        // Asignar el número de pregunta
        (*preguntas)[i].numeroDePregunta = numeroPregunta;

        // Leer el enunciado
        fgets((*preguntas)[i].enunciado, sizeof((*preguntas)[i].enunciado), archivo);

        for (int j = 0; j < 4; j++) {
            // Leer las opciones
            fgets((*preguntas)[i].opciones[j], sizeof((*preguntas)[i].opciones[j]), archivo);
        }

        // Leer el separador de preguntas (por ejemplo, una línea vacía o un carácter especial)
        fgetc(archivo);

    }

    fclose(archivo);
}
void leerRespuestasDesdeArchivo(RespuestaPtr *respuestas, int numRespuestas, const char* archivoNombre) {
    FILE *archivo = fopen(archivoNombre, "r");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de respuestas.\n");
        return;
    }

    *respuestas = malloc(numRespuestas * sizeof(struct Respuesta));

    char linea[100];
    int i = 0;

    while (fgets(linea, sizeof(linea), archivo) != NULL && i < numRespuestas) {
        if (sscanf(linea, "Pregunta %d", &(*respuestas)[i].nroPreguntaAsociada) == 1) {
            // Leer el número de pregunta asociada.
        } else if (sscanf(linea, "Respuesta correcta opcion %d", &(*respuestas)[i].correcta) == 1) {
            (*respuestas)[i].nroRespuesta = i + 1;
            i++;
        }
    }

    fclose(archivo);
}
void temporizadorDeLimpiezaPantalla(int seg) {
    int segundosTranscurridos = 0;

    while (segundosTranscurridos < seg && !_kbhit()) {
        Sleep(1000);
        segundosTranscurridos++;
    }

    if (_kbhit()) {
        _getch();
    }
    fflush(stdin);
    system("cls");
}
void presentarPregunta(PreguntaPtr pregunta) {

    printf("\n      %s  \n\n", pregunta->enunciado);

    for (int j = 0; j < 4; j++) {
        printf("      %d. > %s\n", j + 1, pregunta->opciones[j]);
    }
    printf("\n");
}
void actualizarPuntaje(RespuestaPtr respuestas, PreguntaPtr preguntas, int respuesta, PuntajePtr *puntaje, int tiempo) {
    int puntajeObtenido = 0;

    if (respuesta == -1) {
        printf("\n    Tiempo agotado. No se selecciono ninguna respuesta.\n");
        puntaje->sinResponder++;
    } else {
        if (respuesta == respuestas->correcta) {
            printf("\n    Respuesta correcta! :)\n");
            puntaje->correctas++;
            puntajeObtenido = tiempo * 1;
        } else {
            printf("\n    Respuesta incorrecta. :( \n    La respuesta correcta es: %d %s", respuestas->correcta, preguntas->opciones[respuestas->correcta - 1]);
            puntaje->incorrectas++;
        }
    }

    puntaje->puntajePorTiempos += puntajeObtenido;
    puntaje->total = (puntaje->puntajePorTiempos + (puntaje->correctas * 10));
    printf("    Puntaje por tiempo: %d\n", puntajeObtenido);
    //temporizadorDeLimpiezaPantalla(3);
}
void mostrarResultados(PuntajePtr *puntaje, int numPreguntas, JugadorPtr jugadorActual) {
    system("cls");
    printf("\n    --------------------------------------------------------------------------------------------------");
    printf("\n    *   Estadisticas del jugador                                                                     *");
    printf("\n    --------------------------------------------------------------------------------------------------\n");
    printf("\n\n    Puntuacion final de %s:\n\n", jugadorActual->alias);
    printf("    Preguntas en total:  %d/%d\n", puntaje->correctas+puntaje->incorrectas, numPreguntas);
    printf("    Respuestas correctas: %d\n", puntaje->correctas);
    printf("    Respuestas incorrectas: %d\n", puntaje->incorrectas);
    printf("    Preguntas sin responder: %d\n", puntaje->sinResponder);
    printf("    Puntaje por tiempos: %d\n", puntaje->puntajePorTiempos);
    printf("\n\n    Puntaje total: %d\n", puntaje->total);
    temporizadorDeLimpiezaPantalla(60);
}
int seleccionDeJugadores() {
    int cantidadDeJugadores;

    printf("\n     |-------------------------------------------------------------------------------------------------- |");
    printf("\n     |  NUEVA PARTIDA                                                                                    |");
    printf("\n     |-------------------------------------------------------------------------------------------------- |\n\n");

    printf("\n     Selecciona la cantidad de Jugadores para la siguiente partida:\n\n");
    printf("     De 1 a 4 Jugadores máximo: ");
    scanf("%d", &cantidadDeJugadores);

    while (cantidadDeJugadores < 1 || cantidadDeJugadores > 4) {
        printf("     Opcion no valida. Por favor, selecciona una opcion valida (1 a 4 Jugadores): ");
        scanf("%d", &cantidadDeJugadores);
    }

    return cantidadDeJugadores;
}
void encontrarJugadorGanador(JugadorPtr listaJugadores,int numJugadores) {
    if (listaJugadores == NULL) {
        printf("La lista de jugadores es nula.\n");
        return;
    }
    //JugadorPtr mejorJugador = listaJugadores;
    int valorFijo = 576;

    struct { // Crear un array de jugadores auxiliares
        char alias[20];
        int numeroIngresado;
        int diferencia;
    } vectorDeJugadores[4];

    JugadorPtr jugadorActual = listaJugadores;

    int cont =0;
    int contadorDeMaximo = 0;
    int puntajeMaximo = -1; // Valor máximo inicial


    while (jugadorActual != NULL && cont < numJugadores) { // Encontrar el puntaje máximo entre los jugadores
        if (jugadorActual->puntajeTotal > puntajeMaximo) {
            puntajeMaximo = jugadorActual->puntajeTotal; // Si encontramos un nuevo puntaje máximo, reiniciamos el contador
            contadorDeMaximo = 1;
            } else if (jugadorActual->puntajeTotal == puntajeMaximo) { // Si encontramos un jugador con el mismo puntaje máximo, incrementamos el contador
            contadorDeMaximo++;
            }
        jugadorActual = jugadorActual->siguiente;
        cont++;
    }

    jugadorActual = listaJugadores;


    if(contadorDeMaximo!=1){
        printf("\n\n\n                                        ' HAY UN EMPATE! '\n\n");
        printf("\n               * Pregunta de desempate por +10 puntos, Cuanto es: (8 al cubo + 64)? \n\n\n");

        int contador = 0;
        while (jugadorActual != NULL && contador < numJugadores) {  // Realizar la pregunta matemática a los jugadores con el puntaje máximo
            if (jugadorActual->puntajeTotal == puntajeMaximo) {
                printf("                           Contesta el jugador con Alias (%s) :", jugadorActual->alias);
                strcpy(vectorDeJugadores[contador].alias, jugadorActual->alias);
                hacerPreguntaMatematica(&(vectorDeJugadores[contador].numeroIngresado)); // Hacer la pregunta matemática a cada jugador y almacenar el resultado
                vectorDeJugadores[contador].diferencia = abs(vectorDeJugadores[contador].numeroIngresado - valorFijo);
                contador++;
            }
            jugadorActual = jugadorActual->siguiente;
        }

        int indiceGanador = 0;
        int menorDiferencia = vectorDeJugadores[0].diferencia;

        jugadorActual = listaJugadores;

        int contador3 = 0;

        for (int i = 0; i < contador3; i++) {
            if (vectorDeJugadores[i].diferencia < menorDiferencia) {
            menorDiferencia = vectorDeJugadores[i].diferencia;
            indiceGanador = i;
            }
            jugadorActual = jugadorActual->siguiente;
        }

        int indiceActual = 0;

        while (jugadorActual != NULL && indiceActual < indiceGanador) {
            jugadorActual = jugadorActual->siguiente;
            indiceActual++;
        }

        if (jugadorActual != NULL && indiceActual == indiceGanador) {  // Verifica si se llegó al nodo deseado
            jugadorActual->puntajeTotal = jugadorActual->puntajeTotal+10;  // Asigna el nuevo puntaje al nodo en la posición deseada
            //printf("\n\n      Se ha asignado un nuevo puntaje de +10 al ganador \n");
        } else {
            printf("                          El índice está fuera de los límites de la lista.\n");
        }

        printf("\n\n");
        printf("                             ---------------------------------------\n");
        printf("                             *  EL JUGADOR (%s) ES EL GANADOR!  *   \n",vectorDeJugadores[indiceGanador].alias);
        printf("                             ---------------------------------------\n\n\n");
        printf("\n                       con una diferencia de %d respecto al resultado %d.\n\n",menorDiferencia,valorFijo);

           temporizadorDeLimpiezaPantalla(80);
    }

    JugadorPtr mejorJugador = listaJugadores;

    int contador = 0;

    // Buscar el jugador con la puntuación más alta entre los primeros cuatro
    while (jugadorActual != NULL && contador < numJugadores) {
        if (jugadorActual->puntajeTotal > mejorJugador->puntajeTotal) {
            mejorJugador = jugadorActual;
        }
        jugadorActual = jugadorActual->siguiente;
        contador++;
    }
    if(numJugadores < 2){
        printf("\n\n\n                                  '' GRACIAS POR JUGAR '' \n\n");
    }else{
    printf("\n\n\n                                   '' El GANADOR ES: '' \n\n");
    }
    // Imprimir el mejor jugador de los primeros cuatro

    printf("                            ----------");
    printf("\n                    Alias: | %5s    |       Puntos obtenidos: %d\n", mejorJugador->alias, mejorJugador->puntajeTotal);
    printf("                            ----------\n");

    printf("\n\n");
    printf("                            ___             p*********p\n");
    printf("                          (*   *)        **            **p\n");
    printf("                         (*     *)    **                  **\n");
    printf("                         (*     *)  **      ____`    ___    **\n");
    printf("                         (*     *)**        ( o )   ( o )     **\n");
    printf("                          (*   *)  *                           **\n");
    printf("                        (***********)                          **\n");
    printf("                      (*             )     **                  **\n");
    printf("                      (*             )     **           /      **\n");
    printf("                     (**   ***********)     *9        p*       **\n");
    printf("                     (                 )      `**-**'         **\n");
    printf("                     (*                )                     **\n");
    printf("                      (*   ***********/**                  p**\n");
    printf("                       (            )   **9            pp**\n");
    printf("                         ***********       ***9*9999****\n\n\n\n");
    temporizadorDeLimpiezaPantalla(80);
}
void hacerPreguntaMatematica(int *respuesta) {
    // Genera una pregunta matemática (puedes personalizarla)
    //printf("Pregunta matemática: 5 x 8 = ?");
    scanf("%d", respuesta);
}
