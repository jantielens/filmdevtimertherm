#include <Wire.h>        // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <DallasTemperature.h>
#include <Ticker.h> //Ticker Library
#include <TimeLib.h>

Ticker g_MonitoringTimer;

OneWire oneWire(D5);
DallasTemperature DS18B20(&oneWire);

char temperatureCString[7];
const int loopDelay = 100;
unsigned long loopNr = 0;
SSD1306Wire display(0x3c, D1, D2);
float tempC = 0;

unsigned long startTime;

void setup()
{
    Serial.begin(115200);
    pinMode(D7, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(D7), resettimer, RISING);

    // Initialising the UI will init the display too.
    display.init();

    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);

    DS18B20.begin();

    startTime = millis();

    g_MonitoringTimer.attach(1, readSensor);
    setTime(0);
    Serial.println("setup done");
}

void resettimer()
{
    setTime(0);
}

void loop()
{
    Serial.println("loop");
    // clear the display
    display.clear();
    dtostrf(tempC, 2, 2, temperatureCString);
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.setFont(ArialMT_Plain_10);

    display.drawString(0, 0, "Temperatuur: ");

    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 14, temperatureCString);
    display.setFont(ArialMT_Plain_10);
    display.drawString(60, 26, "°C");
    unsigned long elapsedTime = millis() - startTime;
    unsigned long elapsedSeconds = elapsedTime / 1000;
    display.setFont(ArialMT_Plain_16);

    String secString = String(second());
    if (secString.length() == 1)
        secString = "0" + secString;
    String minString = String(minute());
    if (minString.length() == 1)
        minString = "0" + minString;

    display.drawString(0, 46, minString + ":" + secString);
    // draw the current demo method
    display.display();
    delay(loopDelay);
}

void readSensor()
{
    if (second() == 0)
        tone(D8, 2000, 100);

    Serial.println("readSensor");
    DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
}