#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

// Definici�n de pines (simulaci�n)
#define MOTOR_PIN  5
#define LAMP_PIN   18
#define BUZZER_PIN 19
#define SENSOR_PIN 34

// M�quina de estados
enum State {
    IDLE,
    RUNNING,
    ALARM
};

State currentState = IDLE;

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
    int sensorValue = rand() % 1500; // Simulaci�n de lectura del sensor
    if (sensorValue > 1000) {
        currentState = ALARM;
    } else if (sensorValue > 500) {
        currentState = RUNNING;
    } else {
        currentState = IDLE;
    }
}

void handleState() {
    switch (currentState) {
        case IDLE:
            cout << "Estado: IDLE - Motor OFF, L�mpara OFF, Buzzer OFF" << endl;
            break;
        case RUNNING:
            cout << "Estado: RUNNING - Motor ON, L�mpara ON, Buzzer OFF" << endl;
            break;
        case ALARM:
            cout << "Estado: ALARM - Motor OFF, L�mpara ON, Buzzer ON" << endl;
            break;
    }
}

int main() {
    setup();
    loop();
    return 0;
}
