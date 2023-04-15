#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
Servo servo1;
Servo servo2;
ros::NodeHandle nh;
std_msgs::Float32MultiArray imuData;

void setup() {
  // Инициализация I2C
  Wire.begin();

  // Инициализация MPU6050
  mpu.initialize();

  // Инициализация сервоприводов
  servo1.attach(9);
  servo2.attach(10);

  // Инициализация ROS
  nh.initNode();
  nh.advertise("imu_data", &imuData);

  // Инициализация генератора случайных чисел
  randomSeed(analogRead(A0));
}

void loop() {
  // Генерация случайных углов поворота сервоприводов в диапазоне [-90, 90]
  int servo1Angle = random(-90, 91);
  int servo2Angle = random(-90, 91);
  
  // Управление сервоприводами
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);

  // Чтение данных с MPU6050
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Преобразование данных в градусы
  float accX = ax / 16384.0;
  float accY = ay / 16384.0;
  float accZ = az / 16384.0;
  float gyroX = gx / 131.0;
  float gyroY = gy / 131.0;
  float gyroZ = gz / 131.0;

  // Отправка данных на Raspberry Pi через ROS
  imuData.data_length = 6;
  imuData.data[0] = accX;
  imuData.data[1] = accY;
  imuData.data[2] = accZ;
  imuData.data[3] = gyroX;
  imuData.data[4] = gyroY;
  imuData.data[5] = gyroZ;
  imuData.layout.dim[0].size = 6;
  imuData.layout.dim[0].stride = 6;
  nh.publish(&imuData);

  nh.spinOnce(); // Обработка ROS-сообщений
}