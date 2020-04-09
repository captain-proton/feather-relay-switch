#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"


#define SERIAL_BAUDRATE 115200
 
fauxmoESP fauxmo;

void setupWifi()
{
    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);
    
    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

        // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();
 
    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", 
        WiFi.SSID().c_str(), 
        WiFi.localIP().toString().c_str());
}

void setupFauxmo()
{
    // Fauxmo
    fauxmo.addDevice(DEVICE_NAME);
    // Gen3 Devices or above
    fauxmo.setPort(80);
    
    // Allow the FauxMo to be discovered
    fauxmo.enable(true);
    
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) 
    {
        Serial.print("Device '");
        Serial.print(device_name);
        Serial.print("' state: ");
        const char * onoff = state ? "on" : "off";
        Serial.println(onoff);

        digitalWrite(RELAY_PIN, state);
    });
}

bool areRequirementsSet()
{
    #ifndef WIFI_SSID
        Serial.println("WIFI_SSID not set");
        return false;
    #endif
    #ifndef WIFI_PASS
        Serial.println("WIFI_PASS not set");
        return false;
    #endif
    #ifndef RELAY_PIN
        Serial.println("RELAY_PIN not set");
        return false;
    #endif
    #ifndef DEVICE_NAME
        Serial.println("DEVICE_NAME not set");
        return false;
    #endif
    return true;
}

void setup() 
{
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    
    if (!areRequirementsSet())
    {
        return;
    }
    setupWifi();
    setupFauxmo();

    pinMode(RELAY_PIN, OUTPUT);
}

void loop()
{
    fauxmo.handle();
}