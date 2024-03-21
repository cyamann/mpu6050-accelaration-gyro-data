#include <Wire.h>
#include <TimeLib.h>

const int MPU6050_addr = 0x68; // MPU6050 I2C address
int16_t AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ;
float AccelCalibrationX = 0, AccelCalibrationY = 0, AccelCalibrationZ = 0;
float GyroCalibrationX = 0, GyroCalibrationY = 0, GyroCalibrationZ = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B); 
  Wire.write(0);    // Exit Sleep Mode
  Wire.endTransmission(true);

  // Calibration
  const int CalibrationSamples = 2000;
  for (int i = 0; i < CalibrationSamples; i++) {
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B); // Starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr, 14, true); // Request 14 registers for Accel and Gyro

    AccX = Wire.read() << 8 | Wire.read(); // ACCEL_XOUT
    AccY = Wire.read() << 8 | Wire.read(); // ACCEL_YOUT
    AccZ = Wire.read() << 8 | Wire.read(); // ACCEL_ZOUT
    // Skip the temperature data
    Wire.read(); Wire.read();
    GyroX = Wire.read() << 8 | Wire.read(); // GYRO_XOUT
    GyroY = Wire.read() << 8 | Wire.read(); // GYRO_YOUT
    GyroZ = Wire.read() << 8 | Wire.read(); // GYRO_ZOUT

    AccelCalibrationX += AccX;
    AccelCalibrationY += AccY;
    AccelCalibrationZ += AccZ;
    GyroCalibrationX += GyroX;
    GyroCalibrationY += GyroY;
    GyroCalibrationZ += GyroZ;
    delay(1); // Delay between samples
  }
  // Calculate average calibration values for both accelerometer and gyroscope
  AccelCalibrationX /= CalibrationSamples;
  AccelCalibrationY /= CalibrationSamples;
  AccelCalibrationZ /= CalibrationSamples;
  GyroCalibrationX /= CalibrationSamples;
  GyroCalibrationY /= CalibrationSamples;
  GyroCalibrationZ /= CalibrationSamples;
}

void loop() {
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B); // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr, 14, true); // Request a total of 14 registers

  AccX = Wire.read() << 8 | Wire.read(); // ACCEL_XOUT
  AccY = Wire.read() << 8 | Wire.read(); // ACCEL_YOUT
  AccZ = Wire.read() << 8 | Wire.read(); // ACCEL_ZOUT
  // Skip the temperature data
  Wire.read(); Wire.read();
  GyroX = Wire.read() << 8 | Wire.read(); // GYRO_XOUT
  GyroY = Wire.read() << 8 | Wire.read(); // GYRO_YOUT
  GyroZ = Wire.read() << 8 | Wire.read(); // GYRO_ZOUT

  // Apply calibration offsets
  float CalibratedAccelX = (AccX - AccelCalibrationX);
  float CalibratedAccelY = (AccY - AccelCalibrationY);
  float CalibratedAccelZ = (AccZ - AccelCalibrationZ);
  float CalibratedGyroX = (GyroX - GyroCalibrationX);
  float CalibratedGyroY = (GyroY - GyroCalibrationY);
  float CalibratedGyroZ = (GyroZ - GyroCalibrationZ);
  String timeString = String(hour()) + ":" + String(minute()) + ":" + String(second()) + ":" + String(millis() % 1000);

    


  Serial.print(timeString);Serial.print(",");
  // Display calibrated accelerometer data
  Serial.print(CalibratedAccelX);Serial.print(",");
  Serial.print(CalibratedAccelY);Serial.print(",");
  Serial.print(CalibratedAccelZ);Serial.print(",");

  // Display
  Serial.print(CalibratedGyroX);Serial.print(",");
  Serial.print(CalibratedGyroY);Serial.print(",");
  Serial.println(CalibratedGyroZ);
}