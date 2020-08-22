#ifndef PID_h
#define PID_h

class PID
{
private:
    float *myKp, *myki, *mykd;
    float error, previousError, P, I, D;
public:
    PID(float *kp, float *ki, float *kd);
    int Calcu(int input);
};



#endif