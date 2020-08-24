#ifndef _StateMachine_h
#define _StateMachine_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif

#define debug // in ví trí xe lên serial (dùng tối ưu các góc)


class StateMachine
{
    public:

        StateMachine(int *Bias, int *preBias);
        bool getState();

    private:
        int *myBias;                    //Pointers to the Bias
        int *myPreBias;                 //Pointers to the PreBias
        int bias, prebias;
        
        void printBits(byte); // in giá vị trí xe lên serial.
        int readSensors(); // đọc giá trị sensor vào mảng bolean
        int bitArrayToInt32(bool arr[], int count); // boolean array to int

};



#endif