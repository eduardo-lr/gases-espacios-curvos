#include<iostream>
#include<math.h>
#include<random>
#include<chrono>
#include<ctime>
#include"matplotlibcpp.h"
# define pi 4*atan(1)
# define c 1
# define k 1
# define m 1
# define h 2*pi

namespace plt = matplotlibcpp;

unsigned t1, t2;
double dx = 0.8; // tamaño del paso para las derivadas.
int Tmaxima = 100; 

// Hamiltoniano en relatividad especial.
double H (double p_x, double p_y, double p_z) {
	return c*sqrt(pow(m*c,2) + pow(p_x,2) + pow(p_y,2) + pow(p_z,2))-m*pow(c,2);
}

// funcion de particion de una partícula.
double z (double V, double T) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);
	std::exponential_distribution<double> dist(1/(k*T));
	int N = 20000;
	int M = 100; 
	double s1 = 0;
	for (int i = 0; i < M; i++) {
		double s2 = 0;
		for (int j = 0; j < N; j++) {
			double p_x = dist(gen);
			double p_y = dist(gen);
			double p_z = dist(gen);
			s2 += exp(-1/(k*T)*H(p_x,p_y, p_z))/(pow(1/(k*T),3)*exp(-(1/(k*T))*(p_x + p_y + p_z)));
		}
		double integral = (8*s2)/N;
		s1 += integral;
	}
	double promedio = s1/M;
	return V*promedio/pow(h,3);	
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
		l.push_back(z(1, i+1));
		temperaturas.push_back(i+1);
	}
	t2 = clock();
	double time = (double(t2-t1)/CLOCKS_PER_SEC)/60;
	std::cout << "Tiempo de ejecucion: " << time << " minutos" << std::endl;
	plt::plot(temperaturas,l);
	plt::ylabel("funcion de particion de una particula");
	plt::xlabel("Temperatura");
	plt::title("Espacio-tiempo de Minkowski");
	plt::show();
}

void graficarEvsT () {
	std::vector<double> l;
	std::vector<int> temperaturas;
	t1 = clock();
	for (int i = 0; i < Tmaxima; i++) {
		l.push_back(E(i+1, pow(10,23),1));
		temperaturas.push_back(i+1);
	}
	t2 = clock();
	double time = (double(t2-t1)/CLOCKS_PER_SEC)/60;
	std::cout << "Tiempo de ejecucion: " << time << " minutos" << std::endl;
	plt::plot(temperaturas,l);
	plt::ylabel("Energia");
	plt::xlabel("Temperatura");
	plt::title("Espacio-tiempo de Minkowski");
	plt::show();
}

void graficarPvsT () {
	std::vector<double> l;
	std::vector<int> temperaturas;
	t1 = clock();
	for (int i = 0; i < Tmaxima; i++) {
		l.push_back(p(i+1, pow(10,23),1));
		temperaturas.push_back(i+1);
	}
	t2 = clock();
	double time = (double(t2-t1)/CLOCKS_PER_SEC)/60;
	std::cout << "Tiempo de ejecucion: " << time << " minutos" << std::endl;
	plt::plot(temperaturas,l);
	plt::ylabel("Presion");
	plt::xlabel("Temperatura");
	plt::title("Espacio-tiempo de Minkowski");
	plt::show();
}

int main () {
	graficarFuncionParticion ();
	return 0;
}
