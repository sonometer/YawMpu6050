/**
 * Controlador MPU6050 con DMP
 * Este programa sirve como controlador para el sensor MPU6050 para obtener datos precisos de aceleración y giroscopio. 
 * 
 * Clase MPUController:
 * - initializeMPU(): Inicializa el MPU6050 y configura el DMP. Establece la velocidad de comunicación I2C, 
 * activa el MPU6050 y configura la sensibilidad del acelerómetro y giroscopio. Además, inicializa el DMP y 
 * configura los offsets del MPU6050 para corregir errores.
 * - processMPUData(): Procesa los datos del MPU6050 utilizando el DMP y muestra el ángulo de yaw en grados en la consola.
 * 
 * Métodos Privados:
 * - setMPUOffsets(): Configura los offsets del MPU6050 con valores obtenidos de la calibración. Es recomendable ejecutar el 
 * programa de calibración proporcionado en la carpeta /calibrar para obtener estos valores específicos para cada sensor.
 * - printYawAngle(): Muestra el ángulo de yaw en la consola. La información se presenta en grados.
 * - printAngles(): Muestra los ángulos de yaw, pitch y roll en la consola, también en grados.
 * 
 * Variables y Estructuras de Datos:
 * - mpu: Instancia de la clase MPU6050 para interactuar con el sensor.
 * - devStatus: Estado de inicialización del DMP.
 * - packetSize: Tamaño del paquete FIFO del DMP.
 * - fifoBuffer: Búfer FIFO para almacenar datos del DMP.
 * - q: Cuaternión para almacenar datos de orientación.
 * - gravity: VectorFloat para almacenar la gravedad calculada.
 * - ypr: Matriz de ángulos de yaw, pitch y roll.
 * 
 * Variables Adicionales:
 * - dmpReady: Estado para verificar si el DMP está listo. Actualmente establecido en true.
 * 
 * Notas Adicionales:
 * Es recomendable ejecutar el programa de calibración incluido para obtener offsets precisos para el sensor MPU6050.
 * La clase MPUController proporciona métodos para inicializar el sensor y procesar sus datos, facilitando la 
 * integración del MPU6050 en proyectos más grandes.
 * 
*/

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "math.h"

class MPUController {
public:
  /**
    * Inicializa el MPU6050 y configura el DMP.
    * 
    * True si la inicialización es exitosa, false de lo contrario.
    */
  bool initializeMPU() {
    Wire.begin(); //Inicio de comunicación I2C
    Wire.setClock(400000); // Configura la velocidad de I2C a 400kHz
    Serial.begin(115200);

    /**
    * Encendido y preparación del MPU para su uso 
    * Se activa y saca de suspención al dispositivo lo cual debe hacerse después del arranque)
    * Se configura el acelerómetro y giroscopio en su forma más sensible
    * +/- 2g y +/- 250 grados/seg
    */
    mpu.initialize();
    
    devStatus = mpu.dmpInitialize();
    
    if (devStatus == 0) {
      // Configura los offsets del MPU6050
      setMPUOffsets();
      mpu.setDMPEnabled(true);
      packetSize = mpu.dmpGetFIFOPacketSize();
      return true;
    } else {
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
      return false;
    }
  }

  /**
  * Procesa los datos del MPU6050 y muestra el ángulo de yaw en grados.
  * (Tal vez esté bueno que se pueda configurar que mostrar desde el main)
  */
  void processMPUData() {
    if (!dmpReady) return;

    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      //printYawAngle();
      printCorrectedYawAngle();
    }
  }

private:
  MPU6050 mpu;
  uint8_t devStatus;
  uint16_t packetSize;
  uint8_t fifoBuffer[64];
  Quaternion q;
  VectorFloat gravity;
  float ypr[3];
  
  bool dmpReady = true;

  /**
   * Configura los offsets del MPU6050.
   * Se utilizan valores obtenidos de la calibración
   * Para obtener los valores de un nuevo sensor hay que ejecutar
   * el programa de calibración incluido en la carpeta /src
   */
  void setMPUOffsets() {
    mpu.setXGyroOffset(73);
    mpu.setYGyroOffset(-19);
    mpu.setZGyroOffset(-18);
    mpu.setZAccelOffset(720);
  }

  /**
   * Muestra el ángulo de yaw en la consola.
   */
  void printYawAngle() {
    Serial.println(int(ypr[0] * 180/M_PI)+179);
  }

  /**
   * Muestra el ángulo de yaw corregido en la consola.
   * Se emplea una corrección trigonométrica básica para minimizar el impacto
   * generado por PITCH y ROLL
  */
  void printCorrectedYawAngle(){
    float compensatedYaw = ypr[0] - sin(radians(ypr[1])) * tan(radians(ypr[2]));
    Serial.println(int(compensatedYaw * 180/M_PI) + 179);
  }
  
  /**
   * Muestra yaw, pitch y roll en la consola.
   */
  void printAngles() {
    Serial.println(int(ypr[0] * 180/M_PI));
    Serial.println(int(ypr[1] * 180/M_PI));
    Serial.println(int(ypr[2] * 180/M_PI));
  }
};
