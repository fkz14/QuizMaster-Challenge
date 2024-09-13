#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Para kbhit y getch
#include <time.h>  // Para time / para aleatorio
#include <windows.h>  // Para Sleep / para tomar los tiempo
#include "MENU.h"
#include "JUEGO.h"
#include "JUGADORES.h"

///************************ IMPLEMENTACION DE FUNCIONES DE LA BASE DE DATOS JUGADOR  *********************


JugadorPtr leerJugadoresDesdeArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    JugadorPtr listaJugadores = NULL;
    JugadorPtr jugadorActual = NULL;
    char linea[100];

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        JugadorPtr nuevoJugador = (JugadorPtr)malloc(sizeof(struct Jugador));
        if (nuevoJugador == NULL) {
            perror("Error al asignar memoria para un nuevo jugador");
            break;
        }

        // Leer los datos del jugador desde la línea del archivo
        if (sscanf(linea, "Alias:%s DNI:%d Puntos:%d Fecha:%s", nuevoJugador->alias, &(nuevoJugador->dni), &(nuevoJugador->puntajeTotal), nuevoJugador->fecha) == 4) {
            nuevoJugador->siguiente = NULL;
            if (listaJugadores == NULL) {
                listaJugadores = nuevoJugador;
                jugadorActual = nuevoJugador;
            } else {
                jugadorActual->siguiente = nuevoJugador;
                jugadorActual = nuevoJugador;
            }
        } else {
            printf("Error al leer los datos del jugador desde el archivo.\n");
            free(nuevoJugador);
        }
    }

    fclose(archivo);
    return listaJugadores;
}
void mostrarRankingJugadores(JugadorPtr listaJugadores) {
    system("cls");
    if (listaJugadores != NULL) {
        JugadorPtr jugadorActual = listaJugadores;
        printf("\n     |-------------------------------------------------------------------------------------------------- |");
        printf("\n     | *************************************** RANKING JUGADORES *************************************** |");
        printf("\n     |-------------------------------------------------------------------------------------------------- |\n\n");
        int posicion = 1;
        while (jugadorActual != NULL) {

            printf("   %5d#   Alias:      %10s          DNI:   %d          Puntos: %5d       Fecha: %s\n",posicion, jugadorActual->alias, jugadorActual->dni, jugadorActual->puntajeTotal, jugadorActual->fecha);
            jugadorActual = jugadorActual->siguiente;
            posicion=posicion+1;
        }
    } else {
        printf("No se pudo leer la lista de jugadores desde el archivo.\n");
    }
}
void agregarJugadorPorTeclado(JugadorPtr* listaJugadores) {
    JugadorPtr nuevoJugador = (JugadorPtr)malloc(sizeof(struct Jugador));
    if (nuevoJugador == NULL) {
        perror("Error al asignar memoria para un nuevo jugador");
        return;
    }

    printf("\n\n      Ingrese el alias del jugador: ");
    scanf("%s", nuevoJugador->alias);
    printf("      Ingrese el DNI del jugador: ");
    scanf("%d", &(nuevoJugador->dni));
    nuevoJugador->puntajeTotal = 0;

    // Llamar a obtenerFechaChar para obtener la fecha actual
    obtenerFechaChar(nuevoJugador->fecha);

    nuevoJugador->siguiente = NULL;

    // Buscar si ya existe un jugador con el mismo alias y DNI
    JugadorPtr jugadorActual = *listaJugadores;
    JugadorPtr jugadorAnterior = NULL;
    int jugadorExistente = 0;

    while (jugadorActual != NULL) {
        if (strcmp(nuevoJugador->alias, jugadorActual->alias) == 0 && nuevoJugador->dni == jugadorActual->dni) {
            // Actualizar los puntos del jugador existente
            jugadorActual->puntajeTotal = nuevoJugador->puntajeTotal;
            jugadorExistente = 1;
            break;
        }

        jugadorAnterior = jugadorActual;
        jugadorActual = jugadorActual->siguiente;
    }

    if (!jugadorExistente) {
        // Si el jugador no existe en la lista, agregarlo al final
        if (*listaJugadores == NULL) {
            *listaJugadores = nuevoJugador;
        } else {
            jugadorAnterior->siguiente = nuevoJugador;
        }

        printf("\n      Jugador agregado correctamente!\n");
        temporizadorDeLimpiezaPantalla(2);
    } else {
        printf("\n      Jugador existente sus puntos han sido reiniciados para comenzar una nueva partida.\n");
        temporizadorDeLimpiezaPantalla(2);
        free(nuevoJugador); // Liberar memoria si se creó un nuevo jugador innecesariamente
    }
}
void ordenarJugadoresPorPuntos(JugadorPtr* listaJugadores) {
    int intercambio;
    JugadorPtr ptr1;
    JugadorPtr lptr = NULL;

    if (*listaJugadores == NULL) {
        return; // No hay jugadores que ordenar
    }

    do {
        intercambio = 0;
        ptr1 = *listaJugadores;

        while (ptr1->siguiente != lptr) {
            if (ptr1->puntajeTotal < ptr1->siguiente->puntajeTotal ||
                (ptr1->puntajeTotal == ptr1->siguiente->puntajeTotal && strcmp(ptr1->fecha, ptr1->siguiente->fecha) < 0)) {
                // Intercambiar los jugadores si están en el orden incorrecto
                int tempPuntos = ptr1->puntajeTotal;
                ptr1->puntajeTotal = ptr1->siguiente->puntajeTotal;
                ptr1->siguiente->puntajeTotal = tempPuntos;

                char tempAlias[20];
                strcpy(tempAlias, ptr1->alias);
                strcpy(ptr1->alias, ptr1->siguiente->alias);
                strcpy(ptr1->siguiente->alias, tempAlias);

                int tempDNI = ptr1->dni;
                ptr1->dni = ptr1->siguiente->dni;
                ptr1->siguiente->dni = tempDNI;

                char tempFecha[11];
                strcpy(tempFecha, ptr1->fecha);
                strcpy(ptr1->fecha, ptr1->siguiente->fecha);
                strcpy(ptr1->siguiente->fecha, tempFecha);

                intercambio = 1;
            }
            ptr1 = ptr1->siguiente;
        }
        lptr = ptr1;
    } while (intercambio);
}
void eliminarJugadorPorAlias(JugadorPtr* listaJugadores) {
    if (*listaJugadores == NULL) {
        printf("La lista de jugadores está vacía. No se puede eliminar.\n");
        return;
    }

    char aliasAEliminar[20];
    printf("\n\n     Ingrese el alias del jugador a eliminar: ");
    scanf("%s", aliasAEliminar);

    JugadorPtr jugadorActual = *listaJugadores;
    JugadorPtr jugadorAnterior = NULL;
    int encontrado = 0;

    while (jugadorActual != NULL) {
        if (strcmp(aliasAEliminar, jugadorActual->alias) == 0) {
            encontrado = 1;
            break;
        }

        jugadorAnterior = jugadorActual;
        jugadorActual = jugadorActual->siguiente;
    }

    if (encontrado) {
        // Eliminar el jugador de la lista
        if (jugadorAnterior == NULL) {
            // El jugador a eliminar es el primer elemento de la lista
            *listaJugadores = jugadorActual->siguiente;
        } else {
            jugadorAnterior->siguiente = jugadorActual->siguiente;
        }

        printf("\n     Jugador con alias '%s' eliminado correctamente.\n", aliasAEliminar);
        temporizadorDeLimpiezaPantalla(2);
        free(jugadorActual); // Liberar la memoria del jugador eliminado
    } else {
        printf("\n     No se encontro ningun jugador con alias '%s'.\n", aliasAEliminar);
        temporizadorDeLimpiezaPantalla(2);
    }
}
void guardarJugadoresEnArchivo(JugadorPtr listaJugadores, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        perror("Error al abrir el archivo para escribir");
        return;
    }

    JugadorPtr jugadorActual = listaJugadores;

    while (jugadorActual != NULL) {
        fprintf(archivo, "Alias:%s DNI:%d Puntos:%d Fecha:%s\n", jugadorActual->alias, jugadorActual->dni, jugadorActual->puntajeTotal, jugadorActual->fecha);
        jugadorActual = jugadorActual->siguiente;
    }

    fclose(archivo);
    printf("\n\nLista de jugadores guardada en el archivo '%s' exitosamente.\n", nombreArchivo);
}
void obtenerFechaChar(char fecha[]) {
    time_t tiempo;
    struct tm* infoTiempo;

    time(&tiempo);
    infoTiempo = localtime(&tiempo);

    int dia = infoTiempo->tm_mday;
    int mes = infoTiempo->tm_mon + 1; // Agregar 1 porque en tm_mon enero es 0
    int anio = infoTiempo->tm_year + 1900; // Sumar 1900 porque tm_year cuenta desde 1900

    // Formatear la fecha como una cadena de caracteres "dd/mm/yyyy"
    snprintf(fecha, 11, "%02d/%02d/%04d", dia, mes, anio);
}
void liberarListaJugadores(JugadorPtr* listaJugadores) {
    while (*listaJugadores != NULL) {
        JugadorPtr jugadorAEliminar = *listaJugadores;
        *listaJugadores = (*listaJugadores)->siguiente;
        free(jugadorAEliminar);
    }
}
void ordenarJugadoresPorPuntosMenorAMayor(JugadorPtr* listaJugadores) {
    int intercambio;
    JugadorPtr ptr1;
    JugadorPtr lptr = NULL;

    if (*listaJugadores == NULL) {
        return; // No hay jugadores que ordenar
    }

    do {
        intercambio = 0;
        ptr1 = *listaJugadores;

        while (ptr1->siguiente != lptr) {
            if (ptr1->puntajeTotal > ptr1->siguiente->puntajeTotal ||
                (ptr1->puntajeTotal == ptr1->siguiente->puntajeTotal && strcmp(ptr1->fecha, ptr1->siguiente->fecha) > 0)) {
                // Intercambiar los jugadores si están en el orden incorrecto
                int tempPuntos = ptr1->puntajeTotal;
                ptr1->puntajeTotal = ptr1->siguiente->puntajeTotal;
                ptr1->siguiente->puntajeTotal = tempPuntos;

                char tempAlias[20];
                strcpy(tempAlias, ptr1->alias);
                strcpy(ptr1->alias, ptr1->siguiente->alias);
                strcpy(ptr1->siguiente->alias, tempAlias);

                int tempDNI = ptr1->dni;
                ptr1->dni = ptr1->siguiente->dni;
                ptr1->siguiente->dni = tempDNI;

                char tempFecha[11];
                strcpy(tempFecha, ptr1->fecha);
                strcpy(ptr1->fecha, ptr1->siguiente->fecha);
                strcpy(ptr1->siguiente->fecha, tempFecha);

                intercambio = 1;
            }
            ptr1 = ptr1->siguiente;
        }
        lptr = ptr1;
    } while (intercambio);
}
JugadorPtr copiarListaJugadores(JugadorPtr listaOriginal) {
    JugadorPtr nuevaLista = NULL;  // Inicializa la nueva lista como vacía
    JugadorPtr jugadorActual = listaOriginal;

    while (jugadorActual != NULL) {
        // Crea un nuevo jugador y copia los datos desde el jugador actual
        JugadorPtr nuevoJugador = (JugadorPtr)malloc(sizeof(struct Jugador));
        if (nuevoJugador == NULL) {
            perror("Error al asignar memoria para un nuevo jugador");
            break;
        }
        strcpy(nuevoJugador->alias, jugadorActual->alias);
        nuevoJugador->dni = jugadorActual->dni;
        nuevoJugador->puntajeTotal = jugadorActual->puntajeTotal;
        strcpy(nuevoJugador->fecha, jugadorActual->fecha);
        nuevoJugador->siguiente = NULL;

        // Agrega el nuevo jugador a la nueva lista
        if (nuevaLista == NULL) {
            nuevaLista = nuevoJugador;
        } else {
            // Busca el último elemento de la nueva lista y agrega el nuevo jugador al final
            JugadorPtr ultimoJugador = nuevaLista;
            while (ultimoJugador->siguiente != NULL) {
                ultimoJugador = ultimoJugador->siguiente;
            }
            ultimoJugador->siguiente = nuevoJugador;
        }

        jugadorActual = jugadorActual->siguiente;
    }

    return nuevaLista;
}
