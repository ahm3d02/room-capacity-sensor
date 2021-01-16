#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 4, 3, 2, 1, 0);

double distance1, distance2;
int numPeople = 0, threshold = 325;
bool sensor1 = false, sensor2 = false,
     enter1  = false, enter2  = false, enter3 = false,
     exit1   = false, exit2   = false, exit3  = false;

void setup() {
	lcd.begin(16, 2);
	updateScreen();
	
	for (int i = 6; i <= 10; ++i) pinMode(i, i % 2 ? INPUT:OUTPUT);
}


void loop() {
    if (digitalRead(10) == HIGH) {
		reset();
    }
	digitalWrite(6, LOW);
	delayMicroseconds(2);
	
	digitalWrite(6, HIGH);
	delayMicroseconds(10);
	digitalWrite(6, LOW);
	
	distance1 = pulseIn(7, HIGH)*0.034/2;
	
	digitalWrite(8, LOW);
	delayMicroseconds(2);
	
	digitalWrite(8, HIGH);
	delayMicroseconds(10);
	digitalWrite(8, LOW);
	
	distance2 = pulseIn(9, HIGH)*0.034/2;
	
	sensor1 = distance1 < threshold;
	sensor2 = distance2 < threshold;
	
  	if (!exit1) tripSensors(sensor1, sensor2, &enter1, &enter2, &enter3, 1);
    if (!enter1) tripSensors(sensor2, sensor1, &exit1, &exit2, &exit3, -1);
}

void tripSensors(bool sensor1, bool sensor2, bool *bool1, bool *bool2, bool *bool3, int inc) {
    if (!*bool1) {
        if (sensor1 && !sensor2) {
            *bool1 = true;
        }
    }
    if (*bool1 && !*bool2) {
        if (sensor1 && sensor2) {
            *bool2 = true;
        }
    }
    if (*bool1 && *bool2 && !*bool3) {
        if (!sensor1 && sensor2) {
            *bool3 = true;
        }
    }
    if (*bool1 && *bool2 && *bool3) {
        if (!sensor1 && !sensor2) {
            *bool1 = false, *bool2 = false, *bool3 = false, numPeople += inc;
          	updateScreen();
        }
        else if (sensor1) {
            *bool3 = false;
        }
    }
    if (!sensor1 && !sensor2 && *bool1) {
        *bool1 = false, *bool2 = false, *bool3 = false;
    }
}

void updateScreen() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Number of people");
  	lcd.setCursor(0, 1);
  	lcd.print(numPeople);
}

void reset() {
	numPeople = 0;
  	sensor1 = false, sensor2 = false,
    enter1  = false, enter2  = false, enter3 = false,
    exit1   = false, exit2   = false, exit3  = false;
  	updateScreen();
}