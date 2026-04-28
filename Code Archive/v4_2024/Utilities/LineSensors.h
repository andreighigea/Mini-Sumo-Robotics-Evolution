#ifndef LineSensors_h
#define LineSensors_h

extern const uint8_t RIGHT_LINE_SENSOR;
extern const uint8_t LEFT_LINE_SENSOR;

uint16_t sensorValues[2];
QTRSensors qtr;

uint16_t leftLineSensor;
uint16_t rightLineSensor;

void BeginLineSensors(){
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){RIGHT_LINE_SENSOR, LEFT_LINE_SENSOR}, 2);
}

void ReadLineSensors(){
  qtr.read(sensorValues);
  rightLineSensor = sensorValues[0];
  leftLineSensor = sensorValues[1];
}

void PrintLineSensors(){
  Serial.print("Left: ");
  Serial.print(leftLineSensor);
  Serial.print("\tRight: ");
  Serial.print(rightLineSensor);
  Serial.print("\n");
}

#endif