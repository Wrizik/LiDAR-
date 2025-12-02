import matplotlib.pyplot as plt
import math
import numpy as np
from matplotlib.lines import Line2D

FILE_NAME = "Length_Info.txt"


def read_data(filename):
    timestamps = []
    distances = []

    with open(filename, "r") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) != 2:
                continue

            t, d = parts
            try:
                t = float(t)
                d = float(d)
            except ValueError:
                continue

            timestamps.append(t)
            distances.append(d)

    return timestamps, distances


def plot_polar(distances):
    n = len(distances)
    if n == 0:
        print("Нет данных!")
        return

    distances = np.array(distances)
    angles = np.linspace(0, 2 * math.pi, n, endpoint=False)

    d_min = distances.min()
    d_max = distances.max()

    # Палитра так, чтобы 0->красный, 1->синий: используем перевёрнутый "jet"
    cmap = plt.cm.jet_r

    colors = []
    # Если все расстояния одинаковы — нарисуем все в сером
    if d_max == d_min:
        colors = [(0.5, 0.5, 0.5, 1.0)] * n
    else:
        for d in distances:
            # Точно проверяем на равенство min/max и помечаем строго
            if d == d_min:
                colors.append((1.0, 0.0, 0.0, 1.0))  # красный
            elif d == d_max:
                colors.append((0.0, 0.0, 1.0, 1.0))  # синий
            else:
                # нормируем промежуточное значение 0..1
                norm = (d - d_min) / (d_max - d_min)
                colors.append(cmap(norm))

    plt.figure(figsize=(8, 8))
    ax = plt.subplot(111, projection="polar")

    # рисуем точки с заданными цветами
    ax.scatter(angles, distances, c=colors, s=30, edgecolors='k', linewidths=0.2)

    # дополнительно крупные маркеры для min/max
    min_idx = int(np.argmin(distances))
    max_idx = int(np.argmax(distances))
    ax.scatter([angles[min_idx]], [distances[min_idx]], color='red', s=80, label='Минимум (красный)', zorder=5)
    ax.scatter([angles[max_idx]], [distances[max_idx]], color='blue', s=80, label='Максимум (синий)', zorder=5)

    ax.set_title("Polar distance plot (min→red, max→blue, остальные — радуга)")
    ax.legend(loc="upper right", bbox_to_anchor=(1.3, 1.1))

    plt.show()


if __name__ == "__main__":
    _, distances = read_data(FILE_NAME)
    plot_polar(distances)
