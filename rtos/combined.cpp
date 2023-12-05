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
//printf("init\n");
Motor m1(p23, p16, p17); // pwm, fwd, rev
Motor m2(p24, p19, p20); // pwm, fwd, rev
AnalogOut DACout(p18);
RawSerial blue(p13,p14);
wave_player waver(&DACout);
// PwmOut red(p21);
// PwmOut green(p22);
// PwmOut blue(p23);
int LEDActive = 1;
int uLCDActive = 1;
static XNucleo53L0A1 *board=NULL;
Serial pc(USBTX,USBRX);
DigitalOut shdn(p15);
DigitalOut myled1(LED1);
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

// void led2_thread(void const *args) {
//   while (true) {
//     printf("starting\n");
//     for (float i = 0.0; i < 1.0; i += 0.05) {
//       red = i;
//       green = i;
//       blue = i;
//       wait(0.5);
//     }
//     Thread::wait(500);
//   }
// }
// Mutex uLCD_mutex;
Semaphore uLCD_semaphore(1);
 uLCD_4DGL uLCD(p9,p10,p11);



void ulcd(void const *args) {
    while (true) {
        uLCD_semaphore.wait();
        uLCD.filled_rectangle(0, 64, 127, 127, WHITE);  // Fill bottom half with red
        uLCD_semaphore.release();
        Thread::wait(500);  // Wait for 500 milliseconds
        

        uLCD_semaphore.wait();
        uLCD.filled_rectangle(0, 64, 127, 127, BLUE);  // Fill bottom half with blue
         uLCD_semaphore.release();
        Thread::wait(500);  // Wait for 500 milliseconds
       
    }
}
void fileRead(void const *args) {
    FILE *wave_file;
    printf("\r\n\nHello, wave world!\n\r");
    Thread::wait(500);
    wave_file=fopen("/sd/Carol of the Bells - Trans-Siberian Orchestra - Higher Quality.wav","r");
    if (wave_file == NULL)
        printf("file open error!\n\n\r");
    printf("playing\n");
    waver.play(wave_file);
    printf("closing\n");
    fclose(wave_file);
    printf("done");
    Thread::wait(500);
}
void distSensor(void const *args) {
    int status;
    uint32_t distance;
    DevI2C *device_i2c = new DevI2C(VL53L0_I2C_SDA, VL53L0_I2C_SCL);
    /* creates the 53L0A1 expansion board singleton obj */
    board = XNucleo53L0A1::instance(device_i2c, A2, D8, D2);
    shdn = 0; //must reset sensor for an mbed reset to work
    wait(0.1);
    shdn = 1;
    wait(0.1);
    /* init the 53L0A1 board with default values */
    status = board->init_board();
    while (status) {
        pc.printf("Failed to init board! \r\n");
        status = board->init_board();
    }
    while (1) {
        status = board->sensor_centre->get_distance(&distance);
        if (status == VL53L0X_ERROR_NONE) {
            //pc.printf("D=%ld mm\r\n", distance);
            if (distance < 50) {
                m1.speed(0);
                m2.speed(0);
                pc.printf("Stopped, obstacle detected");
                wait(200);
                m1.speed(-1.0); // reverse one sec
                m2.speed(-1.0);
                wait(1000);
                m1.speed(0); 
                m2.speed(0);
            }
        }
        Thread::wait(500);
    }
}
int main() {
    uLCD.baudrate(3000000);
    printf("starting t1\n");
    Thread t1(fileRead);
    printf("starting t2\n");
    Thread t2(distSensor);
    Thread t3(motors);
    while (1) {
        Thread::wait(500);
    }
}
