#include "cmath"
#include "complex"
#include <math.h>
#include <vector>
#include <iostream>
#include "utils.h"
#include <string>

std::vector<float> execute_recursive_step(std::vector<float> initial_sound, std::vector<float> a_vector, 
                                std::vector<float> b_vector, int brillance, int buffer_size, float & y1, float & y2, float & x1, float & x2){
    std::vector<float> return_vector(buffer_size);
    for (int n = 0; n < buffer_size; n ++){
        //std::cout << x1 << " " << x2 << " " << y1 << " " << y2 << std::endl;  
        //return_vector[n] =  a_vector[0] * initial_sound[n] + a_vector[1] * x1 + a_vector[2] * x2;
        //return_vector[n] += y1 * b_vector[1] + b_vector[2] * y2;
        return_vector[n] = (b_vector[0]/a_vector[0]) * initial_sound[n] + (b_vector[1]/a_vector[0]) * x1 + (b_vector[2]/a_vector[0]) * x2;
        return_vector[n] = return_vector[n] - (a_vector[1]/a_vector[0]) * y1 - (a_vector[2]/a_vector[0]) * y2; 
        x2 = x1;
        y2 = y1;

        x1 = initial_sound[n];
        y1 = return_vector[n];
    }
    return return_vector;
}

std::vector<float> apply_low_pass_filter(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, float & x1, float & x2, 
                            float quality, float omega0, bool use_recursive){
    
    std::vector<float> a_vector(3);std::vector<float> b_vector(3);
    float alpha = std::sin(omega0)/(2*quality);
    b_vector[0] = (1-std::cos(omega0))/2;
    b_vector[1] = (1-std::cos(omega0));
    b_vector[2] = b_vector[0];

    a_vector[0] = 1+alpha;
    a_vector[1] = -2*std::cos(omega0);
    a_vector[2] = 1-alpha;

    if (use_recursive){
        return execute_recursive_step(initial_sound, a_vector, b_vector, brillance, buffer_size, y1, y2, x1, x2);
    }
    return {0.0};
}

std::vector<float> apply_high_pass_filter(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, 
                                float & x1, float & x2, float quality, float omega0, bool use_recursive){
    std::vector<float> a_vector(3);std::vector<float> b_vector(3);
    float alpha = std::sin(omega0)/(2*quality);
    b_vector[0] = (1+std::cos(omega0))/2;
    b_vector[1] = -(1+std::cos(omega0));
    b_vector[2] = b_vector[0];

    a_vector[0] = 1+alpha;
    a_vector[1] = -2*std::cos(omega0);
    a_vector[2] = 1-alpha;

    if (use_recursive){
        return execute_recursive_step(initial_sound, a_vector, b_vector, brillance, buffer_size, y1, y2, x1, x2);
    }
    return {0.0};
}


std::vector<float> additive_synthese(int scintillance, int buffer_size){
    return {0.0};
}

std::vector<float> soustractive_synthese(std::vector<float> initial_sound, int brillance, int buffer_size, float & y1, float & y2, 
                                        float & x1, float & x2, float quality, float omega0, bool use_recursive, bool low_filter, bool high_filter){
    /*
    Substractive synthese y[n], y[n-1], y[n-2] produit de la synthèse soustractive
    x[n], x[n-1], x[n-2] son réel présent dans le vector initial sound

    */
    std::vector<float> current_sound = initial_sound;
    if (low_filter){
        current_sound = apply_low_pass_filter(current_sound, brillance, buffer_size, y1, y2, x1, x2, quality, omega0, use_recursive);
    }
    if (high_filter){
        current_sound = apply_high_pass_filter(current_sound, brillance, buffer_size, y1, y2, x1, x2, quality, omega0, use_recursive);
    }
    return current_sound;
}

