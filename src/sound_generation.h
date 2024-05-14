#include "cmath"
#include "complex"
#include <math.h>
#include <vector>
#include <iostream>
#include "utils.h"
std::vector<float> additive_synthese(int scintillance, int buffer_size);

std::vector<float> soustractive_synthese(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, 
                                        float & x1, float & x2, float quality, float omega0, bool use_recursive, bool low_filter, bool high_filter);

std::vector<float> apply_low_pass_filter(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, float & x1, float & x2, 
                            float quality, float omega0, bool use_recursive);

std::vector<float> apply_high_pass_filter(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, 
                                float & x1, float & x2, float quality, float omega0, bool use_recursive);

std::vector<float> execute_recursive_step(std::vector<float> initial_sound, std::vector<float> a_vector, 
                                std::vector<float> b_vector, int brillance, int buffer_size, float & y1, float & y2, float & x1, float & x2);
#endif
