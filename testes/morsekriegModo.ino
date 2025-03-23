#include <Wire.h>                  // Biblioteca para comunicação I2C
#include <Adafruit_LiquidCrystal.h> // Biblioteca para controle do display LCD

// Definição do LCD (Endereço I2C 0x20, 16 colunas, 2 linhas)
Adafruit_LiquidCrystal lcd_1(0);

// Definição dos pinos
const int botaoPin = 2;    // Pino do botão
const int ledPin = 5;      // Pino do LED
const int buzzerPin = 10;  // Pino do buzzer

// Variáveis de controle
unsigned long tempoPressionado = 0;
unsigned long tempoUltimaPressao = 0;
bool botaoPressionado = false;
String mensagemMorse = "";
String palavra = "";

// Tempo limite para separação de letras e palavras
const int tempoLetra = 700;
const int tempoPalavra = 1400;

// Função para converter código Morse para letras
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

// Função para converter letra em código Morse
String charToMorse(char c) {
  switch (toupper(c)) {
    case 'A': return ".-";
    case 'B': return "-...";
    case 'C': return "-.-.";
    case 'D': return "-..";
    case 'E': return ".";
    case 'F': return "..-.";
    case 'G': return "--.";
    case 'H': return "....";
    case 'I': return "..";
    case 'J': return ".---";
    case 'K': return "-.-";
    case 'L': return ".-..";
    case 'M': return "--";
    case 'N': return "-.";
    case 'O': return "---";
    case 'P': return ".--.";
    case 'Q': return "--.-";
    case 'R': return ".-.";
    case 'S': return "...";
    case 'T': return "-";
    case 'U': return "..-";
    case 'V': return "...-";
    case 'W': return ".--";
    case 'X': return "-..-";
    case 'Y': return "-.--";
    case 'Z': return "--..";
    case '0': return "-----";
    case '1': return ".----";
    case '2': return "..---";
    case '3': return "...--";
    case '4': return "....-";
    case '5': return ".....";
    case '6': return "-....";
    case '7': return "--...";
    case '8': return "---..";
    case '9': return "----.";
    case ' ': return " ";
    default: return "";
  }
}

void setup() {
  pinMode(botaoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  lcd_1.setBacklight(1);
  lcd_1.print(">>>MorseKrieg<<<");
  delay(1000);
}

// Variável para armazenar o código Morse acumulado
String morseAcumulado = "";

void loop() {
  int leituraBotao = digitalRead(botaoPin);  // Lê o estado do botão

  // MODO 1: Envio via botão para código Morse
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
  if (millis() - tempoUltimaPressao > tempoLetra && mensagemMorse.length() > 0) {
    // Convertendo a mensagem morse para letra
    String letra = morseToChar(mensagemMorse);
    if (letra != "") {
      // Adiciona a letra à palavra formada
      palavra += letra;

      // Exibe a palavra completa no LCD
      lcd_1.setCursor(0, 0);
      lcd_1.print("transmissao:    ");

      // Exibe a palavra acumulada
      lcd_1.setCursor(0, 1);
      lcd_1.print(palavra);

      // Se a palavra tiver mais de 16 caracteres, inicia o deslocamento
      if (palavra.length() > 16) {
        lcd_1.scrollDisplayLeft();  // Move o texto para a esquerda
      }

      // Exibe a letra na Serial
      Serial.print("Letra: ");
      Serial.println(letra);
    }
    
    // Resetando a mensagem para a próxima letra
    mensagemMorse = "";
    tempoUltimaPressao = millis();  // Atualiza o tempo da última pressão
  }

  // Verifica a entrada do modo serial para envio de letras manualmente (Modo 2)
  if (Serial.available()) {
    char caractere = Serial.read();
    String codigoMorse = charToMorse(caractere);

    if (codigoMorse != "") {
      // Exibe a letra e seu código Morse no Serial
      Serial.print(caractere);
      Serial.print(" -> ");
      Serial.println(codigoMorse);  // Exibe a letra e o código Morse no formato Letra -> Código Morse

      // Acumula o código Morse com um espaço entre as letras
      if (morseAcumulado.length() > 0) {
        morseAcumulado += " ";  // Adiciona um espaço entre as letras
      }
      morseAcumulado += codigoMorse;

      // Exibe o código Morse acumulado no LCD
      lcd_1.setCursor(0, 0);
      lcd_1.print("telegrama:      ");
      lcd_1.setCursor(0, 1);
      lcd_1.print(morseAcumulado);

      // Transmite o código Morse (com som e LED)
      for (int i = 0; i < codigoMorse.length(); i++) {
        if (codigoMorse[i] == '.') {
          digitalWrite(ledPin, HIGH);
          tone(buzzerPin, 1000);
          delay(200);
        } else if (codigoMorse[i] == '-') {
          digitalWrite(ledPin, HIGH);
          tone(buzzerPin, 1000);
          delay(600);
        }
        digitalWrite(ledPin, LOW);
        noTone(buzzerPin);
        delay(200);
      }
      delay(tempoLetra);
    }
  }
}
