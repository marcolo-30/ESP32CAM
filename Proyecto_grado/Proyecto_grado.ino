#include "ESPino32CAM.h"
ESPino32CAM cam;
#include <Wire.h>
#include "SparkFunBME280.h"


#define FLASH_LED_PIN 4
bool flashState = LOW;



// Define I2C Pins for BME280
#define I2C_SDA 14
#define I2C_SCL 15
const int EnTxPin =  12;

BME280 bme;


// Get BME280 sensor readings and return them as a String variable
String getReadings(){
  float temperature, humidity;
  temperature = bme.readTempC();
  //temperature = bme.readTempF();
  humidity = bme.readFloatHumidity();
  String message = "Temperature: " + String(temperature) + " ºC \n";
  message += "Humidity: " + String (humidity) + " % \n";
  return message;
}


void setup(){

  Serial.begin(9600);
  pinMode(EnTxPin, OUTPUT);
  digitalWrite(12,HIGH);
  // Init BME280 sensor
  Wire.begin(I2C_SDA, I2C_SCL);
  bme.settings.commInterface = I2C_MODE;
  bme.settings.I2CAddress = 0x76;
  bme.settings.runMode = 3;
  bme.settings.tStandby = 0;
  bme.settings.filter = 0;
  bme.settings.tempOverSample = 1;
  bme.settings.pressOverSample = 1;
  bme.settings.humidOverSample = 1;
  bme.begin();
 
}

void loop(){

 String mensaje = getReadings();
 Serial.println (mensaje);

 delay(2000);
}
