#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

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
    
    pinMode(INTERRUPT_PIN, INPUT);

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
      printYawAngle();
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

  const int INTERRUPT_PIN = 2;

  bool dmpReady = true;

  /**
   * Configura los offsets del MPU6050.
   * (Tal vez esté bueno que se pueda modificar desde el main)
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
   * Muestra yaw, pitch y roll en la consola.
   */
  void printAngles() {
    Serial.println(int(ypr[0] * 180/M_PI));
    Serial.println(int(ypr[1] * 180/M_PI));
    Serial.println(int(ypr[2] * 180/M_PI));
  }
};