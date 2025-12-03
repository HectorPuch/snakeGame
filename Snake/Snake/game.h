#pragma once

#include <vector>
#include <ctime>
#include <string> // std::vector, std::string

#include "constants.h"

// Estructura para coordenadas (serpiente y fruta)
struct punto {
	int x;
	int y;
};

// Enumeración para la dirección de la serpiente
enum direccion {
	PARAR = 0,
	ARRIBA,
	ABAJO,
	IZQUIERDA,
	DERECHA
};

// Estructura que contiene el estado del juego
struct estadoJuego {
	bool finDelJuego;
	long long puntuacion;
	direccion direccionActual;
	direccion proximaDireccion;

	char tablero[ALTO_TABLERO][ANCHO_TABLERO];

	std::vector<punto> serpiente;
	punto fruta;
};

// Funciones de control del juego
void configurar(estadoJuego& estado);
void entrada(estadoJuego& estado);
void logica(estadoJuego& estado);
void dibujar(const estadoJuego& estado);

// Funciones de soporte internas
void inicializarTablero(estadoJuego& estado);
void colocarSerpiente(estadoJuego& estado);
void colocarFruta(estadoJuego& estado);
void limpiarPantalla();
void moverSerpiente(estadoJuego& estado);
void comprobarColision(estadoJuego& estado);
void actualizarPuntuacion(estadoJuego& estado);