#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox;

// Пины CNC Shield V3 (ось X)
#define STEP_PIN 2
#define DIR_PIN 5
#define EN_PIN 8

// Количество шагов на оборот
const int TOTAL_STEPS = 400;

// Время вращения = 10 секунд
const unsigned long ROTATION_TIME = 10000;

// Интервал измерений увеличен: 10 Гц -> 20 Гц (50 ms)
const unsigned long MEASURE_INTERVAL = 50;

unsigned long startMillis = 0;
unsigned long lastMeasureMillis = 0;

// этапы работы мотора:
// 0 - выполняется первый (прямой) проход
// 1 - выполняется обратный проход
// 2 - все движения завершены
int phase = 0;

bool measuringEnabled = false;
bool motorActive = true;

void setup() {
  Serial.begin(9600);

  delay(2000); // стартовая пауза 2 секунды

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  digitalWrite(EN_PIN, LOW);   // включить драйвер
  digitalWrite(DIR_PIN, HIGH); // направление: против часовой (первый проход)

  // Запуск VL53L0X
  if (!lox.begin()) {
    Serial.println("VL53L0X error");
    while (1);
  }

  startMillis = millis();
  lastMeasureMillis = millis();
  measuringEnabled = true;
  motorActive = true;
  phase = 0;
}

void loop() {
  unsigned long currentMillis = millis();

  // ======== Управление мотором (фазовый state-machine) ========
  if (motorActive) {
    unsigned long elapsed = currentMillis - startMillis;

    // время на проход — всегда ROTATION_TIME
    if (elapsed <= ROTATION_TIME) {
      static unsigned long lastStepTime = 0;

      unsigned long stepInterval = ROTATION_TIME / TOTAL_STEPS; // 50 ms при 200 шагах

      if (currentMillis - lastStepTime >= stepInterval) {
        lastStepTime = currentMillis;

        // реализуем одиночный шаг
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(600);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(600);
      }
    } else {
      // текущая фаза закончилась
      if (phase == 0) {
        // первый (прямой) проход закончен — переключаемся на обратный
        phase = 1;
        // сменим направление
        digitalWrite(DIR_PIN, LOW); // обратное направление
        // подготовим таймеры для следующей фазы
        startMillis = millis();
        // немедленно обновим lastMeasureMillis, чтобы измерения шли ровно
        lastMeasureMillis = millis();
        // Сбрасываем lastStepTime (статическая переменная внутри блока останется — принудительно обнулим через задержку)
        delay(1); // краткая пауза, чтобы избежать мгновенного шага из-за старого lastStepTime
      } else if (phase == 1) {
        // обратный проход завершён — останавливаем всё
        phase = 2;
        motorActive = false;
        measuringEnabled = false;

        // Жёстко отключаем драйвер и обнуляем STEP
        digitalWrite(EN_PIN, HIGH);   // выключить драйвер (Hi-Z на выходах)
        digitalWrite(STEP_PIN, LOW);
      }
    }
  }

  // ======== Измерения расстояния (если разрешено) ========
  if (measuringEnabled) {
    if (currentMillis - lastMeasureMillis >= MEASURE_INTERVAL) {
      lastMeasureMillis = currentMillis;

      VL53L0X_RangingMeasurementData_t measure;
      lox.rangingTest(&measure, false);

      if (measure.RangeStatus != 4) {
        Serial.println(measure.RangeMilliMeter);
      }
    }
  }
}
