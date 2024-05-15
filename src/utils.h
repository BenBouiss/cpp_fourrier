
#include "cmath"
#include "complex"
#include <math.h>
#include <vector>
#include <iostream>

std::vector<float> get_fourier_transform_from_signal(std::vector<float> sound, int sampling_rate);
void print_array_float(std::vector<float> vect);
void reset_pass_filter_coeff(float & x1, float & x2, float & y1, float & y2);
float pitchToFrequency(int pitch, float A4frequency, int A4pitch);