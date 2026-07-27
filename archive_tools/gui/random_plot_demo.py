"""用 Matplotlib 显示可重复刷新的随机折线图。"""

from __future__ import annotations

import argparse
import random

import matplotlib.pyplot as plt
from matplotlib.widgets import Button


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--points", type=int, default=10)
    parser.add_argument("--seed", type=int)
    args = parser.parse_args()
    rng = random.Random(args.seed)

    figure, axis = plt.subplots()
    figure.subplots_adjust(bottom=0.22)
    x_values = list(range(args.points))
    line, = axis.plot(x_values, [rng.random() for _ in x_values])
    axis.set_title("Random Data")

    button_axis = figure.add_axes([0.4, 0.06, 0.2, 0.08])
    button = Button(button_axis, "Update")

    def update(_event) -> None:
        line.set_ydata([rng.random() for _ in x_values])
        axis.relim()
        axis.autoscale_view()
        figure.canvas.draw_idle()

    button.on_clicked(update)
    plt.show()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
