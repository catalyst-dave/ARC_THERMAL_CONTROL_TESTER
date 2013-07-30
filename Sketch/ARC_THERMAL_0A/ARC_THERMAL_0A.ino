/*
 * File:    readTemperature.ino
 * Version: 1.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * For more information see www.freetronics.com/irtemp
 *
 * IRTemp library uses an Arduino interrupt:
 *   If PIN_CLOCK = 2, then Arduino interrupt 0 is used
 *   If PIN_CLOCK = 3, then Arduino interrupt 1 is used
 */

#include "IRTemp.h"

static const byte PIN_DATA    = 2;
static const byte PIN_CLOCK   = 3;  // Must be either pin 2 or pin 3
static const byte PIN_ACQUIRE = 4;

static const bool SCALE=false;  // Celcius: false, Farenheit: true
int light_sample;
int sample_count;
int high_count;
int low_count;
int duty_cycle;

IRTemp irTemp(PIN_ACQUIRE, PIN_CLOCK, PIN_DATA);

#define DUT1_LIGHT A0

void setup(void) {
  pinMode(DUT1_LIGHT, INPUT);
  analogReference(INTERNAL);
  light_sample = analogRead(DUT1_LIGHT);
  Serial.begin(9600);
  Serial.println("IRTemp example");
  Serial.println("~~~~~~~~~~~~~~");
}

void loop(void) {
  sample_count=0; high_count=0; low_count=0;
  do{
    light_sample = analogRead(DUT1_LIGHT);
    if (light_sample > 500) high_count++;
    else low_count++;
    sample_count++;
  }while(sample_count < 10000);
  duty_cycle = high_count / (sample_count / 100);
  Serial.println(light_sample);
  Serial.println(high_count);
  Serial.println(low_count);
  Serial.println(sample_count);
  Serial.println(duty_cycle);
  
  float irTemperature = irTemp.getIRTemperature(SCALE);
  printTemperature("IR", irTemperature);

  float ambientTemperature = irTemp.getAmbientTemperature(SCALE);
  printTemperature("Ambient", ambientTemperature);

  delay(100);
}

void printTemperature(
  char  *type,
  float  temperature) {

  Serial.print(type);
  Serial.print(" temperature: ");

  if (isnan(temperature)) {
    Serial.println("Failed");
  }
  else {
    Serial.print(temperature);
    Serial.println(SCALE  ?  " F"  :  " C");
  }
}
