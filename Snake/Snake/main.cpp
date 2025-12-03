#include <iostream>
#include <thread>
#include <chrono>

#include "game.h"
#include "constants.h"

// Función principal del programa
int main() {

    // Declaramos la estructuras que contiene todo el estado del juego
    estadoJuego juegoSerpiente;

    // Configuramos e inicializamos el juego
    configurar(juegoSerpiente);

    // Bucle principal del juego. Se ejecuta mientras finDelJuego sea false
    while (!juegoSerpiente.finDelJuego) {

        // Limpiamos la pantalla y mostramos el tablero y la puntuación
        dibujar(juegoSerpiente);

        // Capturamos la pulsación del usuario (W, A, S, D)
        entrada(juegoSerpiente);

        // Aplicamos el movimiento, comprobamos colisiones y actualizamos la puntuación
        logica(juegoSerpiente);

        // Controlamos la velocidad de ejecución
        std::this_thread::sleep_for(std::chrono::milliseconds(TASA_REFRESCO_MS));
    }

    // Mostramos la pantalla final de "fin del juego" por última vez
    dibujar(juegoSerpiente);

    // Mantenemos la consola abierta al finalizar
    std::cout << "Pulsa ENTER para salir..." << std::endl;
    std::cin.get();
}
