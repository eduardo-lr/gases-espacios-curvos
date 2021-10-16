#include<iostream>
#include<math.h>
#include<random>
#include<chrono>
#include<ctime>
#include"matplotlibcpp.h"
# define pi 4*atan(1)
# define c 1
# define k 1
# define G 1
# define m 1
# define h 2*pi
# define M 10 

namespace plt = matplotlibcpp;

unsigned t1, t2;
double dx = 0.8; // tamaño del paso para las derivadas.
int Tmaxima = 100; // temperatura máxima.
double r_s = (2*M*G)/(pow(c,2)); // radio de Schwarzschild.

// Hamiltoniano en la métrica de Schwarzschild.
double H (double r, double theta, double p_r, double p_theta, double p_phi) {
	return c*sqrt((1/(1 - r_s/r))*(pow(m*c,2) + (1 - r_s/r)*pow(p_r,2) + pow(p_theta,2)/pow(r,2) + pow(p_phi,2)/pow(r*sin(theta),2)))-m*pow(c,2);
}

// funcion de particion de una partícula.
double z (double V, double T) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);
	double R_0 = 2*r_s;
	double r_0 = cbrt(3*V/(4*pi)+ pow(R_0,3));
	std::uniform_real_distribution<double> uniforme1(R_0,r_0);
	std::uniform_real_distribution<double> uniforme2(0,pi);
	std::exponential_distribution<double> dist(1/(k*T));
	int N1 = 1000000;
	int N2 = 100; 
	double s1 = 0;
	for (int i = 0; i < N2; i++) {
		double s2 = 0;
		for (int j = 0; j < N1; j++) {
			double r = uniforme1(gen);
			double theta = uniforme2(gen);
			double p_r = dist(gen);
			double p_theta = dist(gen);
			double p_phi = dist(gen);
			s2 += exp(-1/(k*T)*H(r, theta, p_r, p_theta, p_phi))/(exp(-(1/(k*T))*(p_r + p_theta + p_phi)));
		}
		double integral = (16*pow(pi,2)*r_0*s2)/(N1*pow(1/(k*T),3));
		s1 += integral;
	}
	double promedio = s1/N2;
	return promedio/pow(h,3);	
}

// logaritmo de la funcion de particion de N particulas.
double lnZ (double T, double N, double V) {
	// Usamos la aproximacion de Stirling.
	return (N + N*log(z(V,T)) - N*log(N));
}

// Energia interna del sistema.
double E (double T, double N, double V) {
	double derivate = (lnZ(T+dx, N, V) - lnZ(T, N, V))/dx;
	return k*(pow(T,2))*derivate;
}

// Presion
double p (double T, double N, double V)  {
	double derivate = (z(V+dx, T) - z(V, T))/dx;
	return k*T*N*derivate/z(V,T);
}

void graficarFuncionParticion () {
	std::vector<double> l;
	std::vector<int> temperaturas;
	t1 = clock();
	for (int i = 0; i < Tmaxima; i++) {
		l.push_back(z((4*pi*pow(r_s,3)/3)*117, i+1));
		temperaturas.push_back(i+1);
	}
	t2 = clock();
	double time = (double(t2-t1)/CLOCKS_PER_SEC)/60;
	std::cout << "Tiempo de ejecucion: " << time << " minutos" << std::endl;
	plt::plot(temperaturas,l);
	plt::ylabel("funcion de particion de una particula");
	plt::xlabel("Temperatura");
	plt::title("Espacio-tiempo de Schwarzschild");
	plt::show();
}

void graficarEvsT () {
	std::vector<double> l;
	std::vector<int> temperaturas;
	t1 = clock();
	for (int i = 0; i < Tmaxima; i++) {
		l.push_back(E(i+1, pow(10,23),(4*pi*pow(r_s,3)/3)*117));
		temperaturas.push_back(i+1);
	}
	t2 = clock();
	double time = (double(t2-t1)/CLOCKS_PER_SEC)/60;
	std::cout << "Tiempo de ejecucion: " << time << " minutos" << std::endl;
	plt::plot(temperaturas,l);
	plt::ylabel("Energia");
	plt::xlabel("Temperatura");
	plt::title("Espacio-tiempo de Schwarzschild");
	plt::save("./EvsT.png");
}

void graficarPvsT () {
	std::vector<double> l;
	std::vector<int> temperaturas;
	t1 = clock();
	for (int i = 0; i < Tmaxima; i++) {
		l.push_back(p(i+1, pow(10,23),(4*pi*pow(r_s,3)/3)*117));
		temperaturas.push_back(i+1);
	}
	t2 = clock();
	double time = (double(t2-t1)/CLOCKS_PER_SEC)/60;
	std::cout << "Tiempo de ejecucion: " << time << " minutos" << std::endl;
	plt::plot(temperaturas,l);
	plt::ylabel("Presion");
	plt::xlabel("Temperatura");
	plt::title("Espacio-tiempo de Schwarzschild");
	plt::save("./PvsT.png");
}

int main () {
	graficarEvsT();
	return 0;
}
