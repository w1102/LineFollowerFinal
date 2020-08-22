#include "PID.h"

PID::PID(float *kp, float *ki, float *kd) {
    myKp = kp;
    myki = ki;
    mykd = kd;
}

int PID::Calcu(int input)  {
    float kp = *myKp;
    float ki = *myki;
    float kd = *mykd;
    float pidValue = 0;
    error = input;

    P = error;
    I = I + error;
    D = error - previousError;
    previousError=error;

    if (error >= 0) pidValue = (kp*P) + (ki*I) + (kd*D); // cộng 10 vì góc giữa servo là 80. không đối xứng giữa góc phải 110 và trái 10;
    else pidValue = ((kp+10)*P) + (ki*I) + (kd*D);  

    return pidValue;


}