const int botaoPin = 2;    // Pino do botão
const int ledPin = 5;      // Pino do LED
const int buzzerPin = 10;   // Pino do buzzer

int contadorCliques = 0;   // Contador de cliques
bool botaoPressionado = false; // Variável para verificar se o botão foi pressionado

void setup() {
  pinMode(botaoPin, INPUT); // Configura o pino do botão como entrada
  pinMode(ledPin, OUTPUT);  // Configura o pino do LED como saída
  pinMode(buzzerPin, OUTPUT); // Configura o pino do buzzer como saída
  Serial.begin(9600);       // Inicia a comunicação serial (opcional para monitoramento)
}

void loop() {
  // Verifica se o botão está pressionado
  if (digitalRead(botaoPin) == HIGH) {
    // O LED fica aceso enquanto o botão está pressionado
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);  // Gera um som no buzzer com 1000 Hz enquanto o LED está aceso

    if (!botaoPressionado) {
      botaoPressionado = true;  // Marca que o botão foi pressionado
      contadorCliques++;        // Incrementa o contador de cliques
      Serial.print("Cliques: ");
      Serial.println(contadorCliques);  // Exibe o número de cliques na serial
    }
  } else {
    // Quando o botão é solto, o LED pisca no ritmo dos cliques e o som também
    if (botaoPressionado) {
      botaoPressionado = false;  // Marca que o botão foi solto
      
      // Ajuste do tempo entre os pulsos para evitar som "rouco"
      int tempoPisca = 100 / (contadorCliques); // Um valor mínimo de 200 milissegundos
      if (tempoPisca < 50) {
        tempoPisca = 50; // Limite inferior de tempo (minimo de 50ms)
      }

      // Pisca o LED e gera o som simultaneamente
      digitalWrite(ledPin, HIGH);  // Acende o LED
      tone(buzzerPin, 1000);       // Emite som no buzzer
      delay(tempoPisca);           // Aguarda o tempo definido
      digitalWrite(ledPin, LOW);   // Apaga o LED
      noTone(buzzerPin);           // Para o som
      delay(tempoPisca);           // Aguarda pelo mesmo tempo antes de repetir o ciclo
    }
  }
}
