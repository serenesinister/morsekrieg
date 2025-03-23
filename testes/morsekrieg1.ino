#include <Wire.h>                  // Biblioteca para comunicação I2C
#include <Adafruit_LiquidCrystal.h> // Biblioteca para controle do display LCD

// Definição dos pinos para o LCD
Adafruit_LiquidCrystal lcd_1(0);  // Endereço I2C 0x20, 16 colunas e 2 linhas

const int botaoPin = 2;    // Pino do botão
const int ledPin = 5;      // Pino do LED
const int buzzerPin = 10;  // Pino do buzzer

unsigned long tempoPressionado = 0;   // Armazena o tempo em que o botão foi pressionado
unsigned long tempoUltimaPressao = 0; // Armazena o tempo da última pressão
bool botaoPressionado = false; // Variável para verificar se o botão foi pressionado
String mensagemMorse = "";    // Variável para armazenar a mensagem em código Morse
String palavra = "";          // Variável para acumular a palavra formada

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

int deslocamento = 0;  // Conta quantos deslocamentos foram feitos

void setup() {
  pinMode(botaoPin, INPUT);    // Configura o pino do botão como entrada
  pinMode(ledPin, OUTPUT);     // Configura o pino do LED como saída
  pinMode(buzzerPin, OUTPUT);  // Configura o pino do buzzer como saída
  Serial.begin(9600);          // Inicia a comunicação serial
  lcd_1.begin(16, 2);          // Inicializa o LCD de 16x2
  lcd_1.setBacklight(1);       // Liga o backlight do LCD
  lcd_1.print(">>>MorseKrieg<<<"); // Exibe uma mensagem inicial
  delay(1000); // Aguarda 1 segundo para exibição
}

const int tempoEspaco = 700; // Tempo limite para identificar a separação de palavras em milissegundos

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

      // Exibe o código Morse no monitor serial
      Serial.print("Morse: ");
      Serial.println(mensagemMorse);  // Exibe a sequência em Morse no terminal

      // Aguarda um tempo curto antes de poder pressionar novamente
      delay(100);  // Atraso entre os cliques para evitar múltiplos registros em uma única pressão
      tempoUltimaPressao = millis();  // Atualiza o tempo da última pressão
    }
  }

  // Verificar se o tempo sem pressionamento excedeu o limite para separar palavras
  if (millis() - tempoUltimaPressao > tempoEspaco && mensagemMorse.length() > 0) {
    // Convertendo a mensagem morse para letra
    String letra = morseToChar(mensagemMorse);
    if (letra != "") {
      // Adiciona a letra à palavra formada
      palavra += letra;

      // Exibe a palavra completa no LCD
      lcd_1.setCursor(0, 0);
      lcd_1.print("telegrama:      ");

      // Exibe a palavra acumulada
      lcd_1.setCursor(0, 1);
      lcd_1.print(palavra);

      // Se a palavra tiver mais de 16 caracteres, inicia o deslocamento
      if (palavra.length() > 16) {
        lcd_1.scrollDisplayLeft();  // Move o texto para a esquerda
        deslocamento++;             // Conta os deslocamentos

        // Pequeno atraso para que a rolagem seja visível
        delay(200);
      }

      // Exibe a letra na Serial
      Serial.print("Letra: ");
      Serial.println(letra);
    }
    
    // Resetando a mensagem para a próxima letra
    mensagemMorse = "";
    tempoUltimaPressao = millis();  // Atualiza o tempo da última pressão
  }

  // Se o tempo entre as pressões for maior que 'tempoEspaco', insira um espaço entre as palavras
  if (millis() - tempoUltimaPressao > tempoEspaco && palavra.length() > 0 && palavra.charAt(palavra.length() - 1) != ' ') {
    // Adiciona um espaço apenas se o último caractere não for um espaço
    palavra += " ";

    // Exibe na serial que a palavra foi separada
    Serial.println("intervalo");

    tempoUltimaPressao = millis();  // Atualiza o tempo da última pressão
  }
}
