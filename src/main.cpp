#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "MpuMati.h"

MPUController mpuController;

void setup() {
  if (!mpuController.initializeMPU()) {
    while (1); // Detener ejecución si la inicialización falla
  }
} 

void loop() {
mpuController.processMPUData();
}
