import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate
from scipy.misc import derivative

k = 1
m = 1
h = 2*np.pi

# Funcion de particion de una particula.
def z (V,T):
    I = integrate.quad(lambda x: np.exp(-((1/(k*T))*(x**2)/(2*m))), -np.inf, np.inf)
    return np.power(I[0]/h,3)*V

# logaritmo de la funcion de particion de N particulas.
def lnZ (T, N, V):
    # Usamos la aproximacion de Stirling.
    return N + N*np.log(z(V,T)) - N*np.log(N)

# Energia interna del sistema.
def E (T, N, V):
    return k*(T**2)*derivative(lambda x: lnZ(x, N, V), T, dx=1e-5)

# Presion
def p (T, N, V):
    return k*T*N/z(V,T)*derivative(lambda x: z(x, T), V, dx=1e-5)

def graficarVsT (f):
	Tmaxima = 1000
	l = np.array([0]*Tmaxima, dtype=np.float64)
	for i in range(0,Tmaxima):
		l[i] = f(i+1, 1e23, 1)
	plt.plot(range(1,Tmaxima+1), l)
	plt.ylabel(str(f))
	plt.xlabel('T')
	plt.title('Mecanica clasica')
	plt.show()

graficarVsT(p)
