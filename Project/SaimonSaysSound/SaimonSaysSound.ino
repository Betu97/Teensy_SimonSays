/* Mono Peak Meter

   Scrolling peak audio level meter in the Arduino Serial Monitor

   Audio input needs to connect to pin 16 (A2).  The signal range is 0 to 1.2V.
   See the documentation in the Audio System Design Tool for the recommended
   circuit to connect an analog signal.

   This example code is in the public domain
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "notes.h"

const int Leds[4] = {2,3,4,6};
const int Notes[7][3] ={{1,311,360},{2,501,560},{3,626,675},{4,741,830},{5,1026,1110},{6,1231,1360},{7,1471,1600}};
const int playMe [7] = {NOTE_E4,NOTE_C5,NOTE_E5,NOTE_G5,NOTE_C6,NOTE_E6,NOTE_G6};
const int buttonPin = 12; 
int notesJoc [4];
int combination [30];
int fase = 1;
int found = 0;
int count = 0;
int go = 0;
int buttonState = 0; 
int bucle = 0;
int sound = 1;

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=164,95
AudioAnalyzeNoteFrequency         noteFreq1;          //xy=317,123
AudioConnection          patchCord1(adc1, noteFreq1);
AudioAnalyzePeak         peak1;
AudioConnection          patchCord2(adc1, peak1);



// GUItool: end automatically generated code


void setup() {
  AudioMemory(30);
  pinMode(Leds[0], OUTPUT);
  pinMode(Leds[1], OUTPUT);
  pinMode(Leds[2], OUTPUT);
  pinMode(buttonPin, INPUT); 
  Serial.begin(9600);
    /*
     *  Initialize the yin algorithm's absolute
     *  threshold, this is good number.
     */
  noteFreq1.begin(.15);

  
}

// for best effect make your terminal/monitor a minimum of 31 chars wide and as high as you can.

elapsedMillis fps;
void loop() {
   buttonState = digitalRead(buttonPin);
  if(count > 10000){
    count = 1;
  }

  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    count++;
    go = 0;
  } else {
    go = 1;
  }

  if (count != 0 && go == 1){ 
    if(found == 0){
      randomSeed(count);
      notesJoc[0] = random(1, 7);
      while(found == 0){
        notesJoc[1] = random(1,7);
        if(notesJoc[1] != notesJoc[0]){
          found = 1;
        }
      }
      found = 0;
      while(found == 0){
        notesJoc[2] = random(1,7);
        if(notesJoc[2] != notesJoc[0] && notesJoc[2] != notesJoc[1]){
          found = 1;
        }
      }
      found = 0;
      while(found == 0){
        notesJoc[3] = random(1,7);
        if(notesJoc[3] != notesJoc[0] && notesJoc[3] != notesJoc[1] && notesJoc[3] != notesJoc[2]){
          found = 1;
        }
      }
    }
    if (bucle == (fase - 1)){
      if(fase < 5){
        combination[fase-1] = random(0,3);
      }
      for(int i = 0; i < fase ; i++){
        tone(15,playMe[notesJoc[combination[i]]],1000);
        digitalWrite(Leds[combination[i]], HIGH);
        delay(2000);
        digitalWrite(Leds[combination[i]], LOW);
      }
      bucle = fase;
    }
    if (fps > 24) {
      if (peak1.available()) {
        fps = 0;
        int monoPeak = peak1.read() * 30.0;
        if (noteFreq1.available() && monoPeak > 3) {
          float note = noteFreq1.read();
          int nota = 0;
          for(int i = 0; i<7; i++){
            if(note > Notes[i][1] && note < Notes[i][2]){
              nota = i;
              if(nota == notesJoc[combination[bucle-1]]){
                Serial.printf("welldone %d, %d \n", notesJoc[combination[bucle-1]], nota);
                if (bucle == fase){
                  fase++;
                  bucle--;
                }
                delay(1000);
                bucle++;
              }else{
                Serial.printf("gameover %d, %d\n", notesJoc[combination[bucle-1]], nota);
                fase = 1;
                found = 0;
                count = 0;
                go = 0;
                buttonState = 0; 
                bucle = 0;
                sound = 1;
                digitalWrite(Leds[0], HIGH);
                digitalWrite(Leds[1], HIGH);
                digitalWrite(Leds[2], HIGH);
                digitalWrite(Leds[3], HIGH);
                delay(2000);
                digitalWrite(Leds[0], LOW);
                digitalWrite(Leds[1], LOW);
                digitalWrite(Leds[2], LOW);
                digitalWrite(Leds[3], LOW);
             }
            }
          }
          
          for(int j = 0; j<4; j++){ 
            if(nota == notesJoc[j]){
              digitalWrite(Leds[j], HIGH);
              delay(20);
              digitalWrite(Leds[j], LOW);
            }
          }
          Serial.printf("%d, %d, %d, %d, %d\n", notesJoc[0], notesJoc[1], notesJoc[2], notesJoc[3], nota);
        }
      }
    }
  }
}
