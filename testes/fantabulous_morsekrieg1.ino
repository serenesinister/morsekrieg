const int botaoPin = 2;   // Pino do botão
const int ledPin = 5;      // Pino do LED

int contadorCliques = 0;   // Contador de cliques
bool botaoPressionado = false; // Variável para verificar se o botão foi pressionado

void setup() {
  pinMode(botaoPin, INPUT); // Configura o pino do botão como entrada
  pinMode(ledPin, OUTPUT);  // Configura o pino do LED como saída
  Serial.begin(9600);       // Inicia a comunicação serial (opcional para monitoramento)
}

void loop() {
  // Verifica se o botão está pressionado
  if (digitalRead(botaoPin) == HIGH) {
    // O LED fica aceso enquanto o botão está pressionado
    digitalWrite(ledPin, HIGH);
    if (!botaoPressionado) {
      botaoPressionado = true;  // Marca que o botão foi pressionado
      contadorCliques++;        // Incrementa o contador de cliques
      Serial.print("Cliques: ");
      Serial.println(contadorCliques);  // Exibe o número de cliques na serial
    }
  } else {
    // Quando o botão é solto, o LED pisca no ritmo dos cliques
    if (botaoPressionado) {
      botaoPressionado = false;  // Marca que o botão foi solto
      int tempoPisca = 1000 / (contadorCliques); // A cada clique, o tempo entre os pulsos diminui
      digitalWrite(ledPin, HIGH);  // Acende o LED
      delay(tempoPisca);           // Espera pelo tempo definido
      digitalWrite(ledPin, LOW);   // Apaga o LED
      delay(tempoPisca);           // Espera pelo mesmo tempo antes de repetir o ciclo
    }
  }
}
