#include "cmath"
#include "complex"
#include <math.h>
#include <vector>

#include <iostream>
std::vector<float> get_fourrier_transform_from_signal(std::vector<float> sound, int sampling_rate){
	int N = sound.size();

	std::vector<float> fourrier_vector(N);

	const std::complex<double>i (0.0, 1.0);
	double pi = 3.14;

	for (int k = 0; k < N; k++){
		/* exp : std::exp(e) */
		/* Fourrier transform formula: X[k] = sum(N-1, n = 0, x[n] * exp(-2*i*pi*k*n/N))	
		*/
		std::complex<double> sum = (0.0, 0.0);
		for (int n = 0; n < N; n++){
			sum+=std::exp(- 2.0 *i*pi*(double)k*(double)n / (double)N)*(double)sound[n];
		}
		fourrier_vector[k] = std::norm(sum)*(1/std::sqrt(N));

	}

	return fourrier_vector;
}


void print_array_float(std::vector<float> vect){
	for (int i = 0; i < vect.size(); i++){
		std::cout << vect[i] << " " ;
	}
	std::cout << "\n" << std::endl ;
}

