#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

// Definición de estados de la maquina de estado
enum State {
    INICIO,
    ESPERANDO,
    ACTIVADO,
    ERROR,
    ALARMA
};

State currentState = INICIO;
State previousState = INICIO;

void updateState();
void handleState();

void setup() {
    cout << "Inicializando sistema..." << endl;
}

void loop() {
    while (true) {
        updateState();
        handleState();
        this_thread::sleep_for(chrono::milliseconds(50)); // Timer cada 50ms
    }
}

void updateState() {
    int sensorValue = rand() % 1500; // Simulación de lectura del sensor
    previousState = currentState;

    if (sensorValue < 300) {
        currentState = INICIO;
    } else if (sensorValue < 600) {
        currentState = ESPERANDO;
    } else if (sensorValue < 1000) {
        currentState = ACTIVADO;
    } else if (sensorValue < 1300) {
        currentState = ERROR;
    } else {
        currentState = ALARMA;
    }
}

void handleState() {
    cout << "Estado actual: ";
    switch (currentState) {
        case INICIO:
            cout << "INICIO - Sistema en espera" << endl;
            break;
        case ESPERANDO:
            cout << "ESPERANDO - Preparado para activarse" << endl;
            break;
        case ACTIVADO:
            cout << "ACTIVADO - Operación en curso" << endl;
            break;
        case ERROR:
            cout << "ERROR - Se detectó un problema" << endl;
            break;
        case ALARMA:
            cout << "ALARMA - Estado crítico" << endl;
            break;
    }
}

int main() {
    setup();
    loop();
    return 0;
}

