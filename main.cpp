#include "mbed.h"
#include "bbcar.h"
#include <cstdio>

DigitalOut led1(LED1);
Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);

DigitalInOut pin8(D8);

BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

int main() {
    // parallax_ping  ping1(pin8);
    parallax_laserping  ping1(pin8);
    int num = 0;
    int start = 0;
    int code[5] = {0};
    float initAngle = 0;
    while (num != 5) {
        while (start != 1) { 
            car.rotate(30);
            wait_us(1500000);
            car.stop();
            wait_us(1000000);
            if((float)ping1>12) {
                start = 1;
                initAngle = (float)car.servo0.angle;
            }
        }
        float value = 0;
        for (int i = 0; i < 10; i++) {
            value += ping1;
        }
        value /= 10;
        if(value > 12) code[num] = 1;
        else code[num] = 0;
        led1=1;
        car.rotate(35);
        wait_us(1200000);
        car.stop();
        wait_us(1000000);
        num++;
    }
    car.stop();
    float distance = 288-2*144*cos((float)abs(car.servo0.angle - initAngle)*6.5*3.14/10/3.14);
        for (int i = 0; i < 5; i++) {
            if (code[i] == 1) led1 = 1;
            else led1 = 0;
            ThisThread::sleep_for(500ms);
            led1 = 0;
            printf("%d\n", code[i]);
            ThisThread::sleep_for(500ms);
        }
        printf("distance = %lf\n", sqrtf(distance));
        ThisThread::sleep_for(2000ms);
}