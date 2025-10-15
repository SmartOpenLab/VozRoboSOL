#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>

// ======================
// PARÁMETROS CONFIGURABLES
// ======================

// Pines
const int PIN_RX = 21;    //16          // RX del ESP32 (conectado a TX del DFPlayer)
const int PIN_TX = 22;    //17          // TX del ESP32 (conectado a RX del DFPlayer)
const int PIN_BUSY = 25;  // BUSY del DFPlayer para saber si se está reproduciendo una pista
                          //   antes de saltar a la siguiente pista

const int PIN_POT = 34;  // Pin analógico para potenciómetro (ADC1)

const int ADC_MIN = 0;  // Rango analógico ESP32 (ADC 12 bits: 0–4095)
const int ADC_MAX = 4095;
const int POT_TOLERANCIA = 5 * ADC_MAX / 100;  // Tolerancia de construcción para considerar potenciómetro en mínimo o máximo

// Tiempos minimo y máximo de reproducción entre pistas (en milisegundos)
const unsigned long TIEMPO_MIN = 30 * 1000;      // 30 segundos
const unsigned long TIEMPO_MAX = 5 * 60 * 1000;  // 5 minutos

// ======================
// VARIABLES DE CONTROL
// ======================
HardwareSerial mySerial(1);
DFRobotDFPlayerMini myDFPlayer;

int totalPistas = 0;
int ultimaPista = -1;
unsigned long ultimoTiempo = 0;
unsigned long intervalo = 0;

// ======================
// FUNCIONES AUXILIARES
// ======================
int elegirPistaAleatoria() {
  if (totalPistas <= 1) return 1;  // Evitar errores con una sola pista de audio en la SD

  int nueva;
  do {
    nueva = random(1, totalPistas + 1);  // DFPlayer indexa Pistas desde 1
  } while (nueva == ultimaPista);        //Evita que se elija la pista de audio que se acaba de reproducir

  ultimaPista = nueva;
  return nueva;
}

unsigned long calcularIntervalo() {
  int valorPot = analogRead(PIN_POT);

  // Potenciómetro al mínimo (no reproducir nunca)
  if (valorPot <= (ADC_MIN + POT_TOLERANCIA)) {
    return 0;
  }

  // Potenciómetro al máximo (reproducir cada TIEMPO_MIN)
  if (valorPot >= (ADC_MAX - POT_TOLERANCIA)) {
    return TIEMPO_MIN;
  }

  // Escalar linealmente entre TIEMPO_MIN y TIEMPO_MAX (se podría usar la función map)
  float escala = (float)(valorPot - ADC_MIN) / (ADC_MAX - ADC_MIN);
  return (unsigned long)(TIEMPO_MAX - escala * (TIEMPO_MAX - TIEMPO_MIN));
}

// ======================
// PROGRAMA PRINCIPAL
// ======================
void setup() {
  Serial.begin(115200);                              //Para mensaje en la consola
  mySerial.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX);  //Para el DFPlayer
  Serial.println("Inicializando modulo de voz robot...");

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Error inicializando DFPlayer Mini");
    while (true)
      ;  //para el ESP32 para que no siga adelante
  }
  Serial.println("DFPlayer Mini inicializado.");
 
  totalPistas = myDFPlayer.readFileCounts();            //para luego usarlo como tope al calcular un peud-aleatorio
  Serial.printf("Total de Pistas: %d\n", totalPistas);  //para comprobar que no esté vacía la SD

  randomSeed(analogRead(PIN_POT));  // inicialización pseudoaleatoria
  myDFPlayer.play(1);
}


void loop() {
  intervalo = calcularIntervalo();

  if (intervalo > 0) {
    unsigned long ahora = millis();
    if (ahora - ultimoTiempo >= intervalo) {
      int pista = elegirPistaAleatoria();
      myDFPlayer.play(pista);  //se podría usar myDFPlayer.playFolder(folderNumber, trackNumber) y tener folders para comportamientos (e.g., sensor de proximidad...)
      Serial.printf("Reproduciendo canción %d\n", pista);

      ultimoTiempo = ahora;
    }
  }
}
