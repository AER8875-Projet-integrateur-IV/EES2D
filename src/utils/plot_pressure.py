import numpy as np
import matplotlib.pyplot as plt


pressure_data = np.loadtxt("pressure.dat",skiprows=4)

plt.plot(pressure_data[:,0],pressure_data[:,1],"o")

plt.title("Pressure")
plt.xlabel("x/c")
plt.ylabel("Cp")
plt.gca().invert_yaxis()
plt.show()
