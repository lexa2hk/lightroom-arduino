# lightroom-arduino

lightroom-arduino is designed to be an open-source modular controller for photo post-processing software, such as Lightroom. Use buttons, rotary encoders, potentiometers and any other interface to offer ultimate control over your photo editing.

lightroom-arduino is initially designed for use on Arduino, and is currently in very early stages of development.

## Origin

This project originally comes from [lightroom-arduino](https://github.com/rninov/LRduino), thanks [rninov](https://github.com/rninov)

## Prerequisites
lightroom-arduino depends on a number of tools to convert incoming serial into MIDI, which can then be interpreted by Lightroom:
- [Hairless-MIDIserial](https://github.com/projectgus/hairless-midiserial)
- [LoopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html)
- [MIDI2LR](https://rsjaffe.github.io/MIDI2LR/)

## Libraries
lightroom-arduino depends on a number of arduino libraries:
- MIDI Library
- GyverOLED
- EncButton

In future iterations of this project, the intention is to output MIDI directly over USB (e.g. Arduino Pro Micro), negating the need for the Hairless-MIDIserial and LoopMIDI software.

## Simulation

Simulation made with Wokwi: 
- [lightroom-arduino](https://wokwi.com/projects/374480138512988161)

## Setup
Clone this project and upload the lightroom-arduino.ino example to your Arduino UNO. The Baud Rate is 115200.


Once this is setup, open LoopMIDI and open a port. 

Open Hairless-MIDIserial and change the Baud rate to 115200 in the preferences. Select the Arduino UNO as an input, and the LoopMIDI port as an output. Tick "Debug MIDI Messages" and turn the rotary encoder - you should see:

`+1.0 - Serial In: Ch 1: Controller 127 value 64`

The value will change as you turn the encoder - this represents the selected adjustment value.

Clicking the button will cycle the Channel `Ch` - this represents the selected adjustment. By default, 5 adjustments are available to be mapped - these can be configured in lightroom-arduino.ino:

`String adj[] = {"Exposure", "Contrast", "Brightness", "Shadows", "Highlights"};`

Open Lightroom and the MIDI2LR plugin. Turn the encoder, and map the input shown on MIDI2LR to the Lightroom adjustment shoown on the LCD. Click the button and repeat this process until all inputs channels are mapped. 

Open a photo, and start editing!

## Future of project

- Sketch for UNO and NANO as separated branch
- Refactoring for ProMicro
- Encoder in simulation
- Custom Boot Logo as a Bitmap
- Photo switch buttons

Probably:

- Second encoder
