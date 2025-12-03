#include <iostream>
#include <cstdlib>
#include <ctime>

#include "game.h"
#include "keyboard.h"

void limpiarPantalla() {
	system("cls"); // Limpiar pantalla en windows
}

void colocarFruta(estadoJuego& estado) {
	int randX, randY;
	bool colocada = false;

	// Buscamos una posición vacía
	while (!colocada) {

		// Generamos las coordenadas
		randX = 1 + rand() % (ANCHO_TABLERO - 2);
		randY = 1 + rand() % (ALTO_TABLERO - 2);

		// Comprobamos que no haya pared ni serpiente
		if (estado.tablero[randY][randX] == ESPACIO_VACIO) {
			estado.fruta = { randX, randY };
			estado.tablero[estado.fruta.y][estado.fruta.x] = FRUTA;
			colocada = true;
		}
	}
}

void inicializarTablero(estadoJuego& estado) {
	// Rellenamos con espacios vacíos
	for (int i = 0; i < ALTO_TABLERO; i++) {
		for (int j = 0; j < ANCHO_TABLERO; j++) {
			estado.tablero[i][j] = ESPACIO_VACIO;
		}
	}

	// Colocamos paredes horizontales
	for (int i = 0; i < ANCHO_TABLERO; i++) {
		estado.tablero[0][i] = PARED_HORIZONTAL;
		estado.tablero[ALTO_TABLERO - 1][i] = PARED_HORIZONTAL;
	}

	// Colocamos paredes verticales
	for (int i = 0; i < ALTO_TABLERO; i++) {
		estado.tablero[i][0] = PARED_VERTICAL;
		estado.tablero[i][ANCHO_TABLERO - 1] = PARED_VERTICAL;
	}
}

void colocarSerpiente(estadoJuego& estado) {
	// Posición central
	punto posicionInicio = { ANCHO_TABLERO / 2, ALTO_TABLERO / 2 };
	estado.serpiente.push_back(posicionInicio);
	estado.tablero[posicionInicio.y][posicionInicio.x] = CABEZA_SERPIENTE;
}

void configurar(estadoJuego& estado) {
	srand(time(NULL));

	// Inicializamos el estado del juego
	estado.finDelJuego = false;
	estado.puntuacion = 0;
	estado.direccionActual = PARAR;
	estado.proximaDireccion = PARAR;
	estado.serpiente.clear();

	// Inicializamos componentes
	inicializarTablero(estado);
	colocarSerpiente(estado);
	colocarFruta(estado);
}

void entrada(estadoJuego& estado) {
	// Actualizamos la dirección solo si se pulsa una tecla y es un cambio de 90 grados
	if (IsWPressed()) {
		if (estado.direccionActual != ABAJO) estado.proximaDireccion = ARRIBA;
	}
	else if (IsSPressed()) {
		if (estado.direccionActual != ARRIBA) estado.proximaDireccion = ABAJO;
	}
	else if (IsAPressed()) {
		if (estado.direccionActual != DERECHA) estado.proximaDireccion = IZQUIERDA;
	}
	else if (IsDPressed()) {
		if (estado.direccionActual != IZQUIERDA) estado.proximaDireccion = DERECHA;
	}
}

void comprobarColision(estadoJuego& estado) {
	punto cabeza = estado.serpiente.front();

	// Colisión con los límites
	if (cabeza.x <= 0 || cabeza.x >= ANCHO_TABLERO - 1 || cabeza.y <= 0 || cabeza.y >= ALTO_TABLERO - 1) {
		estado.finDelJuego = true;
		return;
	}

	// Colisión consigo misma
	for (size_t i = 1; i < estado.serpiente.size(); i++) {
		if (cabeza.x == estado.serpiente[i].x && cabeza.y == estado.serpiente[i].y) {
			estado.finDelJuego = true;
			return;
		}
	}
}

void actualizarPuntuacion(estadoJuego& estado) {
	// Ganamos 1 punto por la cantidad de elementos que tenga la cola
	if (estado.serpiente.size() > 1) {
		// La cola es el tamaño total - 1 (cabeza)
		estado.puntuacion += (estado.serpiente.size() - 1) * PUNTOS_POR_SEGMENTO_COLA;
	}
}

void moverSerpiente(estadoJuego& estado) {
	punto cabezaAntigua = estado.serpiente.front();
	punto cabezaNueva = cabezaAntigua;

	// Calculamos la nueva posición de la cabeza según la dirección
	switch (estado.direccionActual) {
		case ARRIBA:
			cabezaNueva.y--;
			break;
		case ABAJO:
			cabezaNueva.y++;
			break;
		case IZQUIERDA:
			cabezaNueva.x--;
			break;
		case DERECHA:
			cabezaNueva.x++;
			break;
		default: 
			break;
	}

	// Insertamos la nueva cabeza
	estado.serpiente.insert(estado.serpiente.begin(), cabezaNueva);

	// Marcamos la posición antigua de la cabeza como cuerpo (x)
	estado.tablero[cabezaAntigua.y][cabezaAntigua.x] = CUERPO_SERPIENTE;

	// Comprobamos si come fruta
	bool comioFruta = (cabezaNueva.x == estado.fruta.x && cabezaNueva.y == estado.fruta.y);

	if (comioFruta) {
		estado.puntuacion += PUNTOS_FRUTA; // +15 puntos por fruta
		colocarFruta(estado); // Generamos una fruta nueva
	}
	else {
		punto colaAntigua = estado.serpiente.back();
		estado.tablero[colaAntigua.y][colaAntigua.x] = ESPACIO_VACIO;
		estado.serpiente.pop_back(); // Eliminamos el último segmento
	}

	// Dibujamos la nueva cabeza (X). Comprobación de seguridad para evitar escribir sobre una pared o un cuerpo.
	if (cabezaNueva.y >= 0 && cabezaNueva.y < ALTO_TABLERO && cabezaNueva.x >= 0 && cabezaNueva.x < ANCHO_TABLERO) {
		estado.tablero[cabezaNueva.y][cabezaNueva.x] = CABEZA_SERPIENTE;
	}
}

void logica(estadoJuego& estado) {
	// Iniciamos movimiento si estaba en PARAR
	if (estado.direccionActual == PARAR && estado.proximaDireccion != PARAR) {
		estado.direccionActual = estado.proximaDireccion;
	}
	else if (estado.direccionActual != PARAR) {
		// Aplicamos el cambio de dirección (solo si no es PARAR)
		estado.direccionActual = estado.proximaDireccion;

		// Movemos, comprobamos colision y actualizamos puntos
		moverSerpiente(estado);
		comprobarColision(estado);
		actualizarPuntuacion(estado);
	}
}

void dibujar(const estadoJuego& estado) {
	limpiarPantalla();

	// Mostramos la puntuación
	std::cout << "Puntuacion: " << estado.puntuacion << std::endl;

	// Dibujamos el tablero
	for (int i = 0; i < ALTO_TABLERO; i++) {
		for (int j = 0; j < ANCHO_TABLERO; j++)
		{
			std::cout << estado.tablero[i][j];
		}
		std::cout << std::endl;
	}

	if (estado.finDelJuego) {
		std::cout << "\nFIN DEL JUEGO! Puntuacion final: " << estado.puntuacion << std::endl;
	}
}
