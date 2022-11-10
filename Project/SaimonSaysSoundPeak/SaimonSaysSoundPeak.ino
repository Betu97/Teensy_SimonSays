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

const int LEDPin = 2;
const int LEDPin1 = 3;
const int LEDPin2 = 4;
//const int Notes[22][3] ={{1,,},{2,,},{3,,},{4,,},{5,,},{6,,},{7,,},{8,,},{9,,},{10,,},{11,,},{12,,},{13,,},{14,,},{15,,},{16,,},{17,,},{18,,},{19,,},{20,,},{21,,},{22,,}}

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=164,95
AudioAnalyzeNoteFrequency         noteFreq1;          //xy=317,123
AudioConnection          patchCord1(adc1, noteFreq1);
AudioAnalyzePeak         peak1;
AudioConnection          patchCord2(adc1, peak1);



// GUItool: end automatically generated code


void setup() {
  AudioMemory(30);
  pinMode(LEDPin, OUTPUT);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
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
  if (fps > 24) {
    if (peak1.available()) {
      fps = 0;
      int monoPeak = peak1.read() * 30.0;
      if(monoPeak > 3){
        digitalWrite(LEDPin, HIGH);
      }else{
        digitalWrite(LEDPin, LOW);
      }
      if(monoPeak > 6){
        digitalWrite(LEDPin1, HIGH);
      }else{
        digitalWrite(LEDPin1, LOW);
      }
      if(monoPeak > 15){
        digitalWrite(LEDPin2, HIGH);
      }else{
        digitalWrite(LEDPin2, LOW);
      }
      if (noteFreq1.available() && monoPeak == 6) {
        float note = noteFreq1.read();
        float prob = noteFreq1.probability();
        Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
      }
    }
  }
  
}
