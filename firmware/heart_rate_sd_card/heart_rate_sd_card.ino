/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

const int seconds = 20;
const int buffersize = seconds * 300;
int16_t sensorValues[buffersize];
const unsigned int samplerate = 3333; //sample rate in microseconds/sample
const int min_recording_seconds = 5; // seconds
const int min_recording_count = min_recording_seconds * 300;

const int lead_on_pin = A5;
const int lead_off_pin = A4;
const int ecg_output_pin = A6;

void setup() {
  analogReadResolution(12);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  pinMode(lead_on_pin, INPUT);
  pinMode(lead_off_pin, INPUT);
  pinMode(ecg_output_pin, INPUT);

  Serial.println("Waiting to record ECG...");
  while (digitalRead(lead_on_pin) || digitalRead(lead_off_pin)) {}

  SD.remove("datalog.txt");
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  int count;
  Serial.println("Recording Data");
  while ((!digitalRead(lead_on_pin) && !digitalRead(lead_off_pin))) {
    sensorValues[count] = analogRead(ecg_output_pin);
    delayMicroseconds(samplerate);
        // Serial.println(sensorValues[count]);
    count++;
    if (count >= buffersize) {
      break;
    }
  }
  Serial.print("Finished Recording. Recording Time: ");
  Serial.print((count / 300));
  Serial.println(" seconds. Saving to SD Card");

  if (dataFile && count > min_recording_count) {
    for (int i = 0; i < count; i++) {
      dataFile.println(sensorValues[i]);
    }
  } else {
    Serial.println("error opening datalog.txt");
  }
  dataFile.close();
  Serial.println("Finished Saving to SD Card");
}

void loop() {
  // // make a string for assembling the data to log:
  // String dataString = "";

  // // read three sensors and append to the string:
  // for (int analogPin = 0; analogPin < 3; analogPin++) {
  //   int sensor = analogRead(analogPin);
  //   dataString += String(sensor);
  //   if (analogPin < 2) {
  //     dataString += ",";
  //   }
  // }

  // // open the file. note that only one file can be open at a time,
  // // so you have to close this one before opening another.
  // File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // // if the file is available, write to it:
  // if (dataFile) {
  //   dataFile.println(dataString);
  //   dataFile.close();
  //   // print to the serial port too:
  //   Serial.println(dataString);
  // }
  // // if the file isn't open, pop up an error:
  // else {
  //   Serial.println("error opening datalog.txt");
  // }

  // Serial.print(digitalRead(lead_on_pin));
  // Serial.println(digitalRead(lead_off_pin));
}









