#include "float_representation.h"

#include <math.h>

float fromIntRep(int intRep) {
    return ((float) intRep) / 100; 
}

int toIntRep(float actual) {
    float scaled = actual * 100;
    return (int) (roundf(scaled));
}
