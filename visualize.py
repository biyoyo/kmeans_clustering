import matplotlib.pyplot as plt
import numpy as np

def visualize_clusters(input_file):
    data = np.loadtxt(input_file, delimiter=' ')
    plt.scatter(x=data[:, 0], y=data[:, 1], c=data[:, 2])
    plt.show()

visualize_clusters("results.txt")
