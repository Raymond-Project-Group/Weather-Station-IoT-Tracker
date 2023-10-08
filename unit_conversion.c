#include "app.h"
#include <math.h>
#include "unit_conversion.h"

float temperature_conversion(int start, int end, float temp)
{
    if(start == end){
        return temp;
    }
    float cels = temp;
    if(start == F){
        cels = (cels - 32) * 5/9; //convert to celsius
    }
    else if(start == K){
        cels = cels - 273.15; //convert to celsius
    }

    if(end == F){
        return (cels + 32) * 9/5;
    }
    else if(end == K){
        return cels + 273.15;
    }
    return cels;
}
float humidity_conversion(int startHumid, int startTemp, int end, float humid, float temp) //formula found https://www.calctool.org/atmospheric-thermodynamics/absolute-humidity 
{
    if(startHumid == end){
        return humid;
    }
    float pc = 22.0640; //Critical Pressure in MPa
    float tc = 647.096; //Critical Temperature in Kelvin 
    float t = temperature_conversion(startTemp, K, temp);//convert temp to Kelvin
    float a1 = -7.85951783;
    float a2 = 1.84408259;
    float a3 = -11.7866497;
    float a4 = 22.6807411;
    float a5 = -15.9618719;
    float a6 = 1.80122502;
    float tau = 1 - t/tc;
    float ps = pc * expf((tc/t) * (a1*tau + a2*powf(tau,1.5) + a3*powf(tau,3) + a4*powf(tau,3.5)+a5*powf(tau,4.5) + a6*powf(tau,7.5)));
    float rw = 461.5; // Specific gas constant in J/(kg K)
    if(end == absolute){
        float pa = ps * humid/100;
        return pa/(rw*t); //returns in kg/(m^3)
    }
    else{
        float pa = humid*rw*t;
        return (pa*100)/ps;//returns relative humidity percent
    }
}
float pressure_conversion(int start, int end, float press)
{
    if(start == end){
        return press;
    }
    float mBar = press;
    if(start == psi){//convert to mBar
        mBar = mBar * 68.948;
    }
    else if (start == inHg){
        mBar = mBar * 33.864;
    }
    else if(start == mmHg || start == torr){
        mBar = mBar * 1.333;
    }
    
    if(end == psi){//convert to final
        return mBar/68.948;
    }
    else if(end == inHg){
        return mBar/33.864;
    }
    else if(end == mmHg || end == torr){
        return mBar/1.333;
    }
    return mBar;
}