// ===== Motor_Debug.ino =====
// Прокрут 200 шагов по часовой стрелке -> пауза -> поворот 200 шагов против часовой стрелки

#define STEP_PIN 2 // Внимательно
#define DIR_PIN 5 // Соблюдайте
#define EN_PIN 8 // Распиновку

void setup() {
  Serial.begin(9600);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  digitalWrite(EN_PIN, LOW);   // включить драйвер
  digitalWrite(DIR_PIN, HIGH); // направление
}

void loop() {
  // 200 шагов вперёд
  digitalWrite(DIR_PIN, HIGH);
  for (int i = 0; i < 200; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(800);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(800);
  }

  delay(1000);

  // 200 шагов назад
  digitalWrite(DIR_PIN, LOW);
  for (int i = 0; i < 200; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(800);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(800);
  }

  delay(1000);
}
