// example to test the mbed Lab Board lcd lib with the mbed rtos
// Pot1 changes the contrast
// Pot2 changes the speed of the sin wave

#include "mbed.h"
#include "mbed2/168/AnalogOut.h"
#include "rtos.h"
#include "Small_6.h"
#include "Small_7.h"
#include "Arial_9.h"
#include "stdio.h"
#include "C12832_lcd.h"

volatile bool startStopLight = 0;

//Class to control an RGB LED using three PWM pins
class RGBLed
{
public:
    RGBLed(PinName redpin, PinName greenpin, PinName bluepin);
    void write(float red,float green, float blue);
private:
    PwmOut _redpin;
    PwmOut _greenpin;
    PwmOut _bluepin;
};

RGBLed::RGBLed (PinName redpin, PinName greenpin, PinName bluepin)
    : _redpin(redpin), _greenpin(greenpin), _bluepin(bluepin)
{
    //50Hz PWM clock default a bit too low, go to 2000Hz (less flicker)
    _redpin.period(0.0005);
}

void RGBLed::write(float red,float green, float blue)
{
    _redpin = red;
    _greenpin = green;
    _bluepin = blue;
}


//Setup RGB led using PWM pins and class
RGBLed myRGBled(p26,p25,p24); //RGB PWM pins
AnalogOut redLED (p18);
AnalogOut greenLED (p19);


//RGB HELPER FUNCTION
void updateMoveLight()
{
    startStopLight = !startStopLight;
}


// RGB Thread
void RGBThread(void const *args)
{
    double starCodes[6][3];
    // red
    starCodes[0][0] = 1.0;
    starCodes[0][1] = 0.0;
    starCodes[0][2] = 0.0;
    //blue
    starCodes[1][0] = 0.0;
    starCodes[1][1] = 1.0;
    starCodes[1][2] = 0.0;
    // green
    starCodes[2][0] = 0.0;
    starCodes[2][1] = 0.0;
    starCodes[2][2] = 1.0;
    //magenta 
    starCodes[3][0] = 0.81;
    starCodes[3][1] = 0.92;
    starCodes[3][2] = 0.94;
    //Cyan
    starCodes[4][0] = 0.03;
    starCodes[4][1] = 0.16;
    starCodes[4][2] = 0.79;
    //Yellow
    starCodes[5][0] = 1.0;
    starCodes[5][1] = 1.0;
    starCodes[5][2] = 1.0;
    while(1)
    {
        if (startStopLight) 
        {
            greenLED = 0.0;
            redLED = 1.0;
        }
        else
        {
            redLED = 0.0;
            greenLED = 1.0;
        }
        for (int i = 0; i < 6; ++i) 
        {
            if (startStopLight) 
            {
                greenLED = 0.0;
                redLED = 1.0;
            }
            else
            {
                redLED = 0.0;
                greenLED = 1.0;
            }

            myRGBled.write(starCodes[i][0], starCodes[i][1], starCodes[i][2]);
            
            if (startStopLight) 
            {
                greenLED = 0.0;
                redLED = 1.0;
            }
            else
            {
                redLED = 0.0;
                greenLED = 1.0;
            }
            Thread::wait(150);
        }
    }
}

Ticker flipper;

int main()
{
    Thread t1(RGBThread); //start thread1
    flipper.attach(&updateMoveLight, 2.0);
    while(true) 
    {    
        Thread::wait(500);   // wait 0.5s
    }
}
