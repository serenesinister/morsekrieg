#include <Wire.h> //biblioteca para comunicação I2C
#include <Adafruit_LiquidCrystal.h> //biblioteca para controle do display LCD

//definição do LCD (endereço I2C 0x20, 16 colunas, 2 linhas)
Adafruit_LiquidCrystal lcd_1(0);

//definição dos pinos
const int botaoPin = 2;   //pino do botão
const int ledPin = 5;     //pino do LED (transmissão - Modo 1)
const int ledPin2 = 6;    //pino do LED (telegrama - Modo 2)
const int buzzerPin = 10; //pino do buzzer

//variáveis de controle
unsigned long tempoPressionado = 0;
unsigned long tempoInatividade = 0; //tempo de inatividade
bool botaoPressionado = false;
String mensagemMorse = "";
String palavra = "";

//tempo limite para determinar a inatividade
const int limiteInatividade = 150; //tempo inatividade para separar as letras

//sequência Morse para o comando de reset
const String comandoResetMorse = ".-.-."; //sequência Morse para reset

//função para converter código Morse para letras
String morseChar(String morseCode) {
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

//função para converter letra em código Morse
String charMorse(char c) {
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
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  lcd_1.setBacklight(1);
  lcd_1.print(">>>MorseKrieg<<<");
}

//variável para armazenar o código Morse acumulado
String morseAcumulado = "";

//variável de controle para saber se o espaço foi inserido
bool espacoInserido = false;

//variável para armazenar o modo atual
int modoAtual = -1; //inicia com um valor que não corresponde a nenhum modo

void reset() {
  //limpa as variáveis e o display
  mensagemMorse = "";
  palavra = "";
  morseAcumulado = "";
  espacoInserido = false;
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("  >>>+Reset<<<  ");
  Serial.println("Fim da linha.");
  delay(1000);
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print(">>>MorseKrieg<<<");
}

void loop() {
  int leituraBotao = digitalRead(botaoPin); //lê o estado do botão

  //MODO I: Envio via botão para código Morse
  if (leituraBotao == HIGH) { // Se o botão está pressionado
    //verifica se o modo mudou e, se sim, limpa o display
    if (modoAtual != 1) { //se não está no modo 2
      lcd_1.clear(); //limpa o display
      lcd_1.setCursor(0, 0);
      modoAtual = 1; //atualiza o modo para o modo 2
    }
    
    if (!botaoPressionado) { //se não estava pressionado anteriormente
      tempoPressionado = millis(); //marca o tempo de início de pressionamento
      botaoPressionado = true;
      tempoInatividade = millis(); //reseta o tempo de inatividade ao pressionar o botão
    }
    //o LED fica aceso enquanto o botão está pressionado
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000); //emite som enquanto o botão está pressionado
  } else { //quando o botão é solto
    if (botaoPressionado) { //se o botão estava pressionado anteriormente
      unsigned long duracao = millis() - tempoPressionado; //calcula o tempo de pressionamento
      botaoPressionado = false;
      digitalWrite(ledPin, LOW); //apaga o LED
      noTone(buzzerPin); //para o som

      //determina se foi um ponto ou um traço
      if (duracao < 50) {
        mensagemMorse += "."; //ponto (curto)
      } else {
        mensagemMorse += "-"; //traço (longo)
      }

      //exibe o código Morse no monitor serial
      Serial.print("Morse: ");
      Serial.println(mensagemMorse); //exibe a sequência em Morse no terminal
    }
  }

  //verificar se o tempo de inatividade excedeu o limite para separar letras
  if (millis() - tempoInatividade > limiteInatividade && mensagemMorse.length() > 0) {
    //convertendo a mensagem morse para letra
    String letra = morseChar(mensagemMorse);
    if (letra != "") {
        //adiciona a letra à palavra formada
        palavra += letra;

        //exibe a palavra completa no LCD
        lcd_1.setCursor(0, 0);
        lcd_1.print("transmissao:");
        lcd_1.setCursor(0, 1);
        lcd_1.print(palavra);

        //se a palavra tiver mais de 16 caracteres, inicia o deslocamento
        if (palavra.length() > 16) {
            lcd_1.scrollDisplayLeft(); //move o texto para a esquerda
        }

        //exibe a letra na Serial
        Serial.print("Chave: ");
        Serial.println(letra);
    }

    //resetando a mensagem para a próxima letra
    mensagemMorse = "";
    tempoInatividade = millis(); //reseta o tempo de inatividade após a separação de letras
    espacoInserido = false; //reseta o controle de espaço
  }

  //verificar se o tempo de inatividade excede o dobro do limite (para separar palavras)
  if (millis() - tempoInatividade > 2 * limiteInatividade && palavra.length() > 0 && !espacoInserido) {
    //adiciona um espaço à palavra para separar palavras
    palavra += " ";

    //exibe a palavra com o espaço no LCD
    lcd_1.setCursor(0, 0);
    lcd_1.print("transmissao:");
    lcd_1.setCursor(0, 1);
    lcd_1.print(palavra);

    //exibe o espaço na Serial
    Serial.print("Recado: ");
    Serial.println(palavra);

    //marcando que o espaço já foi inserido
    espacoInserido = true;

    //resetando a variável de código Morse e o tempo de inatividade para a próxima palavra
    mensagemMorse = "";
    tempoInatividade = millis();  //reseta o tempo de inatividade após a separação de palavras
  }

  //verifica se o comando de reset foi inserido
  if (mensagemMorse == comandoResetMorse) {
    reset(); //chama a função de reset
    mensagemMorse = ""; //limpa a mensagem após o reset
  }

  //MODO II: Envio manual via Serial para código Morse
  if (Serial.available()) {
    char caractere = Serial.read();
    String codigoMorse = charMorse(caractere);    

    //verifica se o modo mudou e, se sim, limpa o display
    if (modoAtual != 2) { //se não está no modo 2
      lcd_1.clear(); //limpa o display
      lcd_1.setCursor(0, 0);
      modoAtual = 2; //atualiza o modo para o modo 2
    }

    if (codigoMorse != "") {
      //exibe a letra e seu código Morse no Serial
      Serial.print(caractere);
      Serial.print(" -> ");
      Serial.println(codigoMorse); //exibe a letra e o código Morse no formato Letra -> Código Morse

      //acumula o código Morse com um espaço entre as letras
      if (morseAcumulado.length() > 0) {
        morseAcumulado += " "; //adiciona um espaço entre as letras
      }
      morseAcumulado += codigoMorse;

      //exibe o código Morse acumulado no LCD
      lcd_1.setCursor(0, 0);
      lcd_1.print("telegrama:"); //limpa o título da linha 0
      lcd_1.setCursor(0, 1); //define a posição para a linha 1
      lcd_1.print(morseAcumulado);  //imprime o código Morse acumulado

      //se a palavra tiver mais de 16 caracteres, inicia o deslocamento
      if (morseAcumulado.length() > 16) {
        lcd_1.scrollDisplayLeft();  //move o texto para a esquerda
      }

      //transmite o código Morse (com som e LED)
      for (int i = 0; i < codigoMorse.length(); i++) {
        if (codigoMorse[i] == '.') {
          digitalWrite(ledPin2, HIGH);
          tone(buzzerPin, 1000);
          delay(50);
        } else if (codigoMorse[i] == '-') {
          digitalWrite(ledPin2, HIGH);
          tone(buzzerPin, 1000);
          delay(100);
        }
        digitalWrite(ledPin2, LOW);
        noTone(buzzerPin);
        delay(100);
      }
    }
  }
}
