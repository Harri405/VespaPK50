#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>
#include <mydisplay.h>
#include <mysensors.h>
#include "esp_sleep.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)


const char strHello[] = "Hello World!";
int16_t x=0;
int16_t y=20;
uint16_t textWidth, textHeight;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

// 'vespa_splash', 128x64px
const unsigned char vespa_splash [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x07, 0xfc, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xf8, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xf0, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x00, 
	0x00, 0x0f, 0xe0, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfd, 0xff, 0x00, 
	0x00, 0x1f, 0xc0, 0x0f, 0xff, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 
	0x00, 0x1f, 0xc0, 0x1f, 0xff, 0x00, 0x00, 0x3c, 0x00, 0x07, 0x3f, 0x00, 0x1f, 0xe7, 0xfe, 0x00, 
	0x00, 0x1f, 0x80, 0x3f, 0xfc, 0x00, 0x00, 0x3c, 0x03, 0xff, 0xff, 0xc0, 0x3f, 0xc7, 0xfe, 0x00, 
	0x00, 0x3f, 0x80, 0x7f, 0xe0, 0x00, 0x00, 0x7c, 0x03, 0xff, 0xff, 0xc0, 0x7f, 0x87, 0xfc, 0x00, 
	0x00, 0x3f, 0x00, 0xff, 0x80, 0x00, 0x00, 0x7c, 0x03, 0xff, 0xff, 0xe0, 0xff, 0x07, 0xfc, 0x00, 
	0x00, 0x3f, 0x01, 0xff, 0x00, 0x00, 0x00, 0xfc, 0x00, 0xff, 0xbf, 0xe1, 0xff, 0x07, 0xfc, 0x00, 
	0x00, 0x3f, 0x03, 0xfc, 0x00, 0x00, 0x00, 0xfc, 0x01, 0xff, 0x1f, 0xf1, 0xff, 0x07, 0xfc, 0x00, 
	0x00, 0x3e, 0x03, 0xf8, 0x0f, 0xe0, 0x01, 0xfe, 0x01, 0xfe, 0x1f, 0xf1, 0xfe, 0x07, 0xf8, 0x00, 
	0x00, 0x3e, 0x07, 0xf0, 0x3f, 0xf0, 0x03, 0xfe, 0x01, 0xfc, 0x1f, 0xf3, 0xfe, 0x07, 0xf8, 0x00, 
	0x00, 0x3e, 0x07, 0xe0, 0x7f, 0xf8, 0x03, 0xfe, 0x03, 0xfc, 0x1f, 0xf3, 0xfe, 0x07, 0xf8, 0x00, 
	0x00, 0x3e, 0x0f, 0xc0, 0xff, 0xfc, 0x07, 0xff, 0x03, 0xfc, 0x1f, 0xf7, 0xfe, 0x0f, 0xf0, 0x00, 
	0x00, 0x3e, 0x0f, 0xc1, 0xff, 0xfc, 0x0f, 0xff, 0x03, 0xf8, 0x1f, 0xe7, 0xfc, 0x0f, 0xf0, 0x00, 
	0x00, 0x3e, 0x0f, 0x83, 0xff, 0xf8, 0x1d, 0xff, 0x83, 0xf8, 0x3f, 0xe7, 0xfc, 0x1f, 0xf7, 0x00, 
	0x00, 0x3e, 0x1f, 0x07, 0xfc, 0x70, 0x3d, 0xff, 0x87, 0xf8, 0x3f, 0xe7, 0xfe, 0x3f, 0xfe, 0x00, 
	0x00, 0x3e, 0x1f, 0x07, 0xf8, 0xe0, 0x78, 0xff, 0x87, 0xf8, 0x3f, 0xc7, 0xfe, 0x3f, 0xfe, 0x00, 
	0x00, 0x3e, 0x1e, 0x0f, 0xf1, 0xc0, 0xf0, 0xff, 0xc7, 0xf8, 0x7f, 0xc3, 0xff, 0xff, 0xfc, 0x00, 
	0x00, 0x3e, 0x3e, 0x0f, 0xf3, 0x81, 0xf8, 0xff, 0xcf, 0xf8, 0x7f, 0x83, 0xff, 0xe7, 0xf8, 0xc0, 
	0x00, 0x3e, 0x3c, 0x1f, 0xe7, 0x03, 0xf8, 0x7f, 0x8f, 0xf8, 0xff, 0x81, 0xff, 0xc1, 0xff, 0xc0, 
	0x00, 0x3e, 0x3c, 0x1f, 0xfe, 0x07, 0xf0, 0x7f, 0x8f, 0xfd, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 
	0x00, 0x1e, 0x3c, 0x1f, 0xfc, 0x0f, 0xf0, 0x7f, 0x8f, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xc0, 
	0x00, 0x1e, 0x78, 0x1f, 0xf8, 0x1e, 0x78, 0x7f, 0x1f, 0xef, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xc0, 
	0x00, 0x1f, 0x78, 0x1f, 0xf8, 0x7e, 0x7f, 0xff, 0x1f, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
	0x00, 0x0f, 0x78, 0x1f, 0xff, 0xfc, 0x7f, 0xfe, 0x1f, 0xef, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
	0x00, 0x0f, 0xf8, 0x1f, 0xff, 0xf8, 0x3f, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 
	0x00, 0x07, 0xf8, 0x0f, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0xf8, 0x07, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0xf8, 0x03, 0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x78, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3c, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void mydisplay_setup(){
    Serial.println("# Setup Display #");
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    display.setTextColor(WHITE, BLACK);
    display.clearDisplay();
    display.drawBitmap(0,0,vespa_splash,128,64,WHITE);
    display.display();
    //delay(3000);
    //gpio_wakeup_enable(GPIO_NUM_23,GPIO_INTR_HIGH_LEVEL);
  }

void mydisplay_hello(){
    bool static first_time=true;
    if(first_time){
        display.clearDisplay();
        // Setup text rendering parameters
        display.setTextSize(1);
        display.setTextColor(WHITE, BLACK);
        // Measure the text with those parameters
        display.getTextBounds(strHello, 0, 0, &x, &y, &textWidth, &textHeight);
        // Print out the string
        first_time=false;
    }
    display.clearDisplay();
    if (x+textWidth < SCREEN_WIDTH){
    x++;
    }
    else{
    x=0;
    }
    display.setCursor(x,display.height() / 2 - textHeight / 2);
    display.print(strHello);
    display.display();
}

void mydisplay_update(){
    int rtc_x=0;
    int rtc_y=0;
    int dht_x=0;
    int dht_y=25;
    int thermo_x=0;
    int thermo_y=50;

    display.clearDisplay();

    //Time
    display.setTextSize(2);
    display.setCursor(rtc_x,rtc_y);
    if (time_now.hour()<10){
        display.print("0");
        display.print(time_now.hour(), DEC);
    }
    else{
        display.print(time_now.hour(), DEC);
    }
    display.print(':');
    if (time_now.minute()<10){
        display.print("0");
        display.print(time_now.minute(), DEC);
    }
    else{
        display.print(time_now.minute(), DEC);
    }
    display.print(':');
    if (time_now.second()<10){
        display.print("0");
        display.print(time_now.second(), DEC);
    }
    else{
        display.print(time_now.second(), DEC);
    }
    
    //DHT
    display.setTextSize(2);
    display.setCursor(dht_x,dht_y);
    display.print(dht_temp, 0);
    display.drawCircle(display.getCursorX()+3,display.getCursorY()+2,2,WHITE);
    display.setCursor(display.getCursorX()+8,display.getCursorY());
    display.print("C ");
    display.print(dht_hum, 0);
    display.drawCircle(display.getCursorX()+3,display.getCursorY()+2,2,WHITE);
    display.drawLine(display.getCursorX()+2, display.getCursorY()+10, display.getCursorX()+12, display.getCursorY(),WHITE);
    display.drawCircle(display.getCursorX()+10,display.getCursorY()+10,2,WHITE);

    //Thermo
    display.setTextSize(2);
    display.setCursor(thermo_x,thermo_y);
    display.print("EXH: ");
    display.print(thermo_temp,0);
    display.drawCircle(display.getCursorX()+3,display.getCursorY()+2,2,WHITE);
    display.setCursor(display.getCursorX()+8,display.getCursorY());
    display.print("C ");

    display.display();
}