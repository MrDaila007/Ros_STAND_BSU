#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;
Servo servo1;
Servo servo2;

#define MY_PERIOD 10000  // период в мс
uint32_t tmr1;         // переменная таймера

int servo1Angle = 90;
int servo2Angle = 90;
int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  mpu.initialize();
  Serial.println(mpu.testConnection() ? "MPU6050 OK" : "MPU6050 FAIL");
  delay(1000);
  
  
  Serial.println("MPU6050 initialized successfully!");
  
  // Инициализация сервоприводов
  servo1.attach(9);
  servo2.attach(10);
}

void loop() {
  // Чтение данных с датчика
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Получение значений угловых скоростей и ускорений
  float gx = mpu.getRotationX();
  float gy = mpu.getRotationY();
  float gz = mpu.getRotationZ();
  float ax = mpu.getAccelerationX();
  float ay = mpu.getAccelerationY();
  float az = mpu.getAccelerationZ();
  
  // Отправка данных в серийный порт
  Serial.print("Gyro X: ");
  Serial.print(gx);
  Serial.print("\tGyro Y: ");
  Serial.print(gy);
  Serial.print("\tGyro Z: ");
  Serial.print(gz);
  Serial.print("\tAccel X: ");
  Serial.print(ax);
  Serial.print("\tAccel Y: ");
  Serial.print(ay);
  Serial.print("\tAccel Z: ");
  Serial.println(az);
  
  // Управление сервоприводами на основе значений угловых скоростей
  int angle1 = map(ax, -16384, 16384, 0, 180);
  int angle2 = map(ay, -16384, 16384, 0, 180);

  angle1 = constrain(angle1, 0, 90);
  angle2 = constrain(angle2, 0, 90);

  if (millis() - tmr1 >= MY_PERIOD) {  // ищем разницу
    tmr1 = millis();                   // сброс таймера
  servo1.write(random(angle1, angle1 + 90));
  servo2.write(random(angle2, angle2 + 90));
  }

  
  delay(10);
}

