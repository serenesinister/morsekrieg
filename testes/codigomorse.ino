#include <LiquidCrystal_I2C.h>

// Definição dos pinos para o LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 0x27 é o endereço comum para LCDs com I2C, 16 colunas e 2 linhas

const int botaoPin = 2;    // Pino do botão
const int ledPin = 5;      // Pino do LED
const int buzzerPin = 10;  // Pino do buzzer

unsigned long tempoPressionado = 0;   // Armazena o tempo em que o botão foi pressionado
unsigned long tempoUltimaPressao = 0; // Armazena o tempo da última pressão
bool botaoPressionado = false; // Variável para verificar se o botão foi pressionado
String mensagemMorse = "";    // Variável para armazenar a mensagem em código Morse

// Dicionário para converter código Morse para letras
String morseToChar(String morseCode) {
  if (morseCode == ".-") return "A";
  if (morseCode == "-...") return "B";
  if (morseCode == "-.-.") return "C";
  if (morseCode == "-..") return "D";
  if (morseCode == ".") return "E";
  if (morseCode == "..-.") return "F";
  if (morseCode == "--.") return "G";
  if (morseCode == "....") return "H";
  if (morseCode == "..") return "I";
  if (morseCode == ".---") return "J";
  if (morseCode == "-.-") return "K";
  if (morseCode == ".-..") return "L";
  if (morseCode == "--") return "M";
  if (morseCode == "-.") return "N";
  if (morseCode == "---") return "O";
  if (morseCode == ".--.") return "P";
  if (morseCode == "--.-") return "Q";
  if (morseCode == ".-.") return "R";
  if (morseCode == "...") return "S";
  if (morseCode == "-") return "T";
  if (morseCode == "..-") return "U";
  if (morseCode == "...-") return "V";
  if (morseCode == ".--") return "W";
  if (morseCode == "-..-") return "X";
  if (morseCode == "-.--") return "Y";
  if (morseCode == "--..") return "Z";
  if (morseCode == "-----") return "0";
  if (morseCode == ".----") return "1";
  if (morseCode == "..---") return "2";
  if (morseCode == "...--") return "3";
  if (morseCode == "....-") return "4";
  if (morseCode == ".....") return "5";
  if (morseCode == "-....") return "6";
  if (morseCode == "--...") return "7";
  if (morseCode == "---..") return "8";
  if (morseCode == "----.") return "9";
  return "";
}

void setup() {
  pinMode(botaoPin, INPUT);    // Configura o pino do botão como entrada
  pinMode(ledPin, OUTPUT);     // Configura o pino do LED como saída
  pinMode(buzzerPin, OUTPUT);  // Configura o pino do buzzer como saída
  Serial.begin(9600);          // Inicia a comunicação serial
  lcd.begin(16, 2);            // Inicializa o LCD de 16x2
  lcd.print("Digite em Morse"); // Exibe uma mensagem inicial
}

void loop() {
  int leituraBotao = digitalRead(botaoPin);  // Lê o estado do botão

  if (leituraBotao == HIGH) {  // Se o botão está pressionado
    if (!botaoPressionado) {  // Se não estava pressionado anteriormente
      tempoPressionado = millis();  // Marca o tempo de início de pressionamento
      botaoPressionado = true;
    }
    // O LED fica aceso enquanto o botão está pressionado
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);  // Emite som enquanto o botão está pressionado
  } else {  // Quando o botão é solto
    if (botaoPressionado) {  // Se o botão estava pressionado anteriormente
      unsigned long duracao = millis() - tempoPressionado;  // Calcula o tempo de pressionamento
      botaoPressionado = false;
      digitalWrite(ledPin, LOW);  // Apaga o LED
      noTone(buzzerPin);  // Para o som

      // Determina se foi um ponto ou um traço
      if (duracao < 50) {
        mensagemMorse += ".";  // Ponto (curto)
      } else {
        mensagemMorse += "-";  // Traço (longo)
      }

      // Aguarda um tempo curto antes de poder pressionar novamente
      delay(100);  // Atraso entre os cliques para evitar múltiplos registros em uma única pressão
      tempoUltimaPressao = millis();  // Atualiza o tempo da última pressão
    }
  }

  // Se não houve pressionamento por mais de 300ms, insira um espaço (separação de caracteres ou palavras)
  if (millis() - tempoUltimaPressao > 300 && mensagemMorse.length() > 0) {
    // Convertendo a mensagem morse para letra
    String letra = morseToChar(mensagemMorse);
    if (letra != "") {
      lcd.clear();
      lcd.print("Letra: " + letra);  // Exibe a letra no LCD
      Serial.println(letra);         // Exibe a letra na Serial
    }
    
    // Resetando a mensagem para a próxima letra
    mensagemMorse = "";
    tempoUltimaPressao = millis();  // Atualiza o tempo da última pressão
  }
}
