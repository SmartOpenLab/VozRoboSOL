# VozRoboSOL
El laboratorio tiene un enorme robot, pero está mudo. Con esta primera versión intentaremos darle voz pregrabada.

## Hardware
Los pines de conexión están indicados en el código fuente. Se requiere de un ESP32, un potenciómetro y un DFPlayer Mini con una tarjeta micro SD y un altavoz. 

El código tiene el siguiente comportamiento y por terminal aparecen mensajes:
· Al inicializar el microcontrolador, este busca que el DFPlayer Mini esté conectado correctamente. Si no lo está, el proceso de inicialización finaliza y el microcontrolador se queda parado.
· Si están conectados correctamente el DFPlayer Mini y el altavoz deberíamos oír la primera pista de audio (1.mp3 o 01.mp3 o 001.mp3...). A partir de este momento en adelante, la pista que se reproduzca se elegirá aleatoriamente (elegirPistaAleatoria()). Hemos incluido una restricción para que no pueda reproducirse dos veces seguidas la misma pista.
· El microcontrolador lee el valor del potenciómetro y si está al mínimo, el microcontrolador reproducirá una pista tras otra sin parar y sin espera entre la reproducción de una y otra pista. Si el potenciómetro está al máximo, a partir de este momento el microcontrolador no reproducirá pistas para no molestar. Si el potenciómetro está en un punto intermedio, en base a este punto se calcula el tiempo de espera entre la reproducción de pistas (calcularIntervalo()) que es un valor entre TIEMPO_MIN y TIEMPO_MAX que en la versión sobre la que escribo este comentario tienen valores de 30 segundos y 5 minutos respectivamente. 

Trabajo futuro:
· Si se quiere alimentar con una batería, pasar a modo de ahorro de energía con temporizador para despertarse y eliminar mensajes de debug (#ifdef).
· Otros detalles que se nos vayan ocurriendo
