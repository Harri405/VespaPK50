
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <RTClib.h>
#include <RTClib.h>
#include <DHT.h>
#include <max6675.h>

extern MAX6675 thermocouple;
extern DHT dht;
extern RTC_DS3231 rtc;

extern float thermo_temp;
extern float dht_temp;
extern float dht_hum;
extern DateTime time_now;

void sensors_setup();
void print_time();
void print_dht();
void print_thermo();

#endif //sensors_H