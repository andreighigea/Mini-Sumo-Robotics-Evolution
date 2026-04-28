#include <cstddef>
#ifndef Flags_h
#define Flags_h

#include <ESP32Servo.h>
#include "Settings.h"
#include "MyDelay.h"
using namespace FlagSettings;

Servo RightServo;
Servo LeftServo;

TaskHandle_t taskHandle = NULL;

int pos;
uint8_t degreeDelay;

uint8_t par;

MyDelay leftDelay = MyDelay();
MyDelay rightDelay = MyDelay();

void SetupServos() {
	RightServo.setPeriodHertz(50);    // standard 50 hz servo
	LeftServo.setPeriodHertz(50);
	RightServo.attach(rightServoPin, 650, 2500); // attaches the servo on pin 18 to the servo object
	LeftServo.attach(leftServoPin, 650, 2500);

  degreeDelay = desiredFlagDelay / 85;
}

void FD(void *param){
	for (pos = 95; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
		LeftServo.write(275-pos);
		RightServo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(degreeDelay);             // waits 15ms for the servo to reach the position
	}
	taskHandle = NULL;
	vTaskDelete(NULL); // Delete the task after completion
}

void FU(void *param){
	if (returnUp){
		for (pos = 180; pos >= 95; pos -= 1) { // goes from 180 degrees to 0 degrees
			LeftServo.write(275-pos);
			RightServo.write(pos);    // tell servo to go to position in variable 'pos'
			delay(5);             // waits 15ms for the servo to reach the position
		}
	}
	taskHandle = NULL;
	vTaskDelete(NULL); // Delete the task after completion
}

void Buffer(void *param){
	uint8_t task = *(uint8_t*)param;
	while (taskHandle != NULL){
		delay(10);
		Serial.println("test");
	}
	if (task == 0){
		xTaskCreatePinnedToCore(
      FU,      // Task function
      "FlagsUp",    // Task name
      2048,           // Stack size
      NULL,           // Task parameter
      1,              // Task priority
      &taskHandle,    // Task handle
      0               // Core 0
    );
	}
	else if (task == 1){
		xTaskCreatePinnedToCore(
      FD,      // Task function
      "FlagsDown",    // Task name
      2048,           // Stack size
      NULL,           // Task parameter
      1,              // Task priority
      &taskHandle,    // Task handle
      0               // Core 0
    );
	}

	vTaskDelete(NULL); // Delete the task after completion
}

void FlagsUp(){
	par = 0;
	xTaskCreatePinnedToCore(
		Buffer,      // Task function
		"Buffer",    // Task name
		1024,           // Stack size
		&par,           // Task parameter
		2,              // Task priority
		NULL,    // Task handle
		0               // Core 0
	);
}

void FlagsDown(){
	par = 1;
	xTaskCreatePinnedToCore(
		Buffer,      // Task function
		"Buffer",    // Task name
		1024,           // Stack size
		&par,           // Task parameter
		2,              // Task priority
		NULL,    // Task handle
		0               // Core 0
	);
}

void MatadorCheck(void *param) {
  while (true) {
		if (start){
			int angle = 0;
			if (left <= 280) {
				// Map sensor value [200 → 50] to angle [0 → 45]
				angle = map(constrain(left, 50, 280), 280, 50, 180, 135);
				RightServo.write(angle);
				leftDelay.StartTimer(400);
			}
			if (right <= 250){
				angle = map(constrain(right, 50, 250), 250, 50, 95, 135);
				LeftServo.write(angle);
				rightDelay.StartTimer(400);
			}

			if (leftDelay.IsOver()){
				RightServo.write(180);
			}

			if (rightDelay.IsOver()){
				LeftServo.write(95);
			}
		}

		vTaskDelay(22 / portTICK_PERIOD_MS);  // check every 50ms
	}
}

void Matador (){
	if (matador){
		xTaskCreatePinnedToCore(
			MatadorCheck,  // task function
			"Flag Task",       // name
			2048,              // stack size
			NULL,              // parameters
			1,                 // priority
			NULL,   // task handle
			0                  // core 0
		);
	}
}

#endif