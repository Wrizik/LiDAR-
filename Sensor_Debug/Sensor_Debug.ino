// ===== Sensor_Debug.ino =====

#include <Wire.h> // Установите соответствующие
#include <Adafruit_VL53L0X.h> // библиотеки в Arduino IDE

Adafruit_VL53L0X lox;

void setup() {
  Serial.begin(9600); // Это число должно совпадать со значением в мониторе порта (комбинация ctrl+shift+m)
  delay(1000);

  if (!lox.begin()) {
    Serial.println("ERROR: VL53L0X not found!");
    while (1);
  }

  Serial.println("VL53L0X ready!");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.print("Distance: ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("Out of range");
  }

  delay(100);  // 10 Hz
}

