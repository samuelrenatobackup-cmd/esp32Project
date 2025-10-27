#include <Arduino.h>
#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int led = 26;
const int btnMais = 27;
const int btnMenos = 25;

unsigned long tempoAtual = 0;
unsigned long tempoAnterior = 0;
unsigned long intervalo = 500;

int brilhoPorcentagem = 0;
int brilhoPWM = 0;
bool piscar = false;
bool estadoLed = false;

Bounce debouncerMais = Bounce();
Bounce debouncerMenos = Bounce();

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int canalPWM = 0;
const int freqPWM = 5000;
const int resolucaoPWM = 8;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);

 
  ledcSetup(canalPWM, freqPWM, resolucaoPWM);
  ledcAttachPin(led, canalPWM);

  debouncerMais.attach(btnMais, INPUT_PULLUP);
  debouncerMais.interval(50);

  debouncerMenos.attach(btnMenos, INPUT_PULLUP);
  debouncerMenos.interval(50);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void piscarLed() {
  if (tempoAtual - tempoAnterior >= intervalo) {
    estadoLed = !estadoLed;
    digitalWrite(led, estadoLed);
    tempoAnterior = tempoAtual;
  }
}

void loop() {
  tempoAtual = millis();
  debouncerMais.update();
  debouncerMenos.update();


  if (debouncerMais.fell()) {
    brilhoPorcentagem += 10;
    if (brilhoPorcentagem > 100) brilhoPorcentagem = 100;
    piscar = false;
  }


  if (debouncerMenos.fell()) {
    brilhoPorcentagem -= 10;
    if (brilhoPorcentagem < 0) brilhoPorcentagem = 0;
    piscar = false;
  }

  
  if (debouncerMais.read() == LOW && debouncerMais.currentDuration() >= 2000) {
    piscar = true;
  }

  if (piscar) {
      ledcDetachPin(led);
    pinMode(led, OUTPUT);
    piscarLed();
  } 
  else {
    ledcAttachPin(led, canalPWM);
    brilhoPWM = map(brilhoPorcentagem, 0, 100, 0, 255);
    ledcWrite(canalPWM, brilhoPWM);
  }

 
  lcd.setCursor(0, 0);
  lcd.print("Brilho: ");
  lcd.print(brilhoPorcentagem);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  if (piscar) {
    lcd.print("Pisca: Ativado   ");
  } else {
    lcd.print("Pisca: Desligado ");
  }

  delay(50);
}

// COMENTÁRIOS - ANTIGAS FUNÇÔES:

/*if (estadoAtualBtn == 1 && estadoAnteriorBtn == 0) {
  contador++;
  estadoLed = !estadoLed;
  digitalWrite(ledEsp, estadoLed);
  Serial.println(contador);
}

estadoAnteriorBtn = estadoAtualBtn;*/

/*
void piscarLed(int led){
 if (tempoAtual - tempoAnterior >= intervalo) {
   estadoLed = !estadoLed;
   digitalWrite(led, estadoLed);
   tempoAnterior = tempoAtual;
 }
}
if(debouncer.fell()){
   contador++;
   Serial.println(contador);
   estadoLed = !estadoLed;
   digitalWrite(led, estadoLed);
   piscar = false;
 }
 if (debouncer.read() == LOW && debouncer.currentDuration() >= 2000){
   piscar = true;
 }

 if(piscar){
   piscarLed(led);
 }
 */

/*sem a biblioteca Debouce
 bool leituraAtual = digitalRead(btn);  // lê o botão (pode ter ruído)

  // Se mudou o estado, reinicia o temporizador
  if (leituraAtual != ultimoEstadoLeitura) {
    ultimoTempo = millis();
  }

  // Só aceita a mudança se passou o tempo de debounce
  if ((millis() - ultimoTempo) > delayDebounce) {
    // Se o estado realmente mudou
    if (leituraAtual != estadoBotao) {
      estadoBotao = leituraAtual;

      // Se o botão foi pressionado (nível LOW)
      if (estadoBotao == LOW) {
        estadoLed = !estadoLed;           // alterna o LED
        digitalWrite(led, estadoLed);
        Serial.println("Botão pressionado!");
      }
    }
  }
*/
/*switch (contador) {
    case 1: intervalo = 2000; break;
    case 2: intervalo = 1000;  break;
    case 3: intervalo = 500;  break;
    case 4: intervalo = 50;  break;
    default:
        contador = 1;
        intervalo = 2000;
        break;
}

  if (tempoAtual - tempoAnterior >= intervalo) {
    estadoLed = !estadoLed;
    digitalWrite(ledEsp, estadoLed);
    tempoAnterior = tempoAtual;
  }
*/
