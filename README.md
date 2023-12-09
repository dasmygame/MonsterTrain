# ECE 4180-B Fall 2023 Final Project
Developed by:
<br> Puneet Bansal,
<br> Jordan Beiler,
<br> Rahul Das, &
<br> Benedict Pursell 

## Project Idea
Feeling inspired by the coming Christmas season (and a little bit of Polar Express), our project is a winter-themed Monster truck-train hybrid: the Monster Train. The Monster Train is a remote controller vehicle that can be piloted over bluetooth user the user's cell phone and has a handful of fun features including:
<br>
* Collision detection
* LED movement indicator
* LED smokestack effect
* Train sound effects
* Fun, themed LCD display
<br>
<br> We designed and manufactured a custom housing designed to look like a train engine and a vehicle base including suspension. <br>

## Design of Vehicle Base Drive Train and Enclosure
The Train design was split into 2 parts, the drive train and the shell top enclosure. Both were designed from scratch in TinkerCad and 3D Printed using PLA.
### Drive Train:
* The drive train of the train features 6 wheel drive with the 2 motors attached to the center wheels, making the MonsterTrain center-wheel drive.
* 6 Fenders above each wheel to provide reinforcement from drastic wheel differential suspension changes.
* Suspension was designed and made using a hinge, a spring, and a spring slot holder. This gives feedback and tension for grip when going up elevated surfaces or over obstacles.
* Suspension is loose on non-drive wheels and very tight on center-drive wheel so the vehicle and go in reverse.
* Turning is supported by reverse motion in wheel opposite to turning direction.
[![Screenshot-2023-12-05-225821-removebg-preview.png](https://i.postimg.cc/Y2QBNhxn/Screenshot-2023-12-05-225821-removebg-preview.png)](https://postimg.cc/6T6Pt6gn)
### Shell Top:
* The shell top enclosure of the MonsterTrain is a fully custom PLA plastic 3D printed design, designed in TinkerCad
* It features a smokestack lit up with RGB LED light with V12 engine frame with hot rods emerging from hood
* The hood can be popped from the front or back so you can change anything on the breadboard or reset the mbed
* 2 Train pipes emerging from the top of the cabin of the train.
* Train cabin is hollow for LED displays, 1 window slot on front and back, 2 window slots on teh sides of the cabin
* Slot was etched out in the front of the train for the lidar chip for distance sensing
* Slot was etched out of ceiling of cabin for uLCD display to be placed in
[![Screenshot-2023-12-05-225640-removebg-preview.png](https://i.postimg.cc/HxRSF49G/Screenshot-2023-12-05-225640-removebg-preview.png)](https://postimg.cc/2bdFZWtT)
## Building the Monster Train
## Parts List
* LPC1768 mbed Microcontroller
* Breadboard
* Breadboard wires (as needed)
* 2x hobby DC motors
* 6x 65mm hobby wheels
* 2x battery mount for 4 (each) AA batteries
* 2x Breadboard AC adapter compatable with your battery mounts
* 8x AA batteries
* 1x red LED
* 1x green LED
* 1x RGB LED
* 5x 1k Resistors
* 1x 4D systems uLCD-144-G2 128x128 color LCD display
* 1x SparkFun microSD Transflash breakout
* 2x microSD cards
* 1x Adafruit Bluetooth LE UART Friend
* 1x SparkFun Mono Audio Amp Breakout
* 1x PRT-20660 Through-Hole speaker
  
<img width="384" alt="Screen Shot 2023-12-08 at 10 07 46 PM" src="https://github.com/dasmygame/MonsterTrain/assets/41347340/cf384f6b-2967-44dd-b1ad-6c03412956e4">

####  LED Connections
| mbed Pin # | LED |
| --- | --- |
| p26 | RGB Red |
| p25 | RGB Green |
| p22 | RGB Blue |
| p21 | Red LED |
| p12 | Green LED |

A 1k resistor should be placed in series in between the mbed and the LED.

#### 



## Software
This project makes uses of the mbed RTOS for multi threading support. Each feature is given its own thread.
### uLCD thread
```
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
```
This thread handles the uLCD display. The displayed gif is stored on a micro SD card which is inserted into the micro SD reader onboard the uLCD. The uLCD library is used to initialize the onboard media and displays the video on a loop.

### Light effects thread
```
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
             myRGBled.write(starCodes[i][0], starCodes[i][1], starCodes[i][2]);
             Thread::wait(150);
         }
     }
 }
```
This thread handles the lighting effects of the RGB LED in the smokestack as well as the red and green LED that represent the Monster Train's movement. The RGB smoke stack effects are created using an array of colors which are cycled through. In addition, if the startStopLight global volatile variable is set, the red LED is lit and the green LED is turned off and vice versa. This thread makes use of the inner class RGB Led to simplfy setting the values of the RGB LED. This class is as shown below.
```
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

```

### Sound effects thread and main
```
int main() {
    pc.printf("main\n");
    pc.printf("starting RGB\n");
    Thread t1(RGBThread);
    pc.printf("starting distSensor\n");
    Thread t2(distSensor);
    pc.printf("starting motors\n");
    Thread t3(motors);
    pc.printf("starting uLCD\n");
    Thread t4(uLCDThread);
    while (1) { // music is currently not in a thread, did not work for us in lab 3
        FILE *wave_file;
        pc.printf("\r\n\nHello, wave world!\n\r");
        Thread::wait(500);
        wave_file=fopen("/sd/Carol of the Bells - Trans-Siberian Orchestra - Higher Quality.wav","r");
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
```
Before moving onto main's thread responsibilities, main starts all other threads. Then, this thread makes use of wave player library to read a .wav file from a second micro SD card inserted into the micro SD reader breakout. Library functions read a specific path and store into a `FILE*` pointer and is played in a loop.

### Motor control thread
```
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
```
This thread handles updating the motor speeds and directons based on input from the user using the Adafruit Bluefruit Connect app. The app sends ascii strings to the bluetooth module which are readable by a serial port. In addition, based on user input, helper function updateMoveLight() to update the variable used to set the status of the red and green movement LEDs.
```
//RGB HELPER FUNCTION
void updateMoveLight() {
    startStopLight = !startStopLight;
}
```


## Vehicle Base and Enclosure Video Demo

https://github.com/dasmygame/MonsterTrain/assets/48290252/c7899ea9-2cd0-4a56-8772-985204abce64
<img width="306" alt="Screen Shot 2023-12-08 at 10 08 49 PM" src="https://github.com/dasmygame/MonsterTrain/assets/41347340/4c44ef78-aa62-4bea-8934-ea0c4c3da1fa">

<img width="319" alt="Screen Shot 2023-12-08 at 10 09 09 PM" src="https://github.com/dasmygame/MonsterTrain/assets/41347340/ca92155e-cc23-49df-8c8e-c46239568d65">

<img width="259" alt="Screen Shot 2023-12-08 at 10 09 53 PM" src="https://github.com/dasmygame/MonsterTrain/assets/41347340/d716bd51-4f1c-4edf-9b84-9dcc93654444">

