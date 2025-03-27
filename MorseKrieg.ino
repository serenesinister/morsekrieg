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

//verificar o estado do botão e processar o Morse
void verificarBotao() {
    int leituraBotao = digitalRead(botaoPin); //lê o estado do botão (HIGH ou LOW)
    //MODO I (transmissão): entrada via botão
    if (leituraBotao == HIGH) {
        //se o modo atual não for o modo 1, muda para o modo 1
        if (modoAtual != 1) {
            lcd_1.clear(); //limpa a tela do LCD
            lcd_1.setCursor(0, 0); //posiciona o cursor no início da tela
            modoAtual = 1; //define o modo como 1
        }
        
        //se o botão ainda não foi pressionado, registra o tempo de pressionamento
        if (!botaoPressionado) {
            tempoPressionado = millis(); //registra o tempo de início da pressão do botão
            botaoPressionado = true; //marca que o botão foi pressionado
            tempoInatividade = millis(); //reseta o tempo de inatividade
        }

        digitalWrite(ledPin, HIGH); //acende o LED
        tone(buzzerPin, 1000); //emite um tom no buzzer
    } else {
        //se o botão não estiver pressionado (estado LOW)
        if (botaoPressionado) {
            unsigned long duracao = millis() - tempoPressionado; //calcula a duração do pressionamento do botão
            botaoPressionado = false; //marca que o botão foi solto
            digitalWrite(ledPin, LOW); //apaga o LED
            noTone(buzzerPin); //desliga o buzzer
            
            //se a duração for menor que 50ms, é um ponto (.), senão é um traço (-)
            mensagemMorse += (duracao < 50) ? "." : "-";
            Serial.print("Morse: "); //imprime a mensagem Morse no monitor serial
            Serial.println(mensagemMorse);
        }
    }
}

//processar e traduzir a mensagem Morse em letras
void processarLetra() {
    //se o tempo de inatividade for maior que o limite e houver mensagem Morse para processar
    if (millis() - tempoInatividade > limiteInatividade && mensagemMorse.length() > 0) {
        String letra = morseChar(mensagemMorse); //converte a mensagem Morse para a letra correspondente
        if (letra != "") { //se a letra for válida
            palavra += letra; //adiciona a letra à palavra atual
            lcd_1.setCursor(0, 0);
            lcd_1.print("transmissao:"); //exibe "transmissao:" no LCD
            lcd_1.setCursor(0, 1);  
            lcd_1.print(palavra); //exibe a palavra no LCD
            if (palavra.length() > 16) lcd_1.scrollDisplayLeft();  // Se a palavra for maior que 16 caracteres, faz a rolagem da tela
            Serial.print("Chave: "); //imprime a letra traduzida no monitor serial
            Serial.println(letra);
        }
        mensagemMorse = ""; //limpa a mensagem Morse após processar
        tempoInatividade = millis(); //reseta o tempo de inatividade
        espacoInserido = false; //reseta o indicador de espaço inserido
    }
}

//processar e adicionar um espaço entre palavras
void processarPalavra() {
    //se o tempo de inatividade for maior que o dobro do limite e a palavra não tiver um espaço no final
    if (millis() - tempoInatividade > 2 * limiteInatividade && palavra.length() > 0 && !espacoInserido) {
        palavra += " "; //adiciona um espaço ao final da palavra
        lcd_1.setCursor(0, 0);
        lcd_1.print("transmissao:"); //exibe "transmissao:" no LCD
        lcd_1.setCursor(0, 1);
        lcd_1.print(palavra); //exibe a palavra com o espaço no LCD
        Serial.print("Recado: "); //imprime a palavra no monitor serial
        Serial.println(palavra);
        espacoInserido = true; //marca que o espaço foi inserido
        mensagemMorse = ""; //limpa a mensagem Morse após o espaço
        tempoInatividade = millis(); //reseta o tempo de inatividade
    }
}

//verificar e realizar o reset se a mensagem Morse for igual ao comando de reset
void verificarReset() {
    if (mensagemMorse == comandoResetMorse) { //se a mensagem Morse for o comando de reset
        reset(); //realiza o reset do sistema
        mensagemMorse = ""; //limpa a mensagem Morse
    }
}

//processar entrada serial e converter caracteres para Morse
void processarEntradaSerial() {
    //MODO II (telegrama): entrada manual via Serial 
    if (Serial.available()) { //se houver dados disponíveis na porta serial
        char caractere = Serial.read(); //lê o caractere da entrada serial
        String codigoMorse = charMorse(caractere); //converte o caractere para o código Morse correspondente
        
        //se o modo atual não for o modo 2, muda para o modo 2
        if (modoAtual != 2) {
            lcd_1.clear(); //limpa a tela do LCD
            lcd_1.setCursor(0, 0); //posiciona o cursor no início da tela
            modoAtual = 2; //define o modo como 2
        }
        
        //se o código Morse for válido
        if (codigoMorse != "") {
            Serial.print(caractere); //imprime o caractere no monitor serial
            Serial.print(" -> ");
            Serial.println(codigoMorse); //imprime o código Morse no monitor serial
            
            //adiciona o código Morse acumulado à variável morseAcumulado
            if (morseAcumulado.length() > 0) morseAcumulado += " ";
            morseAcumulado += codigoMorse;
            
            lcd_1.setCursor(0, 0);
            lcd_1.print("telegrama:"); //exibe "telegrama:" no LCD
            lcd_1.setCursor(0, 1);
            lcd_1.print(morseAcumulado); //exibe o código Morse acumulado no LCD
            if (morseAcumulado.length() > 16) lcd_1.scrollDisplayLeft();  // Se o código for muito longo, rola a tela
            
            //envia os sinais Morse para o LED e buzzer
            for (int i = 0; i < codigoMorse.length(); i++) {
                digitalWrite(ledPin2, HIGH); //acende o LED
                tone(buzzerPin, 1000); //emite o tom no buzzer
                delay(codigoMorse[i] == '.' ? 50 : 100); //duração do ponto ou traço
                digitalWrite(ledPin2, LOW); //apaga o LED
                noTone(buzzerPin); //desliga o buzzer
                delay(100); //pausa entre os sinais
            }
        }
    }
}

void loop() {
    verificarBotao(); //verifica o botão e processa a entrada
    processarLetra(); //processa as letras baseadas no Morse recebido
    processarPalavra(); //processa as palavras (adicionando espaços)
    verificarReset(); //verifica se o comando de reset foi recebido
    processarEntradaSerial(); //processa os dados recebidos pela entrada serial
}
