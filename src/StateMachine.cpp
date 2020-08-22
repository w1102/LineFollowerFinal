#include "StateMachine.h"

bool StateMachine::getState()   {
    // 1 là line trắng. 0 là nền đen
    prebias = *myPreBias;
    bias = *myBias;
    bool ret = 0; // sẽ trả về 1 nếu là góc vuông

    switch (readSensors())  { //xử lý các trường hợp

        case B11111: // trượt hẳn ra ngoài
            break;

        case B00000: //trượt  khỏi góc vuông
            bias = 5;
            ret = 1;
            break;

        case B10001: // nằm giữa 2 line trắng
            bias = prebias;
            break;

        case B11001: // trường hợp đặt biệt góc 120o
        case B10011:
            bias = 4;
            break;

        case B01111:    // xe nằm bên trái line cấp 4
        case B00001:    
            if (prebias > 0)
                bias = 4;
            break;

        case B00011:    // xe nằm bên trái line cấp 3 
            if (prebias > 0)
                bias = 4; // tối ưu cho góc 120o
            break;

        case B00111:    // xe nằm bên trái line cấp 2
            //if (prebias > 0)
                bias = 2;
            break;
            break;

        case B00110:    // xe nằm bên trái line cấp 1
            bias = 1;
            break;

        case B01110:    // xe giữa line
            bias = 0;
            break;

        case B01100:    // xe nằm bên phải line cấp 1
            bias = -1;
            break;

        case B11100:    // xe nằm bên phải line cấp 2
            //if (prebias < 0)
                bias = -2;
            break;

        case B11000:    // xe nằm bên phải line cấp 3
            if (prebias < 0)
                bias = -4; // tối ưu rẽ đường cong
            break;

        case B10000:    // xe nằm bên phải line cấp 4
            if (prebias < 0)
                bias = -4;
            break;



        case B11110:   // góc vuông
            if (abs(prebias) < abs(2))    { 
                bias = -4;
                ret = 1;  
            }
            break;

        case B11011:
            bias = prebias;
            break;


        default:
            break;
    }
    prebias = bias;
    *myBias = bias;
    *myPreBias = prebias;
    return ret;

}

StateMachine::StateMachine(int *Bias, int *preBias) {
    myBias = Bias;
    myPreBias = preBias;
}

void StateMachine::printBits(byte number)    {
    for(int i = 4; i >= 0; i--)
        Serial.print(bitRead(number,i));
    Serial.println();   
}

int StateMachine::bitArrayToInt32(bool arr[], int count)    { 
    int ret = 0;
    int tmp;
    for (int i = 0; i < count; i++) {
        tmp = arr[i];
        ret |= tmp << (count - i - 1);
    }
    #ifdef debug
    printBits(ret);
    #endif
    return ret;
}

int StateMachine::readSensors()    { //đọc sensor
    bool sensorValue[5] = {0}; // tạo mảng nhị phân chứa giá trị sensor
    for (int i = 0; i < 5; i++) 
        sensorValue[i] = digitalRead(A0 + i); // đọc sensor
    return bitArrayToInt32(sensorValue, sizeof(sensorValue)/sizeof(bool)); // trả về giá trị sensor dưới dạng int
    
    
}




