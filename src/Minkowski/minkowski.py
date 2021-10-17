import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate
from scipy.misc import derivative
from time import time

c = 1
k = 1
m = 1
h = 2*np.pi

# Funcion de particion de una particula.
def z (V,T):
	# Hamiltoniano en relatividad especial.
	H = lambda p_x, p_y, p_z: (np.sqrt((m**2)*(c**4)+(p_x**2 + p_y**2 +p_z**2)*(c**2))-m*c**2)
    	I = integrate.nquad(lambda x, y, z: np.exp(-H(x ,y ,z)/(k*T)), [[-np.inf, np.inf],[-np.inf, np.inf], [-np.inf, np.inf]])
	return I[0]*V*4*np.pi/(h**3)

# logaritmo de la funcion de particion de N particulas.
def lnZ (T, N, V):
	# Usamos la aproximacion de Stirling.
	return (N + N*np.log(z(V,T)) - N*np.log(N))

# Energia interna del sistema.
def E (T, N, V):
	return k*(T**2)*derivative(lambda x: lnZ(x, N, V), T, dx=1e-5)

# Presion
def p (T, N, V):
	return k*T*N/z(V,T)*derivative(lambda x: z(x, T), V, dx=1e-5)

Tmaxima = 100

def graficarEvsT ():
	l = {1: np.array([0]*Tmaxima, dtype=np.float64), 
	     5: np.array([0]*Tmaxima, dtype=np.float64), 
	     10: np.array([0]*Tmaxima, dtype=np.float64)}
	for i in range(0,Tmaxima):
		for key in l:
			l[key][i] = E(i+1, 1e23, key)
	for key in l:
		plt.plot(range(1,Tmaxima+1), l[key], label = "V = {}".format(key))
	plt.legend(loc='upper left')
	plt.ylabel('Energia')
	plt.xlabel('Temperatura')
	plt.title('Espacio-tiempo de Minkowski')
	plt.show()

def graficarPvsT ():
	l = {1: np.array([0]*Tmaxima, dtype=np.float64), 
	     5: np.array([0]*Tmaxima, dtype=np.float64), 
	     10: np.array([0]*Tmaxima, dtype=np.float64)}
    	for i in range(0,Tmaxima):
		for key in l:
			l[key][i] = p(i+1, 1e23, key)
	for key in l:
		plt.plot(range(1,Tmaxima+1), l[key], label = "V = {}".format(key))
    	plt.legend(loc='upper left')
    	plt.ylabel('Presion')
    	plt.xlabel('Temperatura')
    	plt.title('Espacio-tiempo de Minkowski')
    	plt.show()

def graficarFuncionParticion ():
    	l = np.array([0]*Tmaxima, dtype=np.float64)
    	t1 = time()
    	for i in range(0,Tmaxima):
        	l[i] = z(1,i+1)
    	t2 = time()
    	t = (t2 - t1)/60
    	print ("tiempo de ejecucion: "+str(t)+" minutos")
    	plt.plot(range(1,Tmaxima+1), l)
    	plt.xlabel('T')
    	plt.title('funcion de particion de una particula')
    	plt.show()

graficarEvsT()
graficarPvsT()
