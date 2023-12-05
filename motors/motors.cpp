#include "mbed.h"
#include "rtos.h"
#include "Motor.h"
#include <stdio.h>

Motor m1(p23, p9, p10); // pwm, fwd, rev (left motor)
Motor m2(p24, p11, p12); // pwm, fwd, rev (right motor)
RawSerial blue(p13,p14);
char bnum = '0';
char bhit = '0';
void motors(void const *args) {
    while (1) {
        if (blue.readable()) {
            if (blue.getc() == '!') {
                if (blue.getc() == 'B') { // button data packet
                    bnum = blue.getc(); // button number
                    bhit = blue.getc(); // 1=hit, 0=release
                    switch (bnum) {
                    case '5': // button 5 up arrow
                        if (bhit == '1') {
                            m1.speed(1.0);
                            m2.speed(1.0);
                        }
                        else if (bhit == '0') {
                            m1.speed(0.0);
                            m2.speed(0.0);
                        }
                        break;
                    case '7': // button 7 left arrow
                        if (bhit == '1') {
                            m1.speed(0.0);
                            m2.speed(1.0);
                        }
                        else if(bhit == '0') {
                            m1.speed(0.0);
                            m2.speed(0.0);
                        }
                        break;
                    case '8': // button 8 right arrow
                        if (bhit == '1') {
                            m1.speed(1.0);
                            m2.speed(0.0);
                        }
                        else if(bhit=='0') {
                            m1.speed(0.0);
                            m2.speed(0.0);
                        }
                        break;
                    case '6': // button 8 right arrow
                        if (bhit == '1') {
                            m1.speed(-1.0);
                            m2.speed(-1.0);
                        }
                        else if(bhit=='0') {
                            m1.speed(0.0);
                            m2.speed(0.0);
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    Thread::wait(500);
    }
 }
