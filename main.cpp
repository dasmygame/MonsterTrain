#include "SDFileSystem.h"
#include "mbed.h"
#include "rtos.h"
#include "wave_player.h"
#include "uLCD_4DGL.h"
#include "XNucleo53L0A1.h"
#include "Motor.h"
#include <stdio.h>

#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27

SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD card
Motor m1(p23, p16, p17); // pwm, fwd, rev
Motor m2(p24, p19, p20); // pwm, fwd, rev
AnalogOut DACout(p18); // for sd card waveplayer
RawSerial blue(p13,p14);
wave_player waver(&DACout); // for sd card waveplayer
DigitalOut myled(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);
Serial pc(USBTX,USBRX);
uLCD_4DGL uLCD(p9,p10,p11);
char bnum = '0';
char bhit = '0';
volatile bool startStopLight = false;

//RGB HELPER FUNCTION
void updateMoveLight() {
    startStopLight = !startStopLight;
}

void motors(void const *args) {
    myled = 1;
    while (1) {
        if (blue.readable()) {
            myled2 = 1;
            if (blue.getc() == '!') {
                myled3 = 1;
                if (blue.getc() == 'B') {
                    myled4 = 1;
                    bnum = blue.getc();
                    bhit = blue.getc();
                    switch (bnum) {
                    case '5':
                        if (bhit == '1') {
                            updateMoveLight();
                            m1.speed(1.0);
                            m2.speed(1.0);
                        }
                        else if (bhit == '0') {
                            updateMoveLight();
                            m1.speed(0.0);
                            m2.speed(0.0);
                        }
                        break;
                    case '7':
                        if (bhit == '1') {
                            updateMoveLight();
                            m1.speed(-0.5);
                            m2.speed(0.5);
                        }
                        else if(bhit == '0') {
                            updateMoveLight();
                            m1.speed(0.0);
                            m2.speed(0.0);
                        }
                        break;
                    case '8':
                        if (bhit == '1') {
                            updateMoveLight();
                            m1.speed(0.5);
                            m2.speed(-0.5);
                        }
                        else if(bhit=='0') {
                            updateMoveLight();
                            m1.speed(0.0);
                            m2.speed(0.0);
                        }
                        break;
                    case '6':
                        if (bhit == '1') {
                            updateMoveLight();
                            m1.speed(-1.0);
                            m2.speed(-1.0);
                        }
                        else if(bhit=='0') {
                            updateMoveLight();
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


 void uLCDThread(void const *args) {
     uLCD.cls();
     pc.printf("uLCD init\n");
     uLCD.media_init();
    // uLCD.printf("\n\nAn SD card is needed for image and video data");
     uLCD.set_sector_address(0x000, 0x00);
     uLCD.display_image(0,0);
     wait(10);
     //Play video demo
     while(1) {
         uLCD.cls();
         uLCD.media_init();
         uLCD.set_sector_address(0x00, 0x00);
         uLCD.display_video(0,0);
         Thread::wait(500);
     }
}



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


// //Setup RGB led using PWM pins and class
RGBLed myRGBled(p26,p25,p22); //RGB PWM pins
PwmOut redLED (p26);
PwmOut greenLED (p25);


 void RGBThread(void const *args)
 {
     double colorCodes[6][3];
     // red
     colorCodes[0][0] = 1.0;
     colorCodes[0][1] = 0.0;
     colorCodes[0][2] = 0.0;
     //blue
     colorCodes[1][0] = 0.5;
     colorCodes[1][1] = 0.0;
     colorCodes[1][2] = 0.0;
     // green
     colorCodes[2][0] = 8.0;
     colorCodes[2][1] = 0.0;
     colorCodes[2][2] = 0.0;
     //magenta
     colorCodes[3][0] = 0.87;
     colorCodes[3][1] = 0.21;
     colorCodes[3][2] = 0.05;
     //Cyan
     colorCodes[4][0] = 0.4;
     colorCodes[4][1] = 0.1;
     colorCodes[4][2] = 0.0;
     //Yellow
     colorCodes[5][0] = 0.5;
     colorCodes[5][1] = 0.0;
     colorCodes[5][2] = 0.0;
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

             myRGBled.write(colorCodes[i][0], colorCodes[i][1], colorCodes[i][2]);

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


int main() {
    pc.printf("main\n");
    pc.printf("starting motors\n");
    Thread t2(motors);
    pc.printf("starting uLCD\n");
    Thread t3(uLCDThread);
    pc.printf("starting RGB\n");
    Thread t4(RGBThread);
    while (1) { // music is currently not in a thread, did not work for us in lab 3
        FILE *wave_file;
        pc.printf("\r\n\nHello, wave world!\n\r");
        Thread::wait(500);
        wave_file=fopen("/sd/trainEffect.wav","r");
        if (wave_file == NULL)
            pc.printf("file open error!\n\n\r");
        pc.printf("playing\n");
        waver.play(wave_file);
        pc.printf("closing\n");
        fclose(wave_file);
        pc.printf("done");
        Thread::wait(100);
    }
}
