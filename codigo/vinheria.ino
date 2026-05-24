#include <LiquidCrystal.h>


// CONFIGURAÇÃO DO LCD
// RS, E, D4, D5, D6, D7
// =====================================================
LiquidCrystal lcd(9, 7, 5, 4, 3, 2);


// DESENHO PERSONALIZADO DO LCD
// Cria um ícone simples semelhante a uma taça/vinho
// =====================================================
byte logoVinho[8] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B00100,
  B00100,
  B01110,
  B00000
};


// DEFINIÇÃO DOS PINOS
// =====================================================

// Sensor LDR
int pinoLDR = A0;

// LEDs
int ledVerde = 13;
int ledAmarelo = 12;
int ledVermelho = 11;

// Buzzer
int buzzer = 10;


// VARIÁVEIS
// =====================================================

int valorSensor = 0;
int luminosidade = 0;


// FUNÇÃO SETUP
// Executa apenas uma vez
// =====================================================
void setup() {

  // Inicializa comunicação serial
  Serial.begin(9600);

  // Inicializa LCD
  lcd.begin(16, 2);

  // Cria caractere personalizado
  lcd.createChar(0, logoVinho);

  
  // TELA INICIAL
  // =====================================================

  lcd.setCursor(0, 0);
  lcd.write(byte(0));

  lcd.setCursor(2, 0);
  lcd.print("VINHERIA");

  lcd.setCursor(2, 1);
  lcd.print("AGNELLO");

  delay(4000);

  lcd.clear();

  
  // MENSAGEM DE BOAS VINDAS
  // =====================================================

  lcd.setCursor(0, 0);
  lcd.print("Sistema de");

  lcd.setCursor(0, 1);
  lcd.print("Monitoramento");

  delay(3000);

  lcd.clear();

  
  // CONFIGURAÇÃO DOS PINOS
  // =====================================================

  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  pinMode(buzzer, OUTPUT);
}


// LOOP PRINCIPAL
// Executa infinitamente
// =====================================================
void loop() {

  
  // LEITURA DO SENSOR LDR
  // =====================================================

  valorSensor = analogRead(pinoLDR);

  
  // CONVERSÃO ADC -> PORCENTAGEM
  // 0 a 1023 -> 0 a 100
  // =====================================================

  luminosidade = map(valorSensor, 0, 1023, 0, 100);

  // Garante limite entre 0 e 100
  luminosidade = constrain(luminosidade, 0, 100);

  
  // MONITOR SERIAL
  // =====================================================

  Serial.print("Luminosidade: ");
  Serial.print(luminosidade);
  Serial.println("%");

  
  // EXIBE NO LCD
  // =====================================================

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Luminosidade:");

  lcd.setCursor(0, 1);
  lcd.print(luminosidade);
  lcd.print("%");

  
  // AMBIENTE IDEAL
  // =====================================================

  if (luminosidade <= 20) {

    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);

    noTone(buzzer);

    lcd.setCursor(8, 1);
    lcd.print("OK");

  }

  
  // NÍVEL DE ALERTA
  // =====================================================

  else if (luminosidade <= 40) {

    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);

    // Liga buzzer por 3 segundos
    tone(buzzer, 1000);

    delay(3000);

    noTone(buzzer);

    lcd.setCursor(8, 1);
    lcd.print("ALERTA");

  }

  
  // PROBLEMA DE LUMINOSIDADE
  // =====================================================

  else {

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, HIGH);

  // Buzzer contínuo em situação crítica
  tone(buzzer, 2000);

  lcd.setCursor(8, 1);
  lcd.print("RISCO");

  
  }

  // Pequena pausa
  delay(1000);
}