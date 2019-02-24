/**
 * ESP8266-WROOM-02 Temperature Test
 * 
 * Author: Shawn Hymel
 * Date: February 19, 2019
 * 
 * Sends temperature data (Celsius) to specified 
 * webserver via HTTP GET request:
 * 
 *   http://<hostname>/xx.yy
 *   
 * Where <hostname> is hostname variable and
 * xx.yy is temperature (Celsius to 2 decimal
 * places).
 * 
 * I2C Temperature sensor: MCP9808
 * 
 * License: Beerware
 * As long as you retain this notice you can do 
 * whatever you want with this stuff. If we meet 
 * some day, and you think this stuff is worth it, 
 * you can buy me a beer in return
 */

#include <ESP8266WiFi.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"

// Parameters
const char ssid[] = "<YOUR SSID>";
const char password[] = "<YOUR WIFI PASSWORD>";

// Server, file, and port
const char hostname[] = "<YOUR SERVER'S IP ADDR>";
const int port = 1337;

// How long we want to wait between requests (ms)
const int sleep_ms = 10000;

// Pins
const int LED_PIN = 5;
const int SDA_PIN = 2;
const int SCL_PIN = 14;

// MCP9808 temperature sensor I2C address
const int TEMP_I2C_ADDR = 0x18;

// WiFi client
WiFiClient client;

// Temperature sensor object
Adafruit_MCP9808 temp_sensor = Adafruit_MCP9808();

void setup() {

  // Configure LED. Remember, HIGH is off.
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // Configure I2C pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize temperature sensor
  if ( !temp_sensor.begin(TEMP_I2C_ADDR) ) {

    // Blink rapidly forever on error
    while (1) {
      blink(1, 100);
    }
  }

  // Set temperature sensor to maximum resolution
  temp_sensor.setResolution(3);

  //  Force the ESP into client-only mode
  WiFi.mode(WIFI_STA); 

  //  Enable light sleep
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);

  // Connect to WiFi (blink while connecting)
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    blink(1, 500);
  }
}

void loop() {

  // Turn on LED to show that we are posting temp
  digitalWrite(LED_PIN, LOW);

  // Wake up temperature sensor and take reading
  temp_sensor.wake();
  float temp_c = temp_sensor.readTempC();

  // Construct URI from temperature value
  String temp_uri = "/" + String(temp_c, 2);

  // Connect to server (blink 3 times on error)
  if ( client.connect(hostname, port) == 0 ) {
    blink(3, 200);
  }

  // Send request
  client.print("GET " + temp_uri + 
                " HTTP/1.1\r\n" +
                "Host: " + hostname + "\r\n" +
                "Connection: close\r\n" +
                "\r\n");
  //delay(500);

  // Shut down client
  client.stop();

  // Put temperature sensor to sleep
  temp_sensor.shutdown_wake(1);

  // Turn off LED to show that we are done
  digitalWrite(LED_PIN, HIGH);

  delay(sleep_ms);
}

// Blink a number of times with given delay
void blink(int times, int delay_ms) {
  for ( int i = 0; i < times; i++ ) {
    digitalWrite(LED_PIN, LOW);
    delay(delay_ms);
    digitalWrite(LED_PIN, HIGH);
    delay(delay_ms);
  }
}
