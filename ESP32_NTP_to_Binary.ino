// Display the seconds of the current time on LEDs in binary.
// Uses four LEDs to show the binary equivalent of the seconds.
// This reaches out to the NTP server to get the current time.

/*
Adapted from Rui Santos NTP client for ESP32
Original project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files.

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include "time.h"

const char* ssid     = "Your SSID";
const char* password = "Your PASSWORD";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;     // -5 hours in seconds for EST
const int   daylightOffset_sec = 3600;  // 1 hour in seconds

int hours = 0;
int minutes = 0;
int seconds = 0;
int seconds_mod = 0;

// Connect each of the following output pins to a resistor in series with an LED.
// The short leg of the LED should be connected to ground (0V).
const int ledPin1 = 27;
const int ledPin2 = 14;
const int ledPin3 = 12;
const int ledPin4 = 13;

void printLocalTime(){
  // uncomment in the loop function to use this
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void setup(){
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
}

void loop(){
  // Uncomment the following line to print local time information
  // printLocalTime();

  // Get the current time
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    hours = timeinfo.tm_hour;
    minutes = timeinfo.tm_min;
    seconds = timeinfo.tm_sec;
  }

  // Print the current time
  Serial.print("H:M:S: ");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(":");
  Serial.println(seconds);
  
  // Convert the seconds to intervals of 10 only
  seconds_mod = seconds % 10;

  // Use 4 LEDs to display seconds in binary
  digitalWrite(ledPin1, (seconds_mod >> 0) & 0x01); // LSB
  digitalWrite(ledPin2, (seconds_mod >> 1) & 0x01);
  digitalWrite(ledPin3, (seconds_mod >> 2) & 0x01);
  digitalWrite(ledPin4, (seconds_mod >> 3) & 0x01);

  delay(1000);
}

