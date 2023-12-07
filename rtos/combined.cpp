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

//SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD card
Motor m1(p23, p16, p17); // pwm, fwd, rev
Motor m2(p24, p19, p20); // pwm, fwd, rev
//AnalogOut DACout(p18);
RawSerial blue(p13,p14);
//wave_player waver(&DACout);
DigitalOut myled(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);
int LEDActive = 1;
int uLCDActive = 1;
//static XNucleo53L0A1 *board=NULL;
Serial pc(USBTX,USBRX);
//DigitalOut shdn(p15);
//DigitalOut myled1(LED1);
char bnum = '0';
char bhit = '0';
volatile bool startStopLight = false;
// //RGB HELPER FUNCTION
 void updateMoveLight()
 {
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


// uLCD_4DGL uLCD(p9,p10,p11);

 

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
 PwmOut redLED (p21);
 DigitalOut greenLED (p12);
 

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

// Ticker flipper;
// Mutex motor_mutex;
// void distSensor(void const *args) {
//     pc.printf("dist sensor\n");
//     int status;
//     uint32_t distance;
//     DevI2C *device_i2c = new DevI2C(VL53L0_I2C_SDA, VL53L0_I2C_SCL);
//     /* creates the 53L0A1 expansion board singleton obj */
//     board = XNucleo53L0A1::instance(device_i2c, A2, D8, D2);
//     shdn = 0; //must reset sensor for an mbed reset to work
//     wait(0.1);
//     shdn = 1;
//     wait(0.1);
//     /* init the 53L0A1 board with default values */
//     status = board->init_board();
//     while (status) {
//         pc.printf("Failed to init board! \r\n");
//         status = board->init_board();
//     }
//     while (1) {
//         status = board->sensor_centre->get_distance(&distance);
//         if (status == VL53L0X_ERROR_NONE) {
//             //pc.printf("D=%ld mm\r\n", distance);
//             if (distance < 50) {
//                 motor_mutex.lock();
//                 m1.speed(0);
//                 m2.speed(0);
//                 pc.printf("Stopped, obstacle detected");
//                 wait(200);
//                 m1.speed(-1.0); // reverse 1 sec
//                 m2.speed(-1.0);
//                 wait(1000);
//                 m1.speed(0); 
//                 m2.speed(0);
//                 motor_mutex.unlock();
//             }
//         }
//         Thread::wait(500);
//     }
// }

// void uLCDThread(void const *args) {
//      uLCD.cls();
//      pc.printf("uLCD init\n");
//      uLCD.media_init();

//     // uLCD.printf("\n\nAn SD card is needed for image and video data");
//      uLCD.set_sector_address(0x000, 0x00);
//      uLCD.display_image(0,0);
//      wait(10);
//      //Play video demo
//      while(1) {
//          uLCD.cls();
//          uLCD.media_init();
//          uLCD.set_sector_address(0x00, 0x00);
//          uLCD.display_video(0,0);
//          Thread::wait(500);
//      }
// }

int main() {
    pc.printf("main\n");
    //uLCD.baudrate(3000000);
    //flipper.attach(&updateMoveLight, 2.0);
    pc.printf("starting RGB\n");
    Thread t1(RGBThread);
    pc.printf("starting distSensor\n");
    //Thread t2(distSensor);
    pc.printf("starting motors\n");
    Thread t3(motors);
    pc.printf("starting uLCD\n");
    pc.printf("test print\n");
    //Thread t4(uLCDThread);
    while (1) {
        // FILE *wave_file;
        // pc.printf("\r\n\nHello, wave world!\n\r");
        // Thread::wait(500);
        // wave_file=fopen("/sd/Carol of the Bells - Trans-Siberian Orchestra - Higher Quality.wav","r");
        // if (wave_file == NULL)
        //     pc.printf("file open error!\n\n\r");
        // pc.printf("playing\n");
        // waver.play(wave_file);
        // pc.printf("closing\n");
        // fclose(wave_file);
        // pc.printf("done");
        Thread::wait(100);
    }
}
