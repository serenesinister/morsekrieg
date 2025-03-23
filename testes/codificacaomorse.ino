const int botaoPin = 2;    // Pino do botão
const int ledPin = 5;      // Pino do LED
const int buzzerPin = 10;  // Pino do buzzer

unsigned long tempoPressionado = 0;   // Armazena o tempo em que o botão foi pressionado
unsigned long tempoUltimaPressao = 0; // Armazena o tempo da última pressão
bool botaoPressionado = false; // Variável para verificar se o botão foi pressionado
String mensagemMorse = "";    // Variável para armazenar a mensagem em código Morse

void setup() {
  pinMode(botaoPin, INPUT);    // Configura o pino do botão como entrada
  pinMode(ledPin, OUTPUT);     // Configura o pino do LED como saída
  pinMode(buzzerPin, OUTPUT);  // Configura o pino do buzzer como saída
  Serial.begin(9600);          // Inicia a comunicação serial
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
      Serial.println(mensagemMorse);  // Exibe a mensagem em código Morse

      // Aguarda um tempo curto antes de poder pressionar novamente
      delay(100);  // Atraso entre os cliques para evitar múltiplos registros em uma única pressão
      tempoUltimaPressao = millis();  // Atualiza o tempo da última pressão
    }
  }

  // Se não houve pressionameanto por mais de 1000ms, insira um espaço (separação de caracteres ou palavras)
  if (millis() - tempoUltimaPressao > 300 && mensagemMorse.length() > 0) {
    mensagemMorse += " ";  // Adiciona um espaço entre os caracteres
    Serial.println(mensagemMorse);  // Exibe a mensagem com o espaço
    tempoUltimaPressao = millis();  // Atualiza o tempo da última pressão
  }
}
