// Incluir a biblioteca DHT da ADAFRUIT
#include <DHT.h>

// Definir PIN e tipo sensor
#define DHTPIN 4       // Pino DATA
#define DHTTYPE DHT11  // Tipo de sensor

//Definir PIN LEDS
const int LED_GREEN = 9;// Led Verde
const int LED_RED = 10; // Led Vermelho

// Inicializar o sensor
DHT dht(DHTPIN, DHTTYPE);

// Função para calcular o índice de calor aka (Sensação Térmica)
float computeHeatIndex(float temperature, float humidity) {
  // Converter a temperatura para Fahrenheit
  float T = (temperature * 1.8) + 32;
  float H = humidity;

  // Fórmula simplificada para Heat Index (Valores são constantes obtidas pela NOAA)
  float HI = -42.379 + 2.04901523 * T + 10.14333127 * H - 0.22475541 * T * H 
             - 0.00683783 * T * T - 0.05481717 * H * H + 0.00122874 * T * T * H
             + 0.00085282 * T * H * H - 0.00000199 * T * T * H * H;

  // Converter de volta para Celsius
  return (HI - 32) / 1.8;
}

void setup() {
  // Iniciar a comunicação em série
  Serial.begin(9600);
  Serial.println("DHT11 Sensor Test");

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

  // Inicializar a leitura do sensor
  dht.begin();
}

void loop() {
  // Esperar 2 segundos entre leituras
  delay(2000);

  // Ler temperatura e humidade
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Default in Celsius

  // Verifica que as leituras são válidas
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Apresentar Resultados
  Serial.print("Humidade: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" ºC");
  Serial.print(" %\t");

  // Calcular índice de calor (Heat Index)
  float heatIndex = computeHeatIndex(temperature, humidity);
  Serial.print("Indíce de Calor: ");
  Serial.print(heatIndex);

  // Controlar LEDs com base na temperatura 
  if (heatIndex < 24.90) {
    // Se a temperatura for maior que 30 ºC, ligar LED vermelho
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  } else {
    // Caso contrário, ligar LED verde
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }
}
