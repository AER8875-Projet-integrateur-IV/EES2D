import numpy as np
import matplotlib.pyplot as plt


residual_data = np.loadtxt("Residual.dat",skiprows=6)
plt.plot([i for i in range(residual_data.shape[0])],residual_data[:,0])
plt.plot([i for i in range(residual_data.shape[0])],residual_data[:,1])
plt.plot([i for i in range(residual_data.shape[0])],residual_data[:,2])
plt.plot([i for i in range(residual_data.shape[0])],residual_data[:,3])
plt.title("Residual")
plt.xlabel("Iterations")
plt.ylabel("Residuals")
plt.legend(["rho_V", "rho_u_V", "rho_v_V","rho_H_V"])
plt.yscale("log")
plt.show()
