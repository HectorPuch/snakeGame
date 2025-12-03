#pragma once

// Dimensiones del tablero (20 columnas x 10 filas)
const int ANCHO_TABLERO = 20;
const int ALTO_TABLERO = 10;

// Caracteres del juego
const char PARED_VERTICAL = '|';
const char PARED_HORIZONTAL = '-';
const char CABEZA_SERPIENTE = 'X';
const char CUERPO_SERPIENTE = 'x';
const char FRUTA = 'O';
const char ESPACIO_VACIO = ' ';

// Velocidad del juego
const int TASA_REFRESCO_MS = 150;

// Puntuaciones
const int PUNTOS_FRUTA = 15;
const int PUNTOS_POR_SEGMENTO_COLA = 1;
