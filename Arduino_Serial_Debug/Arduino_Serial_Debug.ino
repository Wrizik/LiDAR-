// ===== Arduino_Serial_Debug.ino =====
// Минимальная проверка: вывод текста в Serial Monitor,
// приём команд из порта, echo-ответ

// Никаких библиотек, никаких датчиков — чистый тест микроконтроллера, достаточно подключить ардуино и выставить COM-порт.

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial.println("Arduino OK.");
  Serial.println("Отправь что-нибудь в монитор порта — я повторю.");
}

void loop() {
  // Если что-то пришло с компьютера:
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    Serial.print("Получено: ");
    Serial.println(input);

    Serial.println("Я работаю нормально!");
  }

  // Периодически отправляем пинг
  static unsigned long last = 0;
  if (millis() - last > 1000) {
    last = millis();
    Serial.println("Ping from Arduino");
  }
}

// Что делает этот скетч?
// Проверяет, что Arduino отправляет сообщения: 
// Arduino OK.
// Отправь что-нибудь в монитор порта — я повторю.
// Ping from Arduino
// Ping from Arduino
// ...

// Проверяет, что Arduino принимает данные:
// Ты пишешь в Serial Monitor: 
// hello
// Arduino отвечает: 
// Получено: hello
// Я работаю нормально!