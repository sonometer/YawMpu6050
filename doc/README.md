#   Documentacion parcial revision firmware ILI1934

### Separacion del proyecto VELOS

 El Jueves 31/8/2023, se realizó la separación del módulo de ILI9341 del proyecto VELOS.  
 No se realizó cambio alguno del código interno del proyecto, más allá de los nombres de las funciones y de colocar/modificar argumentos a algunas de ellas para permitir desvincular de las variables globales del módulo.  

### Revisión del 4/9/2023

 Realizada por EAM; se construyó un prototipo de prueba en la casa de EAM con la finalidad de revisar todo lo actuado.  
 Se revisaron las conexiones al ESP32 de manera de ver si hay alguna que tenga algún tipo de conflicto posible.  
 Se encontró que la conexión TFT_DC se encontraba sobre el GPIO 3 el cual se usa para la conexión Rx de la interfase serie que está conectada al conversor Serie-USB que comunica con la computadora; por lo tanto, se cambió a GPIO 4.  
 Por otro lado, se agregó en el programa de prueba el barrido de una tabla con valores supuestos de las tres mediciones que muestra el display para el proyecto VELOS.  

####    Estado de conexiones

 En orden:

 | VCC | GND | CS | RESET | DC | MOSI | SCK | LED | MISO |
 |:---:|:---:|:--:|:-----:|:--:|:----:|:---:|:---:|:----:|
 |3.3 V| GND | 5  | EN    | 4  | 23   | 18  |3.3 V| NoConn|

####    Notas

 1. Si bien MISO no necesita estar conectado, por lo menos para esta forma de manejo, debería verificarse si eléctricamente la conexión queda flotando o si la placa misma tiene pull-up o pull-down para fijarla; de todas maneras, seria coneveniente conectarla a MISO ya que en el futuro, si se queire utilizar la interfase SD que se encuentra en la placa de display, necesitaria de dicha conexión.
 2. Sería conveniente verificar si la conexión del backplane acepta manejarse mediante PWM de manera que, en el futuro, pueda regularse el brillo automáticamente midiendo la iluminación ambiente



