#include <MIDI.h>
#include <GyverOLED.h>
#include <EncButton.h>


#define S1_PIN 5
#define S2_PIN 6
#define KEY_PIN 7
#define SWITCH_PIN 8


String adj[] = {"Exposure", "Contrast", "Brightness", "Shadows", "Highlights"};
int velocity[5] = {64,64,64,64,64};
int adjlen = sizeof(adj) / sizeof(adj[0]);


struct MIDISettings : public midi::DefaultSettings
{
    static const long BaudRate = 115200;
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MIDISettings);


GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;



int _note;
int _velocity; 
int _channel;
int _velocity_prev; 
int _channel_prev;

int switch_state;
// \ To be moved into library

Button switch_button(SWITCH_PIN);

EncButton eb(S1_PIN,S2_PIN, KEY_PIN);


void setup() {

  // Serial.begin(115200);

  switch_state=0;


  // put your setup code here, to run once:
  MIDI.begin();
  _note = 127;
  _velocity = 64;
  _channel = 1;
  _velocity_prev = 64;
  _channel_prev = 1;


  oled.init();

  oled.clear();
  oled.update();

  oled.setScale(2);
  // oled.home();
  // oled.print("Hello!");
  // oled.update();  


}

void loop() {
  // put your main code here, to run repeatedly:


  //here we need to increment cnt on click and if more than adjlen, then make 0
  // _channel = button0.getChoice() + 1;

  bool hasChanged=false;

  switch_button.tick();

  int switch_state_prev=switch_state;
  if(switch_button.click()){
    hasChanged=true;
    switch_state = (switch_state+1)%adjlen;
  }

  if(switch_state_prev!= switch_state){
    oled.clear();
    oled.home();
    oled.print(adj[switch_state]);
    oled.update();
  }
  _channel = switch_state+1;


  //encoder

  eb.tick();
  

  if (eb.turn()) {
        hasChanged=true;
        // Serial.print("turn: dir ");
        // Serial.print(eb.dir());
        // Serial.print(", fast ");
        // Serial.print(eb.fast());
        // Serial.print(", hold ");
        // Serial.print(eb.encHolding());
        // Serial.print(", counter ");
        // Serial.println(eb.counter);

        velocity[switch_state] = velocity[switch_state] + ((1+eb.fast()*2)*eb.dir());
        if(velocity[switch_state] > 127) velocity[switch_state]=127;
        if(velocity[switch_state] < 0) velocity[switch_state]=0;

        // Serial.println(velocity[switch_state]);
        
  }
  if(eb.click()){
    hasChanged=true;
    velocity[switch_state]=64;
  } 
  if(hasChanged){
    MIDI.sendNoteOn(_note, velocity[switch_state], _channel);
    hasChanged=!hasChanged;
  }
  




  // If the adjustment choice has changed, get the last known position
  // Update the adjustment with the new position
  // Check if button is pressed, and if already pressed, skip, else reset pos 
  //range 0-127, save velocity for each channel
  // _velocity = encoder0.update(button0.getChoice());
  // To be moved into library

  // if (!((_velocity_prev == _velocity) && (_channel_prev = _channel))) {
  //     MIDI.sendNoteOn(_note, _velocity, _channel);
  //     _velocity_prev = _velocity;
  //     _channel_prev = _channel;
  // }

}
