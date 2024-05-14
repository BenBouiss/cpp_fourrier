#ifndef SOUND_GEN_H
#include "cmath"
#include "complex"
#include <math.h>
#include <vector>


std::vector<float> additive_synthese(int scintillance, int buffer_size);

std::vector<float> soustractive_synthese(std::vector<float> initial_sound, int scintillance, int buffer_size);
#endif
