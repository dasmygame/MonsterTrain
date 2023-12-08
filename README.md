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
* Adafruit VL53L0X TOF sensor
* 2x hobby DC motors
* 6x 65mm hobby wheels
* 2x battery mount for 4 (each) AA batteries
* 8x AA batteries
* 1 red LED
* 1 green LED
* 1 RGB LED
* NOT DONE
* FINISH

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

### Sound effects thread (main)
# HAVE TO ADD OTHER TWO THREADS TO MAIN TO COMPLETE IT
```
int main() {
    Thread t1(RGBThread);
    Thread t4(uLCDThread);
    while (1) { 
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
```
This thread makes use of wave player library to read a .wav file from a second micro SD card inserted into the micro SD reader breakout. Library functions read a specific path and store into a `FILE*` pointer and is played in a loop.

### Motor control thread


## Vehicle Base and Enclosure Video Demo

https://github.com/dasmygame/MonsterTrain/assets/48290252/c7899ea9-2cd0-4a56-8772-985204abce64

