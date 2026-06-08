#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN_TMP36   A0
#define PIN_MQ2     A1
#define PIN_LDR     A2
#define PIN_LED_G   8
#define PIN_LED_R   9
#define PIN_BUZZER  10

const float TEMP_ATENCAO   = 42.0;
const float TEMP_CRITICO   = 55.0;
const int   FUMACA_ATENCAO = 300;
const int   FUMACA_CRITICO = 600;
const int   LDR_CLARAO     = 800;

int estadoAtual = 0;
unsigned long ultimaLeitura = 0;
unsigned long intervaloMs = 0; 

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Inicia o ecrã LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("OrbitGuard Edge");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  digitalWrite(PIN_LED_G, HIGH);
  delay(2000);
  lcd.clear();
}

void loop() {
  if (millis() - ultimaLeitura >= intervaloMs) {
    ultimaLeitura = millis();

    int leituraTmp = analogRead(PIN_TMP36);
    float tensao = leituraTmp * (5.0 / 1023.0);
    float temperatura = (tensao - 0.5) * 100.0;

    int fumaca = analogRead(PIN_MQ2);
    int luminosidade = analogRead(PIN_LDR);

    int score = 0;
    
    if (temperatura >= TEMP_CRITICO) score += 50;
    else if (temperatura >= TEMP_ATENCAO) score += 25;

    if (fumaca >= FUMACA_CRITICO) score += 40;
    else if (fumaca >= FUMACA_ATENCAO) score += 20;

    if (luminosidade >= LDR_CLARAO) score += 15;

    if (score >= 70) estadoAtual = 2;
    else if (score >= 35) estadoAtual = 1;
    else estadoAtual = 0;

    // Atualiza o Ecrã LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:"); lcd.print((int)temperatura); lcd.print("C ");
    lcd.print("F:"); lcd.print(fumaca);
    lcd.setCursor(0, 1);

    // Controle dos Alarmes
    if (estadoAtual == 0) {
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_R, LOW);
      noTone(PIN_BUZZER);
      intervaloMs = 5000;
      lcd.print("> NORMAL");
    } 
    else if (estadoAtual == 1) {
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_R, HIGH);
      tone(PIN_BUZZER, 1500, 200);
      intervaloMs = 2000;
      lcd.print("> ATENCAO");
    } 
    else {
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_R, HIGH);
      tone(PIN_BUZZER, 2000, 600); 
      intervaloMs = 2000;
      lcd.print("> CRITICO !!");
    }

    // Envia o JSON para o Monitor Série
    Serial.print("{\"T\":"); Serial.print(temperatura);
    Serial.print(", \"F\":"); Serial.print(fumaca);
    Serial.print(", \"L\":"); Serial.print(luminosidade);
    Serial.print(", \"Status\":");
    
    if (estadoAtual == 0) Serial.println("\"NORMAL\"}");
    else if (estadoAtual == 1) Serial.println("\"ATENCAO\"}");
    else Serial.println("\"CRITICO\"}");
  }
}