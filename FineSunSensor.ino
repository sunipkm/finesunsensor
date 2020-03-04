#include "sunsensor.h"

SunSensor sun_sensor;
int error;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
}

void loop() {
  // reading values... V1 to V4
  sun_sensor.output[0] = analogRead(A0)*POWER_SUPPLY/1024;
  sun_sensor.output[1] = analogRead(A1)*POWER_SUPPLY/1024;
  sun_sensor.output[2] = analogRead(A2)*POWER_SUPPLY/1024;
  sun_sensor.output[3] = analogRead(A3)*POWER_SUPPLY/1024;
  
  error = A60_Algorithm(&sun_sensor);
  
  //print sun sensor's value
  // Serial.print(sun_sensor.output[0]);
  // Serial.print("\t");
  // Serial.print(sun_sensor.output[1]);
  // Serial.print("\t");
  // Serial.print(sun_sensor.output[2]);
  // Serial.print("\t");
  // Serial.print(sun_sensor.output[3]);
  // Serial.print("\t");
  // Serial.print(sun_sensor.X1);
  // Serial.print("\t");
  // Serial.print(sun_sensor.X2);
  // Serial.print("\t");
  // Serial.print(sun_sensor.Y1);
  // Serial.print("\t");
  // Serial.print(sun_sensor.Y2);
  // Serial.print("\tERROR CODE: ");
  // Serial.print(error, BIN);
  // Serial.print("\tAngle X: ");
  // Serial.print(sun_sensor.angle[0]);
  // Serial.print("\tAngle Y: ");
  // Serial.print(sun_sensor.angle[1]);
  // Serial.print("\n");
  Serial.print(sun_sensor.angle[0]);
  Serial.print(",");
  Serial.print(sun_sensor.angle[1]);
  Serial.print(",");
  Serial.print(error);
  Serial.println();
  delay(50);
}
