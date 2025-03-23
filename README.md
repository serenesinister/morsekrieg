# MorseKrieg

**MorseKrieg** é uma recriação interativa do uso do código Morse, inspirado nas táticas de comunicação utilizadas durante a Segunda Guerra Mundial. O projeto utiliza uma plataforma Arduino para simular uma estação de comunicação clandestina, permitindo a transmissão de telegramas por meio de sinais sonoros e visuais, promovendo uma experiência educativa e desafiadora.

---

## 1. Contexto

A comunicação eficiente foi essencial durante a **Segunda Guerra Mundial**, e o código Morse desempenhou um papel crucial na transmissão de informações estratégicas. O **MorseKrieg** busca recriar essa dinâmica de comunicação secreta, oferecendo uma experiência moderna e interativa para que os usuários possam explorar e aprender o código Morse de forma prática.

Composto por um sistema eletrônico modular, que inclui os seguintes componentes:
- **Central de Comando (Arduino)**
- **Projetor (LCD)**
- **Telégrafo (botão)**
- **Sinalizadores (LEDs)**
- **Fonógrafo (buzzer)**

O projeto converte mensagens textuais para código Morse e vice-versa, permitindo que os operadores enviem e recebam sinais, comparem a codificação manual com a entrada serial e verifiquem a precisão de sua transmissão.

---

## 2. Visão Geral do Sistema

O **MorseKrieg** simula uma estação de comunicação clandestina da Segunda Guerra Mundial, onde a interação ocorre por meio de dois modos principais de entrada:

- **Modo I - Transmissão (Botão)**: O operador aciona o Telégrafo (botão) para enviar sinais Morse.
- **Modo II - Telegrama (Serial)**: O operador insere diretamente uma mensagem de texto no terminal Serial, que é automaticamente convertida em código Morse.

### Fluxo de Funcionamento - Entrada, Conversão e Saída

O sistema segue um ciclo de entrada, conversão e saída, que é essencial para a codificação e decodificação de mensagens em tempo real. O fluxo de operação é o seguinte:

![Diagrama de funcionamento](https://drive.google.com/uc?id=1V1_GUCqX0fe6Os-KZOQ0gDDhn9yBK-hm)

### 2.1. Entrada

A comunicação começa com a entrada da mensagem, que pode ocorrer de duas formas:

- **I - Transmissão (Botão)**: O operador pressiona o Telégrafo (botão), onde a duração do pressionamento determina se o sinal é um ponto (dit) ou um traço (dah).
- **II - Telegrama (Serial)**: A mensagem de texto é inserida diretamente no terminal Serial e convertida automaticamente para código Morse.

### 2.2. Conversão

Após a entrada, a mensagem é processada e convertida, garantindo precisão na codificação e decodificação:

- **Morse → Texto (Decodificação)**: No Modo I, os sinais gerados pelo Telégrafo são processados e convertidos em caracteres correspondentes ao código Morse.
- **Texto → Morse (Codificação)**: No Modo II, o Telegrama inserido pelo operador é convertido automaticamente para o código Morse correspondente.

### 2.3. Saída

A mensagem codificada é exibida e transmitida de três formas distintas:

- **Exibição no LCD**:
  - O código Morse ou o texto gerado é exibido no Projetor (LCD), permitindo que o operador acompanhe a sequência transmitida.
  
- **Transmissão Sonora**:
  - O sistema emite sons correspondentes aos sinais Morse através do Fonógrafo (buzzer):
    - **Dit**: Som curto, correspondente ao ponto (duração de 50ms).
    - **Dah**: Som mais longo, correspondente ao traço (duração de 100ms).

- **Luz LED**:
  - Os **Sinalizadores (LEDs)** piscam conforme os sinais transmitidos:
    - **Branco (LED 1)**: Indica a transmissão de sinais durante a codificação no Modo I.
    - **Azul (LED 2)**: Emite sinais para codificação manual no Modo II.

### 2.4. Comando Reset

Um comando especial em código Morse, representado pela sequência **“. - . - .”**, simboliza o sinal de **"+"**, que era amplamente utilizado para finalizar a transmissão de mensagens durante a Segunda Guerra Mundial. Este comando reinicia a operação, garantindo a integridade da comunicação e simulando autenticamente as práticas de segurança utilizadas na época.

---

## 3. Componentes Utilizados

O **MorseKrieg** utiliza os seguintes componentes eletrônicos:

- **Arduino**: Para controle central do sistema e execução do código.
- **LCD**: Exibe mensagens em formato de texto ou código Morse.
- **Botão**: Simula o Telégrafo, usado para enviar sinais Morse manualmente.
- **LEDs**: Indicadores visuais para a codificação do Morse.
- **Buzzer**: Emite sons correspondentes aos sinais Morse.

---

## 4. Como Usar

1. **Conectar os componentes** conforme o circuito no [Tinkercad](https://www.tinkercad.com/things/3OdZ6mru3c9-morsekrieg?sharecode=mjjMoH86qqGMuDnSHXcD04rIFeCH9zL_89Xoxoc-vMw).
2. **Carregar o código no Arduino** através do Arduino IDE.
3. **Interagir com o sistema** usando o botão para o modo manual de transmissão ou inserindo o texto via Serial para a conversão automática.
4. **Acompanhar a transmissão** visual e sonora através do LCD, LEDs e buzzer.

---

## 5. Conclusão

O **MorseKrieg** proporciona uma experiência imersiva, que simula os desafios enfrentados pelos operadores de rádio durante a Segunda Guerra Mundial. Cada transmissão exige precisão e agilidade para garantir o sucesso da comunicação. O projeto serve não apenas como uma ferramenta educativa, mas também como um desafio técnico para aqueles que buscam entender e aplicar o código Morse de maneira prática.

---

## 6. Links Importantes

- **Diagrama de Funcionamento no Draw.io**: [Link para o Diagrama no Draw.io](https://viewer.diagrams.net/?tags=%7B%7D&lightbox=1&highlight=0000ff&layers=1&nav=1&title=MorseKrieg.drawio#Uhttps%3A%2F%2Fdrive.google.com%2Fuc%3Fid%3D1tgG7YJqDOauyHudaGi4Mt96fPksYYsdz%26export%3Ddownload#%7B%22pageId%22%3A%22bYg2sIwUH-PpCp0Aj_3V%22%7D)
- **Repositório no GitHub**: [Link para o Repositório no GitHub](https://github.com/serenesinister/morsekrieg)
- **Circuito no Tinkercad**: [Link para o Circuito no Tinkercad](https://www.tinkercad.com/things/3OdZ6mru3c9-morsekrieg?sharecode=mjjMoH86qqGMuDnSHXcD04rIFeCH9zL_89Xoxoc-vMw).
---

Este projeto foi desenvolvido por **Wemerson Soares**.
