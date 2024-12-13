import numpy as np
import matplotlib.pyplot as plt

a_values = [100, 1000, 10000, 100000, 1000000]
colors = ['b', 'g', 'r', 'c', 'm']

plt.figure(figsize=(10, 6))

for a, color in zip(a_values, colors):
    filename = f"numerov_solution_a_{int(a)}.txt"

    data = np.loadtxt(filename)
    
    x = data[:, 0]
    y = data[:, 1]
    
    plt.plot(x, y, label=f'a = {a}', color=color)

plt.xlabel('x')
plt.ylabel('y')
plt.title('Решения краевой задачи для разных значений параметра a')
plt.legend()
plt.grid(True)

plt.savefig("numerov_solutions_plot.png")

plt.show()
