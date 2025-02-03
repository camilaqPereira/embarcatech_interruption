<h1 align="center">  Contador em WS2812 controlado por pushbuttons (IRQ)</h1>

## [Sobre o projeto](https://drive.google.com/file/d/1Vok09JD78kTA6KN4OpiEUEzqGkNnXWbR/view?usp=sharing)


## Periféricos da BitDogLab utilizados

- WS2812: matriz de leds -> controlado pela PIO (máquina de estádos programável)
- LED RGB vermelho -> controlado pelo loop principal
- Pushbuttons A e B -> gerenciado por meio de IRQ


## Instruções de uso


### Clone o repositório
No seu teminal, digite o seguinte comando:

<code> git clone https://github.com/camilaqPereira/embarcatech_interruption.git </code>

### Importe o projeto no VSCode
No VSCode, importe o projeto utilizando a extensão oficial Rasberry Pi

### Compile o projeto
Compile o projeto utilizando o botão **COMPILE** no fim da página (ou **RUN** caso sua placa já esteja conectada ao computador em modo boot)

### Rode o código
Caso tenha gravado o código na placa seguindo o passo anterior, basta pressionar os pushbuttons para que o contador ative.
