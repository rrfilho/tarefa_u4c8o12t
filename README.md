# tarefa_u4c8o12t
Repositório da tarefa 8 (unidade 4, capítulo 8) do curso de capacitação em sistemas embarcados - embarcatech


### buttons.h e leds.h
O módulo buttons.h é responsável por inicializar e associar eventos ao botão esquerdo (botão A) e ao botão do joystick do kit de desenvolvimento BitDogLab. Este módulo também efetua o debounce via software.

O módulo leds.h é responsável por inicializar o led RGB e oferece 3 funções, permitindo o controle individual das cores do LED RGB. As 3 funções aceitam um valor de brilho que pode variar de -1.0f à 1.0f, onde 0.0f é apagado e ±1.0f é o brilho máximo.
Todas as fuções utilizam PWM para controlar a intesidade das cores.


### display.h
Módulo responsável por inicializar e controlar o display SSD1306 via I2C. A função display_draw_filled_square_and_rect() desenha na tela um quadrado 8x8 e uma borda por toda a tela e possui três parâmetros: 

- x_relative_position: float que representa a posição relativa do quadrado 8x8 no eixo x;
- y_relative_position: float que representa a posição relativa do quadrado 8x8 no eixo y;
- filled: booleano que determina se a borda deve ser estreita (true) ou larga (false);

x_relative_position e y_relative_position podem variar de -1.0f à 1.0f, onde -1.0f é a posição mais à esquerda ou ao topo e 1.0f é a posição mais a direita ou à baixo. O valor 0.0f representa o centro dos respectivos eixos, logo as posição (0.0f, 0.0f) equivale ao quadrado no centro da tela.

### joystick.h
Módulo responsável por inicializar e realizar a leitura da posição do joystick nos eixos X e Y, utilizando o ADC do micrcontrolador RP2040. Oferece duas funções para a leitura do valor relativo do joystick:

- joystick_read_x_axis: retorna a posição do joystick referente ao eixo X (-1.0f à 1.0f), onde -1.0f identifica que o joystick está posicionado no extremo esquerdo, 0.0f no centro e 1.0f no extremo direto;
- joystick_read_y_axis: retorna a posição do joystick referente ao eixo Y (-1.0f à 1.0f), onde -1.0f identifica que o joystick está posicionado no extremo superior, 0.0f no centro e 1.0f no extremo inferior;


### main.h
main.h é responsável por importar todos os módulos supracitados e inicializa-los. Ao chamar o inicializador do botões, 2 funções de callback são passadas como parâmetro: uma função para tratar os eventos do botão esquerdo e outra para tratar os eventos do botão do joystick. O callback do botão esquerdo, habilita/desabilita os leds azul e vermelho. O callback do botão do joystick liga/desliga o led verde e alterna a espessura da borda do display.

O loop principal captura a cada 100ms os valores do joystick nos eixos X e Y e repassa-os para display. Estas coordenadas determinam a posição do quadrado exibido no display, ou seja, acompanha o movimento do joystick exibindo-o de forma gráfica.
