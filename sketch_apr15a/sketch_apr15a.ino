#include <Wire.h>
#include <Servo.h>

#define MPU6050_ADDR 0x68
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_ACCEL_YOUT_H 0x3D
#define MPU6050_REG_ACCEL_ZOUT_H 0x3F
#define MPU6050_REG_GYRO_XOUT_H 0x43
#define MPU6050_REG_GYRO_YOUT_H 0x45
#define MPU6050_REG_GYRO_ZOUT_H 0x47

Servo servo1;
Servo servo2;

int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;

void setup() {
  servo1.attach(9);
  servo2.attach(10);

  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_REG_PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission(true);

  servo1.write(90);
  servo2.write(90);

  delay(1000);
}

void loop() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_REG_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);

  accel_x = Wire.read() << 8 | Wire.read();
  accel_y = Wire.read() << 8 | Wire.read();
  accel_z = Wire.read() << 8 | Wire.read();

  gyro_x = Wire.read() << 8 | Wire.read();
  gyro_y = Wire.read() << 8 | Wire.read();
  gyro_z = Wire.read() << 8 | Wire.read();

  int angle1 = map(accel_x, -16384, 16384, 0, 180);
  int angle2 = map(accel_y, -16384, 16384, 0, 180);

  angle1 = constrain(angle1, 0, 90);
  angle2 = constrain(angle2, 0, 90);

  servo1.write(random(angle1, angle1 + 90));
  servo2.write(random(angle2, angle2 + 90));

  delay(1000);
}

