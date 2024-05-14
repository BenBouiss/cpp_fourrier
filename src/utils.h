#ifndef UTILS_H
#include "cmath"
#include "complex"
#include <math.h>
#include <vector>
#include <iostream>

std::vector<float> get_fourrier_transform_from_signal(std::vector<float> sound, int sampling_rate);
void print_array_float(std::vector<float> vect);
void pause_program_execution();

#endif
