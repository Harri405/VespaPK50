#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <mysensors.h>
#include <mydisplay.h>
#define SPLASH_DELAY 2

void debug();

void Task_10ms(void *parameter) {
  while(1){
    vTaskDelay(10 / portTICK_PERIOD_MS);
    if (digitalRead(GPIO_NUM_23)==HIGH){
      esp_sleep_enable_timer_wakeup(5000 / portTICK_PERIOD_MS);
      esp_deep_sleep_start();
    }
  }
}

void Task_500ms(void *parameter) {
    static uint splash_delay = 0;
    while(1){
      vTaskDelay(500 / portTICK_PERIOD_MS);
      thermo_temp=thermocouple.readCelsius();
      if(splash_delay > SPLASH_DELAY){
        mydisplay_update();
      }
      else{
        splash_delay++;
      }
      digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
    }
}

void Task_1000ms(void *parameter) {
  while(1){
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      time_now = rtc.now();
      debug();
  }
}

void Task_5000ms(void *parameter) {
  while(1){
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      dht_temp=dht.readTemperature();
      dht_hum=dht.readHumidity();
  }
}


void setup() {
  pinMode(GPIO_NUM_23, INPUT);  // sets the digital pin 13 as output
  mydisplay_setup();
  mysensors_setup();
  
  xTaskCreatePinnedToCore (
    Task_10ms,     // Function to implement the task
    "Task_10ms",   // Name of the taske
    1536,      // Stack size in words
    NULL,      // Task input parameter
    0,         // Priority of the task
    NULL,      // Task handle.
    1         // Core where the task should run
  );

  xTaskCreatePinnedToCore (
    Task_500ms,     // Function to implement the task
    "Task_500ms",   // Name of the taske
    1536,      // Stack size in words
    NULL,      // Task input parameter
    2,         // Priority of the task
    NULL,      // Task handle.
    1         // Core where the task should run
  );

  xTaskCreatePinnedToCore (
    Task_1000ms,     // Function to implement the task
    "Task_1000ms",   // Name of the taske
    1536,      // Stack size in words
    NULL,      // Task input parameter
    0,         // Priority of the task
    NULL,      // Task handle.
    1         // Core where the task should run
  );

  xTaskCreatePinnedToCore (
  Task_5000ms,     // Function to implement the task
  "Task_5000ms",   // Name of the taske
  1024,      // Stack size in words
  NULL,      // Task input parameter
  0,         // Priority of the task
  NULL,      // Task handle.
  1         // Core where the task should run
  );
}

void debug(){
  //print_time();
  //print_dht();
  //print_thermo();
}

void loop() {
  vTaskDelay(2000);
  }
