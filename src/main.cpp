/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include "PersonalData.h"

#define NO_KEY_TOILET_PIN 14
#define NO_KEY_BATH_PIN 12
#define NC_MOTION_DETECTOR_PIN 9
#define LIGHT_ON_BATH_PIN 13
#define LIGHT_ON_TOILET_PIN 2
//15

#define MOTOR_BATH_PIN 5
#define MOTOR_TOILET_PIN 4
#define HEAT_TOILET_PIN 10

#define DHT22_PIN 2

#ifndef NODEMCU_PERSONALDATA_H
const char *ssid = "your_ssid";
const char *password = "your_pass";

#endif //NODEMCU_PERSONALDATA_H


boolean out_state = 0;

void enable_MOTOR_TOILET() {
    digitalWrite(MOTOR_TOILET_PIN, HIGH);
}

void enable_MOTOR_BATH() {
    digitalWrite(MOTOR_BATH_PIN, HIGH);
}

void enable_HEAT_TOILET() {
    digitalWrite(HEAT_TOILET_PIN, HIGH);
}

void disable_MOTOR_TOILET() {
    digitalWrite(MOTOR_TOILET_PIN, LOW);
}

void disable_MOTOR_BATH() {
    digitalWrite(MOTOR_BATH_PIN, LOW);
}

void disable_HEAT_TOILET() {
    digitalWrite(HEAT_TOILET_PIN, LOW);
}
long state_changed_millis = 0;
int interupts_bypassed_changed = 0;
void inputs_changed() {
    if(millis()-state_changed_millis<500) {
        interupts_bypassed_changed++;
        return;
    }
    state_changed_millis = millis();
    if (!out_state) {
        enable_HEAT_TOILET();
        enable_MOTOR_BATH();
        enable_MOTOR_TOILET();
    } else {
        disable_HEAT_TOILET();
        disable_MOTOR_BATH();
        disable_MOTOR_TOILET();
    }
    out_state = !out_state;
}

void setup() {
    Serial.begin(115200);
    delay(10);
    pinMode(MOTOR_BATH_PIN, OUTPUT);
    pinMode(MOTOR_TOILET_PIN, OUTPUT);
    pinMode(HEAT_TOILET_PIN, OUTPUT);

    pinMode(NO_KEY_TOILET_PIN, INPUT_PULLUP);
    pinMode(NO_KEY_BATH_PIN, INPUT_PULLUP);
    pinMode(NC_MOTION_DETECTOR_PIN, INPUT_PULLUP);

    pinMode(LIGHT_ON_BATH_PIN, INPUT_PULLUP);
    pinMode(LIGHT_ON_TOILET_PIN, INPUT_PULLUP);


    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
       would try to act as both a client and an access-point and could cause
       network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    attachInterrupt(NO_KEY_TOILET_PIN, inputs_changed, CHANGE);
    attachInterrupt(NO_KEY_BATH_PIN, inputs_changed, CHANGE);
    attachInterrupt(NC_MOTION_DETECTOR_PIN, inputs_changed, CHANGE);
    attachInterrupt(LIGHT_ON_TOILET_PIN, inputs_changed, CHANGE);
    attachInterrupt(LIGHT_ON_BATH_PIN, inputs_changed, CHANGE);
}

long print_millis = 0;

void loop() {
    if (millis() - print_millis < 2000)
        return;
    print_millis = millis();
    if (digitalRead(NO_KEY_BATH_PIN))
        Serial.print("b+");
    else
        Serial.print("b-");
    if (digitalRead(NO_KEY_TOILET_PIN))
        Serial.print("t+");
    else
        Serial.print("t-");
    if (digitalRead(NC_MOTION_DETECTOR_PIN))
        Serial.print("m+");
    else
        Serial.print("m-");
    if (digitalRead(LIGHT_ON_BATH_PIN))
        Serial.print("lb+");
    else
        Serial.print("lb-");
    if (digitalRead(LIGHT_ON_TOILET_PIN))
        Serial.print("lt+");
    else
        Serial.print("lt-");
    Serial.print(" ");
    Serial.println(interupts_bypassed_changed);


}

