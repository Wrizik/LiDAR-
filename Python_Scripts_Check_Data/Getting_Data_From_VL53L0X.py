import serial
import time

PORT = 'COM7'
BAUD = 9600
FILE_NAME = "Length_Info.txt"

MEASURE_TIME = 12.0


def main():
    ser = serial.Serial(PORT, BAUD, timeout=1)
    time.sleep(2)  # даём Arduino перезагрузиться

    print(f"Сбор данных в течение {MEASURE_TIME} секунд...\n")

    distances = []
    timestamps = []

    start_time = time.time()

    with open(FILE_NAME, "w") as file:
        while True:
            # Проверяем прошедшее время
            elapsed = time.time() - start_time
            if elapsed >= MEASURE_TIME:
                print("\nВремя вышло, остановка измерений.")
                break

            raw = ser.readline()
            line = raw.decode('utf-8', errors='ignore').strip()

            if line:
                try:
                    value = float(line)
                except ValueError:
                    continue  # пропускаем мусор

                distances.append(value)
                timestamps.append(elapsed)

                file.write(f"{elapsed:.3f} {value}\n")
                print(f"{value} мм")

    ser.close()
    print(f"\nГотово! Данные сохранены в {FILE_NAME}")


if __name__ == "__main__":
    main()
