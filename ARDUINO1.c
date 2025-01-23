#include <DHT.h>
// Include SoftwareSerial for Bluetooth communication
#include <SoftwareSerial.h>

// Definir PIN e tipo sensor
#define DHTPIN 4       // Pino DATA
#define DHTTYPE DHT11  // Tipo de sensor

//Definir PIN LEDS
const int LED_GREEN = 10; // Led Verde
const int LED_RED = 9;    // Led Vermelho

// Bluetooth module pins (RX and TX)
#define BT_RX 2
#define BT_TX 3

// Inicializar o sensor
DHT dht(DHTPIN, DHTTYPE);

// Variáveis globais para armazenar valores
float heatIndex = 0;
float temperature = 0;
float humidity = 0;
int gasVal = 0;
int alcVal = 0;

SoftwareSerial bluetooth(BT_RX, BT_TX); // RX, TX

// Função para calcular o índice de calor
float computeHeatIndex(float temperature, float humidity) {
  float T = (temperature * 1.8) + 32; // Celsius para Fahrenheit
  float H = humidity;

  // Fórmula simplificada para índice de calor (NOAA)
  float HI = -42.379 + 2.04901523 * T + 10.14333127 * H - 0.22475541 * T * H
             - 0.00683783 * T * T - 0.05481717 * H * H + 0.00122874 * T * T * H
             + 0.00085282 * T * H * H - 0.00000199 * T * T * H * H;

  return (HI - 32) / 1.8; // Retorna em Celsius
}

void setup() {
  // Iniciar a comunicação em série
  Serial.begin(9600);
  bluetooth.begin(9600); // Inicializa comunicação Bluetooth
  Serial.println("DHT11 Sensor Test with Bluetooth");
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

  // Inicializar o sensor DHT
  dht.begin();
}

void loop() {
  // Ler temperatura e umidade
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  gasVal = analogRead(0);     // Leitura do sensor de gás no pino A0
  alcVal = analogRead(1);     // Leitura do sensor de álcool no pino A1
  heatIndex = computeHeatIndex(temperature, humidity);

  // Verifica se as leituras são válidas
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    bluetooth.println("Error reading DHT sensor");
    return;
  }

  // Controlar LEDs e enviar mensagem por Bluetooth
  if (heatIndex < 24.90 && gasVal < 180 && alcVal < 610) {
    // Condições seguras
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    Serial.println("All Values are OK");
    bluetooth.println("All Values are OK");
  } else {
    // Condição de perigo
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);

    // Enviar mensagens detalhadas
    if (heatIndex > 24.90) {
      Serial.println("Heat Index too high!");
      bluetooth.println("Heat Index too high!");
    }
    if (gasVal > 180) {
      Serial.println("Gas Value too high!");
      bluetooth.println("Gas Value too high!");
    }
    if (alcVal > 610) {
      Serial.println("Alcohol Value too high!");
      bluetooth.println("Alcohol Value too high!");
    }
  }

  // Enviar dados atuais ao dispositivo Bluetooth
  bluetooth.print("Temperature: ");
  bluetooth.print(temperature);
  bluetooth.println(" °C");

  bluetooth.print("Humidity: ");
  bluetooth.print(humidity);
  bluetooth.println(" %");

  bluetooth.print("Gas Value: ");
  bluetooth.println(gasVal);

  bluetooth.print("Alcohol Value: ");
  bluetooth.println(alcVal);

  bluetooth.print("Heat Index: ");
  bluetooth.println(heatIndex);

  // Esperar 2 segundos antes da próxima leitura
  delay(2000);
}