#include "../app.h"
#include <math.h>
#include "unit_conversion.h"
#include "../pod.h"

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
        return (cels * 9/5) + 32;
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
    float pc = 22.0640 * 1000000; //Critical Pressure in Pa
    float tc = 647.096; //Critical Temperature in Kelvin 
    float t = temperature_conversion(startTemp, K, temp);//convert temp to Kelvin
    FURI_LOG_I(TAG,"%f",(double)t);
    float a1 = -7.85951783;
    float a2 = 1.84408259;
    float a3 = -11.7866497;
    float a4 = 22.6807411;
    float a5 = -15.9618719;
    float a6 = 1.80122502;
    float tau = 1 - t/tc;
    FURI_LOG_I(TAG,"%f",(double)tau);
    float ps = pc * expf((tc/t) * (a1*tau + a2*powf(tau,1.5) + a3*powf(tau,3) + a4*powf(tau,3.5)+a5*powf(tau,4) + a6*powf(tau,7.5)));
    float rw = 461.5; // Specific gas constant in J/(kg K)
    if(end == absolute){
        float pa = ps * humid/100;
        //return pa;
        return pa*1000/(rw*t); //returns in g/(m^3)
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
    float millbar = press;
    if(start == PSI){//convert to mbar
        millbar = millbar * 68.948;
    }
    else if (start == inHg){
        millbar = millbar * 33.864;
    }
    else if(start == mmHg || start == Torr){
        millbar = millbar * 1.333;
    }
    
    if(end == PSI){//convert to final
        return millbar/68.948;
    }
    else if(end == inHg){
        return millbar/33.864;
    }
    else if(end == mmHg || end == Torr){
        return millbar/1.333;
    }
    return millbar;
}
/*float decimal_dms_conversion(int start, int end, float degree)
{
    if(start == end){
        return degree;
    }
    float ret = degree;
    if(start == decimal){//convert to mbar
        millbar = millbar * 68.948;
    }
    else if (start == inHg){
        millbar = millbar * 33.864;
    }
    else if(start == mmHg || start == Torr){
        millbar = millbar * 1.333;
    }
    
    if(end == PSI){//convert to final
        return millbar/68.948;
    }
    else if(end == inHg){
        return millbar/33.864;
    }
    else if(end == mmHg || end == Torr){
        return millbar/1.333;
    }
    return millbar;
}*/