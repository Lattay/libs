import numpy as np
import matplotlib.pyplot as plt


def load_data(file_name):
    with open(file_name) as f:
        head = f.readline()
        arr = np.zeros((int(head),))
        i = 0
        for line in f:
            x, *_ = line.split()
            arr[i] = float(x)
            i += 1
    return arr


arr = load_data("points.txt")
plt.semilogy(arr/np.max(arr))
plt.show()
