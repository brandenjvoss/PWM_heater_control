/*
This program uses PWM to control output from resistive heaters and uses a DS18B20 digital temperature
sensor. The intent is to hold an ambient temperature of 60 degrees celcius inside a box used for fuse
testing. The heater_output function will provide full heat up to a sensor reading of 55 degrees
celsius. Then, the heater will turn off until a reading of 58 is seen (heater continues to radiate
heat even when off raising the temp a few degrees more). Output is then modulated to 50% duty cycle
from 58-59 and 38% from 59-60. Again, an output of 0 is acheived when the sensor reads higher than 60
degrees celsius. From here, the temperature will fluctuate from 59.31-60.25 degrees celsius and is
successfully maintained.  
*/

/****************************************************************************/
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/****************************************************************************/
#define data 3 // Data from sensor is pin 3
#define heat 5 // PWM control from pin 5

/****************************************************************************/
OneWire oneWire(data); // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensor(&oneWire); // Pass our oneWire reference to Dallas Temperature.

/****************************************************************************/
// function to control heater output
// takes variable 'temp' of type float and modulates duty cycle accordingly 
void heater_output(float temp){
  if(temp < 55){
    analogWrite(heat, 255);
    Serial.println("Heater 255");
  }
  else if(temp >= 55 && temp < 58){
    analogWrite(heat, 0);
    Serial.println("Heater 0");
  }
  else if(temp >= 58 && temp < 59){
    analogWrite(heat, 127);
    Serial.println("Heater 127");
  }
  else if(temp >= 59 && temp < 60){
    analogWrite(heat, 97);
    Serial.println("Heater 97");
  }
  else if(temp >= 60){
    analogWrite(heat, 0);
    Serial.println("Heater 0");
  }
}

/****************************************************************************/
void setup(void) 
{  
  Serial.begin(9600); // start serial port  
  sensor.begin(); // Start up the library
} 

/****************************************************************************/
void loop(void) 
{
  sensor.requestTemperatures(); // Send the command to get temperature readings 
  float temp = sensor.getTempCByIndex(0); // variable to store temperature reading
  // Why "byIndex"?  
  // You can have more than one DS18B20 on the same bus.  
  // 0 refers to the first IC on the wire

  Serial.print("Temperature is: "); 
  Serial.println(temp);
 
  heater_output(temp); // function call to control heater output passing 'temp' 

  delay(500);
  // prevents serial port from spamming temperature readings
  // may be more accurate without a delay?
} 