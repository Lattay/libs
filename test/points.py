import numpy as np
import matplotlib.pyplot as plt


def load_data(file_name):
    with open(file_name) as f:
        head = f.readline()
        arr = np.zeros((2, int(head)))
        i = 0
        for line in f:
            x, y, *_ = line.split()
            arr[:, i] = (float(x), float(y))
            i += 1
    return arr


arr = load_data("points.txt")
plt.scatter(arr[0], arr[1], marker='.')
plt.show()
