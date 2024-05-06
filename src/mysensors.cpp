#include <mysensors.h>

#define DHTTYPE DHT22
#define PIN_DHT22 4
#define RTC_UPDATE 1
#define SW_DO 19
#define SW_CS 5
#define SW_CLK 18
#define DHT_TRIES 5

//#define SET_RTC


DHT dht(PIN_DHT22, DHTTYPE);
RTC_DS3231 rtc;
MAX6675 thermocouple(SW_CLK, SW_CS, SW_DO);

float thermo_temp=0;
float dht_temp=0;
float dht_hum=0;
DateTime time_now;

void mysensors_setup(){
    uint tries = 0;
    Serial.begin(115200);
    Serial.println("Setup Sensors");
    dht.begin();
    rtc.begin();
    pinMode(BUILTIN_LED, OUTPUT);
    #ifdef SET_RTC
    Serial.print("Setup Time: ");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.println(__TIME__);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    #endif
    time_now = rtc.now();
    while (((isnan(dht.readTemperature())||isnan(dht_hum=dht.readHumidity())))&&(tries < DHT_TRIES)){
        Serial.println(F("Failed to read from DHT sensor!"));
        delay(500 / portTICK_PERIOD_MS);
        tries++;
    }
    dht_temp=dht.readTemperature();
    dht_hum=dht.readHumidity();
    thermo_temp=thermocouple.readCelsius();
}   

void print_time(){
    Serial.print("---- ");
    Serial.print(time_now.hour(), DEC);
    Serial.print(':');
    Serial.print(time_now.minute(), DEC);
    Serial.print(':');
    Serial.print(time_now.second(), DEC);
    Serial.println(" ----");
}

void print_dht(){
    if (isnan(dht_temp)||isnan(dht_hum)){
        Serial.println(F("Failed to read from DHT sensor!"));
    }
    else{
        Serial.print("DHT22: ");
        Serial.print(dht_temp); 
        Serial.print("°C - ");
        Serial.print(dht_hum); 
        Serial.println("%");
    }
}

void print_thermo(){
    Serial.print("Thermocouple: ");
    Serial.print(thermo_temp); 
    Serial.println("°C   ");
}