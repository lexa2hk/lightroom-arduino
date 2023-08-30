const uint8_t bitmap_32x32[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0x70, 0x70, 0x30, 0x30, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF0, 0x70, 0x30, 0x30, 0x20, 0x00, 0x00,
  0x00, 0x30, 0x78, 0xFC, 0x7F, 0x3F, 0x0F, 0x0F, 0x1F, 0x3C, 0x78, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0x80, 0x40, 0xE0, 0xF0, 0xF8, 0xFC, 0xFF, 0x7F, 0x33, 0x13, 0x1E, 0x1C, 0x1C, 0x0E, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xF7, 0xEF, 0x5F, 0x3F, 0x7F, 0xFE, 0xFD, 0xFB, 0xF1, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1E, 0x33, 0x33, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00,
};

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


Button switch_button(SWITCH_PIN);

EncButton eb(S1_PIN,S2_PIN, KEY_PIN);




void setup() {

  switch_state=0;

  MIDI.begin();


  oled.init();
  oled.setScale(2);

  oled.clear();
  oled.print(adj[switch_state]);
  oled.rect(velocity[switch_state]-10, 30, velocity[switch_state]+10, 35);
  oled.update();


}

void loop() {

  bool hasChanged=false;
  bool switched=false;


  switch_button.tick();

  int switch_state_prev=switch_state;
  if(switch_button.click()){
    switched=true;
    hasChanged=true;
    switch_state = (switch_state+1)%adjlen;
  }

  if(switch_state_prev!= switch_state){
    oled.clear();
    oled.home();
    oled.print(adj[switch_state]);
    
    oled.update();
  }



  eb.tick();
  

  if (eb.turn()) {
        hasChanged=true;

        velocity[switch_state] = velocity[switch_state] + ((2+eb.fast()*2)*eb.dir());
        if(velocity[switch_state] > 127) velocity[switch_state]=127;
        if(velocity[switch_state] < 0) velocity[switch_state]=0;

        // Serial.println(velocity[switch_state]);
        
  }

  if(eb.click()){
    hasChanged=true;
    
    velocity[switch_state]=64;
    oled.clear(0 , 30, 127, 35);
    oled.update(0 , 30, 127, 35);
  } 
  if(hasChanged){
    if(!switched) MIDI.sendNoteOn(127, velocity[switch_state], switch_state+1);
    hasChanged=!hasChanged;
    oled.rect(velocity[switch_state]-10, 30, velocity[switch_state]+10, 35);
    oled.update(velocity[switch_state]-10 -6 , 30, velocity[switch_state]+10 + 6, 35);
    oled.clear(velocity[switch_state]-10 -6 , 30, velocity[switch_state]+10 + 6, 35);
    
  }




}
